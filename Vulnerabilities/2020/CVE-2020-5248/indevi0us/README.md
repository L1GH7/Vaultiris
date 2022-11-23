# CVE-2020-5248
Proof of Concept (PoC) for CVE-2020-5248.

# Summary
GLPI before before version 9.4.6 has a vulnerability involving a default encryption key. GLPIKEY is public and is used on every instance. This means anyone can decrypt sensitive data stored using this key. It is possible to change the key before installing GLPI. But on existing instances, data must be reencrypted with the new key. Problem is we can not know which columns or rows in the database are using that; espcially from plugins. Changing the key without updating data would lend in bad password sent from glpi; but storing them again from the UI will work.

# PoC:
Replace "INSERT_COIN" string with the one that you would like to decrypt at https://github.com/indevi0us/CVE-2020-5248/blob/main/decrypt_any.php, then execute the .php script.
