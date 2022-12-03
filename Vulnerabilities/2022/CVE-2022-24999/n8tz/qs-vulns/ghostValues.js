const qs = require("qs");
{
	
	// using the default express configuration :
	console.log('Using the default express configuration : \n');
	console.log('qs.parse("foo[__proto__][hidden]=value&foo[bar]=stuffs", { allowPrototypes: true })');
	
	let query = qs.parse("foo[__proto__][hidden]=value&foo[bar]=stuffs", { allowPrototypes: true }),
	    foo   = query.foo;
	
	console.log('foo value : ', foo);
	/*
	 { bar: 'stuffs' }
	 
	 internal structure :
	 {
	 bar: 'stuffs',
	 __proto__ : { hidden : "value" }
	 }
	 */
	
	console.log('(foo instanceof Object) : ', (foo instanceof Object));// true
	console.log("foo.hasOwnProperty(\"hidden\") : ", foo.hasOwnProperty("hidden"));// true
	console.log("Object.keys(foo) : ", Object.keys(foo));// [bar]
	console.log("\nfoo.hidden : ", foo.hidden);// value
}
