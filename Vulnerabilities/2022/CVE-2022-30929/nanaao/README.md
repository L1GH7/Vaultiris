# CVE-2022-30929
CVE-2022-30929 POC


> [Suggested description]
> Mini-Tmall v1.0 is vulnerable to Insecure Permissions via
> tomcat-embed-jasper.
>
> ------------------------------------------
>
> [Vulnerability Type]
> Insecure Permissions
>
> ------------------------------------------
>
> [Vendor of Product]
> github;gitee
>
> ------------------------------------------
>
> [Affected Product Code Base]
> https://github.com/robin-liyong/-Mini-Tmall-:https://gitee.com/project_team/Tmall_demo?_from=gitee_search - v1.0
>
> ------------------------------------------
>
> [Affected Component]
> tomcat-embed-jasper
>
> ------------------------------------------
>
> [Attack Type]
> Remote
>
> ------------------------------------------
>
> [Impact Code execution]
> true
>
> ------------------------------------------
>
> [Attack Vectors]
> without anything
>
> ------------------------------------------
>
> [Reference]
> https://t.me/WangPanBOT?start=file96eb2dc53cc57847
>
> ------------------------------------------
>
> [Discoverer]
> jw5t

Use CVE-2022-30929.




# exp






Global search upload

After auditing, the filter of this framework only verifies user permissions, and the others are not filtered.

Others have restrictions on file types in jsp files, which can be easily bypassed with burp

## Admin avatar upload
![image-20220507165902587](https://user-images.githubusercontent.com/108649390/177091462-75342f68-55be-4ea5-b1f7-61362ddd48e9.png)

![image-20220507170257543](https://user-images.githubusercontent.com/108649390/177091519-8b5192db-c298-41c1-a2d3-55c598e66dcc.png)

![image-20220507165834527](https://user-images.githubusercontent.com/108649390/177091818-27bb8c20-5d1b-4e01-b2ff-713e7bf587a4.png)

![image-20220507170454885](C:\Users\jw5t\AppData\Roaming\Typora\typora-user-images\image-20220507170454885.png)

Three points that need to be modified, and need to intercept return packets

![image-20220507170750082](https://user-images.githubusercontent.com/108649390/177091919-bc22bf8e-5572-43f2-8b2f-c82a49ad7aa0.png)


![image-20220507170822718](https://user-images.githubusercontent.com/108649390/177091979-baaa9590-62e6-48c3-981c-945db85c943f.png)


get filename 09820699-ecd5-4fcd-876a-07f8a46987be.jsp


After saving, according to the image url address of the code audit

/tmall/res/images/item/adminProfilePicture/

do a splicing

get /tmall/res/images/item/adminProfilePicture/09820699-ecd5-4fcd-876a-07f8a46987be.jsp


![image-20220507171206561](https://user-images.githubusercontent.com/108649390/177092112-9b48a8dd-3581-483c-b13c-51303dde9f98.png)

![image-20220507171254740](https://user-images.githubusercontent.com/108649390/177092233-c7286153-c71e-4e26-b19e-b248c9acc5ff.png)

## Front desk Tmall - user change avatar

![image-20220507165816036](https://user-images.githubusercontent.com/108649390/177092301-d02043a5-c1aa-4404-95b1-490a4676349e.png)

Register an account first,and registration successing later

![image-20220507172054500](https://user-images.githubusercontent.com/108649390/177092540-18f70e60-244e-4b31-be01-bf84290de2b0.png)

![image-20220507172508860](https://user-images.githubusercontent.com/108649390/177092602-4def6bcb-e5dc-40e8-bc55-fa9ca312865d.png)


![image-20220507172505871](https://user-images.githubusercontent.com/108649390/177092664-7076f7d8-b8f0-4518-9f8d-1330f34d9f15.png)

obtained after splicing

/tmall/res/images/item/userProfilePicture/e568b7c4-7954-4a18-ab65-707198332d21.jsp

accessing

![image-20220507172635363](https://user-images.githubusercontent.com/108649390/177092739-a3d4d337-c56e-45e9-9d4b-dad59d5ddf93.png)

![image-20220507172740787](https://user-images.githubusercontent.com/108649390/177092783-3adbb33e-c1c1-4c1d-afb6-c314efac10da.png)

## Upload product image-ajax and upload product type image-ajax (there are two file uploads for the same function point)

![image-20220507165802861](https://user-images.githubusercontent.com/108649390/177092887-d496278d-709b-4d21-98b7-26228a7202af.png)

![image](https://user-images.githubusercontent.com/108649390/177092934-6950640b-efda-449b-96bc-f6f1460bca21.png)

![image-20220507173336747](https://user-images.githubusercontent.com/108649390/177093002-5bf3cc18-7aa6-4c85-9113-de936c7657bd.png)

![image-20220507173605690](https://user-images.githubusercontent.com/108649390/177093047-9080dfaf-dea7-491b-8c66-57653679011e.png)

838d284e-e625-48b8-bbc7-8275367d5601.jsp

![image-20220507173704169](https://user-images.githubusercontent.com/108649390/177093106-51d9d905-ba0c-4ea5-a520-2e609047ab56.png)




