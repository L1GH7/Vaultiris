#! /usr/bin/bash

BASE_DIR=$(pwd)

exec gunicorn --chdir=${BASE_DIR} -w 2  -b 0.0.0.0:8000 -u www-data -g www-data --access-logfile - CVE-2014-0472.wsgi:application


