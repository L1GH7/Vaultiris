Code to support my [CVE-2022-28219 analysis](https://attackerkb.com/topics/Zx3qJlmRGY/cve-2022-28219/rapid7-analysis).

To execute, with Ruby and Rubygems installed:

```
gem install httparty
ruby ./manageengine-poc.rb <target> <port> <domain> <your ip>
```

This is designed as a proof of concept, not a stable exploit. It only runs calc. :)
