import usb.core
import usb.util
import time


import time

dev = usb.core.find(idVendor=0x1fc9, idProduct=0x0021)


responses = []
size = 0


try:
    send = dev.ctrl_transfer(0x80, 6, 0x0200, 0x1, 0xff)
    if len(send) >= size:
        print(str(send),len(send))
    send = dev.ctrl_transfer(0x80, 6, 0x0201, 0x1, 0xfff)
    if len(send) >= size:
        responses.append({"resp":list(send)})
        print(str(send), len(send))
except:
    pass

for i in range(0, len(responses)):
            f = open("responses%d.txt" % i, "w")
            f.write("{}\n".format(responses[i]))
            f.close()
            f = open("responses%d.bin" % i, "w")
            f.write("".join([chr(elem) for elem in responses[i]["resp"]]))
            f.close()
