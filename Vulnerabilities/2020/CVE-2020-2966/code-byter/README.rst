===============
Macally WIFISD2-2A82
===============

Writeup for CVE-2020-29669 
by Maximilian Barz (Silky) and Daniel Schwendner (code-byter)

==========================

This is a writeup of exploiting the Macally WIFISD2-2A82 Travel Router (Firmware version: 2.000.010). The Guest user is able to reset its own password. This process has a vulnerability which can be used to take over the administrators account and results in shell access. As the admin user may read the /etc/shadow file, the password hashes of each user (including root) can be dumped. The root hash can be cracked easily which results in a complete system compromise. All this from the guest account which is meant to be given to guests. 

.. image:: images/router.jpg
  :width: 1200

CVSS 3.1 Base Score: 8.7
------------------------

Affected file: ``/protocol.csp``

.. image:: images/base.png
  :width: 1200

Walkthrough / PoC:
==================

Step 1:
-------

Login as guest account on the web interface. Default password for guest and admin is blank. 

.. image:: images/web_login.png
  :width: 1400

| 

When authenticated successfully a similar screen should appear.

.. image:: images/dashboard.png
  :width: 1200

|
|
|

Step 2:
-------

Navigate to the User manager in the settings menu, where you can change the password of your current user.

.. image:: images/password_change.png
  :width: 1800


Guest is able to reset his own password, fill in the blank fields and capture the request in BurpSuite


.. image:: images/burp_1.png
  :width: 2100


Change the value of name to admin and forward the request.

.. image:: images/burp_2.png
  :width: 2100

In the web interface, a pop-up box will appear saying "Password changed successfully"

.. image:: images/password_changed.png
  :width: 1400

|
|

Step 3:
-------

Login as admin via telnet with the previously set password.

.. image:: images/telnet_login.png
  :width: 1200

Admin is able to read ``/etc/shadow`` file exposing the root hash.

.. image:: images/etc_password.png
  :width: 1200


Exploit
=======

The whole exploitation process is automated with a python script. To spawn a root shell (or crack the root hash) run ``macally_exploit.py``.

.. code:: python

  python3 macally_exploit.py 10.10.10.254

.. image:: images/exploit.png
  :width: 1600

CVE MITRE entry: `https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-29669`.

.. footer::

    Maximilian Barz (OSCP), Email: mbzra@protonmail.com, Twitter: S1lky_1337

    Daniel Schwendner, Email: officialcodebyter@gmail.com, Instagram: code_byter
    

