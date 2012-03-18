require "yaml"
require File.expand_path(File.dirname(__FILE__) + '/skanner/version')

module Skanner
  def stub(n)
    raise ArgumentError, "Need the number of substitutions" unless n.kind_of? Numeric
    map = Map.new
    n.times do |i|
      map << Match.new(
        start:"Start of subst \##{i}",
        stop:"Stop of subst \##{i}",
        repl:"Text to be inserted \##{1}"
      )
    end
    map.to_yaml
  end
  
  def write_stub(file, n)
    File.open(file, "w") { |file| file.print(stub(n)) }
  end
  
  def check(map_file)
    map = Map.load(map_file)
    map.each do |m|
      puts "#{m.type}: #{m.args}\n\n"
    end
  end
  
  def run_on(file, map_file)
    map = Map.load(map_file)
    match = map.next
    skip = false
    File.open(file).each do |l|
      unless match
        puts l.chomp
        next
      end
      case match.type
      when :short_del
        if l.include? match.start
          match = map.next
        else
          puts l.chomp 
        end
      when :short_repl
        if l.include? match.start
          puts match.repl
          match = map.next
        else
          puts l.chomp
        end
      when :short_add
        puts l.chomp
        if l.include? match.start
          puts match.add
          match = map.next
        end
      when :long_repl
        if l.include? match.start
          skip = true
        elsif skip and l.include? match.stop
          skip = false 
          puts match.repl
          match = map.next
        elsif not skip
          puts l.chomp
        end
      when :long_del
        if l.include? match.start
          skip = true
        elsif skip and l.include? match.stop
          skip = false 
          match = map.next
        elsif not skip
          puts l.chomp
        end
      when :long_add
        puts l.chomp
        if l.include? match.start
          skip = true
        elsif skip and l.include? match.stop
          skip = false 
          match = map.next
          puts match.add
        end
      else
        puts l.chomp
      end
      # if l.include? match.stop then
      #   if match.add then
      #     puts l.chomp
      #     puts match.add
      #   else
      #     puts match.repl
      #   end
      #   skip = false
      #   match = map.next
      # elsif l.include? match.start then
      #   skip = true
      # else
      #   puts l.chomp unless skip
      # end
    end
  end
  
  class Map < Array
    def Map.load(file)
      map = Map.new
      content = File.open(file) { |file| YAML.load(file) }
      map.replace content if map.kind_of? Array
      return map
    end
    
    def initialize
      @cursor = 0
      super
    end
    
    alias :next :shift
  end #class Map
  
  class Match
    attr_reader :args
    def initialize(args={})
      @args = {:start => nil, :stop => nil, :repl => ""}.merge args
    end
    def start; @args[:start]; end
    def stop;  @args[:stop] || @args[:start]; end
    def repl;  @args[:repl]; end
    def add;   @args[:add]; end
    
    def type
      return :long_add if @args[:start] and @args[:stop] and @args[:add]
      return :short_add if @args[:start] and not @args[:stop] and @args[:add]
      return :long_repl if @args[:start] and @args[:stop] and @args[:repl]
      return :short_repl if @args[:start] and @args[:repl] and not @args[:stop]
      return :long_del if  @args[:start] and @args[:stop] and not @args[:repl]
      return :short_del if @args[:start] and not @args[:repl] and not @args[:stop]
      return :wrong
    end
  end #class Match
  
end


if __FILE__ == $0 then
  include Skanner
  file = "test.map"
  Skanner.write_stub(file, 3) unless File.exist? file
  map = Map.load(file)
  p map.next
  p map.next
  p map.next
  Skanner.run_on("cSlidingMass1_orig.c", file)
end