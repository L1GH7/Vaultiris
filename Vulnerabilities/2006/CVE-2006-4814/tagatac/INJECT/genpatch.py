#!/usr/bin/env python

import os, sys, random, shutil

# Constants
KERNELROOT = 'BUILD/kernel-2.4.21/linux-2.4.21'
WALKPATH = 'arch/i386/kernel'
INCLUDELINE = '#include <asm/delay.h>\n'
SLEEPDURATION = '4000' #microseconds
NUMSLEEPS = 10
INJECTDIR = 'INJECT'
CLEANDIR = 'linux-10000'
MODDIR = 'linux-40000'

os.chdir(os.path.join(os.getenv('HOME'), 'rpmbuild'))
cleandirroot = os.path.join(INJECTDIR, CLEANDIR)
moddirroot = os.path.join(INJECTDIR, MODDIR)
if os.path.isdir(cleandirroot): shutil.rmtree(cleandirroot)
if os.path.isdir(moddirroot): shutil.rmtree(moddirroot)

# checkout the clean source
os.system('git checkout BUILD')

def pathwalk(inject=False):
	filecount = 0
	cfilecount = 0
	linecount = 0
	validlinecount = 0
	# traverse the source tree
	for root, dirs, files in os.walk(os.path.join(KERNELROOT, WALKPATH)):
		for name in files:
			filecount += 1
			# check for C files
			if name.endswith('.c'):
				cfilecount += 1
				bracketcount = 0
				whichline = 0
				f = open(os.path.join(root, name), 'r')
				lines = f.readlines()
				f.close()
				# count every line of code in every C file
				for line in lines:
					whichline += 1
					# count open and close brackets to determine if we are in a valid code region
					bracketcount += line.count('{') - line.count('}')
					if bracketcount > 0 and line.endswith(';\n'):
						validlinecount += 1
						if inject and validlinecount in injection_points:
							# we've found the place where we want to inject the sleep

							# make the directories to diff
							this_cleandir = os.path.join(cleandirroot, root[len(KERNELROOT)+1:])
							this_moddir = os.path.join(moddirroot, root[len(KERNELROOT)+1:])
							if not os.path.isdir(this_cleandir): os.makedirs(this_cleandir)
							if not os.path.isdir(this_moddir): os.makedirs(this_moddir)

							# copy the unmodified file
							shutil.copy(os.path.join(root, name), this_cleandir)

							# create the file with NOP injections
							modpath = os.path.join(this_moddir, name)
							if os.path.isfile(modpath):
								f = open(modpath, 'r')
								copylines = f.readlines()
								f.close()
							else:
								copylines = lines
							f = open(modpath, 'w')
							# include delay.h
							if copylines[0] != INCLUDELINE: f.write(INCLUDELINE)
							# determine appropriate line for injection (acounting for the possibility
							# that there have been other injections in this file
							inject_index = whichline + (len(copylines) - len(lines))
							# write the lines before the injection
							for this_line in copylines[:inject_index]: f.write(this_line)
							# inject the sleep
							f.write('udelay(' + SLEEPDURATION + ');\n')
							# write the lines after the injection
							for this_line in copylines[inject_index:]: f.write(this_line)
							f.close()
				linecount += whichline
	if not inject:
		print 'filecount:', filecount
		print 'cfilecount:', cfilecount
		print 'linecount:', linecount
		print 'validlinecount:', validlinecount
	return validlinecount

# select the source injection points
injection_points = list()
vsloc = pathwalk()
for i in range(NUMSLEEPS): injection_points.append(random.randint(1, vsloc))
pathwalk(inject=True)

# generate the patch file with diff
os.chdir(INJECTDIR)
os.system('diff -urNp ' + CLEANDIR + ' ' + MODDIR + '> sleep.patch')
print 'patch generated'
