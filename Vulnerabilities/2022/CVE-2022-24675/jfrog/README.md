# CVE-2022-24675 tools

## Usage instructions

```
cve2022_24675.sh root_folder
```

Will recursively walk the specified folder and report Golang binaries which use `pem.Decode` function, or are stripped (in which case function name will be missed and the check will not be effective).