# ban-exploitable-bitcoin-nodes
Ban all denial-of-service vulnerability exploitable nodes from your node CVE-2018-17144  

## Requirements for the bash script (ban.sh)

### Command-line JSON processor jq.

Install on Debian-based Linux
```` 
sudo apt-get install jq
```` 

## Download and use the script

`git clone https://github.com/iioch/ban-exploitable-bitcoin-nodes.git`

Run:

````
cd ban-exploitable-bitcoin-nodes   
chmod u+x ban.sh   
./ban.sh
````

### To view banned IP addresses:

````
bitcoin-cli listbanned
````

### To clear banned IP addresses:

````
bitcoin-cli clearbanned
````
