# This file takes a list of IP as input (within a file), checks those IPs against Shodan
# to finde potential other ports where a TeamServer is running and starts spamming it
import argparse
import re
import validators
from urllib.parse import urlparse
import socket
import shodan
import json
import os
import threading
import itertools
import time

SHODAN_API_KEY = "" # Add your Shodan API Key here
api = shodan.Shodan(SHODAN_API_KEY)

ips = []
num_threads = 10
alive_hosts = []

class FastWriteCounter(object):
    def __init__(self):
        self._number_of_read = 0
        self._counter = itertools.count()
        self._read_lock = threading.Lock()

    def increment(self):
        next(self._counter)

    def value(self):
        with self._read_lock:
            value = next(self._counter) - self._number_of_read
            self._number_of_read += 1
        return value


def check_ip(cnt):
    if cnt.value() > len(ips):
        print("No more lines to read waiting for other threads to finish")
        return

    print("Now reading line " + str(cnt.value()))
    ip = list(ips)[cnt.value()]
    cnt.increment()
    try:
        info = api.host(ip)
        try:
            for host in info["data"]:
                os.system("python3 spam.py --url https://" + ip + ":" + str(host["port"]) + "/ --use_tor true --publish_to_threatfox true --print_config false")
                os.system("python3 spam.py --url http://" + ip + ":" + str(host["port"]) + "/ --use_tor true --publish_to_threatfox true --print_config false")
#                os.system("python3 exploit.py --url http://" + ip + ":" + str(host["port"]) + " --use_tor true --max_hits 100")
#                os.system("python3 exploit.py --url https://" + ip + ":" + str(host["port"]) + " --use_tor true --max_hits 100")
        except Exception as e:
            print("Nope: " + str(e))

    except shodan.exception.APIError as e:
        print("Shodan error: " + str(e))

    try:
        os.system("python3 spam.py --url https://" + ip + "/ --use_tor true --publish_to_threatfox true --print_config false")
        os.system("python3 spam.py --url http://" + ip + "/ --use_tor true --publish_to_threatfox true --print_config false")
#         os.system("python3 exploit.py --url https://" + ip + "/ --use_tor true --max_hits 100")
#         os.system("python3 exploit.py --url http://" + ip + "/ --use_tor true --max_hits 100")
    except:
        print("Can't post")



if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("--path", help="Path to textfile with targets")
    args = parser.parse_args()

    if args.path:
        with open(args.path) as fh:
            fstring = fh.readlines()

        pattern = re.compile(r'(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})')

        # First we search for IPs
        for line in fstring:
            try:
                ips.append(pattern.search(line)[0])
            except:
                pass
                #print("No IP found: " + line)

        # Then we search for domains
        for line in fstring:
            try:
                words = line.split(" ")
                for word in words:
                    word = word.replace("\n", "")
                    if len(word) > 0:
                        if validators.domain(word):
                            ip = socket.gethostbyname(word)
                            if ip != "0.0.0.0":
                                ips.append(ip)
            except Exception as e:
                print(e)

        # Then we search for URLs and extract domains
        for line in fstring:
            try:
                words = line.split(" ")
                for word in words:
                    word = word.replace("\n", "")
                    if len(word) > 0:
                        domain = urlparse(word).netloc
                        ip = socket.gethostbyname(domain.split(":")[0])
                        if ip != "0.0.0.0":
                            ips.append(ip)
            except Exception as e:
                print(e)

        # Deduplicate list
        ips = set(ips)
        print("Found " + str(len(ips)) + " unique IPs!")
        for ip in ips:
            print(ip)

        if len(ips) > 0:
            cnt = FastWriteCounter()
            threads = []

            while cnt.value() < len(ips):
#                for i in range(10):
                for i in range(len(ips)):
                    print("Spawning new thread")
                    t = threading.Thread(target=check_ip, args=(cnt,))
                    threads.append(t)
                    t.start()
                    time.sleep(1.1) # Due to Shodan Request limit
                
                # Now join the threads all together and wait for them to end
                for t in threads:
                    t.join()
        else:
            print("Found no IP addresses in target file!")


    else:
        print("Error: No targets specified")
