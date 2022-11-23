# Manage Engine OpManager CVE-2020-28653 Proof of Concept

This proof of concept detects whether a Manage Engine OpManager instance is vulnerable to CVE-2020-28653. Detection is performed by firing off a request containing the serialized payload to the instance. Upon the payload being deserialized, it will cause the instance to invoke a DNS Lookup.

## Installation

1. Clone the repository by running:
`git clone https://github.com/intrigueio/cve-2020-2853-poc`

2. Install the required dependencies by running:
`bundle install`

## Usage

```
bundle exec ruby CVE-2020-28653.rb [options]
    -t target                        target to exploit (including scheme) e.g http://localhost:8060
    -l listener                      DNS Listener e.g 1766x23ymcldy2ppolrnvxngc7ix6m.burpcollaborator.net
    -p /path/to/ysoserial.jar        Path to ysoserial.jar
```

## References

https://haxolot.com/posts/2021/manageengine_opmanager_pre_auth_rce/

## Credits
- [shpendk](https://github.com/shpendk)
- [m-q-t](https://github.com/m-q-t)
- [Haxolot](https://haxolot.com/)
