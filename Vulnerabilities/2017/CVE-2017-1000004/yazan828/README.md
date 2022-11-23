# ATutor SQL Injection Vulnerability
## SQL Injection
In order to examine and study the CVE-2017-1000004 vulnerability, a brief explanation of a SQL Injection Attack is required. 

Any web application that takes user input is vulnerable. SQL injection or SQLi is an injection attack where the attacker can execute malicious SQL statements in the user input that controls the database server behind the web application. In 2017 SQL injection was rated the number one attack on the OWASP top ten [1][1]. It usually allows the attacker to examine data that they are not normally able to view, this includes data that belongs to other users.

In many SQL injection attacks, the attacker can spoof identity,  gain unauthorized access to sensitive data such as passwords, personal user information or credit card information, modify (insert/update/delete) data or execute administrative operations causing permanent adjustments to the web application's content or performance [2][2],[3][3].
## SQL Injection Example
When a user is asked for a username to login which will be used to run a “SELECT” statement to get the user’s information, an attacker might enter a string that makes the query behave differently than intended. The below inputs show the difference between safe input and SQL injection.

**Safe Input**:
```sql
$name = "Alex";
$query = "SELECT * FROM customers WHERE username = '$name'";
```
**Injection**:
```sql
$name = "' OR 1'";
$query = "SELECT * FROM customers WHERE username = '$name'";
```
The normal query is fine as the program selects information from customers that has username equal to "Alex". However, by using the single quote (') the string part of the query is ended. Which makes *username = ''* and the *OR 1* which is (always true) is added the "WHERE" statement with *username = '' OR 1*. Hence, this allows every single entry in the *Customers* table to be selected.
## Protect Against SQL
Before Inspecting ATutor SQL vulnerabilities, the two most common and effective countermeasures for SQL attacks are Sanitizing and Escaping [8][8],[9][9],[10][10].
* Sanitizing: Sanitizing inputs is a good practice for all applications. In the previous example, the attacker used *' OR 1'* as the username which looks pretty suspicious as a username choice. Developers should always make an effort to reject inputs that look suspicious by checking if the supplied field matches the regular expression, ensuring that the input does not contain symbol characters, and reject whitespace and new line characters.
* Escaping: Escape sequences are started with the escaping character backslash (\\) followed by the character which may be an alphanumeric or a special character. This is done to make sure that the database never confuses the user's input with the SQL statement provided by the developer. As in the previous example, the escaped injection will look like:
```sql
$query = "SELECT * FROM customers WHERE username = "\' OR 1\'";
```
Then, the database will reach to the username  *\' OR 1\'* which avoids all the harm.
## ATutor
ATutor is an open-source web-based learning management system released in 2002, that is used in a variety of contexts including online course management, career development, and academic research. ATutor is currently being used all around the world and has been translated into over fifteen languages [4][4],[5][5]. 
## ATutor Vulnerability and Its Severity
ATutor 2.2.1 and the earlier versions are vulnerable to SQL injection attacks in various pages including Assignment Submissions, Blog Post, Group Course Email, Course Enrolment and Unenrollment, Auto-Login, Inbox and Sent Items, and many other pages. This essentially means an attacker can bypass authentication and reach the administrator's interface where they can upload malicious code and modify data.

All the attacks were performed by manipulating the GET and POST parameters. 

The ATutor vulnerability was found in 2017 by Henri Salo and has been declared as critical with a 9.8 CVSS Score [6][6].  The exploitation appears to be very simple and can be initiated remotely with no form of authentication is required for a successful exploitation.
![alt text](CVSS.PNG "CVSS Score [6]")
## ATutor SQL Vulnerabilities
Since ATutor 2.2.1 has been compromised with many SQL injection attacks, the below are some highlighted vulnerabilities extracted from the PHP files of ATutor version 2.2.1 [7][7]

There is a SQL injection vulnerability when marking users as alumni of courses. The user IDs provided in the "id" POST parameter are not sanitized allowing the query to be broken out of.

![alt text](Alumni_2.2.1.PNG)

Plus, there is another SQL injection vulnerability when enrolling students in courses. The user IDs provided in the "id" POST parameter from the course enrolment request are not sanitized.

![alt text](Enroll_2.2.1.PNG)

Where those vulnerabilities have been fixed in version 2.2.2 and later by sanitizing the input by adding the intval() command which returns the integer value of a variable that ensures that the user’s input is always an integer number.

![alt text](Alumni_2.2.4.PNG)
![alt text](Enroll_2.2.4.PNG)

Another SQL injection Vulnerability is when searching for courses in the search bar. The "search" parameter from the search request is not escaped before being interpolated in the search query.

![alt text](Search_2.2.1.PNG)

This issue has been fixed in the next versions by using the “my_add_null_slashes” function that ensures escaping the user input before interpolation into a search query

![alt text](Search_2.2.4.PNG)

This vulnerability is also present when searching photo albums. Again, the "search" parameter is not escaped before being interpolated in the search query. 

![alt text](Album_2.2.1.PNG)

It has been also fixed by using the “my_add_null_slashes” function.

![alt text](Album_2.2.4.PNG)

Moreover, when listing a test's results, the "start_date" and "end_date" parameters from the listing request are not escaped before interpolation into a search query. 

![alt text](Test_Result_2.2.1.PNG)

Where this issue has been also fixed by adding the “my_add_null_slashes” function.

![alt text](Test_Result_2.2.4.PNG)

Similarly, the vulnerability is also present when editing an existing poll. The poll's choices (starting with "c" and followed by a number) from the edit request are not escaped before being interpolated into a query which makes them vulnerable to SQL injection.

![alt text](Poll_Edit_2.2.1.PNG)

And adding the “my_add_null_slashes” function fixes this issue.

![alt text](Poll_Edit_2.2.4.PNG)

One significant vulnerability is when creating new polls. The new poll's choices that start with "c" and followed by a number from the poll creation request are initially escaped using the “addslashes” function. And if they then exceed 100 characters, they are truncated to 100 characters, and then the resulting choices are interpolated into the poll creation query. The vulnerability lies in the fact that the truncation occurs after the escaping. If we have a one choice where the 100th character is a single quote (‘), hence the choice is truncated so that it ends in a single backslash character that successfully escapes the quotation mark that would normally end the choice string. 
This means that the next choice's single quotation mark will actually end the string, allowing the next choice value to inject SQL into the query.
```sql
Input = AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
```

![alt text](Add_Poll_2.2.1.PNG)

This was simply fixed in the next versions by truncating the choice first to 100 characters then escaping the truncated string. 

![alt text](Add_Poll_2.2.4.PNG)

More ATutor SQL injection Vulnerabilities can be found in [7][7]. 

## Additional Resources
You may find additional information on:
* SQL Injection Attack at [1][1],[2][2],[3][3],[8][8],[9][9],[10][10],[11][11]
* ATutot at [4][4],[5][5]
* ATutor Vulnerabilty at [6][6],[7][7],[12][12]


[1]: https://owasp.org/www-project-top-ten/
[2]: https://www.acunetix.com/websitesecurity/sql-injection/
[3]: https://owasp.org/www-community/attacks/SQL_Injection
[4]: https://en.wikipedia.org/wiki/ATutor
[5]: https://atutor.github.io/
[6]: https://www.opencve.io/cve/CVE-2017-1000004
[7]: https://www.openwall.com/lists/oss-security/2016/07/01/3
[8]: https://www.hacksplaining.com/prevention/sql-injection
[9]: https://phppot.com/php/php-escape-sequences/
[10]: https://www.ptsecurity.com/ww-en/analytics/knowledge-base/how-to-prevent-sql-injection-attacks/
[11]: https://www.cisecurity.org/wp-content/uploads/2017/05/SQL-Injection-White-Paper.pdf
[12]: https://nvd.nist.gov/vuln/detail/CVE-2017-1000004
