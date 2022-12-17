# CVE-2021-40845

I. VULNERABILITY
-------------------------
AlphaWeb XE - Authenticated Insecure File Upload leading to RCE


II. CVE REFERENCE
-------------------------
CVE-2021-40845


III. VENDOR
-------------------------
https://www.zenitel.com/


IV. DESCRIPTION
-------------------------

The web part of Zenitel AlphaCom XE Audio Server through 11.2.3.10, called AlphaWeb XE, does not restrict file upload in the Custom Scripts section at php/index.php. Neither the content nor extension of the uploaded files is checked, allowing execution of PHP code under the /cmd/ directory.


V. EXPLOIT
-------------------------

```
python3 CVE-2021-40845.py -u $URL -c $COMMAND
```

![poc](https://raw.githubusercontent.com/ricardojoserf/ricardojoserf.github.io/master/images/alphaweb-rce/poc.png)


VI. VULNERABILITY DETAILS
-------------------------

To exploit this vulnerability, someone must authenticate in the server and access the "Scripts" button in the "Custom scripts" tab. 

![img1](https://github.com/ricardojoserf/ricardojoserf.github.io/blob/master/images/alphaweb-rce/image1.png?raw=true)

Then, the button "Choose file" is clicked and the file is uploaded clicking "Upload". 

![img3](https://github.com/ricardojoserf/ricardojoserf.github.io/blob/master/images/alphaweb-rce/image3.png?raw=true)

The PHP test file is a simple one-line reverse shell:

![img2](https://github.com/ricardojoserf/ricardojoserf.github.io/blob/master/images/alphaweb-rce/image2.png?raw=true)

The new file, with the same name, extension and content is listed in the Scripts page:

![img4](https://github.com/ricardojoserf/ricardojoserf.github.io/blob/master/images/alphaweb-rce/image4.png?raw=true)

The path of these files is /cmd/$FILE$. Knowing the path, as there is not any restriction the file upload functionality, uploading a PHP reverse shell or cmdshell allows to get Remote Code Execution in the server:

![img5](https://github.com/ricardojoserf/ricardojoserf.github.io/blob/master/images/alphaweb-rce/image5.png?raw=true)


VII. REFERENCES
-------------------------
https://wiki.zenitel.com/wiki/AlphaWeb

https://wiki.zenitel.com/wiki/AlphaWeb_Custom_Scripts

https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-40845

