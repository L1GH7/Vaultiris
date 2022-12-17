import usb.core
import usb.util
import time


import time

dev = usb.core.find(idVendor=0x15a2, idProduct=0x0073)


responses = []
size = 200

for i in range(128,133,1):
    for j in range(0, 2, 1):
        for k in range(0, 65535, 1):
            try:
                print(i,j,k)
                send = dev.ctrl_transfer(i, j, k, 0xffff, 0xefff)
                if len(send) >= size:
                    responses.append({"resp":list(send)})
                    print(str(send), len(send))
                    for i in range(0, len(responses)):
                        f = open("responses%d.txt" % i, "w")
                        f.write("{}\n".format(responses[i]))
                        f.close()
                        f = open("responses%d.bin" % i, "w")
                        f.write("".join([chr(elem) for elem in responses[i]["resp"]]))
                        f.close()
            except:
                pass

