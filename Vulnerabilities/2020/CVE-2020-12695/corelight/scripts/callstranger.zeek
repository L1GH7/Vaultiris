@load base/misc/version

# Module that detects CallStranger (CVE-2020-12695) attempts: http://callstranger.com/
# This module does so by looking for three things that could indicate CallStranger activity:
#   1. UPnP SUBSCRIBE commands with a Notify URL that contains an IP address that isn't an RFC1918 or local_nets address. This could be the precursor for DDoS amplification or Data Exfiltration
#   2. UPnP NOTIFY commands that are destined for a non-RFC1918 or local_nets address. This could indicate active DDoS amplification or Data Exfiltration
#   3. UPnP SUBSCRIBE commands with a Notify URL that is longer than CallStrangerDetector::exfiltration_threshold bytes. This could indicate data exfiltration over UPnP
# The module assumes that your site doesn't normally have UPnP SUBSCRIBE or NOTIFY commands destined for the Internet and that you don't have UPnP devices exposed to the Internet.
# If either of these things are true, you will see false positives. You can add particular IPs that create false positives to the CallStrangerDetector::ignore_subnets set to ignore them.

# Add a callback_header value to HTTP::Info to make it easier to track the Callback header for just UPnP traffic
module HTTP;
export {
    redef record Info += {
        callback_header: vector of string &optional;
        saw_nt_header: bool &optional;
    };
}

module CallStrangerDetector;
export {
  redef enum Notice::Type += {
    CallStranger_Data_Exfiltration_Attempt,
    CallStranger_Data_Exfiltration_Success,
    CallStranger_UPnP_To_External_Host,
    CallStranger_UPnP_Request_Callback_To_External_Host
  };

  # Default is 4kb, tune as needed
  option exfiltration_threshold: count = 4096;
  option ignore_subnets: set[subnet] = {};

  # If true, requires the 'NT' header to be present in the UPnP SUBSCRIBE request per the UPnP standards. Less likely to FP if true but easier to bypass
  option strict_upnp_protocol_detection = F;
}

const no_addr = 0.0.0.0;

function process_ip(netlocation_addr: addr, value: string, c: connection) {
    if (netlocation_addr == no_addr) {
        return;
    }

    if (netlocation_addr !in Site::private_address_space && netlocation_addr !in Site::local_nets && netlocation_addr !in ignore_subnets) {
        NOTICE([$conn=c, $note=CallStranger_UPnP_Request_Callback_To_External_Host, $msg="Potential CVE-2020-12695 (CallStranger) exploitation attempt (Requested UPnP Callback to a non-RFC1918 or Local address)"]);
        if (|value| > exfiltration_threshold) {
            NOTICE([$conn=c, $note=CallStranger_Data_Exfiltration_Attempt, $msg="Potential CVE-2020-12695 (CallStranger) data exfiltration attempt (large amount of data in UPnP NOTIFY Callback URI)"]);
        }
    }
}

event http_message_done(c: connection, is_orig: bool, stat: http_message_stat) {
    if (c?$http && c$http?$method) {
        if (c$http$method == "NOTIFY") {
            if (c$id$resp_h !in Site::private_address_space && c$id$resp_h !in Site::local_nets && c$id$resp_h !in ignore_subnets) {
                NOTICE([$conn=c, $note=CallStranger_UPnP_To_External_Host, $msg="Potential CVE-2020-12695 (CallStranger) exploitation success (UPnP NOTIFY to a non-RFC1918 or Local Address)"]);
                if (c$http?$uri && |c$http$uri| > exfiltration_threshold) {
                    NOTICE([$conn=c, $note=CallStranger_Data_Exfiltration_Success, $msg="Potential CVE-2020-12695 (CallStranger) data exfiltration success (large amount of data in UPnP NOTIFY URI)"]);
                }
            }
        } else if (c$http$method == "SUBSCRIBE" && c$http?$callback_header && (strict_upnp_protocol_detection ? c$http?$saw_nt_header && c$http$saw_nt_header : T)) {
            # TODO: If there is more than one value and you see a local address in the callback and also a non-local, could be a port scan attempt
            local values = c$http$callback_header;
            for (i in values) {
                local value = values[i];

                local parsed_uri = decompose_uri(value);
                if (parsed_uri?$netlocation) {
		    if (is_valid_ip(parsed_uri$netlocation)) {
			process_ip(to_addr(parsed_uri$netlocation), value, c);
                    } else {
@if (Version::at_least("5.0"))
                        when [parsed_uri, value, c] ((local addresses = lookup_hostname(parsed_uri$netlocation))) {
@else
                        when ((local addresses = lookup_hostname(parsed_uri$netlocation))) {
@endif
                            for (address in addresses) {
                                process_ip(address, value, c);
                            }
                        }
                    }
                }
            }
        }
    }
}

# When we see headers add any Callback headers to the HTTP connection record so we can use them later
# If we see the UPnP NT header, set a flag on the http record to indicate that.
# The CALLBACK header combined with the NT header indicates UPnP (When used with SUBSCRIBE)
event http_header(c: connection, is_orig: bool, name: string, value: string) {
    if (c?$http) {
        if (name == "CALLBACK") {
            if (!c$http?$callback_header) {
                c$http$callback_header = vector(value);
            } else {
                c$http$callback_header += value;
            }
        } else if (name == "NT") {
            c$http$saw_nt_header = T;
        }
    }
}

