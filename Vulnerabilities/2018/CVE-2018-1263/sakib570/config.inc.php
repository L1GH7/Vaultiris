<?php
/* Servers configuration */
$i = 0;

$i++;
$cfg['Servers'][$i]['verbose'] = 'mysql';
$cfg['Servers'][$i]['host'] = 'mysql';
$cfg['Servers'][$i]['port'] = 3306;
$cfg['Servers'][$i]['socket'] = '';
$cfg['Servers'][$i]['connect_type'] = 'tcp';
$cfg['Servers'][$i]['auth_type'] = 'config';
$cfg['Servers'][$i]['user'] = 'phpmyadmin'; // needed for database connection & should match with the MYSQL_USER value provided in docker-compose.yml file  
$cfg['Servers'][$i]['password'] = 'hello'; // needed for database connection & should match with the MYSQL_PASSWORD value provided in docker-compose.yml file

/* To disable auto login of phpMyAdmin the last three lines above needs to be commented out. Then you will see a login screen and you need to use the same username and pasword to login */

/* End of servers configuration */

$cfg['blowfish_secret'] = 'AEl-k/ijXYWG1AkG0S/WX.d=yJJoDFN9';
$cfg['DefaultLang'] = 'en';
$cfg['ServerDefault'] = 1;
$cfg['UploadDir'] = '';
$cfg['SaveDir'] = '';
