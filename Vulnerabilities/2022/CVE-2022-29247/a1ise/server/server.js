const express = require("express");
const path = require("path");

const app = express();

app.use(express.static('html'))

app.listen(3001, () => {
    console.log('Running server at 3001');
})