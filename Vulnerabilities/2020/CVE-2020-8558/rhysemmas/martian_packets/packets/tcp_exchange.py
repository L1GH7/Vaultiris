import struct
import socket
from packets.tcp_craft import build_tcp_packet

class PacketExchanger:
    def __init__(self,
                 real_dst: str,
                 dst_host: str,
                 dst_port: int,
                 src_host: str,
                 src_port: int,
                 data:     bytes,
                 checksum: int):
        self.real_dst = real_dst
        self.dst_host = dst_host
        self.dst_port = dst_port
        self.src_host = src_host
        self.src_port = src_port
        self.data     = data
        self.checksum = checksum

    def tcp_handshake(self):
        ack = 0
        sequence = 0
        flags = ("syn")
        empty_data = b""
        packet = build_tcp_packet(self.dst_host, self.dst_port, self.src_host, self.src_port, sequence, ack, flags, self.checksum, empty_data)
        self.send_packet(packet)
        response_sequence, _ = self.receive_response()

        ack = response_sequence + 1
        sequence += 1
        flags = ("ack")
        packet = build_tcp_packet(self.dst_host, self.dst_port, self.src_host, self.src_port, sequence, ack, flags, self.checksum, empty_data)
        self.send_packet(packet)
        return sequence, response_sequence

    def tcp_push_data(self, sequence, ack):
        flags = ("psh", "ack")
        packet = build_tcp_packet(self.dst_host, self.dst_port, self.src_host, self.src_port, sequence, ack, flags, self.checksum, self.data)
        self.send_packet(packet)
        _, response = self.receive_response()
        return response

    def send_packet(self, packet):
        s = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_TCP)
        s.setsockopt(socket.IPPROTO_IP, socket.IP_HDRINCL, 1)
        s.sendto(packet, (self.real_dst, 0))
        s.close()

    def receive_response(self):
        r = socket.socket(socket.AF_PACKET, socket.SOCK_RAW, socket.htons(3))
        r.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, 2**30)
        r.bind(("eth0", 0x0003))

        while True:
            ethernet_data, address = r.recvfrom(65536)
            dest_mac, src_mac, protocol, ip_data = self.ethernet_dissect(ethernet_data)

            ip_protocol, source_ip, dest_ip, tcp_data = self.ipv4_packet(ip_data)
            if self.validate_tcp_packet(ip_protocol) != True:
                continue

            dest_port, sequence, tcp_header_length = self.unpack_tcp_header(tcp_data)
            if self.validate_source(source_ip, dest_port) != True:
                continue
            else:
                break

        r.close()

        response_data = tcp_data[tcp_header_length:]
        return sequence, response_data

    def ethernet_dissect(self, ethernet_data):
        dst_mac, src_mac, protocol = struct.unpack('!6s6sH', ethernet_data[:14])
        return self.mac_format(dst_mac), self.mac_format(src_mac), socket.htons(protocol), ethernet_data[14:]

    def mac_format(self, mac):
        mac = map('{:02x}'.format, mac)
        return ':'.join(mac).upper()

    def ipv4_packet(self, ip_data):
        ip_protocol, source_ip, target_ip = struct.unpack('!9xB2x4s4s' , ip_data[:20])
        return ip_protocol, self.ipv4_format(source_ip), self.ipv4_format(target_ip), ip_data[20:]

    def ipv4_format(self, address):
        return '.'.join(map(str, address))

    def validate_tcp_packet(self, ip_protocol):
        if ip_protocol == 0x06:
            return True
        elif ip_protocol != 0x06:
            return False
        else:
            raise ValueError('Unexpected protocol version while reading received packet: ' + ip_protocol)

    def unpack_tcp_header(self, tcp_data):
        tcp_header = tcp_data[:20]
        unpacked_header = struct.unpack('!HHLLBBHHH' , tcp_header)

        source_port   = unpacked_header[0]
        dest_port     = unpacked_header[1]
        sequence      = unpacked_header[2]
        ack           = unpacked_header[3]
        doff_reserved = unpacked_header[4]

        tcp_header_length = doff_reserved >> 4
        tcp_header_length = tcp_header_length * 4
        return dest_port, sequence, tcp_header_length

    def validate_source(self, source_ip, dest_port):
        if source_ip == self.dst_host:
            if dest_port == self.src_port:
                return True
        else:
            return False


def send_tcp_data(real_dst, dst, dst_port, src, src_port, data):
    checksum = ""
    send = PacketExchanger(
        real_dst,
        dst,
        dst_port,
        src,
        src_port,
        data,
        checksum
    )

    sequence, ack = send.tcp_handshake()
    response = send.tcp_push_data(sequence, ack)
    return response
