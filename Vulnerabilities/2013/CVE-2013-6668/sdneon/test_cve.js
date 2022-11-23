setInterval(function() { //just to keep node alive
    console.log('Alive');
}, 1000);

var data = {
    payload: null
};
var connParams = {
    server: '127.0.0.1',
    userName: 'sa',
    password: 'password',
    options:
    {
        port: 49894,
        database: 'SQLDB'
    }
};

var tds = require('tedious');
var db = {
    connect: function(dbName, data, cb){
        if (dbName)
        {
            connParams.options.database = dbName;
        }
        var conn = new tds.Connection(connParams);
        this.handleConnect(conn, data, cb);
    },
    handleConnect: function(conn, data, cb){
        var self = this;
        conn.on('connect', function (err){
            if (err) {
                console.error('Received db conn error: ' + err);
            } else {
                self.executeStatement(conn, data, cb);
            }
        });
    },
    executeStatement: function(conn, data, cb){
        var i,
            sqlStmt = "EXEC " + data.query,
            request,
            sqlResults = {
                results : [],
                rowCount : 0
            };

        if (data.params) {
            for (i = 0; i < data.params.length; i++) {
                for (i = 0; i < data.params.length; i++) {
                    if (data.params[i] === null) {
                        sqlStmt += " null";
                        if (i !== data.params.length - 1) {
                            sqlStmt += ",";
                        }
                    }
                    else {
                        sqlStmt += " '" + data.params[i] + "'";
                        if (i !== data.params.length - 1) {
                            sqlStmt += ",";
                        }
                    }
                }
            }
        }

        //Execution of request
        request = new tds.Request(sqlStmt, function (err){
            if (err) {
                console.log('[' + new Date() + ']' + '[Database] ' + sqlStmt + ' [Error:. ' + err + ']');
                sqlResults.rowCount = -1;
                sqlResults.error = "error - " + err;
                if (cb) {
                    cb(sqlResults);
                }
            } else {
                sqlResults.rowCount = sqlResults.results.length;
                console.log("**Executed: " + sqlStmt);
                console.log("\trow count: " + sqlResults.rowCount);
                if (cb) {
                    cb(sqlResults);
                }
            }
            conn.close();
        });

        request.on('row', function (columns) {
            var aResult = {};
            columns.forEach(function (column) {
                aResult[column.metadata.colName] = column.value;
            });
            sqlResults.results.push(aResult);
        });

        request.on('done', function (rowCount){
            console.log(rowCount + ' rows returned');
        });

        conn.execSql(request);
    }
};

function runSqlQuery(i)
{
    //Run stored procedure GetSomething to retrieve data from Table1
    db.connect("SQLDB", { //exploded when run by itself
        "query" : 'GetSomething',
        "params" : [3]
    }, function (sqlResults) {
        console.log('result:', sqlResults);
        console.log('result #', i, 'complete.');
    });
}

var i;
for (i = 0; i < 20; ++i)
{
    runSqlQuery(i); //somewhere along the way, node exits prematurely without any explicit error message.
                    //only if running debug version of node, then will get call trace for fatal assertion error.
}
