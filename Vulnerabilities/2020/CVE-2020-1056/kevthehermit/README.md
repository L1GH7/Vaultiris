# CVE-2020-10560
CVE-2020-10560 OSSN Arbitrary File Read

For details on how to use this repository refer to https://techanarchy.net/blog/cve-2020-10560-ossn-arbitrary-file-read

#### Starting
`docker-compose up --build`

#### Installing

Once the images are running, you can access the install page at 127.0.0.1 or 10.2.0.101

If you want to use BURP do not install on 127.0.0.1 as you will have issues with URLS redirecting. 

At the installaion page fill in all the details. You can read or edit the compose file for creds. 

- DB: ossn
- username: ossn
- password: ossn
- host: mysqlserver

#### Site key

Read the blog post!


#### POC

There is a PHP and a python script again refer to the blog post for deatils on how to use it. 