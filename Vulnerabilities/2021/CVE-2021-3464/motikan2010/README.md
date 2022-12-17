# CVE-2021-34646 PoC

this vulnerability detail by NVD
> Versions up to, and including, 5.4.3, of the Booster for WooCommerce WordPress plugin are vulnerable to authentication bypass via the process_email_verification function due to a random token generation weakness in the reset_and_mail_activation_link function found in the ~/includes/class-wcj-emails-verification.php file. This allows attackers to impersonate users and trigger an email address verification for arbitrary accounts, including administrative accounts, and automatically be logged in as that user, including any site administrators. This requires the Email Verification module to be active in the plugin and the Login User After Successful Verification setting to be enabled, which it is by default.

## Using

```
1. Edit "CVE-2021-34646.py" file.

2. Run script. "python CVE-2021-34646.py"

3. Access to "Authenticated URL".
```

## Reference

- Critical Authentication Bypass Vulnerability Patched in Booster for WooCommerce  
https://www.wordfence.com/blog/2021/08/critical-authentication-bypass-vulnerability-patched-in-booster-for-woocommerce/
- NVD - CVE-2021-34646  
https://nvd.nist.gov/vuln/detail/CVE-2021-34646
