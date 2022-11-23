import struct
import socket
import array

class PacketConstructor:
    def __init__(self,
                 src_host:  str,
                 src_port:  int,
                 dst_host:  str,
                 dst_port:  int,
                 sequence:  int,
                 ack:       int,
                 flags:     int,
                 checksum:  int,
                 data:      bytes):
        self.src_host = src_host
        self.src_port = src_port
        self.dst_host = dst_host
        self.dst_port = dst_port
        self.sequence = sequence
        self.ack      = ack
        self.flags    = flags
        self.checksum = checksum
        self.data     = data

    def build_ip_header(self):
        ip_header  = b'\x45\x00\x00\x28'  # Version, IHL, Type of Service | Total Length
        ip_header += b'\xab\xcd\x00\x00'  # Identification | Flags, Fragment Offset
        ip_header += b'\x40\x06\x00\x00'  # TTL, Protocol | Header Checksum
        ip_header += socket.inet_aton(self.src_host)
        ip_header += socket.inet_aton(self.dst_host)

        checksum = self.calculate_checksum(ip_header)

        ip_header = ip_header[:4] + struct.pack('H', checksum) + ip_header[6:]
        return ip_header

    def build_tcp_header(self) -> bytes:
        flags = self.compute_tcp_flags(self.flags)

        tcp_header = struct.pack(
            '!HHIIBBHHH',
            self.src_port,  # Source Port
            self.dst_port,  # Destination Port
            self.sequence,  # Sequence Number
            self.ack,       # Acknoledgement Number
            5 << 4,         # Data Offset
            flags,          # Flags
            8192,           # Window
            0,              # Checksum (Initial value of zero, calculated later)
            0               # Urgent pointer
        )

        pseudo_header = struct.pack(
            '!4s4sHH',
            socket.inet_aton(self.src_host),
            socket.inet_aton(self.dst_host),
            socket.IPPROTO_TCP,
            len(tcp_header) + len(self.data)
        )

        if self.validate_checksum(self.checksum) == True:
            checksum = self.checksum
        else:
            checksum = self.calculate_checksum(pseudo_header + tcp_header + self.data)

        tcp_header = tcp_header[:16] + struct.pack('H', checksum) + tcp_header[18:]
        return tcp_header

    def compute_tcp_flags(self, tcp_flags):
        if "fin" in tcp_flags: fin = 1
        else: fin = 0
        if "syn" in tcp_flags: syn = 1
        else: syn = 0
        if "rst" in tcp_flags: rst = 1
        else: rst = 0
        if "psh" in tcp_flags: psh = 1
        else: psh = 0
        if "ack" in tcp_flags: ack = 1
        else: ack = 0
        if "urg" in tcp_flags: urg = 1
        else: urg = 0

        computed_flags = fin + (syn << 1) + (rst << 2) + (psh <<3) + (ack << 4) + (urg << 5)
        return computed_flags

    def validate_checksum(self, checksum):
        try:
            int(checksum, 16)
            return True
        except:
            return False

    def calculate_checksum(self, packet: bytes) -> int:
        if len(packet) % 2 != 0:
            packet += b'\0'

        res = sum(array.array("H", packet))
        res = (res >> 16) + (res & 0xffff)
        res += res >> 16
        return (~res) & 0xffff


def build_tcp_packet(dst, dst_port, src, src_port, seq_number, ack_number, tcp_flags, checksum, data):
    packet = PacketConstructor(
        src,
        src_port,
        dst,
        dst_port,
        seq_number,
        ack_number,
        tcp_flags,
        checksum,
        data
    )

    complete_tcp_packet = packet.build_ip_header() + packet.build_tcp_header() + data
    return complete_tcp_packet
