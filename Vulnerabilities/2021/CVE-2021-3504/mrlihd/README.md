# CVE-2021-35042
SQL injection via QuerySet.order_by() untrusted input

# Setup:
Start the instances using: 
`docker-compose up`

Now open the following URL to load sample data:

http://localhost:8000/vul_app/setup

Then go to the vulnerable page at:
http://localhost:8000/vul_app/

Exploit the parameter at:
http://localhost:8000/vul_app/?order_by=name
