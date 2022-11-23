const express = require('express');
const bodyParser = require('body-parser');
const XMLHttpRequest = require('xmlhttprequest').XMLHttpRequest

const app = express();
app.use(bodyParser.urlencoded({ extended: true }));
app.use(express.static('html'));

app.get('/', function(req, res) {
});

app.post('/', function(req, res) {
  const url = req.body.url
  const body = req.body.body

  const xhr = new XMLHttpRequest()
  xhr.open('POST', url, false)
  xhr.send(body)
  
  res.send('Send Message');
});

app.listen(3000);
