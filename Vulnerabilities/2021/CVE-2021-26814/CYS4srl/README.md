# CVE-2021-26814

A simple python PoC to exploit **CVE-2021-26814** and gain RCE on **Wazuh Manager** (v.4.0.0-4.0.3) through the API service.

To run it, simply make the .py script executable and launch it with the required parameters.

```
PoC.py [-h] -user USERNAME -pwd PASSWORD -lip SRCIP -lport SRCPORT -tip
              DESTIP -tport DESTPORT
```

**WARNING**: the `wazuh-apid.py` file on the manager **WILL BE CHANGED!** In order to restore the original version after the exploit is completed, a local file called `backup.py` is created.

## Authors

**Davide Meacci** - [Twitter](https://twitter.com/WickdDavid)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
