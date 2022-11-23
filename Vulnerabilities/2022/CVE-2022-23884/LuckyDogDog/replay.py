import marshal
import socket,time,threading,sys
def get_s_sck():
    sk = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    return sk
def replay():
    sk=get_s_sck()
    cont = marshal.load(open(sys.argv[3], "rb"))
    for i in cont:
        sk.sendto(i, (sys.argv[1], int(sys.argv[2])))
        time.sleep(0.1)
    print("done")
    sk.close()
replay()
