# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'spiro/version'

Gem::Specification.new do |spec|
  spec.name          = "spiro"
  spec.version       = Spiro::VERSION
  spec.authors       = ["Raph Levien", "George Williams", "Simon George"]
  spec.email         = ["simon@sfcgeorge.co.uk"]
  spec.summary       = %q{Ruby bindings for Spiro}
  spec.description   = %q{Bundle the C library.}
  spec.homepage      = ""
  spec.license       = "GNU GPL 3+"

  spec.files         = `git ls-files -z`.split("\x0")
  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename(f) }
  spec.test_files    = spec.files.grep(%r{^(test)/})
  spec.require_paths = ["lib"]
  spec.extensions    = "ext/spiro/extconf.rb"


  spec.add_development_dependency "bundler", "~> 1.7"
  spec.add_development_dependency "rake", "~> 10.0"
  spec.add_development_dependency "hoe-debugging", "~> 1.2"
  spec.add_development_dependency "hoe", "~> 3.13"
end
