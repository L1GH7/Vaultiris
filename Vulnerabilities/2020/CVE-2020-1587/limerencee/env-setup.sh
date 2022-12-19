# Install Pre-requisites
sudo apt install docker.io
sudo apt install docker-compose
sudo apt install git

# Start docker service
sudo systemctl start docker

# Prep working directory at ~/Downloads/cs4239/
cd ~/Downloads/
mkdir cs4239/ && cd cs4239/

git clone https://github.com/librenms/docker.git

# Setup vulnerable and patched examples
mkdir vulnerable patched
cp -rT docker/examples/compose patched
cp -rT docker/examples/compose vulnerable

# Vulnerable version docker-compose
sed -i -e '/container_name/s/$/_vuln/' vulnerable/docker-compose.yml
sed -i 's/librenms:latest/librenms:1.65/' vulnerable/docker-compose.yml

# Patched version docker-compose
sed -i -e '/container_name/s/$/_patched/' patched/docker-compose.yml
sed -i 's/published: 8000/published: 8001/' patched/docker-compose.yml
sed -i 's/published: 514/published: 515/' patched/docker-compose.yml

# To launch the instances, checkout start.sh and stop.sh
