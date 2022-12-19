# Assumes the working directory to be at ~/Downloads/cs4239/

# Starts the vulnerable instance in background
cd ~/Downloads/cs4239/vulnerable
sudo docker-compose down -d

# Starts the patched instance in background
cd ~/Downloads/cs4239/patched
sudo docker-compose down -d

# Check that the instances are stopped:
sudo docker ps
