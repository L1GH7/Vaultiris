# iOS 11 Cookie Set / Expire Issue

There appears to be an intermittent issue with setting and expiring cookies in iOS 11 home screen web apps. Two erroneous behaviors can be observed:

* When a cookie is set and then the app is closed and re-opened, the cookie may no longer be present upon re-opening the app

* When a cookie is expired and then the app is closed and re-opened, the cookie may still be present upon re-opening the app

#### Steps to reproduce:

1. Launch https://bencompton.github.io/ios11-cookie-set-expire-issue/index.html in Mobile Safari and add to home screen

2. Click the "log in" button

3. Close the app and re-open it. At this point you may find that you are no longer logged in, or you may find that you are still logged in. The issue is intermittent.

4. If you are still logged in, try clicking the "log out" button.

5. Close the app and re-open it. You may find that you have been logged out, or you may still be logged in.

If you don't notice any issues right away, repeat steps 3 - 5 until you either are kept logged in after logging out, or you are logged out when you should be logged in.

