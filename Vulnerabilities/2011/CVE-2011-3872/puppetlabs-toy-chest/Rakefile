require 'rake'
require 'fileutils'

begin
  require 'rspec/core/rake_task'
  HAVE_RSPEC = true
rescue LoadError
  HAVE_RSPEC = false
end

task :default => [:build]

desc "Build Puppet Module Package"
task :build do
  system("gimli README*.markdown")
  system("puppet-module build")
end

desc "Clean the package directory"
task :clean do
  FileUtils.rm_rf("pkg/")
end

if HAVE_RSPEC then
  desc 'Run RSpec'
  RSpec::Core::RakeTask.new(:test) do |t|
    t.pattern = 'spec/{unit}/**/*.rb'
    t.rspec_opts = ['--color']
  end

  desc 'Generate code coverage'
  RSpec::Core::RakeTask.new(:coverage) do |t|
    t.rcov = true
    t.rcov_opts = ['--exclude', 'spec']
  end
end
