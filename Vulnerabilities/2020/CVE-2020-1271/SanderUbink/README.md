# CVE-2020-12712: SOS JobScheduler decryption of stored password
The script in this repository allows you to decrypt the password(s) stored in the (S)FTP configuration file for a JobScheduler instance. 

## Description
SOS JobScheduler is a tool for remote system administration that allows users to call maintenance scripts via a web interface. The tool places the maintenance scripts on the remote systems by means of (S)FTP. It allows the user to save profiles for these connections, in which the password for the (S)FTP connection is optionally stored. When the user chooses to store the password with the profile, it is encrypted using the name of the profile as the encryption key. Since the name of the profile is stored in the same configuration file, the plaintext (S)FTP password can trivially be recovered. The encryption algorithm used is Triple DES (3DES) with three keys, requiring a key length of 24 bytes. The profile name is padded to this length to create the key. Finally, the encrypted password gets base64 encoded before being stored in the configuration file.

## Usage
python jobscheduler-decrypt.py [encrypted password in base64] [profile name]
