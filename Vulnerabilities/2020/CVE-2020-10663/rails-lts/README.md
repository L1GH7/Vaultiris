# Workaround for CVE-2020-10663 (vulnerability in json gem)

The `json` gem has a security vulnerability [CVE-2020-10663](https://www.ruby-lang.org/en/news/2020/03/19/json-dos-cve-2020-10663/). When parsing certain JSON documents, the json gem can be coerced into creating arbitrary objects in the target system.

Users are strongly advised to upgrade to `json` 2.3.0 or later.

For users who cannot upgrade their version of `json`, this gem (`json_cve_2020_10663`) monkey-patches your `json` version against against CVE-2020-10663.

## Requirements

- Ruby 1.8.7 or later
- `json` 1.7.7 or later, but earlier than 2.3.0.

## Installation

Add this line to your application's `Gemfile`:

```ruby
gem 'json_cve_2020_10663'
```

And then execute:

```
$ bundle
```

Or install it yourself as:

```
$ gem install json_cve_2020_10663
```

Require the gem to patch the `json` gem:

```ruby
require 'json_cve_2020_10663'
```

Note that Rails automatically requires all gems in your `Gemfile` when your app is booted.

You can verify that the patch was applied by running the following code from your application environment:

```ruby
JSON::GenericObject.json_creatable = true
JSON('{"json_class":"JSON::GenericObject"}').class
```

If this returns `Hash`, the patch was applied correctly. If it returns `JSON::GenericObject`, the patch was not loaded.


## Development

After checking out the repo, run `bin/setup` to install dependencies. Then, run `rake spec` to run the tests. You can also run `bin/console` for an interactive prompt that will allow you to experiment.

To install this gem onto your local machine, run `bundle exec rake install`. To release a new version, update the version number in `version.rb`, and then run `bundle exec rake release`, which will create a git tag for the version, push git commits and tags, and push the `.gem` file to [rubygems.org](https://rubygems.org).

## License

The gem is available as open source under the terms of the [MIT License](https://opensource.org/licenses/MIT).
