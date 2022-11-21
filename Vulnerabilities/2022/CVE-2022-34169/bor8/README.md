# Description

Checks if CVE-2022-34169 is fixed on your machine. As far as I know, an exploit was found by a Google employee and not published. Since the fix is known, a small test routine was written with this to check whether the fix is available at runtime.

# Build yourself (optional)

Optional: This is only available, if you have JDK:

```
# Optional: To see if `javac` is available: `sudo updatedb && sudo locate /bin/javac`
javac Bcel.java
```

The six warnings can be ignored. The code that is flagged is used to include the Bcel library that is available in OpenJDK. If one were to include the Bcel library oneself, it would be a bit more complicated and the version would be predefined - which is not necessarily in the spirit of this exercise.

# Run

```
# `cd` into (built) folder where `Bcel.class` is, then:
java Bcel
```

# Check output


```
echo $?
```

Don't get confused with the error codes. They are the other way round. The Fix https://github.com/openjdk/jdk/commit/41ef2b249073450172e11163a4d05762364b1297?diff=unified#diff-3d2cb29a8f3fca8e2f3421d2898dce63255b850406456533cce91f26dbf8dbe2R217 throws an error with error code 1 and thus catches the security risk. If the fix is not present, the source code runs through without error with error code 0 - but too many elements are created in the object!

* Error code 1 is good! :)
* Error code 0 is bad! :(

# Results

* OpenJDK 1.8.0_72 - Bad :(
* Oracle 1.8.0_131 - Bad :(
* Oracle 1.8.0_202 - Bad :(
* Oracle 1.8.0_212 - Bad :(
* Oracle 1.8.0_341 - Good :)
* OpenJDK 1.8.0_342 - Good :)

