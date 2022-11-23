Proof of Concept for the CVE-2018-19788
=========

Ansible role to check the vulnerability tracked as [**CVE-2018-19788**](https://people.canonical.com/~ubuntu-security/cve/2018/CVE-2018-19788.html) that impacts [**PolicyKit**](https://www.freedesktop.org/wiki/Software/polkit/) version `0.115` which comes pre-installed on a wide range of Linux distributions such as Ubuntu, Red Hat, CentOs, to mention a few.

## Requirements

Minimum required ansible version **2.4.0**

Role Variables
--------------

```yaml
# The user name to be provisioned to execute the exploit
CVE_2018_19788_test_user: cve_test

# The UID assigned to CVE_2018_19788_test_user (must be equal or greater than 2147483647)
CVE_2018_19788_test_user_uid: 2147483669

# Set the proper privileged group for your distro. Please check the compatibilty matrix for the supported systems
CVE_2018_19788_privileged_group: wheel

# Flag to No to avoid checking and rendering the list of users able to leverage the CVE-2018-19788 vunerability
CVE_2018_19788_list_explotable_users: Yes

# Flag to No if you need to run further testing with the test account;
# Beaware that if your system is vulnerable, this account could become a problem
CVE_2018_19788_remove_test_user: Yes
```

Example Playbook
----------------

Including an example of how to use your role (for instance, with variables passed in as parameters) is always nice for users too:

    - hosts: all
      roles:
         - { role: d4gh0s7.cve_2018_19788 }

## License

**CC0 1.0**

## Author Information

[Francesco Cosentino](https://www.linkedin.com/in/francesco-cosentino/) - 
<fc@hyperd.sh>
