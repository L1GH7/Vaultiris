const express = require('express');
const {trim} = require("axios/lib/utils");

const app = express();
const port = 3000;

function newTrim(str) {
  return str.trim ? str.trim() : str.replace(/^\s+|\s+$/g, '');
}

const implementation = trim;

function runTrim(string, trim) {
  let startTime, endTime;

  startTime = Date.now();
  trim(string);
  endTime = Date.now();
  
  return endTime - startTime;
}

app.use(express.urlencoded({ extended: true}))
app.get("/", (req, res) => {
  res.sendFile(__dirname + '/client.html');
});

app.post("/", (req, res) => {
  console.log("Request recebida!");
  const time = runTrim(req.body.input, implementation);
  console.log(`Processamento finalizado! Tempo gasto: ${time/1000}s`);
  res.status(200).sendFile(__dirname + '/client.html');
});

app.listen(port, () => {
  console.log(`server running at ${port}!`);
});