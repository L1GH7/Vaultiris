#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import os
import sys
import subprocess
import random

BINARY_PATH = '/home/opentftp/opentftpd'

os.chdir('/tmp')

_ = 'ps x | grep opentftpd'

cnt = len(subprocess.check_output(_, shell=True).split(b'\n'))

if cnt > 30:
    sys.stdout.write('reach connection limit. plz wait a minute...\n')
    sys.exit(1)

port = random.randint(60000, 60100)
_ = 'ps x -o command | grep {}'.format(port)
while subprocess.check_call(_, shell=True, stdout=subprocess.PIPE) != 0:
    port = random.randint(60000, 60100)

f = open('/dev/null', 'w')
subprocess.Popen([BINARY_PATH, str(port)], stdin=f, stdout=f, stderr=f)

sys.stdout.write('start challenge on udp port: {}\n'.format(port))
sys.stdout.flush()

sys.exit(0)
