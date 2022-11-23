#!/usr/bin/python

import Queue
import threading
import getopt
import sys
import urllib2
import hashlib
import socket
import time
import os
import re
import netaddr
import subprocess

#Max Scanning Thread Count
max_threads = 50

output=[]

def test_ip(ip_address, identifier):
        #Identifier is not used
        IP = ip_address.strip()
        try:
                socket.inet_aton(IP)
        except:
                #print "%s,invalid IP" % IP
                return

        try:
                result = subprocess.Popen(['timeout','4','openssl','s_client','-connect',ip_address+":443","-cipher","EXPORT"], stderr=subprocess.STDOUT, stdout=subprocess.PIPE).communicate()[0]
                print result
                if "Cipher is EXP" in result:
                        print "%s,Vulnerable" % ip_address
                else:
                        print "%s,NotVulnerable" % ip_address

        except:
                return

def usage():
        #Print usage
        print "\"FREAK\" TLS Export Cipher Scanner"
        print "Author: Scott Pack"
        print "Options:"
        print "-i <inputfile>"
        print "inputfile must consist of line-delimited IPv4 Addresses or CIDR ranges."

def main():
        #read IP Addresses to Scan
        input_filename = ""
        try:
                opts, args = getopt.getopt(sys.argv[1:],"i:p:o:")
        except getopt.GetoptError as err:
                print str(err)
                usage()
                sys.exit()

        #Get our opts in place.
        for o, a in opts:
                if o == "-h":
                        usage()
                        return
                elif o == "-i":
                        input_filename = a

        if input_filename == "":
                usage()
                return

        input_file = open(input_filename,"r")
        ips = []

        for line in input_file:
                line = line.strip()
                try:
                        socket.inet_aton(line)
                        ips.append(line)
                except:
                        pass
                if "/" in line:
                        try:
                                for ip in netaddr.IPNetwork(line):
                                        ips.append(str(ip))
                        except:
                                pass

        count = len(ips)
        dur = 3 + 5 + (count/max_threads)*20

        print >> sys.stderr, "This scan for %s IPs will likely take %s seconds" % (count, dur)
        print >> sys.stderr,  "Starting scan now..."

        for IP in ips:
                t=threading.Thread(target=test_ip,args=(IP,""))
                #t.daemon = True
                t.start()
                while (threading.activeCount()) >= max_threads:
                        #print "Hit max thread count (%s/%s), waiting 2 seconds\n" % (str(threading.activeCount()),max_threads)
                        time.sleep(5)

        while (threading.activeCount() > 2):
#               print "Waiting for %s threads to close" % threading.activeCount()
                time.sleep(5)
        time.sleep(5)

main()
