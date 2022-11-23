var http = require('http');
http.createServer(function (req, res) {
  res.writeHead(301, {'Location': 'http://127.0.0.1:2147426192'});
  res.end();
}).listen(8080);
