#!/usr/bin/env python

import os, shutil

builddir = os.path.join(os.getenv('HOME'), 'rpmbuild')
try: os.remove(os.path.join(builddir, 'INJECT/genpatch.pyc'))
except: None
import genpatch

def patchfile(patchnum):
	return 'sleep' + str(patchnum).zfill(3) + '.patch'

os.chdir(builddir)
patchnum = 1
while os.path.exists(os.path.join('SOURCES', patchfile(patchnum))):
	patchnum += 1
shutil.copy('INJECT/sleep.patch', os.path.join('SOURCES', patchfile(patchnum)))
print patchfile(patchnum) + ' copied'
os.chdir('SPECS')
f = open('kernel-2.4.spec', 'r')
lines = f.readlines()
f.close()
f = open('kernel-2.4.spec', 'w')
for line in lines:
	if line.split()[:-1] == ['%define', 'release']: f.write('%define release 50.EL.hmc_source_auto' + str(patchnum).zfill(3) + '\n')
	elif line.split()[:-1] == ['Patch40000:']: f.write('Patch40000: ' + patchfile(patchnum) + '\n')
	else: f.write(line)
f.close()
print 'spec file updated'
os.system('rpmbuild -bb --target=`uname -m` kernel-2.4.spec 2> build-err' + str(patchnum).zfill(3) + '.log | tee build-out' + str(patchnum).zfill(3) + '.log')
