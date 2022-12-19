
lib = File.expand_path("../lib", __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require "json_cve_2020_10663/version"

Gem::Specification.new do |spec|
  spec.name          = "json_cve_2020_10663"
  spec.version       = JSON_CVE_2020_10663::VERSION
  spec.authors       = ["Henning Koch"]
  spec.email         = ["henning.koch@makandra.de"]

  spec.summary       = 'Workaround for CVE-2020-10663'
  spec.description   = 'Workaround for CVE-2020-10663 for users who cannot upgrade to json 2.3+'
  spec.homepage      = "https://github.com/rails-lts/json_cve_2020_10663"
  spec.license       = "MIT"

  spec.files         = `git ls-files -z`.split("\x0").reject do |f|
    f.match(%r{^(test|spec|features)/})
  end
  spec.bindir        = "exe"
  spec.executables   = spec.files.grep(%r{^exe/}) { |f| File.basename(f) }
  spec.require_paths = ["lib"]

  spec.add_runtime_dependency 'json', '>=1.7.7', '<2.3'

  spec.add_development_dependency "bundler", "~> 1.16"
  spec.add_development_dependency "rake", "~> 10.0"
  spec.add_development_dependency "rspec", "~> 3.0"
end
