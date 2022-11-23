# ServiceCheater
Poc of CVE-2020-0113 &amp; CVE-2020-0108
# Android前台服务权限提升漏洞分析
## 漏洞背景
在AOSP的2020-08补丁中，披露了一个框架层AMS中的漏洞，编号为CVE-2020-0108，评级为高，另一个编号为CVE-2020-0313，评级为中。AMS中对前台服务的处理中逻辑漏洞，成功利用该漏洞的攻击者可以绕过前台服务的通知显示并持续在后台运行。攻击需要由本地的恶意应用发起，不需要用户交互，如果用户授予了应用其它权限，可造成更大的危害，例如在持续追踪位置或静默录音等。

前台服务是Google在Android 8.0中引入的概念，由于Android 8.0不允许后台启动后台服务，所以设计了前台服务的概念，前台服务优先级较高，可以长时间在后台运行，但是前台服务在启动后5秒必须绑定一个通知，否则会被杀死。实际上前台服务依旧是在“后台”运行的，只是由于绑定了用户可视的一个通知，所以Google称之为“前台服务”。
## 漏洞详情及利用
- CVE-2020-0113

在NotificationManagerService中的onNotificationError方法，没有正确处理通知显示中的异常情况。由于该方法中只是调用了cancelNotification方法来取消通知，而没有去终止服务或终止整个应用程序，这时候前台服务就会在不显示通知的情况下继续运行。
```java
cancelNotification(callingUid, callingPid, pkg, tag, id, 0, 0, false, userId,REASON_ERROR, null);}
```
- MalServiceA

创建RemoteViews对象的时候，指定了Layout ID为-1，显然这是一个不合法的值，这样就可以触发onNotificationError回调。
```java
RemoteViews remoteViews = new RemoteViews(getPackageName(), -1);
```
- CVE-2020-0108

在ServiceRecord.java中的postNotification方法中，没有正确处理通知显示中的异常情况，而是将异常抛出给了用户程序。
```java
ams.crashApplication(appUid, appPid, localPackageName, -1,
                        "Bad notification for startForeground: " + e);
```
- MalServiceB

直接使用了一个无效的Channel ID来构建Notification
```java
Notification notification = new NotificationCompat.Builder(this, "InvalidInvalidInvalid")
```
另外两个Service都需要捕获主线程的异常，这样app就不会崩溃了。

## 漏洞验证
- 环境

Genymotion模拟器，Android 8.0，2017年9月5日补丁
- 后台录音

使用MediaRecord实现录音10秒
- 更新地理位置

使用LocationManager获得
- 对比

正常的前台服务（NormalService）在通知栏有常驻通知，Poc则没有此通知。
## 漏洞补丁
在onNotificationError回调中强制使应用崩溃，并且在postNotification方法的异常处理中，也强制使应用崩溃。
## 参考文献
[1] https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-0108

[2] https://github.com/wrlu/vulnerabilities/tree/master/CVE-2020-0108

[3] https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-0313



