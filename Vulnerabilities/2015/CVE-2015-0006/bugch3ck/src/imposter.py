#!/usr/bin/env python

"""
Main module of the IMPOSTER application.
Copyright 2014, Jonas Vestberg

"""

import sys
import signal
from threading import Thread

import imposter
from servers import *

VERSION = 0.1
BANNER = \
    "    _                     __         \n" \
    "   (_)_ _  ___  ___  ___ / /____ ____\n" \
    "  / /  ' \/ _ \/ _ \(_--/ __/ -_) __/\n" \
    " /_/_/_/_/ ___/\___/___/\__/\__/_/   \n" \
    "        /_/               by bugch3ck"

def show_banner():
	print BANNER
	print "# version %s" % (VERSION)
	print ""

def signal_handler(signum, frame):
	print ""
	imposter.stop()
	exit(0)

global_instance = None # shared instance 

class imp():

	def __init__(self, host):
		self.host = host
		self.servers = []
		self.threads = []

		imposter.global_instance = self

	def add_server(self, server_class):

		# Create server instance and run in separate thread.

		server = server_class(self.host)

		server_thread = Thread(target=server.serve_forever)
		server_thread.daemon = True
		server_thread.start()

		self.servers.append(server)
		self.threads.append(server_thread)

	def shutdown(self):
		# Shutdown all running services
		for server in self.servers:
			server.shutdown()

def start(host):

	# Create (shared) instance
	obj = imp(host)

	# Add servers
	obj.add_server(DNSServer)
	obj.add_server(CLDAPServer)
	obj.add_server(LDAPServer)

def stop():
	# Graceful shutdown of servers
	global_instance.shutdown()
	

def main(argc, argv):

	show_banner() # mmm, asciiart...

	if argc <= 1:
		print '[-] error: need server host argument'
		exit(-1)

	print "(use ctrl+c to shut down)"
	print ""

	start(argv[1])

	# Wait until ctrl+c and shutdow gracefully
	signal.signal(signal.SIGINT, signal_handler)
	signal.pause()

if __name__ == "__main__":
	main(len(sys.argv),sys.argv)

