# Metabase_Nmap_Script
It is a nmap script for metabase vulnerability (CVE-2021-41277)

USAGE

-- nmap -Pn -n -p443 --script metabase.nse <target>

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
