sudo chmod +x *.sh
sudo docker kill $(sudo docker ps -q)

service apache stop

sudo mvn clean install

docker build --tag=text4shell .

sudo docker run -p 80:8080 text4shell &


