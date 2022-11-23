# CobaltSpam
Tool based on [CobaltStrikeParser](https://github.com/Sentinel-One/CobaltStrikeParser) from SentinelOne which can be used to DoS a CobaltStrike TeamServer (4.2 or 4.3) leveraging CVE-2021-36798 (HotCobalt) discovered by SentinelOne

![alt text](https://github.com/hariomenkel/CobaltSploit/raw/main/CS.PNG)

## Description
Use `exploit.py` to start spamming a server with malicious tasks

## Usage
```
usage: exploit.py [-h] [-u URL | -f FILE]

optional arguments:
ptional arguments:
  -h, --help            show this help message and exit
  -u URL, --url URL     Target a single URL
  -f FILE, --file FILE  Read targets from text file - One CS server per line
  --print_config PRINT_CONFIG
                        Print the beacon config
  --use_tor USE_TOR     Should tor be used to connect to target?
  --publish_to_threatfox PUBLISH_TO_THREATFOX
                        Publish your findings to ThreatFox
  --parse_only PARSE_ONLY
                        Only download beacon and parse it without spamming
  --max_hits MAX_HITS   Send maximum amount of exploit attempts (0 for endless) Default is 200
```

## Note
You might want to use a tool like [TorghostNG](https://github.com/GitHackTools/TorghostNG) on your VM to hide your real IP or use [Whonix](https://www.whonix.org/)

# Prerequisites
Please install Tor before using this script and make sure it is running and listening on Port 9050

Afterwards install the following package:<BR>
<BR>
`pip install PySocks`<BR>
`pip install stem`<BR>
`pip install requests`<BR>
<BR>  
Please follow these steps to make sure this script is able to change the TOR IP programmatically<BR>
<BR>
`$ tor --hash-password MyStr0n9P#D`<BR>
`16:160103B8D7BA7CFA605C9E99E5BB515D9AE71D33B3D01CE0E7747AD0DC`<BR>
<BR>
Add this value to `/etc/torrc` (Path may vary depending on our distribution) for the value `HashedControlPassword` so it reads<BR>
`HashedControlPassword 16:160103B8D7BA7CFA605C9E99E5BB515D9AE71D33B3D01CE0E7747AD0DC`<BR>
<BR>
Afterwards uncomment the line<BR>
`ControlPort 9051`<BR>
Restart your tor service:
<BR>
`$ sudo service tor restart`
<BR>
Finally add your hash-password (In this example MyStr0n9P#D) to spam_utils.py as "tor_password"

## Disclaimer
While this should be clear, this tool should be used only against infrastructure you own. Don't mess with systems you don't own! 


