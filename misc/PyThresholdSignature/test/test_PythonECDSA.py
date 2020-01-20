#!/usr/bin/env python3

import random
import ecdsa
import random
import time
import sys
sys.path.append('../')
import FiniteGroup

def __test_PythonECDSA_SL364(msg_to_sign, private_key_hex, s_hex, r_hex):  ##
    G = ecdsa.SECP256k1.generator
    q = ecdsa.SECP256k1.order
    Gx_hex = hex(G.x())
    Gy_hex = hex(G.y())
    q_hex = hex(ecdsa.SECP256k1.order)

    ## s = k^{-1}[H(m) + dr] mod q
    Hm = FiniteGroup.hash_mod(msg_to_sign, q)
    Hm_nonmode = FiniteGroup.hash_mod(msg_to_sign,None)
    d = int(private_key_hex, 16)
    P= d*G
    s = int(s_hex, 16)
    r = int(r_hex, 16)
    ## Recalculate k knowing all (including private key d)
    s_inv = FiniteGroup.inv_mod(s, q)
    k_nonmode = s_inv * (Hm + d*r) #s^{-1}[H(m) + dr]
    k = FiniteGroup.normalize_mod(k_nonmode,q)
    inv_k = FiniteGroup.inv_mod(k,q)
    k_hex = hex(k)
    inv_k_hex = hex(inv_k)

    pub_key = ecdsa.ecdsa.Public_key(G, P)
    key = ecdsa.ecdsa.Private_key(pub_key, d)
    ecdsa_sig = key.sign(Hm, k)
    ecdsa_sig_s = ecdsa_sig.s
    ecdsa_sig_r = ecdsa_sig.r
    ##print('\n\nTest signature with native Python ecdsa module ---- ')
    ##print('Signing message = [{}]'.format(msg_to_sign))
    ##print('input d = [{}]'.format(private_key_hex))
    ##print('input k = [{}]'.format(k_hex[2:].upper()))
    ##print('   invk = [{}]'.format(inv_k_hex[2:].upper()))
    ##print('py::ecdsa s = {}'.format( hex(ecdsa_sig_s)))
    ##print('Imported  s =   {}'.format(s_hex.lower()))
    ##print('py::ecdsa r = {}'.format(hex(ecdsa_sig_r)))
    ##print('Imported  r =   {}'.format(r_hex.lower()))
    ##print('Hm modulo = {}'.format(hex(Hm)))
    ##print('Hm naked  = {}'.format(hex(Hm_nonmode)))


def test_PythonECDSA():
    ## Testcase helping to reproduce a bug reported in  https://nchain.atlassian.net/browse/SL-364
    private_key_hex = 'B0BD078D3A70DA8407398E764712680B762EDC0AE417B71AC29DB1DB19E6135F'
    msg_to_sign = 'I love deadlines. I love the whooshing noise they make as they go by.'
    s_hex = 'B1164242138D07C2828CF4E71FECFFB414EA78DA828C352E3381773788065F6B'
    r_hex = '7872B5465A46C3858C75FB1FC7684621C3E17C096CF9A31A6CC480645BE20990'
    __test_PythonECDSA_SL364(msg_to_sign, private_key_hex, s_hex, r_hex)
