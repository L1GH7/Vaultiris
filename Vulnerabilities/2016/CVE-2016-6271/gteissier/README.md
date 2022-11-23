# CVE-2016-6271

[CVE-2016-6271](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2016-6271) impacts [libbzrtp](https://github.com/BelledonneCommunications/bzrtp), which is a ZRTP library developped by Belledonne Communications.

This library is embedded in end-user applications, for example linphone, which is available as an [Android app on Play store](https://play.google.com/store/apps/details?id=org.linphone&hl=fr). Current version 3.2.7 embeds a version of libbzrtp **shall not**  be vulnerable to CVE-2016-6271.

## TLDR;

[![asciicast](https://asciinema.org/a/130042.png)](https://asciinema.org/a/130042)

### Build vulnerable ZRTP agent

`cd vulnerable-bzrtp && docker build -t vulnerable-bzrtp .`

### Build ZRTP enabled mallory

`cd mitm-bzrtp && docker build -t mitm-bzrtp .`

### Put it all together

`docker-compose -f cve-2016-6271.yaml up`

## ZRTP, specs and container

### End-to-end media encryption

ZRTP is a solution to secure voice over IP calls.

The following is an extract from IETF [rfc6189](https://tools.ietf.org/html/rfc6189):

```
   ZRTP is a key agreement protocol that performs a Diffie-Hellman key
   exchange during call setup in the media path and is transported over
   the same port as the Real-time Transport Protocol (RTP) [RFC3550]
   media stream which has been established using a signaling protocol
   such as Session Initiation Protocol (SIP) [RFC3261].  This generates
   a shared secret, which is then used to generate keys and salt for a
   Secure RTP (SRTP) [RFC3711] session.  ZRTP borrows ideas from
   [PGPfone].  A reference implementation of ZRTP is available in
   [Zfone].

   The ZRTP protocol has some nice cryptographic features lacking in
   many other approaches to media session encryption.  Although it uses
   a public key algorithm, it does not rely on a public key
   infrastructure (PKI).  In fact, it does not use persistent public
   keys at all.  It uses ephemeral Diffie-Hellman (DH) with hash
   commitment and allows the detection of man-in-the-middle (MiTM)
   attacks by displaying a short authentication string (SAS) for the
   users to read and verbally compare over the phone.
```

To summarize:

* ZRTP shares the same media path as RTP: IP endpoints and UDP ports
* ZRTP uses ephemeral Diffie-Hellman to generate crypto material for SRTP
* ZRTP uses hash commitment and displays a short authentication string to detect man-in-the-middle attempts

### Vulnerable image

A vulnerable ZRTP agent is compiled from a [single C file](vulnerable-bzrtp/agent.c):

```
  ctx = bzrtp_createBzrtpContext(self_ssrc);
  assert(ctx != NULL);

  ret = bzrtp_setCallbacks(ctx, &bzrtp_callbacks);
  assert(ret == 0);

  bzrtp_initBzrtpContext(ctx);

  bzrtp_setClientData(ctx, self_ssrc, ctx);

  ret = bzrtp_startChannelEngine(ctx, self_ssrc);
  assert(ret == 0);

  for (now = 0; now += 50;) {
    usleep(500000);

    ret = recv(sd, buffer, sizeof(buffer), MSG_DONTWAIT);
    if (ret > 0) {
      received = ret;
      bzrtp_processMessage(ctx, self_ssrc, buffer, received);
    }

    bzrtp_iterate(ctx, self_ssrc, now);
  }
```

To enjoy it, build docker image using `cd vulnerable-bzrtp && docker build -t vulnerable-bzrtp .`. Beware that Dockerfile defines start.sh as an entrypoint, which sets routing via mallory - more on that later.

## ZRTP man-in-the-middle

### Hash-commitment _is_ important

**Disclosed on 30th of March 2016 to Belledone Communications, this vulnerability has been swiftly fixed with [this commit](https://github.com/BelledonneCommunications/bzrtp/commit/bbb1e6e2f467ee4bd7b9a8c800e4f07343d7d99b).**

ZRTP performs Diffie-Hellman in two messages called `DHPart1` and `DHPart2`. Bob _commits_ the `DHPart2` message it will send after receiving Alice's `DHPart1`.

```
    |        Commit (Bob's ZID, options, hash value) F5 |
    |<--------------------------------------------------|
    | F6 DHPart1 (pvr, shared secret hashes)            |
    |-------------------------------------------------->|
    |            DHPart2 (pvi, shared secret hashes) F7 |
    |<--------------------------------------------------|
```

**The vulnerability discovered in bzrtp is the absence of hash-commitment verification, leaving room for an attacker to forge `pvi` with interesting properties.**

This proof of concept implements the weakness described in rfc6189:

```
   The use of hash commitment in the DH exchange constrains the attacker
   to only one guess to generate the correct Short Authentication String
   (SAS) (Section 7) in his attack, which means the SAS can be quite
   short.  A 16-bit SAS, for example, provides the attacker only one
   chance out of 65536 of not being detected.  Without this hash
   commitment feature, a MiTM attacker would acquire both the pvi and
   pvr public values from the two parties before having to choose his
   own two DH public values for his MiTM attack.  He could then use that
   information to quickly perform a bunch of trial DH calculations for
   both sides until he finds two with a matching SAS.  To raise the cost
   of this birthday attack, the SAS would have to be much longer.  The
   Short Authentication String would have to become a Long
   Authentication String, which would be unacceptable to the user.  A
   hash commitment precludes this attack by forcing the MiTM to choose
   his own two DH public values before learning the public values of
   either of the two parties.
```

### Introducing Mallory

With Mallory as active attacker, the DH exchange above becomes:

```
   Bob                    Mallory                     Alice
    |                         | Commit (Alice's ZID...) |
    |                         |<------------------------|
    | Commit (Alice's ZID...) |                         |
    |<------------------------|                         |
    |                         |                         |
    |       DHPart1 (pvr...)  |                         |
    |------------------------>|                         |
    |                         |     DHPart1 (pvr'...)   |
    |                         |------------------------>|
    |                         |       DHPart2 (pvi...)  |
    |                         |<------------------------|
    | * SAS(Mallory, Alice) is known at this time     * |
    | * now find a pvi' such as SAS(Bob, Mallory)     * |
    | * equals SAS(Mallory, Alice)                    * |
    |       DHPart2(pvi'...)  |                         |
    |<------------------------|                         |
    
    | * SAS(Mallory, Bob) = SAS(Alice, Mallory)       * |
    | * Both parties will confirm they share the same * |
    | * SAS value                                     * |
    
    | * Note that SRTP material (Alice, Mallory) will * |
    | * not match SRTP material (Mallory, Bob)        * |
    
    | * However, Mallory will be able to handle SRTP  * |
    | * flows from both Bob and Alice, giving         * |
    | * interception and tampering capabilities.      * |
```

It is built on Python3 and asyncio. Raw IP packets are captured using a `PF_PACKET` socket, listening on IPv4 frames only. Scapy helps to dissect raw frames.
It analyzes ZRTP packets and rewrite them, in particular, it recomputes HMAC authentication tags.

SAS bruteforcer is C based and takes as input:

* initiator-chain: `Hello of responder || Commit || DHPart1`
* pvr: public value sent by responder in `DHPart1`
* zidi: initiator ZID
* zidr: responder ZID
* sasval: target SAS value

Build docker image using `cd mitm-bzrtp && docker build -t mitm-bzrtp . && cd ..`.

The man-in-the-middle will be setup with the help of:

* two Docker images will host [ZRTP vulnerable agents](vulnerable-bzrtp/Dockerfile), and [attacker](mitm-bzrtp/Dockerfile);
* a single [compose file](cve-2016-6271.yaml) will launch two instances of agents, and one attacker in man-in-the-middle position.

**Alice** and **Bob** will exchange through **Mallory**, which opportunistically performs a man-in-the-middle attack.

### Drilling for a matching SAS

It boils down to test various `svi` and verify if derived SAS matches the already obtained SAS. All the gory details can be found in [bruteforce source](mitm-bzrtp/bruteforce.c).

### Ensuring authenticity

ZRTP messages are authenticated using a reversed chain of iterated hashes as keys. If `DHPart2` is modified on the fly by Mallory, Alice will detect Mallory's mangled `DHPart2` as not authentic, because the computed HMAC will not match the received HMAC. In order to pass this check, Mallory has to use a whole chain of iterated hashes, and she needs to sign all the sent messages using this chain.
