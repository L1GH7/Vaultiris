def rotate(a, n):
    return a[n:] + a[:n]


def getNextMulticastMAC(addr):
    addr = addr.split(":")
    ret_addr = ""
    ret_addr = addr[0] + ":" + addr[1] + ":" + addr[2] + ":"
    ret_addr += str(format((int(addr[-1], 16) + 1) % 101, "02x"))
    ret_addr += ":"
    ret_addr += str(format((int(addr[-2], 16) + 1) % 101, "02x"))
    ret_addr += ":"
    ret_addr += str(format((int(addr[-3], 16) + 1) % 101, "02x"))

    if(ret_addr == "01:00:5e:00:00:00"):
        return "01:00:5e:01:01:01"
    else:
        return ret_addr


def printPacket(packet):
    p_len = 0
    if("IP" in str(packet.layers)):
        p_len = int(packet['IP'].get_field_value('len'))
    else:
        p_len = int(packet.layers[3].get_field_value('len'))
    p_sa = str(packet.layers[2].get_field_value('sa_resolved'))
    p_bssid = str(packet.layers[2].get_field_value('bssid_resolved'))
    p_da = str(packet.layers[2].get_field_value('da_resolved'))
    p_ta = str(packet.layers[2].get_field_value('ta_resolved'))

    print("Got packet with len (" + str(p_len) + ") - Source Address: " + str(p_sa) +
          " | bssid: " + str(p_bssid) + " | Destination Address: " + str(p_da) + "| Transmitter Address: " + str(p_ta))


def checkPacketQoS(packet):
    if(str(packet.layers[2].get_field_value('fc_type_subtype')) == "40"):
        return True
    else:
        return False


def checkPacketFCS(packet):
    if(str(packet.layers[2].get_field_value('fcs_status')) != "0" and
        str(packet.layers[2].get_field_value('fc_type')) == "2" and
        str(packet.layers[2].get_field_value('fc_subtype')) == "8" and
        str(packet.layers[2].get_field_value('fc_version')) == "0" and
            str(packet.layers[2].get_field_value('flags')) == "0x00000041"):
        return True
    else:
        return False


def isMulticastAddress(address):
    address_splitted = str(address.upper()).split(':')
    #address_splitted = address_splitted
    if(address_splitted[0] == "01" and
        address_splitted[1] == "00" and
        address_splitted[2] == "5E" and
        address_splitted[-1] == address_splitted[-2] and
            address_splitted[-1] == address_splitted[-3]):
        return True
    else:
        return False
