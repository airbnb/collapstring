require 'bundler/gem_tasks'
require 'rake/testtask'

require 'rake/extensiontask'

Rake::ExtensionTask.new 'collapstring'

Rake::TestTask.new do |t|
  t.libs = %w[lib test]
  t.test_files = FileList['test/**/test*.rb']
end

Rake::Task[:test].prerequisites.unshift :compile

desc 'Run tests'
task :default => :test
