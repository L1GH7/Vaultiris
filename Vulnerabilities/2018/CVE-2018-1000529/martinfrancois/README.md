# CVE-2018-1000529

NVD CVE-2018-1000529:
https://nvd.nist.gov/vuln/detail/CVE-2018-1000529

MITRE CVE-2018-1000529:
https://cve.mitre.org/cgi-bin/cvename.cgi?name=2018-1000529

# Stored XSS in Grails Fields Plugin <=2.2.7

This repository was used to demonstrate a proof of concept of the vulnerability for the responsible disclosure to the [Grails team](https://objectcomputing.com/products/grails/).

The XSS vulnerability is present in all projects using Grails v3.3.5 and below, since they all include the affected Grails Fields plugin v2.2.7 and below.
It was fixed in the Grails Fields plugin v2.2.8, which is being used in Grails v3.3.6.

**All projects using Grails v3.3.5 or lower are recommended to upgrade to at least v3.3.6 or to define the version of the Grails Fields plugin dependency in `build.gradle` to be v2.2.8 or higher.**

Since the Grails Fields plugin is widely used in Grails Scaffolding, all scaffolded applications using Grails v3.3.5 and below are potentially vulnerable.
Even though Grails has [good XSS countermeasures](https://docs.grails.org/latest/guide/security.html#xssPrevention) in place by default, any domain objects being displayed by tags of the Grails Fields plugin containing HTML or JavaScript code are not being encoded or validated and are getting executed by the browser.

## Usage

Clone this project and then start the application locally by running:
### Windows
In a command prompt / powershell:
```
grailsw.bat run-app
```
### Mac / Linux
In a terminal window:
```
./grailsw run-app
```

### Running the PoC
When the Grails application is fully initialized, it will display:
```
Grails application running at http://localhost:8080 in environment: development
```

1. The URL should automatically open in the default browser, else open your browser and manually enter the URL.  
2. Click on the link `myapp.TestController`  
3. When the next page has loaded, a JavaScript alert dialog with the text `XSS` will open.  

The domain object `Test1` is being created during initialization and is defined in the `grails-app/init/myapp/BootStrap.groovy` file.  
The vulnerability is not only present when the object is being created during BootStrap, but also when being created in the scaffolded GUI by user input.  
This can be tested by removing the `Test1` object and adding a new `Test` object containing the name: `Test1<script>alert('XSS');</script>`  

## Timeline
- **22nd of May 2018**: Discovery and responsible disclosure of the vulnerability by [@martinfrancois](https://github.com/martinfrancois)
- **24th of May 2018**: Acknowledgement of the vulnerability and submission of [CVE request](https://docs.google.com/spreadsheets/d/1PlDOsZ4Q36JU4Dz9zyBB2F3814dScppCRCe1muCT7JI/edit#gid=404258366&range=A210)
- **24th of May 2018**: [Pull request](https://github.com/grails-fields-plugin/grails-fields/pull/277) with fix for the vulnerability for Grails v3.x merged into [grails-fields-plugin](https://github.com/grails-fields-plugin/grails-fields)
- **24th of May 2018**: [Release](https://github.com/grails-fields-plugin/grails-fields/releases/tag/v2.2.8) of Grails Fields Plugin v2.2.8 for Grails v3.x
- **25th of May 2018**: [Pull request](https://github.com/grails-fields-plugin/grails-fields/pull/279) with fix for the vulnerability for Grails v2.x merged into [grails-fields-plugin](https://github.com/grails-fields-plugin/grails-fields)
- **25th of May 2018**: [Release](https://github.com/grails-fields-plugin/grails-fields/commit/011f85e1c599577f88ad96156f6739397cb5c8fa) of Grails Fields Plugin v1.6 for Grails v2.x
- **15th of June 2018**: [Release](https://github.com/grails/grails-core/releases/tag/v3.3.6) of Grails v3.3.6, including the updated dependency of the fixed Grails Fields plugin v2.2.8
- **22nd of June 2018**: [CVE-2018-1000529](https://cve.mitre.org/cgi-bin/cvename.cgi?name=2018-1000529) assigned
- **26th of June 2018**: [CVE-2018-1000529](https://cve.mitre.org/cgi-bin/cvename.cgi?name=2018-1000529) published

Thanks a lot to the Grails team for quickly resolving this vulnerability, including:  
- [@sdelamo](https://github.com/sdelamo)
- [@sbglasius](https://github.com/sbglasius)
- [@ilopmar](https://github.com/ilopmar)
