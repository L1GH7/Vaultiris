Vendor: perfSONAR<br>
Link: https://github.com/perfsonar/<br>
Affected Versions: v4.x <= v4.4.5<br>
Vulnerability Type: Partial Blind CSRF<br>
Discovered by: Ryan Moore<br>
CVE: CVE-2022-41413

# Summary

A partial blind CSRF vulnerability exists in perfSONAR v4.x <= v4.4.5 within the /perfsonar-graphs/ test results page.  Parameters and values can be injected/passed via the URL parameter, forcing the client to connect unknowingly in the background to other sites via transparent XMLHTTPRequests.  This partial blind CSRF bypasses the built-in whitelisting function in perfSONAR.<br><br>

This vulnerability was patched in perfSONAR v4.4.6.<br>

# Proof of Concept 

## Examples

Here are two examples of this vulnerability.  For further details, review the Technical Overview section below.

### Example 1:

Client browser connects to www.google.com in the background.<br>
http://192.168.68.145/perfsonar-graphs/?source=1&dest=2&url=https://www.google.com 

### Example 2:

Client browser connects to arbitrary IP and port in the background, passing delete parameter to /api endpoint.<br>
http://192.168.68.145/perfsonar-graphs/?source=8.8.8.8&dest=%26action%3Ddelete&url=http://192.168.68.113:4444/api 

# Technical Overview

In these examples, the graphs.json whitelist was enabled by uncommenting url_whitelist.  By uncommenting these, it enables whitelisting and perfSONAR should no longer be allowed to connect to sites not found in the whitelist.  But unfortunately that was not the case.

![This is an image](https://github.com/renmizo/CVE-2022-41413/blob/main/screenshot1.jpg)

## Example 1:

With the whitelist enabled and navigating to the following URL, the site returns “URL is not whitelisted” output to the screen as intended.  This is to prevent you from connecting to unwanted sites.<br><br>

http://192.168.68.145/perfsonar-graphs/?source=1&dest=2&url=https://www.google.com 

![This is an image](https://github.com/renmizo/CVE-2022-41413/blob/main/screenshot2.jpg)

But, if you observe via the browser developer console (or via a web proxy), there is a Jquery module that forces the client to make transparent requests in the background to whatever URL was specified.

Open your web browser, right click, Inspect, and go to Network.  Now with Inspect open, navigate to the same URL and observe.  In the background, you will see requests from the client to the URL that was specified, in this case www.google.com.<br><br>

http://192.168.68.145/perfsonar-graphs/?source=1&dest=2&url=https://www.google.com 

![This is an image](https://github.com/renmizo/CVE-2022-41413/blob/main/screenshot3.jpg)

To compound this issue, it was discovered that URL encoded parameters could be injected into the dest parameter value and passed to the XMLHTTPRequests.  This allows us to inject parameters into the GET request URI and send those as a CSRF attack to the URL of our choosing (transparently in the background).

For example purposes, here we will use the https://www.google.com/search URL and inject the q=csrf parameter and value, which tells Google what to search for.  We open the following URL from our browser with Inspect open, to observe the transparent connection and search to Google.<br><br>

http://192.168.68.145/perfsonar-graphs/?source=1&dest=s%3Dcsrf2&url=https://www.google.com 

![This is an image](https://github.com/renmizo/CVE-2022-41413/blob/main/screenshot4.jpg)

Fortunately Google and CORS prevents the query from working, but the site still connects to Google (or any site specified) without the user knowing, transparently in the background.  Many sites have CSRF protections, but some do not.  Now I will start a web server on another host and demonstrate how the CSRF attack might be used to attack other sites.

## Example 2:

Here we are using another internal web server for testing.  We simulate an API with a delete function.  While the web server is listening on another internal machine, we open the following URL in the victim’s browser.<br><br>

http://192.168.68.145/perfsonar-graphs/?source=8.8.8.8&dest=%26action%3Ddelete&url=http://192.168.68.113:4444/api 

Here we observe the web server output, you can see the requests coming from the client.  Notice the third line down, you can see the parameter injection worked correctly, we have sent the action=delete parameter to the /api endpoint on the designated server. (In this case fictitious for demonstration purposes).

![This is an image](https://github.com/renmizo/CVE-2022-41413/blob/main/screenshot5.jpg)

This CSRF attack is unauthenticated by default, unrestricted, and the existing whitelist mechanism does not protect against it.

# Remediation

Update perfSONAR to v4.4.6 or newer.

# References

* https://lists.internet2.edu/sympa/arc/perfsonar-user/2022-11/msg00008.html
* https://www.perfsonar.net/releasenotes-2022-11-09-4-4-6.html
* https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2022-41413


