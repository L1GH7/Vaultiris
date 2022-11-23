# CVE-2022-24611
Details regarding the Z-Wave S0-No-More attack. For a full analysis and report how this works and how to reproduce the findings see the attached PDF file. 


## Short description: 

Denial of Service attack against S0 and S2 devices (tested with the Z-
Wave ZW5xx product line), here specifically Z-Wave enabled Ama-
zon Ring Gen. 1 devices. An attacker can use the S0 NonceGet
request to continuously send a minimal amount of nonce requests
(1 per 2 seconds) to the Z-Wave gateway, effectively blocking it
from issuing new nonces to other devices while the attack is run-
ning. This is due to the Z-Wave specification demanding a partici-
pant to wait for at least 3 and up-to 20 seconds for the reply of the
device requesting the nonce and the fact that the attacker can spoof
any device within the network. This attack relies on a spoofable
device NodeID and therefore a device which has been successfully
included but is offline during the attack. This does include devices,
which have not been correctly excluded using the smartphone app,
e.g. a smart power socket. This attack can be used to target specific
networks while leaving others untouched and only needs a minimum
amount of packets compared to jamming attacks to block a controller
/ device.

## Vulnerarbility Type: 

DoS

## Vendor of Product: 

Silicon Labs (manufacturer of the Z-Wave ZW5xx SoC used in the
specific product tested)

## Specific Product tested: 

(Amazon) Ring Alarm Security Kit, 5 piece

## Affected product codebase: 

Unknown, affects both S0 and S2 Z-Wave networks of Gen. 5 of
the Z-Wave specification; S2 only if S0 connections, especially S0
NonceGet, are allowed by the gateway.

## Attack Type: 

Local attack, attacker needs to be in range of the victims Z-Wave
network.

## Impact: 

Complete Denial of Service against the target network, rendering it
unusable for the duration of the attack. The network resumes opera-
tion after the attack without noticeable traces. There seems to be no
limitation to the attack duration. The attack only needs to minimum
amount of packets to start the blocking process. The controller stays
blocked till all requests in its incoming buffer have been timeouted,
even if the attacker is no longer sending.
