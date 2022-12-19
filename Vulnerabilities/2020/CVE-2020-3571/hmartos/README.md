# CVE-2020–35717

[zonote](https://github.com/zonetti/zonote) allows XSS via crafted note, with resultant Remote Code Execution (because Node.js integration is enabled).

## Steps to exploit the vulnerability

- Download any zonote [affected version](https://github.com/zonetti/zonote/releases/tag/v0.4.0)
- Open zonote app
- Import [xss-rce.znt](./xss-rce.znt) in zonote via Menu > Open
- Hover over the different links in imported notes

<img src="./xss-rce.gif" width="95%">

## Disclosure Timeline

- 2020-12-26 Issue discovered and contact with the owner
- 2020-12-26 Owner express his intention of not maintaining the repository nor fixing the vulnerability
- 2021-01-01 Public disclosure of the vulnerability
