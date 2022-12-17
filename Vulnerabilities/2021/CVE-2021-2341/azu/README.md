Test for https://snyk.io/vuln/SNYK-JS-MSGPACK-1296122

This looks like not vulerbility.

```js
var assert = require('assert');
var msgpack = require('msgpack');
function sleep(ms) {
  return new Promise((resolve) => {
    setTimeout(resolve, ms);
  });
}
async function init() {
  var normal = { "a": 1, "b": 2, "c": [1, 2, 3] };
  var malicious = msgpack.pack({
    exploit: function () {
      // Note(by @azu): This Poc just run child_process.exec
      // It is not related to msgpack
      require('child_process').exec('echo code_executed!;sleep 3', function (error, stdout, stderr) {
        console.log(stdout)
      });
    }(),
  });
  var rce = msgpack.unpack(malicious);
  assert.deepEqual(rce, normal);
}

init();
```

> PoC by Adi Malyanker

This PoC just run `child_pross.exec`, so This is not vulnerbiliy of `msgpack`.

## Test

    git clone git@github.com:azu/msgpack-CVE-2021-23410-test.git
    cd msgpack-CVE-2021-23410-test
    npm install
    npm test

## Related

- [Security Issue in msgpack · Issue #56 · msgpack/msgpack-node](https://github.com/msgpack/msgpack-node/issues/56)
- [Deserialization of Untrusted Data in msgpack | Snyk](https://snyk.io/vuln/SNYK-JS-MSGPACK-1296122)