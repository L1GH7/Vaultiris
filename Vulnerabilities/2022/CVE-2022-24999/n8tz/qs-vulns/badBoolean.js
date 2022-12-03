const qs = require("qs");
{
	
	// using the default qs configuration :
	console.log('Using the default qs configuration : \n');
	console.log('qs.parse("foo[bar]=stuffs&foo=toString", { allowPrototypes: false })');
	
	let query     = qs.parse("foo=123&foo[bar]=stuffs", { allowPrototypes: false }),
	    foo = query.foo;
	
	console.log('foo value : ', foo);
	// { foo: { '123': true, bar: 'stuffs' } }
}
