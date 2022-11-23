#!/usr/bin/python
import os, subprocess, sys, time, datetime

def ccs_check(host):
    checkresult = ""
    ccscommand = "perl fake-client-early-ccs.pl " + host 
    ccsrun = os.popen(ccscommand)
    result = ccsrun.readlines()
    for line in result:
        if line.startswith("FAIL"):
            checkresult = "VULNERABLE"
            print "VULNERABLE"
        else:
            checkresult = "not VULNERABLE"
    return checkresult

def check_host(host):
    checkcommand = "nmap -p80,443,8000,8080,8081,8443 " + host +" > hostportcheckresults.txt"
    os.system(checkcommand)
    if "open" in open('hostportcheckresults.txt').read():
        return "check"
    else:
        return "nocheck"


if __name__ == '__main__':
        completedhosts = open('completed-hosts-'+sys.argv[1], "a")
        start = time.time()
        hostlist = []
        for line in open(sys.argv[1]):
                hostlist.append(line.strip())
        hostlist = set(hostlist)
        print "Unique hosts: " + str(len(hostlist))
        print "\n"
        for host in hostlist:
                results = open('local-results-'+sys.argv[1], "a")
                now = datetime.datetime.now().strftime("%m-%d-%Y-%H-%M")
                completedhosts = open('completed-hosts'+sys.argv[1], "a")
                print "Checking " + host
                if check_host(host)=="check":
                        print "Ports open, doing ccs check...\n"
                        ccs = ccs_check(host)
                        results.write(host+","+ccs+","+now+","+"aws-assets"+"\n")
                        completedhosts.write(host+"\n")
                        completedhosts.close()
                        results.close()
                else:
                     	print "Chosen ports not open, skipping " + host + "\n"
                        completedhosts.write(host+"\n")
                        completedhosts.close()
                #insert(iplist, sitevulnlist, nmapvulnlist, bleedvulnlist, departmentlist, timelist)
                #conn.commit()
        print "Elapsed Time: %s" % (time.time() - start)