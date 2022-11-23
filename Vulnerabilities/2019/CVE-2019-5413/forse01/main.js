var express = require('express');
var morgan = require('morgan');
var app = express();

app.use(morgan('combined'));
app.get('/', function (request, response) {
  response.send('Hello Express');
});

var f = morgan('25 \\" + console.log(\'hello!\'); +  //:method :url :status :res[content-length] - :response-time ms');
f({}, {}, function () {
    app.listen(3000);
});
