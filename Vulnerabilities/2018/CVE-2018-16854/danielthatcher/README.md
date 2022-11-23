This repository contains the files used in finding and exploiting two moodle bugs, MSA-18-0020 (CVE-2018-16854) and MSA-19-0004 (CVE-2019-3847), which leverage the ability for users to add JavaScript to their own dashboards. MSA-18-0020 relies on CSRF on the login form, whereas MSA-19-0004 requires an administrator to impersonate a user.

More details can be found in [this blog post](https://medium.com/@daniel.thatcher/obtaining-xss-using-moodle-features-and-minor-bugs-2035665989cc).

## Fixed moodle versions
* MSA-18-0020 (CVE-2018-16854): 3.6, 3.5.3, 3.4.6, 3.3.9, and 3.1.15.
* MSA-19-0004 (CVE-2019-3847): 3.6.3, 3.5.5, 3.4.8, and 3.1.17.
