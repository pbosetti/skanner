#!/usr/bin/env ruby 

# compile with: $  gcc -bundle linax.c -o liblinax.bundle

require "rubygems"
require "ffi"

module LinAx
  extend FFI::Library
  libs = ffi_lib "./liblinax.bundle"
  NDIFF = 2
  NDFA  = 2
  NEQ   = 8
  NPAR  = 1
  NINP  = 1
  NDISC = 0
  NIX1  = 6
  NOUT  = 3
  NCON  = 0
  NEVT  = 0

  callback :inpfn_cb, [:double, :pointer], :void
  attach_variable :dsn_zero, :double
  attach_variable :dsn_undef, :pointer
  class SolverStruct < FFI::Struct
    layout :h, :double,
      :w, :pointer,
      :iw, :pointer,
      :err, :long,
      :buf, :pointer,
      :inpfn, :inpfn_cb    
  end

  attach_function :eulerStep, :EulerStep, [:pointer, :pointer], :void
  attach_function :solverSetup, :SolverSetup, [:double, :pointer, :pointer, :pointer, :pointer, :double, SolverStruct], :void
  attach_function :solverOutputs, :SolverOutputs, [:pointer, SolverStruct], :void
  
  def LinAx::solve(t0, inpfn, dt, npts, p)
    u = FFI::MemoryPointer.new(:double, NINP)
    y = FFI::MemoryPointer.new(:double, NOUT)
    w = FFI::MemoryPointer.new(:double, 1+2*NEQ+NPAR+NDFA+NEVT)
    s = LinAx::SolverStruct.new
    out = []

    s[:w] = w
    s[:inpfn] = inpfn
    s[:err] = 0
    s[:buf] = FFI::MemoryPointer.from_string("")
    solverSetup(t0, nil, u, p, y, dt, s)
    out[0] = [s[:w].get_double(0)] + y.get_array_of_double(0,NOUT)
    (1..npts).each do |i|
      eulerStep(u, s)
      solverOutputs(y, s)
      out << [s[:w].get_double(0)] + y.get_array_of_double(0,NOUT)
    end
    out
  end

end


inpfn = Proc.new do |t, u_pt|
  if t < 1 then
    u_pt.put_array_of_double(0, [50.0])
  else
    u_pt.put_array_of_double(0, [0.0])
  end
end

p = FFI::MemoryPointer.new(:double, 1)
p.put_array_of_double(0, [1.0])
out = LinAx::solve(0.0,inpfn,0.01,200,p)
out.each do |v|
  puts v * " "
end
