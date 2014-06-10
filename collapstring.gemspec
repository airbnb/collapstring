# coding: utf-8

Gem::Specification.new do |spec|
  spec.name          = 'collapstring'
  spec.version       = '0.1.2'
  spec.authors       = ['Pierre Carrier']
  spec.email         = ['pierre@gcarrier.fr']
  spec.description   = 'Collapse quoted strings in strings'
  spec.summary       = 'Collapse quoted strings in strings. Does not support JRuby.'
  spec.homepage      = 'https://github.com/airbnb/collapstring'
  spec.license       = 'MIT'

  spec.files         = %w(LICENSE.txt README.md)
  spec.test_files    = %w(test/collapstring/tests.rb)
  spec.require_paths = %w(lib)

  if RUBY_PLATFORM == 'java'
    spec.platform    = 'java'
    spec.files      << 'lib/collapstring.jar'
  else
    spec.files      += %w(ext/collapstring/extconf.rb ext/collapstring/collapstring.c)
    spec.extensions  = %w(ext/collapstring/extconf.rb)
  end

  spec.add_development_dependency 'bundler',       '~> 1.3'
  spec.add_development_dependency 'rake',          '~> 10.3.2'
  spec.add_development_dependency 'rake-compiler', '~> 0.9.2'
end
