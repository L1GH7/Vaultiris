FROM tomcat:9.0.4

ADD tomcat-users.xml /usr/local/tomcat/conf/
ADD context.xml /usr/local/tomcat/webapps/manager/META-INF/context.xml
ADD vuln/target/vuln-1.0-SNAPSHOT.war /usr/local/tomcat/webapps/vuln.war

CMD ["catalina.sh", "run"]
