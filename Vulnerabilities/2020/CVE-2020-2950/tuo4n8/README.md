# Oracle-BI (CVE-2020-2950) 

## AMF deseiralize

> **Version:**  5.5.0.0.0, 11.1.1.9.0, 12.2.1.3.0
>
> **Install:**https://www.sql.edu.vn/obiee/oracle-business-intelligence-12c/
>
> **Ref:** https://peterjson.medium.com/cve-2020-2950-turning-amf-deserialize-bug-to-java-deserialize-bug-2984a8542b6f

---

---

## Exploit - PoC

> [amf.bin](https://github.com/tuo4n8/CVE-2020-2950/blob/main/amf.bin)
>
> Header cmd with base64 and child !!

![](README.assets/rce.jpg)

---

## Debug trace bug

URL: `/analytics/jbips/messagebroker/cs/`

- Handle request -> processCall()

![image-20210521104317854](take-note.assets/image-20210521104317854.png)

- Get inputstream -> deserialize AMF package

![image-20210521104423834](take-note.assets/image-20210521104423834.png)

- Get Object AMF -> deserilize

![image-20210521104538758](take-note.assets/image-20210521104538758.png)

![image-20210521104730919](take-note.assets/image-20210521104730919.png)

![image-20210521104755256](take-note.assets/image-20210521104755256.png)

![image-20210521104844150](take-note.assets/image-20210521104844150.png)

- If matching type -> AMF readobject (AMF3DATA.class)

![image-20210521104956342](take-note.assets/image-20210521104956342.png)

- AMF3DATA.class -> AMF3ObjectInput
- In AMF3ObjectInut -> readComplexObject

![image-20210521105409912](take-note.assets/image-20210521105409912.png)

- In readComplexObject: 
  - If class deseriliaze is externalizable -> radExternalchain
  - else setFiled

![image-20210521105523270](take-note.assets/image-20210521105523270.png)

- AMF deserialize  chain -> readExternal Chain

