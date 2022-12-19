# CVE-2020-27358 / CVE-2020-27359

Exploitation steps for **CVE-2020-27358 and CVE-2020-27359**


## CVE-2020-27358 

An issue was discovered in REDCap 8.11.6 through 9.x before 10. The messenger's CSV feature (that allows users to export their conversation threads as CSV) allows non-privileged users to export one another's conversation threads by changing the thread_id parameter in the request to the endpoint Messenger/messenger_download_csv.php?title=Hey&thread_id={THREAD_ID}. 

#### Testing Steps 
1. Login as any user. 
2. Navaigate to a existing message thread.
3. Click option the export as CSV.
4. Intercept the request and change the thread_id paramter to any valid thread ID.



## CVE-2020-27359 

A cross-site scripting (XSS) issue in REDCap 8.11.6 through 9.x before 10 allows attackers to inject arbitrary JavaScript or HTML in the Messenger feature. It was found that the filename of the image or file attached in a message could be used to perform this XSS attack. A user could craft a message and send it to anyone on the platform including admins. The XSS payload would execute on the other account without interaction from the user on several pages. 

#### Testing Steps 
1. Login as any user. 
2. Navaigate to a existing message thread.
3. Click add an image and upload an image.
4. Intercept the request and change filename to `\"><svg/onload=alert(document.domain)>`
5. Send the message.

You can send the XSS payload to any user on the platform and it will execute without interaction. 
