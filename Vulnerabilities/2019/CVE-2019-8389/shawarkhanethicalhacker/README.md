# CVE-2019-8389 - Arbitrary file read in Musicloud for iOS v1.6
 [CVE-2019-8389] An exploit code for exploiting a local file read vulnerability in Musicloud v1.6 iOS Application 

### Description:
A file-read vulnerability was identified in the Wi-Fi transfer feature of Musicloud 1.6. By default, the application runs a transfer service on port 8080, accessible by everyone on the same Wi-Fi network. An attacker can send the POST parameters downfiles and cur-folder (with a crafted ../ payload) to the download.script endpoint. This will create a MusicPlayerArchive.zip archive that is publicly accessible and includes the content of any requested file (such as the /etc/passwd file).

## Information:
Affected Application: Musicloud for iOS

Version Affected: v1.6

Discovered by : Shawar Khan from Hackersray

## Proof of Concept:
[![asciicast](https://asciinema.org/a/228052.svg)](https://asciinema.org/a/228052)


[![CVE-2019-8389 Proof of Concept ](https://img.youtube.com/vi/NWLKaXwW_PU/0.jpg)](https://www.youtube.com/watch?v=NWLKaXwW_PU)
