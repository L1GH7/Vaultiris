require 'socket'

http_server = TCPServer.new(4444)
c = http_server.accept()
puts "Received HTTP connection!"

puts c.recv(1024)
c.print "HTTP/1.1 200 OK\r\n"
c.print "Connection: keep-alive\r\n"
c.print "\r\n"
c.print "This is a test file!"

sleep()
