# spring-rce-poc
Testing CVE-2022-22968  
Simple app vulnerable to CVE-2022-22968
- `Dockerfile` could be used to build it on vulnerable version of `Tomcat (9.0.59)`  
- `exploit.sh` is a shell script which is trying to exploit this cve on 8080 port of localhost  

If attack with `exploit.sh` was succesfull, on context of `http://localhost:8080/shell.jsp` on a target app should be accessible  
simple JSP website with basic webshell.

