# CVE-2021-24545

WP HTML Author Bio <= 1.2.0 - Author+ Stored Cross-Site Scripting

The plugin does not sanitise the HTML allowed in the Bio of users, allowing them to use malicious JavaScript code, which will be executed when anyone visit a post in the frontend made by such user. As a result, user with a role as low as author could perform Cross-Site Scripting attacks against users, which could potentially lead to privilege escalation when an admin view the related post/s.

# Proof of Concept

Login as an author, add the following payload in the Biographical Info field of the profile:
```<script>alert(/XSS/)</script>``` 

Then view post made by the author to trigger the XSS 

# Video POC:
https://www.youtube.com/watch?v=fhzJZ5hSI-g
