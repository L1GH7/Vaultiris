Exploit for CVE-2008-2019 - Simple Machines Form (SMF) 1.1.4 and below.

SMF uses an audio captcha made up of a woman's voice saying each letter of the alphabet. The user, (who could be blind) needs to type in these letters in order to pass the challenge response. The vulnerability identified by CVE-2008-2019 is actually bypass a patch introduced to fix CVE-2007-3308.  This exploit just used simple string compare (https://www.securityfocus.com/archive/1/471641/100/0/threaded).  To prevent a string compare from working, random bits where flipped in the pulse code modulation (PCM) that makes up the WAV files - which changes the value enough to prevent a string compare - but sounds like slight signal noise to a human.

Fuzzing matching using a hamming distance calculation or levenshtein distance can be used to compare the pulse code modulation (PCM) from the orignal audio source to the composite generated challenge response of the audio captcha.  Using a hamming distance calculation to look for close matches based off of a static heuristic, a cut of a few hundred vs a few thousand is enough to determine the answer needed to solve the captcha challenge response.

A hamming distance calculation works here because the noise introduced by Simple Machine Form (SMF) was only flipping random bits,  insertions or deletions where not used.  A later revision of the captcha added short pauses to the PCM, these insertions of white space could be identified with levenshtein whereas even a small insertion would throw off a hamming distance calculation. 


posted:
https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2008-2019
https://www.exploit-db.com/exploits/32462
https://simplemachines.org/community/index.php?P=c3696c2022b54fa50c5f341bf5710aa3&topic=236816.0
