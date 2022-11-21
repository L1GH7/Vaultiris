sudo chmod +x *.sh
sudo docker kill $(sudo docker ps -q)
sudo rm -r target
