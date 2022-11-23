#!/usr/bin/python

# poodle_protector.py - a script for protecting your
# servers against POODLE (CVE-2014-3566)
#
# 2014 By Christian Stankowic
# <info at stankowic hyphen development dot net>
# https://github.com/stdevel
#

from optparse import OptionParser
import os
import time

def get_distro():
	#try to guess Linux distribution
	try:
		result = os.popen("lsb_release -d|tr -d '[:space:]'|cut -d: -f 2").read().lower()
		if "redhat" in result: return "redhat"
		elif "centos" in result: return "centos"
		elif "fedora" in result: return "fedora"
		elif "debian" in result: return "debian"
		elif "ubuntu" in result: return "ubuntu"
		elif "opensuse" in result: return "suse"
		elif "suse" in result: return "sles"
		else: return "unknown"
	except:
		return "unknown"

if __name__ == "__main__":
        #define description, version and load parser
        desc='''%prog is used to protect your servers against POODLE vulnerability (CVE-2014-3566). It automatically detects apache configuration files vulnerable to POODLE and customizes them after creating backups.
	Checkout the GitHub page for updates: https://github.com/stdevel/poodle_protector'''
	parser = OptionParser(description=desc,version="%prog version 0.2")
	
        #-c / --custom-string
        parser.add_option("-c", "--custom-string", dest="customString", metavar="STRING", help="defines a custom SSLProtocol configuration string")
	
	#-p / --custom-path
	parser.add_option("-p", "--custom-path", dest="customPath", metavar="PATH", help="defines a custom path for apache configuration files in case you're not using distribution defaults")

        #-q / --quiet
        parser.add_option("-q", "--quiet", action="store_false", dest="verbose", default=True, help="don't print status messages to stdout")

        #-d / --debug
        parser.add_option("-d", "--debug", dest="debug", default=False, action="store_true", help="enable debugging outputs")

        #-n / --no-backup
        parser.add_option("-n", "--no-backup", dest="noBackup", default=False, action="store_true", help="don't create backups if you like to like dangerously")

        #-l / --dry-run
        parser.add_option("-l", "--dry-run", dest="listOnly", default=False, action="store_true", help="only simulates what would be done")
	
	#-r / --service-restart
	parser.add_option("-r", "--service-retart", dest="serviceRestart", default=False, action="store_true", help="restarts the affected service(s) using the appropriate wrapper")

        #parse arguments
        (options, args) = parser.parse_args()
	
	#debug
	if options.debug: print "DEBUG: options: " + str(options) + "\nargs: " + str(args)
	
	#try to guess distribution
	distro=get_distro()
	
	#default paths to have a look at
	try:
		if len(options.customPath) > 0:
			default_paths=[options.customPath]
	except:
		if distro in ['redhat','centos','fedora']:
			#RH(EL)-like distro
			default_paths=["/etc/httpd/conf.d"]
		elif distro in ['debian','ubuntu']:
			#Debian-like distro
			default_paths=["/etc/apache2/mods-available","/etc/apache2/sites-available"]
		elif distro in ['suse','sles']:
			#SUSE-like distro
			default_paths=["/etc/apache2/vhosts.d"]
		else:
			#don't know
			default_paths=["/etc/httpd/conf.d","/etc/apache2/mods-available","/etc/apache2/sites-available","/etc/apache2/vhosts.d"]
	if options.debug: print "DEBUG: distro =",distro
	
	#string replacement
	try:
		if len(options.customString) > 0 and "SSLProtocol" in options.customString:
			if options.debug: print "DEBUG: valid custom string supplied: '" + options.customString + "'"
			strReplace = options.customString
		else:
			print "ERROR: custom string '" + options.customString + "' invalid - choosing default (SSLProtocol All -SSLv2 -SSLv3)"
			strReplace = "SSLProtocol All -SSLv2 -SSLv3"
	except:
		if options.debug: print "DEBUG: custom string invalid or non-existent - choosing default (SSLProtocol All -SSLv2 -SSLv3)"
		strReplace = "SSLProtocol All -SSLv2 -SSLv3"
	
	#service restart commands
	if distro in ['redhat','centos']:
		#RH(EL)-like distro
		serviceCmds=["service httpd restart"]
	elif distro in ['fedora']:
		#Fedora
		serviceCmds=["systemctl restart httpd.service","service restart httpd"]
	elif distro in ['debian','ubuntu']:
		#Debian-like distro
		serviceCmds=["service apache2 restart"]
	elif distro in ['suse','sles']:
		#SUSE-like distro
		serviceCmds=["service apache2 restart"]
	else:
		#don't know, try various service/systemd stuff
		serviceCmds=["service httpd restart","service apache2 restart","systemctl restart httpd.service","systemctl restart apache2.service"]
	
	#check _all_ the paths for vulnerable files
	for path in default_paths:
		if options.debug: print "DEBUG: checking path '" + str(path) + "'..."
		command = "grep SSLProtocol " + path + " -R|grep -v 'All -SSLv2 -SSLv3'|grep -v 'All -SSLv3 -SSLv2'|cut -d: -f1|grep -v '*conf*.*'"
		if options.debug: print "DEBUG: " + command
		hits = os.popen(command).read().split("\n")
		hits.remove("")
		if options.debug: print "DEBUG: hits: " + str(hits)
		
		if len(hits) == 0:
			print "No configuration vulnerable to POODLE found."
			exit(0)
		
		for hit in hits:
			if options.listOnly:
				#dry-run
				if options.noBackup == False: print "I'd like to create a backup of '" + hit + " as '" + str(hit + "." + time.strftime("%Y%m%d-%H%M")) + "' ..."
				print "I'd like to insert '" + strReplace + "' into " + hit + " using the following command: sed -i '/SSLProtocol/ c\\" + strReplace + "' " + hit + " ..."
				if options.serviceRestart: print "I'd also like to restart the service using: " + str(serviceCmds).replace("[","").replace("]","")
			else:
				#backup and customize configuration
				if os.access(os.path.dirname(hit), os.W_OK):
					if options.debug: print "Write permissions to '" + os.path.dirname(hit) + "'"
					if options.noBackup == False:
						result = os.system("cp " + hit + " " + hit + "." + time.strftime("%Y%m%d-%H%M"))
					else: result = 0
					if result == 0:
						#run sed to customize file
						result = os.system("sed -i '/SSLProtocol/ c\\" + strReplace + "' " + hit)
						if result == 0:
							if options.debug: print "Successfully customized file '" + hit + "' ..."
						else:
							print "ERROR: unable to customize file '" + hit + "' ..."
					else:
						print "ERROR: unable to copy '" + hit + "' to '" + hit + "." + time.strftime("%Y%m%d-%H%M")
				else:
					#no write permissions so we're dying in a fire
					print "No write permissions to '" + os.path.dirname(hit) + "'"
					exit(1)
		#restart service if requested
		if options.serviceRestart and options.listOnly == False:
			for command in serviceCmds: os.system(command)
