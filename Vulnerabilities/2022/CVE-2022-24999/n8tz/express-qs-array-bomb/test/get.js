/*
 * Copyright 2021 BRAUN Nathanael
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT.
 */

const axios   = require("axios");
const fs      = require("fs");
const payload = fs.readFileSync(process.argv[2]) + "";
const start   = Date.now();
const timer   = setInterval(timeout, 1000);

console.log("Send payload : ", payload);

axios.get("http://localhost:3000/?" + payload)
     .then(
	     res => {
		     clearInterval(timer);
		     console.log('Status ', res.status);
		     console.log('___________________');
		     console.log(res.data);
		     console.log('___________________');
		     console.log("Duration : ", (Date.now() - start) / 1000, "s");
		     process.exit();
	     },
	     err => {
		     clearInterval(timer);
		     console.warn('Fail ', err + "");
		     console.log("Duration : ", (Date.now() - start) / 1000, "s");
		     process.exit();
	     }
     )


function timeout() {
	console.log("Duration : ", (Date.now() - start) / 1000, "s");
}