## Introduction
Android is one of the most widely used operating systems for mobile phones around the globe. One of the interesting features of the Android OS is the intercommunication between processes, apps and with the system.

An Android “INTENT” is basically an abstract description of the specific operation/action which is to be performed. A broadcast (message) using an intent allows an application or the OS itself to send a message system-wide. All apps can register themselves to receive wished broadcasts.

As a result it’s important that broadcast messages do not contain sensitive information - independent which app sent it.
The vulnerability that is being described in the following is based on an system broadcast which contains sensitive data. 
This way I are able to receive system information (concretely the devices location) without declaring the corresponding location permissions in my app.

## Related Vulnerability

CVE-2018-15835 Sensitive Data Exposure via Battery Information Broadcast.
Android System Broadcast expose the detailed information about the battery. It requires no special permissions and with availability of high precise information, it is possible to uniquely track and identify users. It is still not classified as a bug by Google and has no fix measures currently in place

“android.intent.action.BATTERY_CHANGED” intent exposes this battery information such as charging level, voltage and temperature.

## Vulnerability details 

In late 2018, a vulnerability has been found by “Yakov Shafranovich”  which broadcasts sensitive system information via WiFi broadcasting signals. That is, it sends out the data such as WiFi network name, BSSID, local IP Address, DNS server information and the MAC address to all the application running on the device.

Analysis Description
Base Score. By NIST: 7.5 HIGH 
Impact Score: 3.6 
Exploitability Score: 3.9
Attack Vector (AV): Network 
Attack Complexity (AC): Low 
Privileges Required (PR): None 
User Interaction (UI): None 
Scope (S): Android Broadcast Information Exposure
Confidentiality (C): High 
Integrity (I): None 
Availability (A): None
Additional Information: Allows unauthorized disclosure of information

## Roadmap!
Usually, this data is tougher to access but bypassing the security notions by the native apps to pay attention to the system broadcasts led to this vulnerability. 

Android broadcasts information about the WiFi connection and it’s interface regularly using two particular intents: WifiManager’s NETWORK_STATE_CHANGED_ACTION and WifiP2pManager’s WIFI_P2P_THIS_DEVICE_CHANGED_ACTION. These intents are responsible for sharing the details like MAC and BSSID.

Now, this poses two serious threats. Firstly, since MAC address is out, the device in question can be tracked since MAC addresses are unique to the particular devices they represent even if MAC address randomisation is used. Secondly, the network name and BSSID can be used to geolocate the device via a simple matching against a database like “WiGLE” or “SkyHook” which are the databases of BSSIDs and map known BSSIDs to coordinates. This is a severe violation of the privacy of the user. A malicious app might send this continuous location to an attacker in the backend.

Thus, the app provider is able to receive location updates (when the BSSID is known to the database) without declaring the permissions to do so - namely android.permission.ACCESS_COARSE_LOCATION and android.permission.ACCESS_FINE_LOCATION.

The permissions have the protection level “dangerous”, which increases the severity of the vulnerability, since I are now able to reconstruct information which is normally protected by dangerous permissions.

I implemented this vulnerability where I get the info from the broadcasts and logged the latitude and longitude of the device. These can also be then sent via a background service like email to a malicious person.

## Implementation details

I designed a simple Hello world application which exploits this vulnerability in the backend.  I built the broadcast class which listens to the 2 intents NETWORK_STATE_CHANGED_ACTION and WIFI_P2P_THIS_DEVICE_CHANGED_ACTION. Whenever user changes his WiFi State, these 2 intents send by Android OS is captured by my Broadcast manager.  Once I get the intent I extract the BSSID which is 48bit identity used to identify a particular BSS (Basic Service Set) within an area. In Infrastructure BSS networks, the BSSID is the MAC (Medium Access Control) address of the AP (Access Point) and in Independent BSS or ad hoc networks, the BSSID is generated randomly. In short, BSSID is linked to WiFi access point that this information will always be available in broadcast intent whenever User joins a Wifi service.

Using SSID which tells us about the name of the network and BSSID I extracted the current location of user by making use of API from Wigle. I have to send the information(BSSID & SSID) using a HTTP request and after doing authentication with the server, I get the information about the latitude and longitude which are stored in log files for demonstration purposes and which can be sent via backend mail to server. The following screenshot demonstrates the output.

![Working proof!](Screenshot.png)
## Mitigation Strategies
Since the vulnerability is a part of Android OS inter process communication where OS itself send out the broadcast, it was fixed by Google in the Latest Android 9 version. The root cause of was improper management of Intents and filling them with sensitive information. So the best way to mitigate the vulnerability is by upgrading to the latest Android version.
Both of these flaws are also reminiscent of the man-in-the-disk problem discussed at DEFCON 2018, which also concerns cross-application information leakage. The problem lies in the fact that Android’s OS makes use of two types of storage: Internal storage, which provides every app with its own sandbox; and an external storage mechanism that uses a removable SD card. Developers need to be careful while designing the intents sent in the broadcast and allow a proper use of external storage to make sure that vulnerabilities  are not created in the future.
