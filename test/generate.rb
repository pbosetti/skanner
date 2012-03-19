require "ffi_gen"

FFIGen.generate(
  ruby_module: "linax_gen",
  ffi_lib:     "liblinax.bundle",
  headers:     ["liblinax.bundle"],
  output:      "linax_gen.rb"
)