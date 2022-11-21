import socket
import ssl
import sys
import warnings
import enum
import argparse
import ipaddress

warnings.filterwarnings("ignore", category=DeprecationWarning)


TIMEOUT = 0.2


class OpSll(enum.Enum):
    Error = -1
    Cert_not_required = 0
    Cert_required = 1

def fileload(filename):
    # This get input from text file and converts to list
    f= open(filename, "r")
    content=f.read()
    f.close()
    content=content.split("\n")
    while("" in content):
        content.remove("")
    return content

def Server_Connection_Status(host, port):
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    client = ssl.wrap_socket(client)

    try:
        client.connect((host, port))
    except Exception as e:
        print(e)
        return OpSll.Error

    client.settimeout(TIMEOUT)
    try:
        client.read(1)

    except ssl.SSLError as err:
        if "CERTIFICATE_REQUIRED" in str(err):
            return OpSll.Cert_required
    except TimeoutError:
        return OpSll.Cert_not_required

    except Exception as e:
        return OpSll.Error

    return OpSll.Cert_not_required


def reporting(host, port, status):
    print('[*] Host information: {0}:{1}'.format(host,port))
    if OpSll.Cert_not_required == status:
        print('[+] Status: {0}'.format('Not Vulnerable'))
        print('[+] Reason: {0}'.format('Client certificate not required!'))
        
    if OpSll.Cert_required == status:
        print('[+] Status: {0}'.format('Vulnerable'))
        print('[+] Reason: {0}'.format('Client certificate is required!'))
        
    if OpSll.Error == status:
        print('[-] Status: {0}'.format('Unable to connect'))
        print('[-] Reason: {0}'.format('Either Host is down or crashed!'))
    


# adding argparse modules
parser = argparse.ArgumentParser()
parser.add_argument("-t","--target", help="Single IP with port separate by colon. Example: -t 192.168.0.3:3000",type=str)
parser.add_argument("-T","--targets", help="List of IP and port separate by colon ssin text file",type=str)
args = parser.parse_args()
if len(sys.argv) < 2:
    parser.print_help()
    sys.exit(1)


if __name__ == "__main__":
    print('[!] CVE: CVE-2022-3602, CVE-2022-3786')
    print('[!] This script will detect whether openssl \n[!] server is vulnerable or not based on')
    print('[!] whether certificated is required by server or not\n')

    info=dict()

    if args.target:
        ip_list=[args.target]
        
    if args.targets:
        ip_list=fileload(args.targets)

    if len(ip_list)==0:
        print("Required argument:\n-t or -T         Single Ip/file with ip list")
        sys.exit(1)
    
    for host in ip_list:
        host=host.split(":")
        res = Server_Connection_Status(host[0],int(host[1]))
        reporting(host[0],int(host[1]), res)
        print('\n')
    
