# SiteCore-RCE-Detection
For detection of sitecore RCE - CVE-2021-42237
Sitecore Experience Platform Pre-Auth RCE - CVE-2021-42237

Relies on sitecore version detection and response when a request is made to vulnerale Report.ashx via Get and Post.

The script takes a file containing list of urls in format www.url.com on each line.

Usage :
python3 check-for-sitecore-rce.py -h

python3 check-for-sitecore-rce.py -u urls.txt

May result in false positives if the web application handles ther error differently. Recommended to check pages with 200 responses.

PoCs

![3](https://user-images.githubusercontent.com/61792333/191764846-2a03beb2-5bdf-451e-9093-5e4662f941da.PNG)

![2](https://user-images.githubusercontent.com/61792333/191758706-9d6a80dd-4d14-404a-ae88-541e78e079b6.PNG)

Reference : https://blog.assetnote.io/2021/11/02/sitecore-rce/ , https://support.sitecore.com/kb?id=kb_article_view&sysparm_article=KB1000776
