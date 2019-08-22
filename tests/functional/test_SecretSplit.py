import PyAsymKey
from random import randint

maxshares = 100
privKeyPEMHeader = "BEGIN EC PRIVATE KEY"


def test_SetKeyFromPEM():

    for x in range(10):

        # Generate pair of keys in pem format
        pubKey, privKey = PyAsymKey.GenerateKeyPairPEM()
        assert privKeyPEMHeader in privKey, "Test failed"

        # Sets a key from a PEM format
        pubValue, priValue = PyAsymKey.SetKeyFromPem(privKey)
        assert priValue == privKey, "Test failed"
        assert pubValue == pubKey, "Test failed"


def test_KeySplit():

    for x in range(10):

        #Randomly generate two digit number that ranges from 3 to 100
        threshold = randint(3, 100)

        # Generate pair of keys in pem format
        pubKey, privKey = PyAsymKey.GenerateKeyPairPEM()
        assert privKeyPEMHeader in privKey, "Test failed"

        # Split a private key into a given number of shares
        splitKeyList = PyAsymKey.SplitKey(privKey, threshold, maxshares)
        assert len(splitKeyList) == maxshares, "Test failed"


def test_RestoreKey():

    for x in range(10):

        thresholdList = []
        # Randomly generate two digit number that ranges from 3 to 100
        threshold = randint(3, 100)

        # Generate pair of keys in pem format
        pubKey, privKey = PyAsymKey.GenerateKeyPairPEM()
        assert privKeyPEMHeader in privKey, "Test failed"

        # Split a private key into a given number of shares
        splitKeyList = PyAsymKey.SplitKey(privKey, threshold, maxshares)
        assert len(splitKeyList) == maxshares, "Test failed"

        for i in range(threshold):

            thresholdList.append(splitKeyList[i])

        assert len(thresholdList) == threshold, "Test failed"

        # convert list to String
        thresholdString = ';'.join(thresholdList)

        # Restore a private key from a given number of shares
        restorePubKey, restorePrivKey = PyAsymKey.RestoreKey(thresholdString)
        assert restorePrivKey == privKey, "Test failed"


def test_SetSplitRestoreKey():

    for x in range(10):

        thresholdList = []
        # Randomly generate two digit number that ranges from 3 to 100
        threshold = randint(3, 100)

        # Generate pair of keys in pem format
        pubKey, privKey = PyAsymKey.GenerateKeyPairPEM()
        assert privKeyPEMHeader in privKey, "Test failed"

        # Sets a key from a PEM format
        pubValue, priValue = PyAsymKey.SetKeyFromPem(privKey)
        assert priValue == privKey, "Test failed"
        assert pubValue == pubKey, "Test failed"

        # Split a private key into a given number of shares
        splitKeyList = PyAsymKey.SplitKey(privKey, threshold, maxshares)
        assert len(splitKeyList) == maxshares, "Test failed"

        for i in range(threshold):

            thresholdList.append(splitKeyList[i])

        assert len(thresholdList) == threshold, "Test failed"

        # convert list to String
        thresholdString = ';'.join(thresholdList)

        # Restore a private key from a given number of shares
        restorePubKey, restorePrivKey = PyAsymKey.RestoreKey(thresholdString)
        assert restorePrivKey == privKey, "Test failed"


def test_ThresholdEqualMaxShareTestcase():

    thresholdList = []
    #Randomly generate two digit number that ranges from 3 to 100
    threshold = 100

    # Generate pair of keys in pem format
    pubKey, privKey = PyAsymKey.GenerateKeyPairPEM()
    assert privKeyPEMHeader in privKey, "Test failed"

    # Split a private key into a given number of shares
    splitKeyList = PyAsymKey.SplitKey(privKey, threshold, maxshares)
    assert len(splitKeyList) == maxshares, "Test failed"

    for i in range(threshold):

        thresholdList.append(splitKeyList[i])

    assert len(thresholdList) == threshold, "Test failed"

    # convert list to String
    thresholdString = ';'.join(thresholdList)

    # Restore a private key from a given number of shares
    restorePubKey, restorePrivKey = PyAsymKey.RestoreKey(thresholdString)
    assert restorePrivKey == privKey, "Test failed"