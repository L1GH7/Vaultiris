const Waterline = require('waterline')

const User = {
    identify: 'user',
    tableName: 'users',
    connection: 'default',
    attributes: {
        id: { type: 'integer', primaryKey: true, autoIncrement: true },
        username: { type: 'string', maxLength: 40 },
        password: { type: 'string', maxLength: 80 }
    }
}

module.exports = Waterline.Collection.extend(User)