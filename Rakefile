require 'bundler/gem_tasks'
require 'rake/testtask'

require 'rake/extensiontask'

Rake::ExtensionTask.new 'collapstring'

Rake::TestTask.new do |t|
  t.libs = %w[lib test]
  t.test_files = FileList['test/**/test*.rb']
end

Rake::Task[:test].prerequisites.unshift :compile

task :fuzz => :compile do |t|
  require 'collapstring'

  sizes = [10, 100, 1000, 10_000]
  loop do
    puts Collapstring.collapse! Collapstring.fuzz(sizes.sample)
  end
end

desc 'Run tests'
task :default => :test
