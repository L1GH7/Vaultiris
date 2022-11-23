# Base curve setup
l = 7237005577332262213973186563042994240857116359379907606001950938285454250989
G = Integers(l)


class AddGadget(object):
    """sc_add gadget, counting operations"""
    def __init__(self):
        self.ctr = 0

    def add(self, x, y):
        self.ctr += 1
        return x + y


ADDER = AddGadget()
ADD = ADDER.add

# Create base: {2^i * a}_{i \in [0, 255]}
a = G.random_element()
base = [a]
while len(base) < 256:
    base.append(ADD(base[-1], base[-1]))

# Select base elements required to assemble l*a
acc = [base[i] for i in range(256) if (l & 2**i) > 0]

print('Additions needed to assemble l*a: %s' % len(acc))
zero = reduce(lambda x, y: ADD(x, y), acc)

print('Correctness: %s, additions: %s' % (zero == 0, ADDER.ctr))

# Enc oracle:
for i in range(100):
    x = G.random_element()  # reconstruction target
    z = (a^-1) * x
    acc = [base[i] for i in range(256) if (int(z) & 2**i) > 0]
    xx = sum(acc)
    if x-xx != 0:
        raise ValueError('Error in enc oracle for a: %s' % x)


# sc_sub gcd
def sc_sub_gcd(a, b):
    c = 0
    while a != b and min(a, b) > 0:
        a, b = (a, b-a) if b > a else (a-b, b)
        c += 1
    return a, c


# Probability that two random numbers will be co-prime
def coprime_prop(N=10000, comp_steps=False, gres=1):
    cops = 0
    steps = 0
    for i in range(N):
        a1 = G.random_element()
        a2 = G.random_element()
        if gcd(int(a1), int(a2)) == gres:
            cops += 1
            if comp_steps:
                r, s = sc_sub_gcd(a1, a2)
                steps += s

    return cops/N, steps/cops


def sc_sub_gcd_limit(a, b, limit=1):
    c = 0
    while a != b and min(a, b) > 0:
        a, b = (a, b-a) if b > a else (a-b, b)
        c += 1
        if a < limit:
            return a, b, c
        if b < limit:
            return b, a, c
    return a, b, c


def tabulated_sim(N=10000, tab_limit=1000000000):
    cops = 0
    steps = 0
    lastn = 0
    attmpts = []
    for i in range(N):
        a1 = G.random_element()
        a2 = G.random_element()
        g, g2, csteps = sc_sub_gcd_limit(a1, a2, tab_limit)
        if g > tab_limit:
            print('toob: ', g)
            continue

        attempt = i - lastn
        cops += 1
        steps += csteps
        lastn = i
        attmpts.append(attempt)

    return cops / N, steps / cops, sum(attmpts)/len(attmpts)

