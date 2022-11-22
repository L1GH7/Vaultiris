require 'fiddle'
$SAFE = 1

begin
  h = Fiddle.dlopen(ENV["FOO"])
  foo = Fiddle::Function.new(h['foo'],[],Fiddle::TYPE_VOIDP)

  ptr = foo.call
  puts ptr[0, Fiddle::SIZEOF_CHAR * 255]
rescue SecurityError
  puts "CVE-2009-5147 fixed"
end
