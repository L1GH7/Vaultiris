# CodePath Week 7-8
CodePath Assignment for Weeks 7 &amp; 8: CVE-2017-14719, CVE-2019-9787 &amp; Unauthenticated Page/Post Content Modification via REST API

# Project 7 - WordPress Pentesting

Time spent: **16** hours spent in total

> Objective: Find, analyze, recreate, and document vulnerabilities affecting an old version of WordPress

## Pentesting Report

## 1. CVE-2017-14719
  - [ ] Summary: A path traversal vulnerability was discovered in the file unzipping code. 
    - Vulnerability types: Path Traversal
    - Tested in version: 3.8
    - Fixed in version: 3.8.22
<img src="https://github.com/PalmTreeForest/CodePath_Week_7-8/blob/master/CVE-2017-14719/DirTravPreExploit.png" width="800">
<img src="https://github.com/PalmTreeForest/CodePath_Week_7-8/blob/master/CVE-2017-14719/DirTravPreExploit2.png" width="800">
<img src="https://github.com/PalmTreeForest/CodePath_Week_7-8/blob/master/CVE-2017-14719/DirTravPostExploit.png" width="800">
<img src="https://github.com/PalmTreeForest/CodePath_Week_7-8/blob/master/CVE-2017-14719/DirTravPostExploit2.png" width="800">
<img src="https://github.com/PalmTreeForest/CodePath_Week_7-8/blob/master/CVE-2017-14719/DirTravPostExploit3.png" width="800">
<img src="https://github.com/PalmTreeForest/CodePath_Week_7-8/blob/master/CVE-2017-14719/DirTravPostExploit4.png" width="800">
  - [ ] Steps to recreate: If an attacker has managed to gain access to an Admin account, they can click on the "Plugins" button and view installed plugins. Next to each plugin listed is a "Delete" button, which when pressed, will take the attacker to a page where the Admin is able to view the contents of the directory that he/she is trying to delete before making a permanent change. From this page, the attacker can delete the name of the plugin from the URL (located directly after checked[0]= and before the '&'). The attacker replaces the plugin name with one or more " ../ " characters until they have reached the particular directory they would like to delete. 
  - [ ] Affected source code:
    - https://core.trac.wordpress.org/browser/tags/3.8/src/wp-admin/plugins.php

#######################################################################################

## 2. CVE-2019-9787
  - [ ] Summary: 
    - Vulnerability types: Authenticated XSS in Comment Field
    - Tested in version: 4.6.1
    - Fixed in version: 4.6.14
    <img src="https://github.com/PalmTreeForest/CodePath_Week_7-8/blob/master/CVE-2019-9787%20XSS/CVE-2019-9787_Pre-Exploit.png" width="800">
    <img src="https://github.com/PalmTreeForest/CodePath_Week_7-8/blob/master/CVE-2019-9787%20XSS/CVE-2019-9787_Post-Exploit.png" width="800">
  - [ ] Steps to recreate: In the Reply to Post field, type /*<script>alert('text you want to display');</script>*/ but, without /* and */
  - [ ] Affected source code:
    - https://core.trac.wordpress.org/browser/tags/4.6.1/src/wp-comments-post.php

########################################################################################

## 3. Unauthenticated Page/Post Content Modification via REST API
  - [ ] Summary: 
    - Vulnerability types: Unauthenticated Page/Post Content Modification via REST API
    - Tested in version: 4.7
    - Fixed in version: 4.7.2
    <img src="https://github.com/PalmTreeForest/CodePath_Week_7-8/blob/master/REST%20API%20Vuln/REST_API_Vuln_WP_Pre-Exploit.png" width="800">
    <img src="https://github.com/PalmTreeForest/CodePath_Week_7-8/blob/master/REST%20API%20Vuln/REST_API_Vuln_Exploit_Part_1_Discover_Endpoints.png" width="800">
    <img src="https://github.com/PalmTreeForest/CodePath_Week_7-8/blob/master/REST%20API%20Vuln/REST_API_Vuln_Content_to_Inject.png" width="800">
    <img src="https://github.com/PalmTreeForest/CodePath_Week_7-8/blob/master/REST%20API%20Vuln/REST_API_Vuln_Exploit_Part_2_Content_Injected.png" width="800">
    <img src="https://github.com/PalmTreeForest/CodePath_Week_7-8/blob/master/REST%20API%20Vuln/REST_API_Vuln_WP_Post-Exploit.png" width="800">
  - [ ] Steps to recreate: Copy exploit code that the very nice hacker called leonjza posted to exploit-db.com (https://www.exploit-db.com/exploits/41223). Save the copied code into a .py file. At the command line, change to the directory where you saved your exploit and run 'python [exploit_file_name].py http://wpdistillery.vm'. I then created a text file with the content I wanted to inject into the WordPress site. I injected this content by running 'python [exploit_file_name].py http://wpdistillery.vm [Post ID] [path to content being injected].txt'. I then went back to my browser and reloaded the page, and saw that my content injection was successful.
  - [ ] Affected source code:
    - https://core.trac.wordpress.org/browser/tags/4.7/src/wp-includes/rest-api/endpoints/class-wp-rest-attachments-controller.php


## Assets

I used python exploit code from this url: https://www.exploit-db.com/exploits/41223

## Resources

- [WordPress Source Browser](https://core.trac.wordpress.org/browser/)
- [WordPress Developer Reference](https://developer.wordpress.org/reference/)


## Notes

While working on the 'Unauthenticated Page/Post Content Modification via REST API' I encountered an import error for the python file due to not having the lxml package installed. 
After installing lxml, I tried to run 'python [exploit_file_name].py http://wpdistillery.vm' again. This is where I ran into my second problem: I received a UnicodeEncodeError. I solved this problem by deleting extraneous posts which contained the ordinal that was not in range. 
<img src="https://github.com/PalmTreeForest/CodePath_Week_7-8/blob/master/REST%20API%20Vuln/REST_API_Vuln_Second_Problem.png" width="800">
<img src="https://github.com/PalmTreeForest/CodePath_Week_7-8/blob/master/REST%20API%20Vuln/REST_API_Vuln_Second_Problem_Solution.png" width="800">

I made a new post with plain text content and ran the script again. It executed successfully, discovering the API endpoints and getting the available post id, title, and link.

## License

    Copyright [yyyy] [name of copyright owner]

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
