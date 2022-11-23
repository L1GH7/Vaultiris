#!/usr/bin/env python

import subprocess, sys, time, os
from multiprocessing import Pool, TimeoutError
from datetime import datetime

def cleanup():
	while(True):
		try: subprocess.check_call(['pkill', '-9', 'virsh'])
		except: break
	while(True):
		try: subprocess.check_call(['pkill', '-9', 'libvirtd'])
		except: break
	try: os.remove(os.path.join(os.getenviron('HOME'), '.libvirt', 'libvirtd.pid'))
	except: None

def measure_runs():
	cleanup()
	count=0
	server_env = dict(os.environ)
	server_env['LD_PRELOAD'] = os.path.join(os.getcwd(), 'interpose.so')
	server = subprocess.Popen('server/install/sbin/libvirtd', env=server_env, stderr=open(os.devnull))
	while(not server.poll()):
		count += 1
		subprocess.Popen('./bug.sh', stderr=open(os.devnull))
	return count

total_count = 0
total_elapsed = 0
num_crashes = 0
for i in range(5):
	pool = Pool(processes=1)
	time.sleep(1)
	t1 = datetime.now()
	result = pool.apply_async(measure_runs)
	try:
		this_count = result.get(timeout=600)
		t2 = datetime.now()
		pool.terminate()
		this_elapsed = (t2 - t1).total_seconds()
		print str(this_count) + ',' + str(this_elapsed)
		num_crashes += 1
		total_count += this_count
		total_elapsed += this_elapsed
	except TimeoutError:
		pool.terminate()
		print '-1,600'

cleanup()
if num_crashes:
	print str(float(total_count)/num_crashes) + ' average runs per failure.'
	print str(total_elapsed/num_crashes) + ' seconds average time needed to crash libvirtd.'
else:
	print '-1 (No failures)'
	print '600 (No failures)'
