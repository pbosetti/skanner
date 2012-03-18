# -*- encoding: utf-8 -*-
require File.expand_path('../lib/skanner/version', __FILE__)

Gem::Specification.new do |gem|
  gem.authors       = ["Paolo Bosetti"]
  gem.email         = ["paolo.bosetti@ing.unitn.it"]
  gem.description   = %q{Simple command to perform automated edits on text files}
  gem.summary       = %q{Simple command to perform automated edits on text files}
  gem.homepage      = ""

  gem.files         = `git ls-files`.split($\)
  gem.executables   = gem.files.grep(%r{^bin/}).map{ |f| File.basename(f) }
  gem.test_files    = gem.files.grep(%r{^(test|spec|features)/})
  gem.name          = "skanner"
  gem.require_paths = ["lib"]
  gem.version       = Skanner::VERSION
  gem.add_runtime_dependency("commander", ">= 1.1.1")
end
