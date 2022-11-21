# Exploit-For-CVE-2022-36067
This repo contains payload for the CVE-2022-36067

## How to exploit?
Paste the payload given in payload.js along with the command you want to run.

## Test on local machine.
To test on local machine, git clone the repo, install the dependecies and run app.js using node. 

#### If the application is vulnerable, the code will get executed successfully. If not, it will throw an error which would like this:
<code>TypeError: 
Cannot read properties of undefined (reading 'mainModule')
(Use `node --trace-uncaught ...` to show where the exception was thrown)</code>

