# Assumes the working directory to be at ~/Downloads/cs4239/

# Starts the vulnerable instance in background
cd ~/Downloads/cs4239/vulnerable
sudo docker-compose up -d

# Starts the patched instance in background
cd ~/Downloads/cs4239/patched
sudo docker-compose up -d

# Check that the instances are up:
sudo docker ps

# Vulnerable instance served at: http://localhost:8000/
# Patched instance served at: http://localhost:8001/
