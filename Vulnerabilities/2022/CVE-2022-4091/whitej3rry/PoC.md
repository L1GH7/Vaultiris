Tiny File Manager v 2.4.7 and below are vulnerable to Session Fixation 

Reproduction Steps:

Step 1: Login to filemanager
Step 2: Intercept the response and change the cookie to any 26 character string "ThisIsDefinatelyIncorectId" or "aaaaaabbbbbbddddddeeeeeerr"
Step 3: Forward the response to browser

Verify:

Step 1: Logout from the filemanager

<img width="971" alt="Screenshot 2022-09-16 at 6 58 21 PM" src="https://user-images.githubusercontent.com/60222364/190659376-8b7e3727-a2bf-43e9-b6d1-426ada02fa19.png">

Step 2: The session id provided earlier is still valid
