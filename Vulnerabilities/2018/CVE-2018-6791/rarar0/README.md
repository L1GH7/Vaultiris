### Thoughts on KDE vFAT vuln.

#1: The scope isn't as large as I first imagined. 

Most systems are probably updated to newer versions of KDE or are running a different GUI.
I'd have to look at some actual statistics to get a proper number though. 
Getting physical access to a machine is already a job in itself, so having your exploit not work simply because the target machine is running a different GUI than you were expecting seems like a pain.

#2: 11 Character Limit on vFAT Label is frustrating.

After some quick browsing of twitter, reddit, and other forums, I was unable to find a way to bypass this.
Current hypothesis on how to circumvent this would be to use fdisk to edit the label as a different type while keeping the filesystem as vFAT.
Other option would be use NTFS instead, I found one source which said NTFS may have been the original cause of this vuln. (In which case, the label size is much larger)
