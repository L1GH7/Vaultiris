CVE-2009-5147 and CVE-2015-7551 PoC

to run it:

````
make foo
export FOO=./foo.so
ruby -v foo.rb
````

with different versions of ruby you should get different results:

````
vpereira@kimura:~/poc> rvm use 2.1.7
vpereira@kimura:~/poc> ruby -v foo.rb
ruby 2.1.7p400 (2015-08-18 revision 51632) [x86_64-linux]
some trash fom your environment variables...

vpereira@kimura:~/poc> rvm use 2.3.0
vpereira@kimura:~/poc> ruby -v foo.rb
ruby 2.3.0p0 (2015-12-25 revision 53290) [x86_64-linux]
CVE-2009-5147 fixed
````

The file ```foo2.rb``` is for the CVE-2015-7551
