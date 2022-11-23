# Ledger Monero master spend key extraction, v2

This vulnerability report is a follow-up on the previous report `ledger_monero_vulnerability_disclosure.md`.
I will use the same notation and assume a reader is familiar with the first report.

## Stronger vulnerability (patched protocol is affected)

The new protocol contains following fixes:

- `sc_sub`, `sc_add` removed
- `get_subaddress_secret_key_with_ephemeral` added

The new vulnerability uses a smaller set of provided API functions, 
requires fewer assumptions and uses the provided functions very similarly 
to the original Monero codebase client, which makes it difficult to 
avoid.

### Notation 
- `x_m` corresponds to `x || hmac(x)`, thus message plus its HMAC, for brevity.
- `x_{scalar}` is value `x` decoded as a scalar value.
- `H()` is a Keccak hash function, as used in Monero.
- `x || y` represents a binary concatenation of `x` and `y`.
  
  
### Description

- Get A (public view key) from the Ledger or the wallet address (doable off-line).
- Find a scalar `x`, while the following holds:
     - `Pb = encode_point(8*x*a*G)`, `P = 8*x*a*G = 8*x*A`
     - `Pb == encode_scalar(decode_scalar(Pb))`
     - i.e., the encoding `Pb` of the point `P` can be interpreted both as a EC point and 
     as a scalar (without modular reduction required)
     - This is performed off-line, in the PoC, card interaction is not required as we have `A`
- Call `monero_apdu_generate_key_derivation(x*G, a_placeholder)` to obtain `{enc(8*a*(x*G))_m} = {enc(P)_m}`
     - We thus know `{P, enc(P)_m}`, i.e., plaintext-ciphertext pair
- Call `monero_apdu_derive_secret_key(enc(P)_m, 0, b_placeholder)` to get `se = enc(Hs(P||0) + b)_m`
- Call `mlsag_hash(p2=1, opt=0x80)` to get `c` in plaintext.
- Call `mlsag_sign(se, enc(P)_m)`
     - We obtain `r = se - c*P = se - c*(8*x*a*G)_{scalar}`
     - Note the `P` is now decoded as a scalar value
     - Compute the master spending key `b` as `b = r - Hs(P||0) + c*P_{scalar}`
     - We can compute `Hs(P||0)` as `P` plaintext value is known.


### Notes
- Step 2 for searching `x` meeting the criteria is for convenience so we can 
obtain an encrypted form of a known value, usable both as Point and also as a scalar. 
We could use `x=1`, but the modular reduction in the last step in `mlsag_sign`
would change `c*P_{scalar}` value, and we would need to correct the final result a bit.
Moreover, it is better to supply already reduced scalars as non-reduced scalars could 
be rejected by additional countermeasures, thwarting the attack vector

- According to the numerical simulation, the `E[steps_finding_x(A)] = 15`, i.e., 
on average in 15 steps we find suitable `x` value. 

- The attack uses an only small set of functions, all function calls besides the last one `mlsag_sign()`
are legit and could appear in the normal transaction construction process. It is thus hard to prevent
this from working. 

- The PoC is implemented in `poc.py` as `poc2()` method. It was successfully tested
on my personal Ledger Nano S.

### Functions used
```python
reset()
set_mode()
open_tx()
gen_derivation()
derive_secret_key()
mlsag_hash()
mlsag_sign()
```

## Requirements

- Connected Ledger, entered PIN, selected Monero app.
    - Usually when sending a transaction, setting up the Monero wallet.
    - If the master view key was not exported, then the scenario happens with each blockchain scanning.

## Impact

- No user confirmation is required to mount the attack.
- The user is not notified about the transaction being in progress. No error is shown. The display does not change.
- The user has no chance to notice his master spend key was extracted.
- PoC works for the updated Ledger Monero app. The exploitation is possible from the initial
protocol deployment date. User spend keys could have been silently exfiltrated without users knowing.
There is no way to tell whether this attack was executed in the wild. 
- Existing spend keys should thus be considered leaked and not secure to use.
- Ledger Monero app currently does not support changing the BIP-44 derivation path for 
Monero master key derivation, thus users are currently not able to use Ledger to store Monero securely
if they used it with the Monero before.

## Observations

