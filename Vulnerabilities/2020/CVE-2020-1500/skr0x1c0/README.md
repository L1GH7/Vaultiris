# Summary

The URL validation logic applied when handling `/appsuite/api/oxodocumentfilter&action=addfile` suffers from three defects which can be used to execute Time of Check Time of Use (ToCToU) SSRF attack. This issue allows malicious actors to execute HTTP GET requests on internal network services and may lead to protected information leak.

## Defect #1: Time of Check Time of Use (ToCToU) vulnerability

URL validation logic in `URITools.getFinalURL` method implemented [here](https://gitlab.open-xchange.com/middleware/core/-/blob/develop/com.openexchange.server/src/com/openexchange/tools/net/URITools.java#L108) suffers from ToCToU vulnerability  

```java
URL u = new URL(url);
if (validator.isPresent()) {
    Optional<OXException> exception = validator.get().apply(u);
    if (exception.isPresent()) {
        throw exception.get();
    }
}

URLConnection urlConnnection = u.openConnection();
```

In the above code, DNS resolution of input URL hostname is done during both time of check (`validator.get().apply(u)`) and time of use (`u.openConnection()`) by calling `InetAddress.getByName` method. So if an attacker can change the DNS resolution result after time of check and before time of use, he can bypass the validator. This defect alone is extremely difficult to exploit because of following limitations

1. By default, `InetAddress.getByName` caches the DNS resolution results for 30 seconds if the DNS resolution succeeds and for 10 seconds if the DNS resolution fails. 
2. Time interval between `validator.get().apply(u)` and `u.openConnection()` is extremely small (less than one millisecond for normal URLs). Defect #2 is used to bypass this limitation

## Defect #2: Length of input URL is not limited

The URL validator `AddFileAction.validator` implemented [here](https://gitlab.open-xchange.com/documents/office/-/blob/develop/com.openexchange.office.rest/src/com/openexchange/office/rest/AddFileAction.java#L353) does not validate the length of input URL. By providing a URL with large size, the attacker can increase the time interval between `InetAddress.getByName` method calls at time of check and time of use. Larger time interval allows the attacker to easily exploit Defect #1

## Defect #3: URL validator allows input URL if its dns resolution fails

`HostList.contains` method implemented [here](https://gitlab.open-xchange.com/middleware/core/-/blob/develop/com.openexchange.net/src/com/openexchange/net/HostList.java#L234) returns false if the DNS resolution of input URL hostname fails

```java
// Need to resolve as last resort
try {
    return contains(InetAddress.getByName(toCheck), false);
} catch (UnknownHostException e) {
    // Cannot be resolved
    return false;
}
```

The `AddFileAction.validator` calls `blackImageUrlHostlist.contains` method to check if the input URL hostname or the IP address it resolves to is in blacklist. If an attacker makes the DNS resolution of hostname to fail, the IP address checks are skipped and URL is accepted.

# Attack strategy

Consider a hostname `randomid.dns.pointer.pw` with authoritative nameserver controlled by attacker. Attack strategy is to make the DNS resolution of `randomid.dns.pointer.pw` to fail at time of check (`validator.get().apply(u);`) and to succeed at time of use (`u.openConnection()`). This allows the attacker to bypass IP address blacklist filters and execute HTTP GET request on any blacklisted IP address. Following steps may be followed by attacker to execute this attack

1. Trigger the `InetAddress.getByName` cache on server by executing `addfile` action with request data `{"add_imageurl": "http://randomid.dns.pointer.pw"}`. At this time, the authoritative DNS server will be set to fail the DNS resolution of `randomid.dns.pointer.pw` by returning `SERVFAIL` status. `InetAddress.getByName` will cache this failure result for 10 seconds
2. Note the time when above request response was received. This is the cache start time
3. Tell the authoritative name server of domain `pointer.pw` to return 127.0.0.1 for type A dns requests of hostname `randomid.dns.pointer.pw`
4. Sleep for specified duration from cache start time such that in the next step `InetAddress.getByName` will return cached response (SERVFAIL) at time of check and will return 127.0.0.1 at time of use (url.openConnection())
5. Execute SSRF request with `add_imageurl` value set to format `http://u{25MB}:password@randomid.dns.pointer.pw/path/to/internal/resource.png`. Here u{25MB} means the username part is a string of length 25 x 1024 x 1024 charachters

# Steps to reproduce

1. Install golang from https://golang.org/dl/
2. Install Open-Xchange and Documents in a virtual machine with atleast 4GB RAM by following guides https://oxpedia.org/wiki/index.php?title=AppSuite:Open-Xchange_Installation_Guide_for_Debian_9.0 and https://oxpedia.org/wiki/index.php?title=AppSuite:Documents_Installation_Guide#Debian_GNU.2FLinux_9.0_.28valid_from_v7.10.29
3. Download and extract poc.zip file
4. Open terminal / command line and set current directory to extracted poc.zip folder
5. Run command
   
   ```shell
    go run . -serverRoot="http://172.16.66.130" -username="testuser" -password="secret" -targetPath="appsuite/v=7.10.3-9.20200409.083030/apps/themes/logo.png" -targetPort="80" -payloadSize=25 -startSleepDuration=6600
   ```
   
   where `172.16.66.130` is the IP address of VM
