/*
 * Copyright 2021 BRAUN Nathanael
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT.
 */

const express = require('express');

const app  = express()
const port = 3000

//app.set('query parser', function ( str ) {
//	return qs.parse(str, { allowPrototypes: false });
//});
//app.set("query parser", "simple");
app.use(express.urlencoded({ extended: true }));
//app.use(bodyParser.urlencoded({  }))

app.get('/', ( req, res, next ) => {
	const categories = req.query.categories;
	
	console.log('[server] Received query : ', req.query);
	console.log('[server] (categories instanceof Array) : ', (categories instanceof Array));
	
	if ( !categories || !(categories instanceof Array) )
		return next(new Error("'categories' must be an Array"))
	
	console.log('[server] Do : req.query.categories.indexOf("123")');
	console.time('[server] req.query.categories.indexOf("123")');
	req.query.categories.indexOf("123")
	console.timeEnd('[server] req.query.categories.indexOf("123")');
	
	res.send('Hello World!');
	process.exit(0)
})
app.post('/', ( req, res, next ) => {
	//console.log(':::41: ', req);
	const categories = req.body.categories;
	
	console.log('[server] Received body : ', req.body, req.body.hasOwnProperty);
	console.log('[server] (categories instanceof Array) : ', (categories instanceof Array));
	
	if ( !categories || !(categories instanceof Array) )
		return next(new Error("'categories' must be an Array"))
	
	console.log('[server] Do : categories.indexOf("123")');
	console.time('[server] categories.indexOf("123")');
	categories.indexOf("123")
	console.timeEnd('[server] categories.indexOf("123")');
	
	res.send('Hello World!');
	process.exit(0)
})

app.listen(port, () => {
	console.log(`Example app listening at http://localhost:${port}`)
})