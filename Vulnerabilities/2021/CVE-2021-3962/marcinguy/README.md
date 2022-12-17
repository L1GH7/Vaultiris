# CVE-2021-39623

Forcedentry state of the art exploit (as I read) used by NSO made it big. Libstagefright (Media Framework on Android) with OOB write on the heap (with Scudo) which can possibly own your Mobile by playing an audio file, didn't. Note: Not sure if you can do RCE with it. Leave it to experts :P

Media Framework is sandboxed as I know. So you can read/write media, make lateral movements on the mobile.


Some players are not using SimpleDecodingSource

Used stagefright utlity on LineageOS, player on Andoird 11


Here is the crash/stractrace:

Andoird 11 
![image](https://user-images.githubusercontent.com/20355405/148828359-59aed243-0c0b-4067-adee-f100937603dc.png)

LineageOS 18.1

![image](https://user-images.githubusercontent.com/20355405/148826657-a3f78600-7b98-492f-840d-b636ae898a29.png)


Will upload the reproducer later on.

### Update 1

Here is the reporoducer file. GitHub limits uploads to 25 Mb and the file is 50 Mb.

https://drive.google.com/file/d/1Obg0tzDz7gcwoEffj1RWPEAR2az4J7an/view?usp=sharing

Maybe later will release generating code. But above should let you verify it and also adjust the paylods (your might need to correct CRCs)

Exploitability

Hmmm seems like you can overwrite only 4 bytes past allocated HEAP, but it crashes .... 

Don't have now rooted phone with debugger. Maybe will check later.



