
# coding: utf-8

# In[1]:

import hashlib
import rsa
import binascii
import os
from gmpy2 import mpz, iroot, powmod, mul, t_mod


# In[2]:

def to_bytes(n):
    """ Return a bytes representation of a int """
    return n.to_bytes((n.bit_length() // 8) + 1, byteorder='big')

def from_bytes(b):
    """ Makes a int from a bytestring """
    return int.from_bytes(b, byteorder='big')

def get_bit(n, b):
    """ Returns the b-th rightmost bit of n """
    return ((1 << b) & n) >> b

def set_bit(n, b, x):
    """ Returns n with the b-th rightmost bit set to x """
    if x == 0: return ~(1 << b) & n
    if x == 1: return (1 << b) | n

def cube_root(n):
    return int(iroot(mpz(n), 3)[0])


# In[94]:

# Message dont on veut contrefaire une signature
# message = "Ciao, ma888".encode("ASCII") # plante pour un hash pair (dernier bit = 0)
message = "Ciao, mamma!!".encode("ASCII")
# On calcule son hash
message_hash = hashlib.sha256(message).digest()


# In[53]:

# La partie ASN.1
ASN1_blob = rsa.pkcs1.HASH_ASN1['SHA-256']
# Suffixe que doit avoir la signature une fois décodée
suffix = b'\x00' + ASN1_blob + message_hash


# In[54]:

# Aperçu du suffix a obtenir après application de la clé publique sur la signature
binascii.hexlify(suffix)


# In[55]:

len(suffix)


# In[56]:

# On vérifie que le dernier bit du hash vaut 1
suffix[-1]&0x01 == 1 # easy suffix computation works only with odd target


# In[81]:

# suffixe de la signature avant déchiffrement avec la clé publique (avant mise au cube)
sig_suffix = 1
# On boucle sur chaque bit du suffixe a obtenir.
# poids faible -> poids fort
for b in range(len(suffix)*8):
    # On fait en sort que le cube de sig_suffixe soit égal à suffix, au moins sur la taille du suffix à obtenir
    if get_bit(sig_suffix ** 3, b) != get_bit(from_bytes(suffix), b):
        sig_suffix = set_bit(sig_suffix, b, 1)


# In[82]:

# On vérifie que la fin du cube du suffixe à obtenir est égale au suffixe à obtenir
to_bytes(sig_suffix ** 3).endswith(suffix) # BOOM


# In[83]:

len(to_bytes(sig_suffix ** 3)) * 8


# In[84]:

# On construit le prefix de la signature
while True:
    # Après application du cube, on aimerait obtenir ce prefixe
    prefix = b'\x00\x01' + os.urandom(2048//8 - 2)
    
    # On prend la racine cubique du prefix visé (On fait l'hyp que (00 01 *)^3 ~ 00 01)
    # A la fin, ajout de zéros de la taille du suffixe (pour faire l'addition)
    sig_prefix = to_bytes(cube_root(from_bytes(prefix)))[:-len(suffix)] + b'\x00' * len(suffix)
    
    # On crée la signature en additionnant suffixe et prefixe
    sig = sig_prefix[:-len(suffix)] + to_bytes(sig_suffix)
    
    # Il ne faut pas de zéro qui soit dans le préfix de la signature déchiffrée
    if b'\x00' not in to_bytes(from_bytes(sig) ** 3)[:-len(suffix)]: break


# In[85]:

to_bytes(from_bytes(sig) ** 3).endswith(suffix)


# In[86]:

to_bytes(from_bytes(sig) ** 3).startswith(b'\x01')


# In[87]:

len(to_bytes(from_bytes(sig) ** 3)) == 2048//8 - 1


# In[88]:

b'\x00' not in to_bytes(from_bytes(sig) ** 3)[:-len(suffix)]


# In[89]:

# Contrefaçon de la signature
binascii.hexlify(sig)


# In[90]:

# La signature déchiffrée
binascii.hexlify(to_bytes(from_bytes(sig) ** 3))


# In[91]:

key = rsa.newkeys(2048)[0]
key.e = 3


# In[92]:

rsa.verify(message, sig, key)


# In[ ]:




# In[ ]:



