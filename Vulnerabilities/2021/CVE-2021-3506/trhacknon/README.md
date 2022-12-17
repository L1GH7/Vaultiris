## CVE-2021-36356  and CVE-2021-35064  PoC


Usage: 


```bash
   ______     _______     ____   ___ ____  _      _________   ___   __   _  _   
  / ___\ \   / / ____|   |___ \ / _ \___ \/ |    |___ / ___| / _ \ / /_ | || |  
 | |    \ \ / /|  _| _____ __) | | | |__) | |_____ |_ \___ \| | | | '_ \| || |_ 
 | |___  \ V / | |__|_____/ __/| |_| / __/| |_____|__) |__) | |_| | (_) |__   _|
  \____|  \_/  |_____|   |_____|\___/_____|_|    |____/____/ \___/ \___/   |_|  
                                                                                
		         Coded By Valentin Lobstein

usage: CVE-2021-35064.py [-h] [-i I] [-f F]

Example : python3 CVE-2021-35064.py -i 127.0.0.1

options:
  -h, --help  show this help message and exit
  -i I        IP address (not url)
  -f F        IP file
  
```
  
  
### Zoomeye CLI Dork:

```bash

zoomeye search '"Welcome to VIA Collaboration Hub"'  -num 780  -filter=ip,port

```

### Shodan CLI Dork:

```bash

shodan  search 'http.html:"Welcome to VIA Collaboration Hub"' --fields=ip_str,port --separator ":" --limit 1000 | grep ''

```
