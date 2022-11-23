# CVE-2022-0543
Fully featured exploit for Redis RCE through Lua Sandbox Escape vulnerability. Based on https://thesecmaster.com/how-to-fix-cve-2022-0543-a-critical-lua-sandbox-escape-vulnerability-in-redis/.

Features:
  - Automatic reverse shell (-I + -P)
  - Single command execution (-x)
  - Basic shell (Default)

TO DO:
  - Need better checks to prevent false-positives

DISCLAIMER: This script is made to audit the security of systems. Only use this script on your own systems or on systems you have written permission to exploit.
