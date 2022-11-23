> Other language [git](https://notabug.org/Jul10l1r4/Identificador-CVE-2018-11759) - pt-BR
# Check if your instances  are expose the CVE 2018-11759
The archive `main` are a script in bash for exploiting.
> The urls shall use the protocol and complete addres, example: https://test.site.com.br

[![asciicast](https://asciinema.org/a/222760.svg)](https://asciinema.org/a/222760)

For more urls in one consult, can be used the here-document, example:
```bash
# Call the bash, no request permission for exec
bash main << EOF
https://vulpagepagevul.com
http://pamonhaemandioca.gov.br
https://youtube.com
# ...
EOF
```
If no want use the here-document, he can be open with `bash main`

# Audit
To facilitate the audit report a file is created in `files_cap/` with name of target.data that have all details of target load balancer, with internal address, ports, timestemp, distros, routes, etc.

# License
This exploit is free, GNU GPL version 3

<h3 align="center">This project created by @jul10l1r4 are part of Segmentation fault</h3>
<p align="center">
		<img src="https://jul10l1r4.github.io/assets/segmentation-fault.png" alt="Segmentation fault">
</p>
