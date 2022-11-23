const fastify = require('fastify')()
const port = process.env.PORT || 5000

fastify.listen(port, (err) => {
    if (err) throw err
    console.log(`server listening on ${fastify.server.address().port}`)
})

require('./src/routes')(fastify)