FROM node:8-alpine

WORKDIR /usr/src/app

COPY package*.json ./
RUN npm install
RUN npm rebuild node-sass --force

COPY . .

EXPOSE 4000

ADD https://github.com/ufoscout/docker-compose-wait/releases/download/2.2.1/wait /wait
RUN chmod +x /wait

CMD /wait && node index
