import PyAsymKey

def test_GenerateKeyPairHex():
    for x in range(100):
        # Generate pair of keys in hex format
        privKey, pubKey = PyAsymKey.GenerateKeyPairHEX()

        # Verifying the the length of actual value as 66
        assert len(privKey) == 66, "Test failed"

def test_GenerateKeyPairPEM():
    privKeyPEMHeader = "BEGIN EC PRIVATE KEY"
    for x in range(100):
        # Generate pair of keys in pem format
        pubKey, privKey = PyAsymKey.GenerateKeyPairPEM()

        # Verifying the the length of actual value as 66
        assert privKeyPEMHeader in privKey, "Test failed"

def test_ExportKeyPairHEX():

    for x in range(100):
        # Generate pair of keys in pem format
        pubKeyPEM, privKeyPEM = PyAsymKey.GenerateKeyPairPEM()
        pubKeyHEX, privKeyHEX = PyAsymKey.ExportKeyPairHEX(privKeyPEM)

        # Keys hex are generated
        assert len(pubKeyHEX) > 0, "Test failed"
        assert len(privKeyHEX) > 0, "Test failed"

def test_ExportPublicKeyPEM():

    for x in range(100):
        # Generate pair of keys in pem format
        pubKeyPEM, privKeyPEM = PyAsymKey.GenerateKeyPairPEM()
        test_pubKeyPEM = PyAsymKey.ExportPublicPEM(privKeyPEM)

        # Calculated public key should match the generated one
        assert pubKeyPEM==test_pubKeyPEM, "Test failed"

def test_Sign_Verification():

    msg = 'Hello, I am a message, sign me'
    for x in range(100):
        # Generate pair of keys in PEM format
        pubKey, priKey = PyAsymKey.GenerateKeyPairPEM()

        # Sign message with private Key
        rSig, sSig = PyAsymKey.Sign(msg, priKey)

        # Verify message's signature with public key
        verify_ok = PyAsymKey.Verify(msg, pubKey, rSig, sSig)
        assert verify_ok, "Test failed"

def test_ShareSecret():

    for x in range(100):
        # Generate keys for alice and bob
        alice_pubKeyPEM, alice_privKeyPEM = PyAsymKey.GenerateKeyPairPEM()
        bob_pubKeyPEM, bob_privKeyPEM = PyAsymKey.GenerateKeyPairPEM()

        #Calculate shared secret from my private key and their public key
        secret_share_from_alice = PyAsymKey.ShareSecret(alice_privKeyPEM, bob_pubKeyPEM)
        secret_share_from_bob = PyAsymKey.ShareSecret(bob_privKeyPEM, alice_pubKeyPEM)
        assert secret_share_from_alice==secret_share_from_bob, "Test failed"

def test_KeyDerive():

    additive_msg = 'I am a random message used for key derivation'
    for x in range(100):
        # Generate keys for alice and bob
        alice_pubKeyPEM, alice_privKeyPEM = PyAsymKey.GenerateKeyPairPEM()
        bob_pubKeyPEM, bob_privKeyPEM = PyAsymKey.GenerateKeyPairPEM()

        # Derive public key from a given public key and a additive message
        alice_derived_pub = PyAsymKey.DerivePublic(alice_pubKeyPEM, additive_msg)
        bob_derived_pub = PyAsymKey.DerivePublic(bob_pubKeyPEM, additive_msg)

        # Derive pirvate key from a given private key and a additive message
        alice_derived_private = PyAsymKey.DerivePrivate(alice_privKeyPEM, additive_msg)
        bob_derived_private = PyAsymKey.DerivePrivate(bob_privKeyPEM, additive_msg)

        # Export public key PEM given the private key PEM
        calc_alice_derived_pub = PyAsymKey.ExportPublicPEM(alice_derived_private)
        calc_bob_derived_pub = PyAsymKey.ExportPublicPEM(bob_derived_private)
        assert calc_alice_derived_pub == alice_derived_pub, "Test failed"
        assert calc_bob_derived_pub == bob_derived_pub, "Test failed"