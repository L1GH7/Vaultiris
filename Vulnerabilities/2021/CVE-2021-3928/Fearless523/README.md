# CVE-2021-39287-Stored-XSS
CVE-2021-39287 Stored XSS in TastyIgniter v3.0.7 Restaurtant CMS

A stored Cross Site Scripting (XSS) vulnerability exists within TastyIgniter v3.0.7 Restaurant CMS. The description input field, located in /admin/locations/settings is not properly sanitized, and allows arbitrary code to be stored within the site.

## Affected Component
### URL: /admin/locations/settings

### POC 
- Scroll down to "Descriptions"
- Insert any random sentence, and any arbitraty code: "<script>alert("All Your Cookie are Belong to Us")</script>"
- Save the page, and a pop up will appear with the sentence All Your Cookie are Belong to Us
- Refresh, and it will remain.

![StoredXSSTastyIgniter](https://user-images.githubusercontent.com/56332039/129657178-853aa251-d4d3-4dfb-8769-b2990b192b88.gif)

## Discovered by
Mike Padrick (Fearless) August 16th, 2021
