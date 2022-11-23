# CVE-2021-32157
Make it possible to build a vulnerable webmin virtual environment as a container using docker

## Build 
If there is an error related to download, please comment on line 15 of dockerfile and run line 14 of dockerfile.
<pre>
 docker build -t VulnWebmin .
 docker run --name VulnWebmin -d --rm -p 10000:10000 VulnWebmin
</pre>

## Login 

<pre>
Go to http://[SERVER_IP]:10000
ID/Passsword : root/123456
</pre>

## Exploit
To use exploit script, Go to [Link](https://github.com/Mesh3l911/CVE-2021-32157/blob/main/eXploit.py)

<pre>
python3 eXploit.py
</pre>

## reference 
https://github.com/Mesh3l911/CVE-2021-32157/blob/main/eXploit.py
