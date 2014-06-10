lib = File.expand_path '../lib', __FILE__
$LOAD_PATH.unshift lib unless $LOAD_PATH.include? lib

require 'rake/testtask'

if RUBY_PLATFORM == 'java'
  require 'rake/javaextensiontask'
  Rake::JavaExtensionTask.new 'collapstring'
else
  require 'rake/extensiontask'
  Rake::ExtensionTask.new 'collapstring'
end

Rake::TestTask.new do |t|
  t.libs = %w[lib test]
  t.test_files = FileList['test/**/test*.rb']
end

Rake::Task[:test].prerequisites.unshift :compile

task :fuzz => :compile do
  require 'collapstring'

  sizes = [10, 100, 1000, 10_000]
  loop do
    puts Collapstring.collapse! Collapstring.fuzz(sizes.sample)
  end
end

desc 'Run tests'
task :default => :test
