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
  alphabet = [' ', '\'', '\"', '\\']

  itr = 0

  begin
    loop do
      string = sizes.sample.times.inject('') { |s| s << alphabet.sample }
      Collapstring.collapse! string
      putc '.'
      itr += 1
    end
  rescue Interrupt
    puts itr
  end
end

desc 'Run tests'
task :default => :test
