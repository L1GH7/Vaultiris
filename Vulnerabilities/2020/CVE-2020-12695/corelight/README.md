# Zeek Package that detects CallStranger (CVE-2020-12695) attempts (http://callstranger.com/)

 This package attempts to detect CallStranger (CVE-2020-12695) exploitation attempts and data exfiltration. It does so by looking for three key things:

   1. UPnP `SUBSCRIBE` commands with a Notify URL that contains an IP address that isn't an RFC1918 or local_nets address. This could be the precursor for DDoS amplification or Data Exfiltration
   2. UPnP `NOTIFY` commands that are destined for a non-RFC1918 or local_nets address. This could indicate active DDoS amplification or Data Exfiltration
   3. UPnP `SUBSCRIBE` commands with a Notify URL that is longer than `CallStrangerDetector::exfiltration_threshold` bytes. This could indicate data exfiltration over UPnP

 The module assumes that your site doesn't normally have UPnP `SUBSCRIBE` or `NOTIFY` commands destined for the Internet and that you don't have UPnP devices exposed to the Internet. If either of these things are true, you will see false positives. You can add particular IPs that create false positives to the `CallStrangerDetector::ignore_subnets` set to ignore them.

## Usage
### Standalone Mode
To use this script against a PCAP, simply clone the Git repository and run Zeek with `zeek -Cr your.pcap scripts/__load__.zeek`

### As a Package
To install the package, clone the Git repository and execute `zkg install .` from the package directory

## Notice Types
 The module will add notices to `notice.log` if it detects CallStranger-like activity. The notices are as follows:
* `CallStranger_Data_Exfiltration_Attempt`: Observed an attempt to have a UPnP device exfiltrate traffic
* `CallStranger_Data_Exfiltration_Success`: Observed a UPnP device likely exfiltrating data
* `CallStranger_UPnP_Request_Callback_To_External_Host`: Observed an attempt to have a UPnP device call back to an external host
* `CallStranger_UPnP_To_External_Host`: Observed a UPnP device calling back to an external host

## Configuration Options
There are three configuration options that you can set:
* `CallStrangerDetector::exfiltration_threshold`: The number of bytes that need to be observed in a UPnP Callback URL in order to be classified as data exfiltration.
* `CallStrangerDetector::ignore_subnets`: A set of subnets that should be ignored for UPnP detections. Use this if you have false positives where UPnP is traversing the Internet legitimately. 
* `CallStrangerDetector::strict_upnp_protocol_detection`: This boolean controls whether the script will be more strict when trying to identify UPnP SUBSCRIBE commands. It does so by requiring the “NT” (Notification Type) header to be observed. The default is false because the NT header doesn’t actually do anything, it’s just required by the UPnP specification. If you have non-UPnP SUBSCRIBE traffic, you may have to set this to true

## Disclaimer
I have tested this against local UPnP traffic and by using the proof of concept code provided by the CallStranger author (https://github.com/yunuscadirci/CallStranger). If you have any issues, please open a GitHub issue or contact us. Thanks!
