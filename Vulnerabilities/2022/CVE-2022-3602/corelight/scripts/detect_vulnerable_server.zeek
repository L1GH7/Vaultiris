module CVE20223602;

export {
	redef enum Notice::Type += { CVE_2022_3602_Vulnerable_Server };
}

global vulnerable = /OpenSSL\/3\.0\.[0-6]/;
global suppress_vuln = 3600 sec;

event http_header(c: connection, is_orig: bool, original_name: string,
    name: string, value: string)
	{
	if ( is_orig || c$http$trans_depth > 1 )
		return;
	if ( name == "SERVER" && vulnerable in value )
		NOTICE([$note=CVE_2022_3602_Vulnerable_Server, $conn=c, $identifier=cat(
		    c$id$resp_h, value), $msg="Potential OpenSSL CVE_2022_3602 vulnerable server version (v3.0.0-3.0.6)",
		    $suppress_for=suppress_vuln, $sub=fmt(
		    "SERVER value in HTTP header = '%s'", value)]);
	}
