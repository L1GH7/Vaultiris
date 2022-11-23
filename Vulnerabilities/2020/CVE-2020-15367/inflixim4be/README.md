<h2 align="center">
  CVE-2020-15367 
  <br/>
  Brute Force on Supravizio BPM 10.1.2
</h2>

<p align="center">
  <img src="https://user-images.githubusercontent.com/49153346/88341987-f021ac00-cd14-11ea-836b-0fba611d7540.png" width="350" />
</p>

<hr>

### Description

Venki Supravizio BPM 10.1.2 does not limit the number of authentication attempts. An unauthenticated user may exploit this vulnerability to launch a brute-force authentication attack against the Login page.


### Exploitation

To exploit this vulnerability, it is necessary using the user enumeration vulnerability in Password Recovery (CVE-2020-15392) to enumerate the valid users and after could perform an arbitrary number of authentication attempts using different passwords, and eventually gain access to the targeted account.


### PoC

* Login Page

<img src="https://user-images.githubusercontent.com/49153346/86490915-74e55f80-bd3f-11ea-8806-7babbd5f1491.JPG"/>

<br />

* Brute Force Login - Invalid User 

<img src="https://user-images.githubusercontent.com/49153346/86490924-7878e680-bd3f-11ea-8a0e-4792f8ed33f6.jpg"/>

<br />

* Brute Force Login - Valid User 

<img src="https://user-images.githubusercontent.com/49153346/86490926-7adb4080-bd3f-11ea-9331-e389fa44783f.jpg"/>
