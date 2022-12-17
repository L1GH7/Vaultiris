module CVE_2021_38647;
# A Zeek package that detects attempts to exploit CVE-2021-38647 (Windows OMI remote RCE)
# References:
# https://www.wiz.io/blog/omigod-critical-vulnerabilities-in-omi-azure
# https://msrc.microsoft.com/update-guide/vulnerability/CVE-2021-38647
# https://corelight.com/blog/detecting-cve-2021-38647-omigod
# Author: Ben Reardon, Research Team @Corelight. ben.reardon@corelight.com, @benreardon


redef enum Notice::Type += {
        EXPLOIT_ATTEMPT,
        EXPLOIT_REQUEST,
        EXPLOIT_RESPONSE
    };

redef record HTTP::Info += {
        CVE_2021_38647_candidate: bool &default=F;
    };

# The default OMI ports, add ports from your local OMI implementation if required. 
option OMI_ports = set(1270/tcp, 5985/tcp, 5986/tcp);

# To assist with IR triage for EXPLOIT_REQUEST and EXPLOIT_RESPONSE notices, 
# The 'sub' field will include the first 'bytes_of_data_in_notice' in the notice.
# Set this to a high number to collect all of the payload - the default of 10000 should be high enough to capture all relevant data.  
option bytes_of_data_in_notice = 10000;

# To assist with IR triage for EXPLOIT_ATTEMPT notices, set to T,
# as the client header names and values will appear in the EXPLOIT_ATTEMPT notice 'sub' field. 
option raise_seperate_notice_for_missing_auth_header = T;

option wsman_uri_pattern_post = /^\/wsman/i | /^\/$/ ;

option xml_artifacts = /xmlns/i | /xmlsoap.org/i | /Envelope/i | /command/i | /encoded/i ;

# Use this UA whitelisting very sparingly - to silence False Positives from your own scanner or legitimate system. 
# Remember, an attacker can simply spoof this user-agent. example: 
# option user_agent_whitelist = /^Microsoft WinRM Client$/;
option user_agent_whitelist = /<ARE_YOU_SURE?_replace_this_with_user_agent_to_whitelist>/ ;


# Returns T if the given header name/value combination indicates that
# this request is not a candidate, F otherwise.
function header_disqualifies(hname: string, hval: string): bool
	{
	# The payload is XML soap
	if (/^Content-Type/i in hname && /application\/soap\+xml/i !in hval)
	    return T;

	# Not interested in Zero Length content
	if (/^Content-Length/i in hname && hval == "0")
	    return T;

	# The primary indicator for this exploit is that the Authorization header is missing.
	if (/^Authorization/i in hname) 
	    return T;

	# Some other FPs that have been noted in testing.
	if (/^ORIGINALCONTENT/i in hname && /Length=/i in hval)
	    return T;

	if (/^WSMANIDENTIFY/i in hname && /unauthenticated/i in hval) 
	    return T;

	# User Agent whitelisting - use sparingly!
	if (user_agent_whitelist !in "<ARE_YOU_SURE?_replace_this_with_user_agent_to_whitelist>" &&
	    /^User-Agent/i in hname && user_agent_whitelist in hval)
		return T;

	return F;
	}

