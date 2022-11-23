# CVE-2021-27850 Exploit #

## Overview ##

CVE-2021-27850 is a critical unauthenticated remote code execution vulnerability that was found in all recent versions of Apache Tapestry, by downloading the AppModule.class file we can leak the HMAC Secret key used to sign all the serialized objects in apache Tapestry.

We encountered this CVE in a real life assessment (CTF) and as far as I know there are no public exploits available on how Tapestry signs the serialized objects so we decided to publish the following POC that we have used after digging in apache Tapestry source code for a long time x) .


## Usage ##

1- Clone this repo

2- Run the following command 

```sh
javac -classpath commons-codec-1.15/commons-codec-1.15.jar:. Exploit.java
```

3- Finally run the following:

```sh
java -cp commons-codec-1.15/commons-codec-1.15.jar:. Exploit [Tapestry Key] [Ysoserial Payload] [Command To Execute]
```

Where [Tapestry Key] is the Hmac key leaked from the AppModule.class , [Ysoserial Payload] is the payload you want to use from ysoserial and [Command To Execute] the command you want to execute.

![IMG](https://imgur.com/Je8bWC9.png)

**Note:** Unlike the usual Java deserialization exploits where the commands you run are limited ( no pipes or special chars .. ) you can use here any complex command you want since we are appending the following before executing the command ``` sh -c $@|sh . echo ``` .

**References:**

http://cve.mitre.org/cgi-bin/cvename.cgi?name=2021-27850

https://github.com/apache/tapestry-5
