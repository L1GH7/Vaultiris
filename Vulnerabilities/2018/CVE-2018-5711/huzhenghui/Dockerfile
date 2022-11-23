FROM php:7.2.0

MAINTAINER huzhenghui hu@daonao.com

RUN	apt-get update && \
	apt-get install -y libpng-dev xxd && \
	docker-php-ext-install gd && \
	curl https://raw.githubusercontent.com/huzhenghui/Test-7-2-0-PHP-CVE-2018-5711/master/poc -o /poc && \
	xxd -r /poc /poc.gif
