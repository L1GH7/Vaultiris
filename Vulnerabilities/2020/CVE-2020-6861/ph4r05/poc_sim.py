from monero_glue.xmr import crypto


def pttest2(N=10000):
    a = crypto.random_scalar()
    a8 = crypto.sc_mul(a, crypto.sc_init(8))
    for i in range(1, N):
        ca = crypto.sc_mul(a8, crypto.sc_init(i))
        A8 = crypto.scalarmult_base(ca)
        A8bin = crypto.encodepoint(A8)
        red = crypto.encodeint(crypto.decodeint(A8bin))
        if red == A8bin:
            return i, red
    return None


def sim():
    aa = [pttest2(10000) for _ in range(10000)]
    return sum(x[0] for x in aa)/len(aa)

