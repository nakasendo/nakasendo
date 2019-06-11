import PyAsymKey

def test_GenerateKeyPairHex():

    # Generating Random private key ranging from 10 to 10000 in step 10
    for x in range(10, 1000, 10):

        # Generate pair of keys in hex format
        privKey, pubKey = PyAsymKey.GenerateKeyPairHEX()

        # Verifying the the length of actual value as 66
        assert len(privKey) == 66, "Test failed"

def test_GenerateKeyPairPEM():

    privString = "BEGIN PRIVATE KEY"

    # Generating Random private key ranging from 10 to 10000 in step 10
    for x in range(10, 1000, 10):

        # Generate pair of keys in pem format
        pubKey, privKey = PyAsymKey.GenerateKeyPairPEM()

        # Verifying the the length of actual value as 66
        assert privString in privKey, "Test failed"

def test_GetKeyPairHEX():

    privString = "BEGIN PRIVATE KEY"

    # Generating Random private key ranging from 10 to 10000 in step 10
    for x in range(10, 1000, 10):

        # Generate pair of keys in hex format
        puKey, prKey = PyAsymKey.GenerateKeyPairPEM()

        # Verifying the the length of actual value as 66
        assert privString in prKey, "Test failed"

        #Get pair of keys in hex format from a private PEM key
        privKey, pubKey = PyAsymKey.GetKeyPairHEX(prKey)

        # Verifying the the length of actual value as 66
        assert len(privKey) == 66, "Test failed"