# Universal Cross Site Scripting PoC

This is a PoC for CVE-2015-0072 for sequentialy get the targeted websites cookies.

## Disclaimer

This Proof of Concept is for educational purpose only. Please do not use it against any system without prior permission. You are responsible for yourself for what you do with this code.

## Improvement

In order for the exploit to work, the javascript injection inside the first frame location must occur after the second frame redirect. The first solution, proposed in the other PoC, deployed sleeps and timeouts. However, if the server syncronize the redirect and sleep requestes, one can exploit the vulnerability without sleeps.

Actually there is a little setTimeout, but 500 ms is big improvement from the previous 5000! Note: this code is a PoC, it was never tested outside my test environment.

## Usage

    npm install
    node app.js

For basic logging launch

    node app.js > cookies.txt

## Configuration

In `conf.json`, set `host` to the value of your public host and `targets` to
the sites to retrieve the cookies.

Note that targets must not set the HTTP header `x-frame-options`.

## References

* [Original PoC](http://www.deusen.co.uk/items/insider3show.3362009741042107/)
* [Evolved PoC](http://packetstormsecurity.com/files/130308/Microsoft-Internet-Explorer-Universal-XSS-Proof-Of-Concept.html)
* [Defence analysis](http://sijmen.ruwhof.net/weblog/427-mitigations-against-critical-universal-cross-site-scripting-vulnerability-in-fully-patched-internet-explorer-10-and-11)
