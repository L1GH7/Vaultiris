# CVE-2021-38149
Chikitsa Patient Management System 2.0.0 Stored Cross-Site Scripting (XSS)

An instance of stored cross-site scripting (XSS) exists in multiple pages on version 2.0.0 of Chikitsa Patient Management System that allows for arbitrary JavaScript to be executed in a user's browswer that could potentially allow for a user to escalate privileges.

Vulnerable Pages:
- /index.php/admin/add_user
- /index.php/appointment/todos
- /index.php/appointment/insert_patient_add_appointment/(hr of apppointment)/(minute of appointment)/<date>/Appointments//0/
  
Known Cross-Site Scripting Payloads That Work:
  - ```<script>alert('xss');</script>```
  - ```<img src=x onerror=alert(document.domain)>```

<h2>Proof of Concept:</h2>

![image](https://user-images.githubusercontent.com/67240643/128488299-3d9747b9-1664-4666-a76f-f7e47e049dce.png)
<i>A user with privileges to create other users has the ability to create users can input a XSS payload into any of the user's name fields shown above.</i>
  
![image](https://user-images.githubusercontent.com/67240643/128488944-04f3d355-8f0d-4f58-84d7-c50f68a47b76.png)
  <i>Observing the application's response reveals that the JavaScript is being reflected.</i>
  
![image](https://user-images.githubusercontent.com/67240643/128489227-1f2021c6-7716-4526-a912-55d987343444.png)
<i>The created user containing the malicious XSS payload has successfully been created and will execute the JavaScript everytime a user visits the users the application contains.</i>

Discovered By: Joe Aguilar Jr.
