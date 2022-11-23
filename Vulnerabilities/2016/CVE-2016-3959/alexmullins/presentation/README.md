# 0\. Intro

- **Software Vulnerability Analysis** - Finding and reporting on software bugs that, if exploited, could cause significant damage to the operations of a piece of software or computer system.

- **Purpose** - To bring awareness to the community affected by a vulnerability. An exploitation of a software vulnerability could allow a hacker to obtain customer information, cause denial of service, or allow complete control of a computer system.

- **Heartbleed** - <http://heartbleed.com/> (CVE-2014-0160)

  > The (1) TLS and (2) DTLS implementations in OpenSSL 1.0.1 before 1.0.1g do not properly handle Heartbeat Extension packets, which allows remote attackers to obtain sensitive information from process memory via crafted packets that trigger a buffer over-read, as demonstrated by reading private keys, related to d1_both.c and t1_lib.c, aka the Heartbleed bug.

  - **Ashley Madison** - <https://en.wikipedia.org/wiki/Ashley_Madison_data_breach>
  - **Jeep Hacked** - <http://www.wired.com/2015/07/hackers-remotely-kill-jeep-highway/>
  - **Internal Revenue Service (IRS)** - <http://fortune.com/2015/08/21/irs-sued-data-breach/>
  - **Office of Personnel Management** - <https://en.wikipedia.org/wiki/Office_of_Personnel_Management_data_breach>
  - **Apple vs. FBI iPhone Case** - <https://en.wikipedia.org/wiki/FBI%E2%80%93Apple_encryption_dispute><br><br>

- **Live attacks** - <http://map.norsecorp.com/#/>

- **Distributed Denial of Service (DDOS) Attack Stats** -

  > Monday's DDoS proved these attacks aren't just theoretical. To generate approximately 400Gbps of traffic, the attacker used 4,529 NTP servers running on 1,298 different networks. On average, each of these servers sent 87Mbps of traffic to the intended victim on CloudFlare's network. **_Remarkably, it is possible that the attacker used only a single server running on a network that allowed source IP address spoofing to initiate the requests._**

  > ...

  > For comparison, the attack that targeted Spamhaus used 30,956 open DNS resolvers to generate a 300Gbps DDoS. On Monday, with 1/7th the number of vulnerable servers, the attacker was able to generate an attack that was 33% larger than the Spamhaus attack.

  > <cite>Matthew Prince- Cloudflare CEO</cite>

  > 13 Feb 2014

  > <https://blog.cloudflare.com/technical-details-behind-a-400gbps-ntp-amplification-ddos-attack/>

# 1\. A Summary of Vulnerability Analysis (CVE-2016-3959)

- **Summary** - There is a bug in the signature verification method for the Digital Signature Algorithm (DSA) in Go's standard library crypto/dsa package. If exploited, an attacker can cause a denial of service (DOS) against a target server. Alice and Bob example.<br><br>
  If a DSA public key's P parameter is set to 0 an attacker can cause the verification to hang in an infinite loop. Will lead to memory and CPU to continually grow and the system will become unresponsive.<br><br>
  Supposed to calculate: `b = x ^ y mod z`, but instead calculate `b = x ^ y` when P = 0\. When x is a 300 digit number and y is a 50 digit number the resulting number b will contain more digits than there are atoms in the universe.

  > Go has an infinite loop in several big integer routines that makes Go programs vulnerable to remote denial of service attacks. Programs using HTTPS client authentication or the Go ssh server libraries are both exposed to this vulnerability. This is being addressed in the following CL: <https://golang.org/cl/21533>

  > --

  > <cite>Jason Buberel - Product Manager, Go Language, Tools &amp; Ecosystem at Google</cite>

- **Video** - <https://youtu.be/FXie8T5P6PI>
