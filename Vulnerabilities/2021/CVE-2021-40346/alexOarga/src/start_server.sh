#!/bin/sh

# directory where static pages will be located in the container (see docker-compose file)
cd /usr/src/app

# run flask server through gunicorn wsgi
gunicorn --keep-alive 10 -k gevent --bind 0.0.0.0:5000 -w 1 app:app