- Scalars / points can be used interchangeably in the protocol. This *type confusion* 
is a significant vulnerability. Especially when the attacker manages to obtain known 
plaintext-ciphertext pair, which can then later be used in both contexts (scalar, point).
Knowing the plaintext value is important for the computation of `Hs(P||0)` and `c*P` elimination.

- When the view key is extracted (for faster blockchain scanning), the plaintext-ciphertext
pair cannot be prevented for scalars, as the attacker knows `a` and can use `a_placeholder` to make
Ledger computes scalars with `a`. E.g., `monero_apdu_derive_secret_key(deriv, idx, a)` can be 
used to construct scalar plaintext-ciphertext pair. 
 
- `mlsag_sign` is important for all attacks as it returns an unencrypted scalar value from
originally encrypted scalar inputs. It is used as a decryption oracle.


# Countermeasures

To make the protocol secure against the mentioned family of attacks the aforementioned 
weak spots have to be eliminated.


## Remove simple scalar functions

As correctly proposed by the Ledger, removing `sc_sub()` and `sc_add()` helps significantly. 
As demonstrated in the previous report, the attacker can construct many usable scalar values that
can be later used in the attack. 

Note that `sc_sub()` can be emulated just with `sc_add()` as it holds that:
- `l*x == 0 (mod) l`, where `l` is the group order
- `(l-1)*x == -x (mod) l`. Construction is similar to the one described in the previous report, by
constructing a binary base `[enc(x^i)_m]_{i \in [1, 255]}`, using binary representation of `(l-1)` to 
add basis elements to obtain `(l-1)*x` by adding bases from the binary representation. 
- One subtraction thus requires 252 + 73 + 1 additions.
- Obtaining plaintext-ciphertext pairs is doable via GCD and pre-computed tables with small scalars.


## User confirmation / notification

- As the HMAC key is changed with each new transaction, the user should be 
explicitly asked to confirm the transaction signing process once `open_tx()` is called 
in the real transaction mode. I.e., Ledger should ask the user whether he wants to continue
with the transaction signature. The user confirms by pressing a button. 

- User confirmation is required to mount any attack. Attack surface is thus reduced 
to the point when the user is actively sending a new transaction, the time window is 
significantly reduced.

- Ledger should display information on the display when `open_tx` was called, even for fake
transactions. Any display change would be nice, so the user is able to notice that Ledger is
performing some tasks. 

- When the transaction is finished with error (e.g., some security assertion fails), user should be 
notified on the screen and optionally asked for confirmation to continue in a normal 
operation. The attacker thus cannot just flash the error message over short period of time
without user noticing. 

- Some other attacks we considered require more transaction openings so limiting it 
by requiring the confirmation lowers the attack surface significantly.


## Proper input validation

- Not sure if this is performed in the current version, better check it.
- Each EC point should be verified that it lies on the ED25519 curve.
    - After each decryption, if it is not already checked in every EC operation.
- Each scalar should be checked if it is reduced.
- Scalar values should not be `0`. 
- If any assertion fails, abort the transaction, reset keys, notify the user and ask for confirmation to continue.
- Stronger requirement: if the assertion fails, ask for PIN re-entry.


## Symmetric key hierarchy

This is the primary countermeasure that blocks all attacks we considered. 
For the sake of simplicity, we will assume just HMAC keys for now and address `spk` key later.

- The HMAC key is changed with each new transaction (as now), let call it `khm`, i.e., the master hmac key
- HMAC key used for particular parameters is derived from `khm` based on the following
   - Value type, scalar / point
   - Content type, derived secret / random mask
   - Function calling context. e.g., some scalars are accepted only in some functions.
- Encrypted values are thus usable only in a particular context, i.e., the context with the same HMAC key.
- This also prevents the *type confusion*.  
   
Example:
- HMAC key for EC points - derivation: `H(khm || "0")`.  
- HMAC key for scalars: `H(khm || "1")`
- HMAC key for random scalar masks: `H(khm || "2")`
- HMAC key for amount key: `H(khm || "3")`

Other EC points than derivations are not exported in an encrypted form in the protocol. 
If there are more EC point types later, differentiate them.

