## POC for CVE-2020-28948 & CVE-2020-28949

The files here contain PoC for CVE-2020-28948 & CVE-2020-28949 to achieve remote exploit

### The server

The server folder contains a simple upload server which uses the vulnerable Archive_Tar library, located in `server/Archive`. The server accepts a Tar archive from the user, extracts and store it in the `server/uploads/` folder.

To start the server with the vulnerable library:
```sh
cd server
make build
make start
```

To start the server with the patched library:
```sh
cd server
make build-patched
make start-patched
```

Access the remote server through http://localhost:8080

### CVE-2020-28948 (PHAR deserialisation attack)

1. Navigate to corresponding PoC folder.
2. Specify target for arbitrary file deletion, by modifiying the `$delete_target` in `create_phar.php`
    - To view confidential `secret.md` file, delete `.htaccess` file
    - To do DoS, delete `index.html`
3. Create `exploit.tar`
    ```
    make create_exploit
    ```
4. Upload `exploit.tar` to remote server
5. Observe the file deletion on the server.
    - Can access http://localhost:8080/uploads/secret.md if `.htaccess` is deleted.

### CVE-2020-28949 (PHAR inclusion attack)

1. Navigate to corresponding PoC folder.
2. Create `exploit.tar`
    ```
    make create_exploit
    ```
3. Upload `exploit.tar` to remote server
4. Observe that `shell.php` is uploaded.
    - Can access http://localhost:8080/shell.php
5. Achieve some RCE (eg. execute `whoami` on server)
    - http://localhost:8080/shell.php?cmd=whoami
