const postgresAdapter = require('sails-postgresql')
const Waterline = require('waterline')

const orm = new Waterline()

const config = {
    adapters: {
        postgresql: postgresAdapter
    },

    connections: {
        default: {
            adapter: 'postgresql',
            host: 'localhost',
            user: 'postgres',
            database: 'pentest'
        }
    }
}

var fs = require('fs');
var path      = require("path");

fs
  .readdirSync(__dirname)
  .filter(function(file) {
    return (file.indexOf(".") !== 0) && (file !== "index.js");
  })
  .forEach(function(file) {
    var model = require(path.join(__dirname, file));
    orm.loadCollection(model);
  });

module.exports = {waterline: orm, config: config};