Ideally, the encryption key should also be changed with each new transaction (random), if possible. 
Definitely, for values we are sure were produced after `open_tx()`. Thorough protocol analysis
or just simple testing will reveal which values need to have fixed `spk` key.
We would suggest to start testing this improvement with the encryption key `spk` being randomly generated after `open_tx()`.
After transaction finish/abort the key is reverted back to static `spk`. 

Different encryption key strictly limits attacker to the scope of one transaction with respect 
to the data confidentiality, which is useful for security arguments. I.e., no long-term analysis 
and data collection can be performed.

The specified HMAC key hierarchy is also usable for encryption, which decreases the attack surface 
significantly as values are valid only in a particular context. This is especially important as the 
initialization vector (IV) is zero = encryption has no semantic security, i.e., same plaintexts encrypt
to the same ciphertexts. The zero IV allows the attacker to test values for equality without knowing the plaintext values.

The key hierarchy significantly restricts the potential combinations attacker can use,
restricting to explicitly allowing ones by the protocol designer.

## MLSAG Sign 

Recall `mlsag_sign(alpha, xx) = alpha - c*xx`, where:
   - `c` is parameter known to attacker
   - `alpha` is a random scalar mask
   - `xx` is a secret scalar value
   
Notice that if `alpha` is allowed to be used more than once, we have a decryption oracle: 
- `mlsag_sign(alpha1, xx1) = r1`
- `mlsag_sign(alpha1, xx2) = r2`
- `r1 - r2 = (alpha1 - c*xx1) - (alpha1 - c*xx2) = alpha1 - c*xx1 - alpha1 + c*xx2 = c*(xx2-xx1)`
- As `c` is known, attacker can recover `xx2-xx1`. If attacker knows plaintext value for one scalar secret, 
let say `xx1` he can recover scalar value for `xx2`.
- `xx1` can be constructed by calling `monero_apdu_derive_secret_key(deriv, idx, a)` as we usually know `a` as it was exported to the client.
- Similarly, if `xx1` is known, then `alpha = r1 - c*xx1`.
- We do not consider type confusion and other attacks as those are eliminated by key hierarchy.

Monero currently uses only the `MLSAG_SIMPLE` signature scheme. The `MLSAG_FULL` is not needed with Bulletproof transactions
and thus, Ledger does not have to support it. This reduces the attack surface and simplifies countermeasures design. 
Thus it holds that `mlsag_prepare()` is called only once per signature (for non-multisig transaction),
followed by exactly one `mlsag_sign()` call (it holds `dsRows==1`).

We propose to extend the state by adding a `sign_counter`, which is incremented in the beginning 
of the `mlsag_prepare()` call.
The encryption and HMAC keys for `alpha` are then derived as:
`H(khm || "a" || sign_counter)`.

This guarantees that only `alpha` generated by the `mlsag_prepare()` can be passed to the `mlsag_sign()`
as the first `alpha` parameter. Separation of `alpha` and `xx` domains via different keys restricts the 
attack surface.

It is easy to show that if `alpha` is a random scalar, then the attacker can derive no information about `xx1`
from `alpha - c*xx1`. The reason is that `alpha` can be generated only in `mlsag_prepare()` and 
used only in `mlsag_sign()` as a first parameter, nowhere else.
It is essential that `alpha` can be used only once as input to the `mlsag_sign()`. 
Otherwise, the attacker can eliminate it.
 
Thus the attacker can derive no information about `alpha` using other functions than `mlsag_sign()` as it fails
HMAC check in those. The attacker could learn `alpha` if he knows decryption of `xx1`, but such `alpha` is just a 
random scalar, and this knowledge cannot be reused in another `mlsag_sign()` call, making the knowledge useless.   

This countermeasure likely breaks the multisig compatibility as `alpha[i] = kLRki->k;` for multisig.
However, if alpha reuse is possible the `mlsag_sign()` can be used as decryption oracle 
and this cannot be simply prevented as the result of the `mlsag_sign()` directly appears in
the transaction signature. 

We are not sure whether the multisig is currently supported. If yes, there is a separate analysis required 
to study how to do the multisig support securely. 


## Strict state model checking

Due to the low-level nature of the API functions, it is difficult to capture the 
explicit state model as the function call flow highly depends on the transaction being signed, 
i.e., a number of inputs, outputs, use of sub-addresses, UTXO types - aux keys used, etc...

