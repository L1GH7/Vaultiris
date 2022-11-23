# CVE-2018-0202
Security & Privacy in Computing(Fall 2018) Final Group Project, clamscan vulnerability in Cisco's ClamAV 0.99.3
Members: Jay Chow, Harry Luo, Benfang Wang, Johns Hopkins University Information Security Institute

Detailed steps on how we installed this vulnerable version of ClamAV 0.9.33:

1)From https://www.clamav.net/downloads, we scrolled down and went to Previous Stable Releases -> clamav-0.99.3 and downloaded clamav-0.99.3.tar.gz(since the vulnerable versions are before 0.99.4.

2)After downloading this .tar.gz, we extracted the folder and clicked on INSTALL found within the clamav-0.99.3.

3)We ran “./configure” within the folder clamav-0.99.3.

4)We ran “make” within the folder clamav-0.99.3. This took some time.

5)We ran  “sudo make install” within the folder clamav-0.99.3.

6)Then within clamav-0.99.3/clamscan, we got clamscan, which a shell script, ready for usage.

7)Finally, we could run clamscan by "sudo clamscan <sample.pdf>".
