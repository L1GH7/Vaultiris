require 'rubocop/rake_task'
require 'foodcritic'
require 'emeril/rake'


# Style tests. Rubocop and Foodcritic
namespace :style do
  # desc 'Run Ruby style checks'
  # Rubocop::RakeTask.new(:ruby)

  desc 'Run Chef style checks'
  FoodCritic::Rake::LintTask.new(:chef) do |t|
    t.options = {
      fail_tags: ['any'],
      tags: []
    }
  end
end

desc 'Run all style checks'
task style: ['style:chef']

# Default
task default: ['style']

desc 'Run default tests'
task test: ['style']
