const {ipcRenderer, shell, contextBridge} = require('electron');

contextBridge.exposeInMainWorld('api', {
  open: (arg) => ipcRenderer.send('openFile', arg),
});

document.addEventListener('click', (e) => {
  if (e.target.nodeName === 'A'){
    let protocols = ['http', 'https'];
    let link = e.target;
    console.log(Array.prototype.indexOf())
    if(protocols.indexOf(link.protocol) !== -1){
      ipcRenderer.send('openFile', {path: link.href})
    }else {
      alert('信頼できないスキームです');
    }
  } 
});
