inetutils hasn't been updated in 5 years, and neither has the official Arch package. A patch has been released that addresses CVE-2019-0053 (buffer overflow exploit from the use of sprintf instead of snprintf), but there's no official release in sight.

Honestly, there's a lot more wrong with using telnet than just buffer exploits, but this was a high urgency issue that took 10 minutes to address. Waiting on a response from the official maintainers right now, but this repo will do for the meantime.
