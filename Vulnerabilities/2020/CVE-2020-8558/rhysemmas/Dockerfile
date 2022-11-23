FROM python:3.8-alpine3.12

COPY ./start.sh /start.sh
COPY ./martian_packets /martian_packets

RUN chmod 0744 /start.sh

CMD ./start.sh
