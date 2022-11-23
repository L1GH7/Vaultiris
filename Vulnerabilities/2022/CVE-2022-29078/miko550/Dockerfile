FROM node:16

# Create app directory
WORKDIR /usr/src/app

# Install app dependencies
# A wildcard is used to ensure both package.json AND package-lock.json are copied
# where available (npm@5+)
COPY package*.json ./

RUN npm install -S express ejs@3.1.6
RUN npm install -g nc

# Bundle app source
COPY . .

EXPOSE 8080
CMD [ "node", "app.js" ]
