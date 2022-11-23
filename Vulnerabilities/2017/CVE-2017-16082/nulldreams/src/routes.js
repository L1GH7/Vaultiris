var models = require('./models');

module.exports = (fastify) => {

    fastify.get('/api/v1/users', async (req, reply) => {
            try {
                let { id } = req.query
                let _models = await initialize()
                const Users = _models.collections.users
                const sql = `SELECT * FROM users WHERE "id" = ${id}`

                Users.query(sql, (err, res) => {
                    if (err) return console.error(f)

                    reply.code(200).send(JSON.parse(JSON.stringify(res.rows)))
                })
            } catch (e) {
                console.log(e)
            }
        })
}

async function initialize() {
    return new Promise((resolve, reject) => {
        models.waterline.initialize(models.config, async function (err, models) {
            if (err) return reject(err)

            resolve(models)
        })
    })
}