# legacy-rails-CVE-2020-5267-patch

[![CI](https://github.com/GUI/legacy-rails-CVE-2020-5267-patch/workflows/CI/badge.svg)](https://github.com/GUI/legacy-rails-CVE-2020-5267-patch/actions?workflow=CI)

A patch for [CVE-2020-5267](https://github.com/advisories/GHSA-65cv-r6x7-79hv) for Rails 4 and Rails 3. Upgrading Rails would definitely be better, but in the meantime if you're stuck on older versions of Rails, this provides the monkey patch noted in the security advisory packaged and tested as a gem.

## Installation

Add this line to your application's Gemfile:

```ruby
gem 'legacy-rails-CVE-2020-5267-patch'
```

And then execute:

```
$ bundle install
```
