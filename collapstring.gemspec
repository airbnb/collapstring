# coding: utf-8
lib = File.expand_path '../lib', __FILE__
$LOAD_PATH.unshift lib unless $LOAD_PATH.include? lib

Gem::Specification.new do |spec|
  spec.name          = 'collapstring'
  spec.version       = '0.1.0'
  spec.authors       = ['Pierre Carrier']
  spec.email         = ['pierre@gcarrier.fr']
  spec.description   = 'Collapse quoted strings in strings'
  spec.summary       = 'Collapse quoted strings in strings. Does not support JRuby.'
  spec.homepage      = 'https://github.com/airbnb/collapstring'
  spec.license       = 'MIT'

  spec.files         = `git ls-files`.split $/
  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename f }
  spec.test_files    = spec.files.grep %r{^(test|spec|features)/}
  spec.require_paths = %w[lib]

  spec.extensions = %w[ext/collapstring/extconf.rb]

  spec.add_development_dependency 'bundler', '~> 1.3'
  spec.add_development_dependency 'rake'
  spec.add_development_dependency 'rake-compiler'
end
