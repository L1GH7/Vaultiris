# Black Duck CoPilot Gradle/Travis CI Example

[![Travis CI](https://travis-ci.org/BlackDuckCoPilot/example-gradle-travis.svg?branch=master)](https://travis-ci.org/BlackDuckCoPilot/example-gradle-travis) [![Black Duck Security Risk](https://copilot.blackducksoftware.com/github/repos/BlackDuckCoPilot/example-gradle-travis/branches/master/badge-risk.svg)](https://copilot.blackducksoftware.com/github/repos/BlackDuckCoPilot/example-gradle-travis/branches/master)

Shows a working setup for using the Black Duck CoPilot integration to analyze the risk of project dependencies

## Travis CI Setup

The `.travis.yml` file has been modified to upload generated dependency data to Black Duck CoPilot:

```yaml
after_success:
  - bash <(curl -s https://copilot.blackducksoftware.com/ci/travis/scripts/upload)
```
