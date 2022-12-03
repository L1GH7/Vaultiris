const qs = require("qs");
{
	
	// using the default express configuration ( allowPrototypes = true ) :
	console.log('Using the default express configuration ( allowPrototypes = true ) : \n');
	console.log('qs.parse("categories[__proto__]=cats&categories[__proto__]=dogs&categories[some][json]=toInject", { allowPrototypes: true })');
	
	let query      = qs.parse("categories[__proto__]=cats&categories[__proto__]=dogs&categories[some][json]=toInject", { allowPrototypes: true }),
	    categories = query.categories;
	
	console.log('categories value : ', categories);
	/*
	 Array { some: { json: 'toInject' } }
	 
	 internal structure :
	 {
	 some    : {json:"toInject"},
	 __proto__ : ['cats', 'dogs']
	 }
	 */
	
	console.log('(categories instanceof Array) : ', (categories instanceof Array));
	console.log("categories.map(item=>item) : ", categories.map(item => item)); // [ 'cats', 'dogs' ]
	console.log("\nJSON.stringify(categories) : ", JSON.stringify(categories)); // {"some":{"json":"toInject"}}
}