# The intent is to return as quickly as possible from the http_all_headers event, as it can be high volume.
event http_all_headers(c: connection, is_orig: bool, hlist: mime_header_list) 
    {
    # We are only interested in Client headers, as these contain the indicators of an attack.
    if (!is_orig)
        return;

    # The server port must be in the list of defined OMI ports list
    if (c$id$resp_p !in OMI_ports)
        return;

    # The exploit is triggered when a payload XML is POSTed
    if ( !c?$http || !c$http?$method || c$http$method != "POST")
        return;

    # Throw out small list of headers
    if (|hlist| < 4)
        return;

    # Check if the uri field exists, as this would create a non fatal reporter.log with the following check.
    if (!c$http?$uri)
        return;

    # Check for a complaint URI.
    if (wsman_uri_pattern_post !in c$http$uri)
        return;
    
    # At this stage we can mark this connection as a "Candidate" as it meets the coarser grained requirements.
    c$http$CVE_2021_38647_candidate = T;

    # Now step through the header list to carve out legitimate and/or non-attack traffic from being a Candidate.
    for (i in hlist)
	if ( header_disqualifies(hlist[i]$name, hlist[i]$value ) )
            {
            c$http$CVE_2021_38647_candidate = F;
            return;
            }
    
    # If we have not returned, the connection has passed all of the qualifiers, 
    # and has not been been dis-qualified. We assume this is an exploit ATTEMPT and raise a notice.

    if (raise_seperate_notice_for_missing_auth_header)
        {
        NOTICE([$note=EXPLOIT_ATTEMPT,
            $conn=c, 
            # $identifier=cat(c$id$orig_h,c$id$resp_h,c$id$resp_p),
            # $suppress_for=3600sec,
            $msg=fmt("A request to an OMI/WMI uri is missing the Authorization header, this is possibly a CVE-2021-38647 (AKA OMIGOD) exploit attempt. Refer to https://www.wiz.io/blog/secret-agent-exposes-azure-customers-to-unauthorized-code-execution, see sub field for raw data"),
            $sub=fmt("headers= '%s'", cat(hlist))]);
        }
    
    }

# The intent is to return as quickly as possible from the http_entity_data event, as it can be high volume.
event http_entity_data(c: connection, is_orig: bool, length: count, data: string)
    {
    # If this is not a candidate connection, return immediately.
    if (!c$http$CVE_2021_38647_candidate)
        return;

    # We are only interesting in the first transaction level 
    if (c$http$trans_depth > 1)
        return;

    if (length == 0)
        return;

    if (is_orig)
        {
        # Return if the data is NOT ascii, as the exploit uses text XML 
        if (!is_ascii(data))
            {
            c$http$CVE_2021_38647_candidate = F;
            return;
            }

        # Disqualify if there are no common XML artifacts in the POST 
        if (xml_artifacts !in data)
            {
            c$http$CVE_2021_38647_candidate = F;
            return;
            }

        # Raise a notice for Request from the client, include relevant data for IR triage in the 'sub' field:
        NOTICE([$note=EXPLOIT_REQUEST,
            $conn=c, 
            # $identifier=cat(c$id$orig_h,c$id$resp_h,c$id$resp_p),
            # $suppress_for=3600sec,
            $msg=fmt("A REQUEST to an OMI/WMI uri has a missing Authorization header - this is possibly a CVE-2021-38647 (AKA OMIGOD) exploit. See sub of this notice field for the raw Request data. Refer to https://www.wiz.io/blog/secret-agent-exposes-azure-customers-to-unauthorized-code-execution"),
            $sub=fmt("The first %s bytes of data = '%s'", bytes_of_data_in_notice, sub_bytes(data,0,bytes_of_data_in_notice))]);
            return;
        }
    
    # A non vulnerable server responds with 'Bad Request', so we can disqualify this
    if (/\<TITLE\>Bad\ Request\<\/TITLE\>/ in data)
        {
        c$http$CVE_2021_38647_candidate = F;
        return;
        }
    
    # Raise a notice for response from the server.
    # Include relevant data for IR triage in the 'sub' field:
    NOTICE([$note=EXPLOIT_RESPONSE,
        $conn=c, 
        # $identifier=cat(c$id$orig_h,c$id$resp_h,c$id$resp_p),
        # $suppress_for=3600sec,
        $msg=fmt("A Server RESPONSE has been sent following a request to an OMI/WMI uri with a missing Authorization header - this is possibly a successful CVE-2021-38647 (AKA OMIGOD) exploit. See sub of this notice field for the raw Request data. Refer to https://www.wiz.io/blog/secret-agent-exposes-azure-customers-to-unauthorized-code-execution"),
        $sub=fmt("The first %s bytes of data = '%s'", bytes_of_data_in_notice, sub_bytes(data,0,bytes_of_data_in_notice))]);
    }
