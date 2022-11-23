# Stegcrack

Stegcrack is an open-source program for exploiting a vulnerability in Steghide (specifically CVE-2021-27211).

Stegcrack detects whether a given file contains data hidden with Steghide, and can sometimes fully extract the hidden data, all with no password.


## Installation

Clone this repository
```bash
git clone https://github.com/b4shfire/stegcrack
```

Move into the source directory
```bash
cd stegcrack/src
```

Compile the program
```bash
clang++ main.cc utils.cc file_handling.cc ui.cc Extractor.cc -ljpeg -pthread -lz -std=c++11 -O2 -o ../stegcrack
```

Move out of the source directory
```bash
cd ..
```


## Usage
```
./stegcrack <filepath> <number of threads> (default: 4)
```

### Example Usage
```bash
./stegcrack flowers.jpg
./stegcrack ../images/rocket.jpg 8
```


## Dependencies

- jpeglib
- zlib


## How it Works

To hide data inside a cover file, Steghide first turns the provided password into a 32-bit seed.

The seed is created as follows:
- The password is MD5 hashed
- This 128-bit hash is split into four 32-bit chunks
- These four chunks are XORed together to create a single 32-bit seed

This seed is then used to generate a sequence of positions in the file where data is to be hidden.

The flaw in this method is that no matter how complex the password is, it must generate one of the 2^32 possible seeds. This means that the password can be bypassed altogether by brute-forcing the seed variable.

Once the seed has been generated, Steghide first embeds metadata (magic bytes, Steghide version, data size...) into the cover file.

This metadata can be used to determine whether or not each seed has been used to hide data. For example, the first 3 bytes embedded are always 0x73 0x68 0x8d (very nearly the ascii string "shm" - StegHide Magic). If a given seed does not generate these bytes, it can be discarded as it has definitely not been used to hide data. This continues until either all seeds are discarded, or a valid seed is found. A valid seed indicates hidden data.

By default, Steghide encrypts data using the full password before embedding. This means that in most cases, Stegcrack will only be able to extract this encrypted data. However, if encryption was disabled during embedding, Stegcrack can fully extract the hidden data in plain text.
