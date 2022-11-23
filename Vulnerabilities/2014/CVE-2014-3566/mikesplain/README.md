# CVE-2014-3566-poodle

This cookbook will error out your chef run if your server is vulnerable to [CVE-2014-3566](http://web.nvd.nist.gov/view/vuln/detail?vulnId=CVE-2014-3566).

Although this cookbook can be used to watch for failed chef runs, it is also a proof of concept.

## Supported Platforms

- Ubuntu 12.04
- Centos-6.4

## Attributes

<table>
  <tr>
    <th>Key</th>
    <th>Type</th>
    <th>Description</th>
    <th>Default</th>
  </tr>
  <tr>
    <td><tt>['CVE-2014-3566-poodle']['ports']</tt></td>
    <td>Array</td>
    <td>Ports to check for vulnerability</td>
    <td><tt>['443','8443']</tt></td>
  </tr>
</table>

## Usage

### CVE-2014-3566-poodle::default

Include `CVE-2014-3566-poodle` in your node's `run_list`:

```json
{
  "run_list": [
    "recipe[CVE-2014-3566-poodle::default]"
  ]
}
```

## Testing
This cookbook includes a psuedo test suite.  The first 2 suites should pass as they test a blank instance and an instance with defaunt nginx.  The third squite should fail on all platforms as port 443 ssl is opened in Nginx.

## License and Authors

Author:: Mike Splain (<mike.splain@gmail.com>)
