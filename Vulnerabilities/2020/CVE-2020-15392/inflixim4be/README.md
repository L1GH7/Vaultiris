<h2 align="center">
  CVE-2020-15392 
  <br/>
  User Enumeration on Supravizio BPM 10.1.2
</h2>

<p align="center">
  <img src="https://user-images.githubusercontent.com/49153346/88341987-f021ac00-cd14-11ea-836b-0fba611d7540.png" width="350" />
</p>

<hr>

### Description

A user enumeration vulnerability flaw was found in Supravizio BPM, version 10.1.2. This issue occurs during password recovery, where a difference in messages could allow an attacker to determine if the user is valid or not, enabling a brute force attack with valid users.


### Exploitation

To exploit this vulnerability, it is necessary to request a password recovery, when adding a invalid contact email the message: "email not found" is displayed and when an valid email: "contact the system administrator".


### PoC

* Invalid User

<img src="https://user-images.githubusercontent.com/49153346/86146371-8116b600-bace-11ea-877b-e8dbeead56ae.JPG"/>

<br />

* Valid User

<img src="https://user-images.githubusercontent.com/49153346/86146379-8411a680-bace-11ea-96d5-3ae081913fe7.JPG"/>

<br />

* Brute Force - Invalid User

<img src="https://user-images.githubusercontent.com/49153346/86146392-87a52d80-bace-11ea-9b9e-109738b907f3.jpg"/>

<br />

* Brute Force - Valid User

<img src="https://user-images.githubusercontent.com/49153346/86146400-8aa01e00-bace-11ea-9efd-e9ceb232ad1c.jpg"/>
