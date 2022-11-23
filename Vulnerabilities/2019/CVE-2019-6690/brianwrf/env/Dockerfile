FROM mintproject/base-ubuntu18
MAINTAINER avfisher <security_alert@126.com>

RUN set -x \
    && apt update \
    && apt install -y git python3.7 libmojolicious-perl python3-flask \
    && python3.7 -m pip install python-gnupg==0.4.3 \
    && git clone https://github.com/stigtsp/CVE-2019-6690-python-gnupg-vulnerability
CMD python3.7 /CVE-2019-6690-python-gnupg-vulnerability/server.py

EXPOSE 5000
