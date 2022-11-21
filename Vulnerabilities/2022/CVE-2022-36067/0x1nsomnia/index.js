let http = require('http');
let formidable = require('formidable');
let fs = require('fs');
let path = require('path');
const os = require('os');
const { NodeVM } = require('vm2');

const runTestCode = (executionPath) => {
    // Run Test Code
    console.log("Starting runTestCode....\n");
    try {

        console.log("Creating VM....\n");
        let code = fs.readFileSync(executionPath, 'utf8');
        const vm = new NodeVM({
            sandbox: {},
            require: {
                external: true,
                console: 'inherit',
            }
        });
        console.log("Code:\n", code);
        console.log("Executing test code in sandbox....\n");

        console.log("============RESULT================\n", vm.run(code, executionPath));
        //debugger;
    } catch (err) {
        console.log(err);
        console.error("Failed to create vm.", err);
        console.error("Failed to create vm.", JSON.stringify(err));

    }

    process.on('uncaughtException', (err) => {
        console.error('Asynchronous error caught.', err);
    });
}

function main() {
    http.createServer(function (req, res) {   //create web server
        if (req.url == '/') { //check the URL of the current request

            // set response header
            res.writeHead(200, { 'Content-Type': 'text/html' });

            // set response content    
            res.write('<html><body><p>Visit <a href="/poc">/poc</a> to submit reverse shell or code to run in vm2 sandbox</p></body></html>');
            res.end();

        }
        else if (req.url.includes("/poc")) {

            if (req.url == "/poc/upload") {
                var form = new formidable.IncomingForm();
                form.parse(req, function (err, fields, files) {
                    console.log("Parsing file...\n");
                    let oldpath = files.file.filepath;
                    // store input file in /tmp for vm2 sandbox evaluation
                    let newpath = path.join(os.tmpdir(), files.file.originalFilename);
                    fs.rename(oldpath, newpath, function (err) {
                        if (err) throw err;
                        res.end();
                    });
                    res.write('File uploaded and moved to /tmp!');
                    console.log("2newpath:\n", newpath);
                    try {
                        runTestCode(newpath);
                        res.writeHead(200, { 'Content-Type': 'text/html' });
                        res.write('called runTestCode');
                    } catch (err) {
                        console.error("Error:\n", err);
                        res.writeHead(500, { 'Content-Type': 'text/html' });
                        res.write('Something went wrong!');
                    } finally {
                        res.end();
                    }
                    if (err) {
                        res.writeHead(500, { 'Content-Type': 'text/html' });
                        res.write('Something weird happened while parsing the file!');
                        res.end();
                        throw err;
                    }
                });
            }
            else {
                res.writeHead(200, { 'Content-Type': 'text/html' });
                res.write('<html><body><p>Upload file here</p></body></html>');
                res.write('<form action="/poc/upload" method="post" enctype="multipart/form-data">');
                res.write('<input type="file" name="file"><br>');
                res.write('<input type="submit">');
                res.write('</form>');
                res.end();
            }
        }
        else {
            res.end('Invalid Request!');
        }
    }).listen(5000);
    console.log('Node.js web server at port 5000 is running..');
}

if (require.main === module) {
    main();
}