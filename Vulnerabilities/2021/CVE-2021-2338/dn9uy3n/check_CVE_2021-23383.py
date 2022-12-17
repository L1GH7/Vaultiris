import requests,sys
from bs4 import BeautifulSoup
import re

regex = r"handlebars(.*)"
base_url = sys.argv[1]

r = requests.get(base_url)

soup = BeautifulSoup(r.text, 'html.parser')

script_tag = soup.findAll("script")
for i in range(len(script_tag)):
    source = script_tag[i].get('src')
    hander=re.search(regex,source, re.M|re.I)
    if (hander):
        r = requests.get(base_url+source)
        content = r.text
        matchObj = re.search(regex,content, re.M|re.I)
        version = matchObj.group(1)
        version = version.strip()[1::]
        version_arr = version.split(".")
        if(int(version_arr[0],10)<4):
            print("waiting minute")
            print("...")
            print("...")
            print("...")
            print("...")
            exit("CVE 2021-23383 detected on website "+base_url)
        elif (int(version_arr[1],10)<7):
            print("waiting minute")
            print("...")
            print("...")
            print("...")
            print("...")
            exit("CVE 2021-23383 detected on website "+base_url)
        elif (int(version_arr[2],10)<7):
            print("waiting minute")
            print("...")
            print("...")
            print("...")
            print("...")
            exit("CVE 2021-23383 detected on website "+base_url)
        print("waiting minute")
        print("...")
        print("...")
        print("...")
        print("...")
        exit("NO CVE 2021-23383")           
