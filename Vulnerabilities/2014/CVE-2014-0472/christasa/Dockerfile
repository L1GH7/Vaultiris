FROM python:2.7

ENV SECRET_KEY="7#u05zrv1fs9@i%zg1*be@e&yfeko56+e6$wt&gl2zl*y+7v5&" 

ARG PIP_MIRROR="http://mirrors.aliyun.com/pypi/simple/"
ADD ./requirements.txt /requirements.txt

RUN set -ex \
    && pip install -i $PIP_MIRROR -r /requirements.txt   --trusted-host mirrors.aliyun.com 


COPY ./vuln  /usr/src/

RUN chmod +x /usr/src/run.sh && \
    chown -R www-data:www-data /usr/src/ && \
    chmod -R 755 /usr/src/



WORKDIR /usr/src/

EXPOSE 8000
CMD ["sh", "run.sh"]


