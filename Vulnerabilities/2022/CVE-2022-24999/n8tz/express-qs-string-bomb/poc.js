/*
 * Copyright 2021 BRAUN Nathanael
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT.
 */

const express = require('express')

const app  = express()
const port = 3000

//const qs      = require('qs');
//app.set('query parser', function ( str ) {
//	// By default express use { allowPrototypestrue }
//	// but 'qs' disable this option by default
//	return qs.parse(str);
//});
app.use(express.urlencoded({ extended: true }));

function doSomeTests( params ) {
	let tmp,
	    someString = params.someString;
	console.log('[server] Received params ', params);
	console.log('[server] Run someString + 0');
	console.time('[server] someString + 0 ');
	tmp = someString + 0;
	console.timeEnd('[server] someString + 0 ');
	
	console.log('[server] Run someString == "123" ');
	console.time('[server] someString == "123" ');
	tmp = someString == "123";
	console.timeEnd('[server] someString == "123" ');
	
	console.log('[server] just write `Got ${someString}`');
	console.time('[server] just write `Got ${someString}` ');
	tmp = `Got ${someString}`;
	console.timeEnd('[server] just write `Got ${someString}` ');
	
}

app.get('/', ( req, res, next ) => {
	doSomeTests(req.query)
	res.send('Hello World!');
	process.exit(0)
})
app.post('/', ( req, res, next ) => {
	doSomeTests(req.body)
	res.send('Hello World!');
	process.exit(0)
})

app.listen(port, () => {
	console.log(`Example app listening at http://localhost:${port}`)
})