# 설치 및 실행 순서

#### 0. type3 설치
<pre> $docker-compose up  </pre>

#### 1. typo3 웹 서버 가동 및 설치
http://[web-server ip]:port/typo3로 이동, 다음 문구는 무시합니다.
<pre> Directory / is not writable
Path /var/www/html/typo3 exists, but no file underneath it can be created.  </pre>

#### 2. typo3 설치
컨테이너에 올린 typo3 사이트에 들어가 설치를 진행합니다.
mysql db의 호스트는 db입니다.

#### 3. typo3 XSS injection
설치하는 중간 4/5에서 site name에 XSS payload를 입력합니다.
![image](https://user-images.githubusercontent.com/43310843/120443055-3d54bc80-c3c1-11eb-844c-d3dc52764855.png)

#### 4. XSS execution
설치가 완료되고 로그인을 하면 아래와 같이 XSS가 실행되는 것을 볼 수 있습니다.
![image](https://user-images.githubusercontent.com/43310843/120443542-b5bb7d80-c3c1-11eb-8f2d-ecc85a1bc5e3.png)




#### 구축 에러  

##### 아래와 같이 debconf에서 CLI로 몇가지 물어보는 사항을 자동화 하기 위해 debconf-utils를 설치하고 수정했습니다.
<pre>
debconf: unable to initialize frontend: Readline
debconf: (This frontend requires a controlling tty.)
debconf: falling back to frontend: Teletype
</pre>

<pre>
RUN apt-get install -y debconf-utils
RUN echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections
RUN apt-get install -y -q [package]
</pre>
www-data user로 실행되는 apache2서버 의 특성 상, www-data로 typo3 폴더를 생성하였습니다.

#####  호환되는 DB 버전은 아래와 같습니다. 
mysql image를 5.8버전으로 설정하였습니다.
<pre>
System Requirements 
For more information as well as installation instructions see the Installation guide.
Operating System	Linux, Windows or Mac, or common cloud infrastructure setups
Webserver	Apache httpd, Nginx, Microsoft IIS, Caddy Server
Supported Browsers	Chrome (latest)
Edge (latest)
Firefox (latest)
Internet Explorer >= 11
Safari (latest)
Database	MariaDB >= 10.0 <= 10.3
Microsoft SQL Server
MySQL >= 5.0 <= 5.7
PostgreSQL
SQLite
Hardware	RAM >= 256 MB
PHP	PHP >= 7.2 <= 7.4
</pre>

#####  mysql의 character-set에 문제가 있습니다.
mysql container에 command 옵션을 설정하였습니다.
<pre>
Invalid Charset
Your database uses character set "latin1", but only "utf8" is supported with TYPO3. You probably want to change this before proceeding.
</pre>
