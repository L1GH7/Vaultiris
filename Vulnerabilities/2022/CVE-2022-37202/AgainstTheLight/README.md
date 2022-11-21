# CVE-2022-37202
CVE-2022-37202 POC



> [Suggested description]
> JFinal CMS 5.1.0 is vulnerable to SQL Injection.
>
> ------------------------------------------
>
> [Vulnerability Type]
> SQL Injection
>
> ------------------------------------------
>
> [Vendor of Product]
> the development group
>
> ------------------------------------------
>
> [Affected Product Code Base]
> https://github.com/jflyfox/jfinal_cms - JFinal CMS 5.1.0
>
> ------------------------------------------
>
> [Affected Component]
> These interfaces do not use the same component, nor do they have filters, but each uses its own SQL concatenation method, resulting in SQL injection
>
> ------------------------------------------
>
> [Attack Type]
> Remote
>
> ------------------------------------------
>
> [Impact Code execution]
> true
>
> ------------------------------------------
>
> [Impact Information Disclosure]
> true
>
> ------------------------------------------
>
> [Attack Vectors]
> User login is required
>
> ------------------------------------------
>
> [Reference]
> https://github.com/AgainstTheLight/someEXP_of_jfinal_cms/blob/main/jfinal_cms/sql1.md
>
> ------------------------------------------
>
> [Discoverer]
> jw5t
