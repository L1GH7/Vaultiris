const express = require("express");
const path = require("path");

const app = express();

app.get("/", (req, res, next) => {
  res.sendFile(path.join(__dirname, "test.html"));
});

app.get("/download_redirect.html", function(req, res) {
	res.sendFile(path.join(__dirname, "download_redirect.html"));
});

app.get("/location.php", function(req, res) {
	res.redirect(req.query.url);
});

app.listen(3000);
