# CVE 2022-34305
Did you heard something about CVE-2022-34305?

Are you trying to hunting this CVE on the wild?

I've bad news: It's authenticated.

![image](https://user-images.githubusercontent.com/12475057/175760295-fa46f734-53f9-4514-b7b6-d8be34f87ee6.png)

The ilustration above was grabbed from [here](https://cwiki.apache.org/confluence/display/DIRxSRVx10/4.2.+Apache+Tomcat)

Maybe you can have some luck trying to authenticating with the following default logins bellow:

```
<user username="tomcat" password="tomcat" roles="tomcat" />
<user username="both" password="tomcat" roles="tomcat,role1" />
<user username="role1" password="tomcat" roles="role1" />
```

## Fixing commit

The commit from Apache fixing this vulnerability can be found [here](https://github.com/apache/tomcat/commit/d6251d1cfb683f1bdd00ed022ac8e9b9a7e7792c)