However, the more the state model is restricted, the lesser is the attacker space.
It is recommended to study the valid transaction construction paths and enforce obvious state transitions.

For instance, enforce a rule that the `mlsag_prepare()` has to be followed exactly by the `mlsag_hash()` 
(several times, depends on mixin, not critical to enforce number of the `mlsag_hash()` calls).
Enforce that the `mlsag_sign()` can be called only after the `mlsag_hash()` and only once per `mlsag_prepare()`.
Ideally if the `mlsag_sign()` increments the `sign_counter` as well after it computes the `ss` result, 
to enforce state change, which prevents malicious state transitions.

Client change:
Commit to the {mixin, number of UTXO, number of transaction outputs} in the initial `open_tx()` call.
Then enforce the rule that number of calls to the `mlsag_prepare()` and `mlsag_sign()` has to be 
equal to the number of `UTXO` (as we have one signature per UTXO).

Note the basic state model enforcement can be done without changing the client. 
However, the more precise check requires to commit to the number of transaction inputs. 

## Conclusion

All aforementioned fixes are directly applicable on the Ledger side without the need to touch the Monero codebase.
The mentioned changes fix the whole family of attacks similar to those presented and effectively blocks 
the main attack vectors and leaks.

It is thus possible to fix the critical vulnerability without need to release a new Monero client version, 
which significantly speeds up the patch roll-out. 

# Client-changing countermeasures

Here follow the measures that require client modifications to work.
They improve security significantly but are not necessary to block the vulnerability. 

## Encrypt-then-reveal

We propose not to return plaintext values from `mlsag_sign()` directly, but to return encrypted versions,
under a new, transaction-specific encryption key `kse`, which is used specifically for this purpose.

After the transaction is successfully constructed, i.e., no security assertion was violated, the Ledger
returns the `kse` to the host client so it can decrypt the MLSAG signature. 

This countermeasure strictly enforces correct state transitions and blocks the attacker's reactivity.
I.e., the attacker cannot use results from the previous `mlsag_sign()` calls to adapt an attacking strategy
as he learns the result only after the protocol finishes successfully. This property is important for 
security proofs and to strictly guard the potential attacker space.

This change is very easy to implement and brings significant security benefits.
However, it requires minor client code change. 

We recommend using this measure with a new Ledger Monero protocol version.
After some time (all users migrate to new Monero clients enforcing new signing protocol) the support
for unencrypted `mlsag_sign()` can be dropped. 


## Support multiple BIP derivation paths

Allow user to specify BIP derivation path (or its part) when creating the wallet from the Ledger 
device to allow multiple cryptographically separated master (view, spend) keys derived from the seed.

Currently, it is fixed, to the best of our knowledge.
Having fixed derivation paths blocks the user from using a new set of keys with the same device seed.

For example, each user should consider current master keys leaked and dangerous to use. 
He cannot then use Ledger device without seed reset, which affects all other apps on the 
Ledger, i.e., the Bitcoin app. 

With the fixed path user also cannot transfer all funds to another safe address without using
software wallet (risk of spend key leak) or another Ledger device (lack of resources, need to buy new Ledger).

If the user can specify another path, the migration to a safe (non-leaked) account is simple.
The user creates another wallet with a different path and sweeps the old account to the new one. 


## Strict state model checking

As mentioned in the similarly named section above, the more precise checking can be done if
the `open_tx()` transaction message contains information about mixin, number of UTXOs, and transaction outputs.


# Recommendations

- All affected users should be notified the malware could have silently extracted their spend keys,
that there is no way to tell whether that happened or not.

- The users should consider existing keys leaked and transfer the funds to a secure location
(a bit tricky if they don't possess another unused Ledger). 
You have to assess the risk of waiting for Monero client to support multiple paths or to 
find another solution. 


------------------------------------

Dusan Klinec (ph4r05)
- contact: ph4r05 -at- gmail.com
- GPG ID: CCBCE103
- GPG fingerprint: AB35 9D7F B6BF B9AA 542C EAAB 6337 E118 CCBC E103

Created 18. Jan, 2020 in Brno, Czech Republic.
