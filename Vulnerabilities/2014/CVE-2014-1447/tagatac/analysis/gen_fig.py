#!/usr/bin/env python
import csv, sys
import matplotlib.pyplot as plt
BASELINE = 1.1143858

if len(sys.argv) < 2:
	print "Usage: gen_fig.py <results filename> [<microbench column>] [<exploit measure column>]"
	sys.exit()
filename = sys.argv[1]
if len(sys.argv) > 2: microbench_column = sys.argv[2]
else: microbench_column = -1
if len(sys.argv) > 3: exploit_column = sys.argv[3]
else: exploit_column = 3

microbench = list()
exploit = list()

with open(filename, 'r') as csvfile:
	csvreader = csv.reader(csvfile)
	for row in csvreader:
		microbench.append(float(row[microbench_column]))
		exploit.append(float(row[exploit_column]))

plt.scatter(microbench, exploit)
xmin = min(microbench)
xmax = max(microbench)
xborder = (xmax-xmin) * 0.05
ymin = min(exploit)
ymax = max(exploit)
yborder = (ymax-ymin) * 0.05
plt.semilogy()
plt.axis((xmin-xborder, xmax+xborder, ymin/2, ymax*2))
plt.axhline(y=BASELINE, color='r')
plt.title('Post-synchronization NOP Injection\n(Libvirt CVE-2014-1447)')
plt.xlabel(r'Microbenchmark ($\mu s$)')
plt.ylabel('Cost to Exploit (seconds)')
plt.savefig(filename[:-3] + 'pdf', bbox_inches='tight')
