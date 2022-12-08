FROM python:3.8.13-slim-buster

RUN sed -i "s@http://deb.debian.org@https://mirrors.tuna.tsinghua.edu.cn@g" /etc/apt/sources.list

RUN apt-get update -y
RUN apt-get install python3-dev default-libmysqlclient-dev build-essential -y

RUN pip3 install django==4.0.5 mysqlclient -i https://pypi.tuna.tsinghua.edu.cn/simple/

WORKDIR /app
ADD . /app

RUN python manage.py makemigrations

CMD ["python", "manage.py", "runserver", "0.0.0.0:8000"]