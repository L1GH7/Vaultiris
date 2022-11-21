# CVE-2022-34919

Contensis Content Management System prior to version 15.2.1.79 has a flaw in their 'Classic' interface that allows an unauthenticated user to upload and approve arbitrary files. By uploading an aspx file, it is possible to execute commands in the context of the web server.

This vulnerability was discovered in March 2022 and patched by Zengenti in their release on April 8, 2022.


Steps to reproduce:

1:

The initial file upload is performed with a POST request to the following endpoint:

https://[example].cloud.contensis.com/REST/UI/Wizards/CreateContent/UploadFile?folderId=1&projectId=1&context=%7B%22UserID%22%3A1%2C%22LanguageID%22%3A1%2C%22SecurityToken%22%3A%22%22%7D

The context parameter indicates which user is uploading the file; in this case, UserId 1, System Administrator. The SecurityToken parameter is required but may be blank. This request (screenshot 1) generates a temporary file on the server with a UUID. In this example, I used a webshell available at: https://github.com/tennc/webshell/blob/master/fuzzdb-webshell/asp/cmd.aspx
![screenshot1](https://user-images.githubusercontent.com/63816088/184942299-80658637-11d7-4acf-a271-731dbfd46dd5.jpg)


2:

The file must then be approved with a POST request to the following endpoint:

https://[example].cloud.contensis.com/REST/UI/Wizards/CreateContent/CreateContent

This request (screenshot 2) will include the path to the temporary file from the initial request and include parameters to approve for publishing and modify the content type to bypass file restrictions. In this instance, ContentTypeId is set to 43, indicating it is an HTML document, rather than ASPX.
The file is then available on the ‘preview’ and ‘live’ domains below:

https://preview-[example].cloud.contensis.com/cmd2.aspx

https://iis-live-[example].cloud.contensis.com/cmd2.aspx
![screenshot2](https://user-images.githubusercontent.com/63816088/184942304-02d05daa-ef5f-4e4f-a9ec-10a845d50f87.jpg)


3:

Be responsible and only test this on authorized hosts (screenshot 3)![screenshot3](https://user-images.githubusercontent.com/63816088/184942307-0bc6b822-c5de-4890-a535-301f3ba93157.jpg)


