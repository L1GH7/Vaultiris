# CVE-2020-6514

The exploit
When writing the exploit, I originally altered the SCTP packets sent to the target device by altering the source of WebRTC and recompiling it. This wasn’t practical for attacking closed source applications, so I eventually switched to using Frida to hook the binary of the attacking device instead. Frida’s hooking functionality allows for code to be executed before and after a specific native function is called, which allowed my exploit to alter outgoing SCTP packets as well as inspect incoming ones. Functionally, it is equivalent to altering the source of the attacking client, but instead of the alterations being made in the source at compile time, they are made dynamically by Frida at run time. The source for the exploit is available here.

There are seven functions that the attacking device needs to hook, as follows.

usrsctp_conninput // receives incoming SCTP
DtlsTransport::SendPacket // sends outgoing SCTP
cricket::SctpTransport::SctpTransport // detects when SCTP transport is ready
calculate_crc32c // calculates checksum for SCTP packets
sctp_hmac // performs HMAC to guess secret key
sctp_hmac_m // signs SCTP packet
SrtpTransport::ProtectRtp // suppresses RTP to reduce heap noise

These functions can be hooked as symbols, or as offsets in the binary.

There are also three address offsets from the binary of the target device that are needed for the exploit to work.  The offset between the system function and the malloc function, as well as the offset between the gadget described in the previous post and the malloc function are two of these. These offsets are in libc, which is an Android system library, so they need to be determined based on the target device’s version of Android. The offset from the location of the cricket::SctpTransport vtable to the location of malloc in the global offset table is also needed. This must be determined from the binary that contains WebRTC in the application being attacked.

Note that the exploit scripts provided have a serious limitation: every time memory is read, it only works if bit 31 of the pointer is set. The reasons for this are explained in Part 2. The exploit script has an example of how to fix this and read any pointer using FWD_TSN chunks, but this is not implemented for every read. For testing purposes, I reset the device until the WebRTC library was mapped in a favorable location.
Android Applications
A list of popular Android applications that integrate WebRTC was determined by searching APK files on Google Play for a specific string in usrsctp. Roughly 200 applications with more than five million users appeared to use WebRTC. I evaluated these applications to determine whether they could plausibly be affected by the vulnerabilities in the exploit, and what the impact would be.

It turned out the ways applications use WebRTC are quite varied, but can be separated into four main categories.

Projection: the screen and controls of a mobile application is projected into a desktop browser with user consent for enhanced usability
Streaming: audio and video content is sent from one user to many users. Usually there is an intermediary server, so the sender does not need to manage possibly thousands of peers, and the content is recorded for later viewing
Browsers: all major browsers contain WebRTC to implement the JavaScript WebRTC API
Conferencing: two or more users communicate via audio or video in real time

The impact of the vulnerabilities used in the exploit is different for each of these categories. Projection is low risk, as a lot of user interaction is required to set up the WebRTC connection, and the user has access to both sides of the connection in the first place, so there is little to gain by compromising the other side. 

Streaming is also fairly low risk. While it’s possible that some applications use peer-to-peer connections when a stream has a low number of viewers, they usually use an intermediary server that terminates the WebRTC connection from the sending peer, and starts new connections with the receiving peers. This means that the attacker usually cannot send malformed packets directly to a peer. Even with a set-up where streaming is performed peer-to-peer, user interaction is required for the target to view the stream, and there’s often no way to limit who can access a stream. For this reason, streaming applications that use WebRTC are probably not useful for targeted attacks. Of course, it is possible that these vulnerabilities affect the servers used by streaming services, but this was not investigated in this research.

Browsers are almost certainly vulnerable to most bugs in WebRTC, because they allow a large amount of control over how it is configured. To exploit such a bug in a browser, an attacker would need to set up a host that acts like the other peer in the peer-to-peer connection, and convince the target to visit a webpage that starts a call to that host. In this case, the vulnerability would have a similar impact to other memory corruption vulnerabilities in JavaScript.

Conferencing is the highest risk usage of WebRTC, but the actual impact of a vulnerability depends on a lot of how users of an application contact each other. The highest risk design is an application where any user can contact any other user based on an identifier. Some applications require the callee to have interacted in a specific way with the caller before a call can be made, which makes users harder to contact a target and generally reduces risk. Some applications require users to enter a code or visit a link to start a call, which has a similar effect. There is also a large group of applications where it is difficult or impossible to call a specific user, for example chat roulette applications, and applications which have features that allow a user to start a call to customer support. 

For this research, I focused on conferencing applications that allow users to contact specific other users. This reduced my list of 200 applications to 14 applications, as follows.


