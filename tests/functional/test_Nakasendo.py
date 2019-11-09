import Nakasendo
import random
import string
from random import randint

##################################################################################################
#       All test data are generated using third parties online web application.                  #
#       The web sites used are as follow:-                                                       #
#       https://emn178.github.io/online-tools/base64_encode.html                                 #
#       https://incoherency.co.uk/base58/                                                        #
#       https://emn178.github.io/online-tools/sha256.html                                        #
##################################################################################################

def randomString(stringLength):

    # Generate a random string of fixed length
    letters = string.ascii_lowercase
    return ''.join(random.choice(letters) for i in range(stringLength))

def polynomial_evaluate(a_list, fx, mod):

    ans = 0
    for n, a in enumerate(a_list):
        ans += a * fx ** n

    ans %= mod
    return ans

def test_Polynomial():

    # for example usage, see Polynomial tests in :  $SDKLIBRARIES_ROOT/examples/python/TestNakasendo.py 
    # Test1 randomPolynomial( degree, modulo )
    # Test2 randomPolynomial( degree, modulo, fixed a_0 )
    # Test3 randomPolynomial( degree, modulo, min, max )
    # Test4 evalute for x=2
    # Test5 create polynomial from list
    # Test6 create polynomial from list with modulo

    assert True

def test_LGInterpolator():

    # for example usage, see LGInterpolation tests in :  $SDKLIBRARIES_ROOT/examples/python/TestNakasendo.py 
    # Test Interpolation for value x (full)
    # Test Interpolation for value x at ith basis (single point)

    assert True 

def test_EncodeBase58API(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir / "testData_Encode58Decode", "r") as encodeFile_txt:
        for x in encodeFile_txt.readlines():
            encode_Value = x.split(",")
            # Encode given string in base58
            actual_Value = Nakasendo.MessageHash(encode_Value[0])

            # Verifying the actual value with expected value
            assert actual_Value.Base58Encode() == encode_Value[1].rstrip("\n"), "Test failed"

def test_DecodeBase58API(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir / "testData_Encode58Decode", "r") as decodeFile_txt:
        for x in decodeFile_txt.readlines():
            decode_Value = x.split(",")
            # Decode given string in base58
            actual_Value = Nakasendo.MessageHash(decode_Value[1].rstrip("\n"))
            actual_Value = actual_Value.Base58Decode(decode_Value[1].rstrip("\n"))

            # Verifying the actual value with expected value
            assert actual_Value == decode_Value[0], "Test failed"

def test_EncodeBase58CheckedAPI(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir / "testData_Encode58CheckedDecode",
              "r") as encode58File_txt:  # Test data were generated using our python interface to encode the input value and decode the output for same input value

        for x in encode58File_txt.readlines():
            encode58_Value = x.split(",")
            # Encode given string in base58
            actual_Value = Nakasendo.MessageHash(encode58_Value[0])

            # Verifying the actual value with expected value
            assert actual_Value.Base58CheckedEncode() == encode58_Value[1].rstrip("\n"), "Test failed"

def test_DecodeBase58Checked58API(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir / "testData_Encode58CheckedDecode",
              "r") as decode58File_txt:  # Test data were generated using our python interface to encode the input value and decode the output for same input value

        for x in decode58File_txt.readlines():
            decode58_Value = x.split(",")
            # Decode given string in base58
            actual_Value = Nakasendo.MessageHash(decode58_Value[1].rstrip("\n"))
            actual_Value = actual_Value.Base58CheckedDecode(decode58_Value[1].rstrip("\n"))

            # Verifying the actual value with expected value
            assert actual_Value == decode58_Value[0], "Test failed"

def test_EncodeBase64API(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir / "testData_Encode64Decode", "r") as encodeFile_txt:
        for x in encodeFile_txt.readlines():
            encode_Value = x.split(",")
            # Encode given string in base64
            actual_Value = Nakasendo.MessageHash(encode_Value[0])

            # Verifying the actual value with expected value
            assert actual_Value.Base64Encode() == encode_Value[1].rstrip("\n"), "Test failed"

def test_DecodeBase64API(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir / "testData_Encode64Decode", "r") as decodeFile_txt:
        for x in decodeFile_txt.readlines():
            decode_Value = x.split(",")
            # Decode given string in base64
            actual_Value = Nakasendo.MessageHash(decode_Value[1].rstrip("\n"))
            actual_Value = actual_Value.Bas64Decode(decode_Value[1].rstrip("\n"))

            # Verifying the actual value with expected value
            assert actual_Value == decode_Value[0], "Test failed"

def test_GenerateKey():

    for x in range(10):
        # Generate a random string of fixed length
        String = randomString(10)

        # Generate a key from random generated nonce value
        actual_value = Nakasendo.SymEncDec(String)
        actual_value.GenerateKey()
        actual_value = actual_value.GetKey()

        # verifying the actual value with the expected value
        assert len(actual_value) == 64, "Test failed"

def test_EncodeDecodeAES(test_data_dir):

    for x in range(10):
        # Generate a random string of fixed length as secret code
        secretCode = randomString(100)

        # Generate a random string of fixed length
        String = randomString(10)

        # Generate a key from random generated nonce value
        keyAsHex = Nakasendo.SymEncDec(String)
        keyAsHex.GenerateKey()

        # verifying the length of generated key as 64
        assert len(keyAsHex.GetKey()) == 64, "Test failed"

        # Encoding the secret code using AES encryption
        encode_value = keyAsHex.Encode(secretCode)

        # Decoding the secret code using AES encryption
        decode_value = keyAsHex.Decode(encode_value)

        # Verifying decode value same as secret code
        assert decode_value == secretCode, "Test failed"

def test_AddFromDec_HexWithBigNumApi():

    for i in range (10):

        hexValue = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141"
        num = int(hexValue, 16)
        b = str(randint(100000000, num))
        c = str(randint(100000000, num))
        x = Nakasendo.BigNum()
        y = Nakasendo.BigNum()
        b = Nakasendo.BigNum(b,isDec=True)
        c = Nakasendo.BigNum(c,isDec=True)

        z = y.__add__(x)
        a = b.__add__(c)

        assert z == (x + y) and a == (b + c), "Test failed"

def test_GetAffineCoOrdinates():

    # Generating Random EC
    for x in range(10):
        # using the integer value of NID secp256k1, which is 714
        nID = 714

        # Generate a Random EC Point with default NID ==> NID_secp256k1
        hexValue = Nakasendo.ECPoint(nID)

        # EC Point GetAffineCoOrdinates_GFp with default NID => NID_secp256k1
        x_axis, y_axis = hexValue.GetAffineCoOrdinates()
        assert len(x_axis) == 62 or len(x_axis) == 64, "Test failed"

def test_CheckOnCurveFromHexOnCurve():

    # Generating Random EC Points
    for x in range(10):
        # using the integer value of NID secp256k1, which is 714
        nID = 714

        # Generate a Random EC Point with default NID ==> NID_secp256k1
        hexValue = Nakasendo.ECPoint(nID)
        assert hexValue.IsPointOnCurve() is True, "Test failed"

def test_MultiplyScalarMOnCurve():

    # Generating Random EC
    for x in range(10):
        # using the integer value of NID secp256k1, which is 714
        nID = 714

        # Generate a Random EC Point with default NID ==> NID_secp256k1
        hexValue = Nakasendo.ECPoint(nID)

        # EC Point GetAffineCoOrdinates_GFp with default NID => NID_secp256k1
        x_axis, y_axis = hexValue.GetAffineCoOrdinates()
        assert len(x_axis) == 62 or len(x_axis) == 64, "Test failed"

        # EC Point Scalar multiply on curve with supplied ID
        actualValue = str(hexValue.multipleScalar(hexValue))

        # Verifying the actual value with expected value
        assert len(actualValue) == 66, "Test failed"

def test_MultiplyScalarMNOnCurve():

    # Generating Random EC
    for x in range(10):
        # using the integer value of NID secp256k1, which is 714
        nID = 714

        # Generate a Random EC Point with default NID ==> NID_secp256k1
        mValue = Nakasendo.ECPoint(nID)
        nValue = Nakasendo.ECPoint(nID)

        # EC Point GetAffineCoOrdinates_GFp with default NID => NID_secp256k1
        x_axis, y_axis = mValue.GetAffineCoOrdinates()
        assert len(x_axis) == 62 or len(x_axis) == 64, "Test failed"

        # EC Point Scalar multiply with supplied curve ID
        actualValue = str(mValue.multipltScalarEx(mValue, nValue))

        # Verifying the the length of actual value as 66
        assert len(actualValue) == 66, "Test failed"

def test_GetGeneratorPoint():

    # Generating Random EC
    for x in range(10):
        # using the integer value of NID secp256k1, which is 714
        nID = 714

        # Generate a Random EC Point with default NID ==> NID_secp256k1
        hexValue = Nakasendo.ECPoint(nID)

        # EC Point GetAffineCoOrdinates_GFp with default NID => NID_secp256k1
        x_axis, y_axis = hexValue.GetAffineCoOrdinates()
        assert len(x_axis) == 62 or len(x_axis) == 64, "Test failed"
        assert hexValue.IsPointOnCurve() is True, "Test failed"
        # Generate a Elliptic curve point
        xPoint = str(hexValue.GetGeneratorPoint())
        assert len(xPoint) == 66, "Test failed"

def test_MultiplyByGenerator():

    # Generating Random EC
    for x in range(10):
        # using the integer value of NID secp256k1, which is 714
        nID = 714

        # Generate a Random EC Point with default NID ==> NID_secp256k1
        hexValue = Nakasendo.ECPoint(nID)
        bigNumB = Nakasendo.BigNum()
        # EC Point GetAffineCoOrdinates_GFp with default NID => NID_secp256k1
        x_axis, y_axis = hexValue.GetAffineCoOrdinates()
        assert len(x_axis) == 62 or len(x_axis) == 64, "Test failed"

        # EC Point Scalar multiply on curve with supplied ID
        actualValue = str(Nakasendo.MultiplyByGenerator(bigNumB,compressed=True))
        assert len(actualValue) == 66, "Test failed"


def test_SetKeyFromPem():

    for x in range(10):
        # Generate pair of keys in pem format
        pub_priKey = Nakasendo.ECKey256K1()
        pub_priKey2 = Nakasendo.ECKey256K1()

        # Retrieve the private from the PEM string
        privKey = pub_priKey.priKey

        # Sets a key from a PEM format
        pub_priKey2.FromPEMStr(privKey)

        # Calculated private key should match the generated one
        assert str(pub_priKey) == str(pub_priKey2), "Test failed"

def test_KeyDerive():

    additive_msg = 'I am a random message used for key derivation'
    for x in range(100):
        # Generate keys for alice and bob
        alice_pub_privKeyPEM = Nakasendo.ECKey256K1()
        bob_pub_privKeyPEM = Nakasendo.ECKey256K1()

        # Derive public key from a given public key and a additive message
        alice_derived_pub = alice_pub_privKeyPEM.derivePublicKey(additive_msg)
        bob_derived_pub = bob_pub_privKeyPEM.derivePublicKey(additive_msg)

        # Derive pirvate key from a given private key and a additive message
        alice_derived_private = alice_pub_privKeyPEM.derivePrivateKey(additive_msg)
        bob_derived_private = bob_pub_privKeyPEM.derivePrivateKey(additive_msg)

        # Get public key PEM given the private key PEM
        alice_pub_priKey2 = Nakasendo.ECKey256K1()
        bob_pub_priKey2 = Nakasendo.ECKey256K1()
        alice_pub_priKey2.FromPEMStr(alice_derived_private)
        bob_pub_priKey2.FromPEMStr(bob_derived_private)
        assert alice_pub_priKey2.pubKey == alice_derived_pub, "Test failed"
        assert bob_pub_priKey2.pubKey == bob_derived_pub, "Test failed"

def test_Sign_Verification():

    msg = 'Hello, I am a message, sign me'
    for x in range(100):
        # Generate pair of keys in PEM format
        pub_privKey = Nakasendo.ECKey256K1()
        pub_key = pub_privKey.pubKey

        # Sign message with private Key
        rSig, sSig = pub_privKey.sign(msg)

        # Verify message's signature with public key
        verify_ok = Nakasendo.verify(msg, pub_key, rSig, sSig)
        assert verify_ok, "Test failed"

def test_ShareSecret():

    for x in range(100):
        # Generate keys for alice and bob
        alice_pub_privKeyPEM = Nakasendo.ECKey256K1()
        bob_pub_privKeyPEM = Nakasendo.ECKey256K1()

        # alice_pubKeyPEM, alice_privKeyPEM = PyAsymKey.GenerateKeyPairPEM()
        # bob_pubKeyPEM, bob_privKeyPEM = PyAsymKey.GenerateKeyPairPEM()

        # Calculate shared secret from my private key and their public key
        secret_share_from_alice = alice_pub_privKeyPEM.CalculateSharedSecret(bob_pub_privKeyPEM.pubKey)
        secret_share_from_bob = bob_pub_privKeyPEM.CalculateSharedSecret(alice_pub_privKeyPEM.pubKey)
        assert secret_share_from_alice == secret_share_from_bob, "Test failed"

def test_SetSplitRestoreKey():

    for x in range(10):

        maxshares = 100
        thresholdList = []
        # Randomly generate two digit number that ranges from 3 to 100
        threshold = randint(3, 100)

        # Generate pair of keys in pem format
        pub_priKey = Nakasendo.ECKey256K1()
        pub_priKey2 = Nakasendo.ECKey256K1()

        # Retrieve the private from the PEM string
        privKey = pub_priKey.priKey

        # Sets a key from a PEM format
        pub_priKey2.FromPEMStr(privKey)
        assert str(pub_priKey) == str(pub_priKey2), "Test failed"

        # Split a private key into a given number of shares
        splitKeyList = pub_priKey2.SplitKey(threshold, maxshares)
        assert len(splitKeyList) == maxshares, "Test failed"

        for i in range(threshold):
            thresholdList.append(splitKeyList[i])

        assert len(thresholdList) == threshold, "Test failed"

        # Restore a private key from a given number of shares
        pub_priKey2.RecoverKey(thresholdList)
        assert pub_priKey2.priKey == privKey, "Test failed"

def test_RandomPolynomialDec():

    for x in range(10):
        fx = str(randint(1000, 1000000))
        degree = randint(10, 15)
        modulo = str(randint(10000, 1000000))

        # create a random polynomial
        listCoefficients = Nakasendo.Polynomial.initRandomDec(degree, modulo)
        assert listCoefficients.degree == degree, "Test failed"

        # calling evaluate polynomial function
        polynomial_EvaluateFX = listCoefficients(fx)

        # convert list of coefficients from string to decimal
        lst = []
        for i in (listCoefficients.coefficients):
            lst.append(int(i))

        fx = int(fx)
        modulo = int(modulo)
        actualValue = polynomial_evaluate(lst, fx, modulo)
        assert polynomial_EvaluateFX == str(actualValue), "Test failed"

def test_RandomPolynomialHex():

    for x in range(10):
        fx = hex(randint(10000, 1000000)).lstrip("0x")
        degree = randint(10, 15)
        modulo = hex(randint(10000, 1000000)).lstrip("0x")

        # create a random polynomial
        listCoefficients = Nakasendo.Polynomial.initRandomHex(degree, modulo)
        assert listCoefficients.degree == degree, "Test failed"

        # calling evaluate polynomial function
        polynomial_EvaluateFX = listCoefficients(fx)

        # convert list of coefficients from string to decimal
        lst = []
        for i in (listCoefficients.coefficients):
            lst.append(int(i, 16))

        fx = int(fx, 16)
        modulo = int(modulo, 16)
        actualValue = polynomial_evaluate(lst, fx, modulo)
        assert polynomial_EvaluateFX.lstrip("0") == hex(actualValue).upper().lstrip("0X"), "Test failed"

def test_RandomPolynomialFixed_a_0_Dec():

    for x in range(2):
        fx = str(randint(10000, 1000000))
        degree = randint(10, 15)
        modulo = str(randint(10000, 1000000))
        a_0 = str(randint(1000, 2000))

        # create a random polynomial with fixed a_0
        listCoefficients = Nakasendo.Polynomial.initRandomFixed_a_0_Dec(degree, modulo, a_0)
        assert listCoefficients.coefficients[0] == a_0, "Test failed"
        assert len(listCoefficients.coefficients) == (degree + 1), "Test failed"

        # calling evaluate polynomial function
        polynomial_EvaluateFX = listCoefficients(fx)

        # convert list of coefficients from string to decimal
        lst = []
        for i in (listCoefficients.coefficients):
            lst.append(int(i))

        fx = int(fx)
        modulo = int(modulo)
        actualValue = polynomial_evaluate(lst, fx, modulo)
        assert polynomial_EvaluateFX == str(actualValue), "Test failed"

def test_RandomPolynomialFixed_a_0_Hex():

    for x in range(10):
        fx = Nakasendo.BigNum().value
        degree = randint(10, 15)
        modulo = hex(randint(10000, 1000000)).lstrip("0x")
        a_0 = hex(randint(1000, 2000)).lstrip("0x").upper()

        # create a random polynomial with fixed a_0
        listCoefficients = Nakasendo.Polynomial.initRandomFixed_a_0_Hex(degree, modulo, a_0)
        assert listCoefficients.coefficients[0].lstrip("0") == a_0, "Test failed"
        assert len(listCoefficients.coefficients) == (degree + 1), "Test failed"

        # calling evaluate polynomial function
        polynomial_EvaluateFX = listCoefficients(fx)

        # convert list of coefficients from string to decimal
        lst = []
        for i in (listCoefficients.coefficients):
            lst.append(int(i, 16))

        fx = int(fx, 16)
        modulo = int(modulo, 16)
        actualValue = polynomial_evaluate(lst, fx, modulo)
        assert polynomial_EvaluateFX.lstrip("0") == hex(actualValue).upper().lstrip("0X"), "Test failed"

def test_RandomPolynomialMinMaxDec():

    for x in range(10, 30):
        fx = str(randint(10, 100000))
        degree = randint(10, 15)
        modulo = str(randint(1000, 100000))
        min = str(randint(100, 100000))
        max = str(randint(100001, 200000))

        # create a random polynomial with range of min..max
        listCoefficients = Nakasendo.Polynomial.initRandomMinMaxDec(degree, modulo, min, max)
        assert len(listCoefficients.coefficients) == (degree + 1), "Test failed"

        # calling evaluate polynomial function
        polynomial_EvaluateFX = listCoefficients(fx)

        # convert list of coefficients from string to decimal
        lst = []
        for i in (listCoefficients.coefficients):
            lst.append(int(i))

        fx = int(fx)
        modulo = int(modulo)
        actualValue = polynomial_evaluate(lst, fx, modulo)
        assert polynomial_EvaluateFX == str(actualValue), "Test failed"

def test_RandomPolynomialMinMaxHex():

    for x in range(10):
        fx = Nakasendo.BigNum().value
        degree = randint(10, 15)
        modulo = hex(randint(10000, 1000000)).lstrip("0x")
        min = hex(randint(10000, 10000000)).lstrip("0x")
        max = hex(randint(10000001, 100000000)).lstrip("0x")

        # create a random polynomial with range of min..max
        listCoefficients = Nakasendo.Polynomial.initRandomMinMaxHex(degree, modulo, min, max)
        assert len(listCoefficients.coefficients) == (degree + 1), "Test failed"

        # calling evaluate polynomial function
        polynomial_EvaluateFX = listCoefficients(fx)

        # convert list of coefficients from string to decimal
        lst = []
        for i in (listCoefficients.coefficients):
            lst.append(int(i, 16))

        fx = int(fx, 16)
        modulo = int(modulo, 16)
        actualValue = polynomial_evaluate(lst, fx, modulo)
        assert polynomial_EvaluateFX.lstrip("0") == hex(actualValue).upper().lstrip("0X"), "Test failed"

def test_InitFromListModuloDec():

    for x in range(10, 20):

        modHex = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141"
        fx = str(randint(10, 100000))
        modulo = str(int(modHex, 16))
        # modulo = str(PyBigNumbers.GenerateRandPrimeDec(100))
        degree = randint(10, 15)

        # create a random polynomial
        listCoefficients = Nakasendo.Polynomial.initRandomDec(degree, modulo)
        assert len(listCoefficients.coefficients) == (degree + 1), "Test failed"

        # create a Polynomial from a list of coefficient with modulo
        coefficientsWithModulo = Nakasendo.Polynomial.initFromListModuloDec(listCoefficients.coefficients, modulo)
        assert len(coefficientsWithModulo.coefficients) == degree + 1, "Test failed"

        # calling evaluate polynomial function
        polynomial_EvaluateFX = coefficientsWithModulo(fx)

        # convert list of coefficients from string to decimal
        lst = []
        for i in (coefficientsWithModulo.coefficients):
            lst.append(int(i))

        fx = int(fx)
        modulo = int(modulo)
        actualValue = polynomial_evaluate(lst, fx, modulo)
        assert polynomial_EvaluateFX == str(actualValue), "Test failed"

def test_InitFromListModuloHex():

    for x in range(10, 20):
        modHex = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141"
        fx = Nakasendo.BigNum().value
        degree = randint(10, 15)

        # create a random polynomial
        listCoefficients = Nakasendo.Polynomial.initRandomHex(degree, modHex)
        assert len(listCoefficients.coefficients) == (degree + 1), "Test failed"

        # create a Polynomial from a list of coefficient with modulo
        coefficientsWithModulo = Nakasendo.Polynomial.initFromListModuloHex(listCoefficients.coefficients, modHex)
        assert len(coefficientsWithModulo.coefficients) == degree + 1, "Test failed"

        # calling evaluate polynomial function
        polynomial_EvaluateFX = coefficientsWithModulo(fx)

        # convert list of coefficients from string to decimal
        lst = []
        for i in (coefficientsWithModulo.coefficients):
            lst.append(int(i, 16))

        fx = int(fx, 16)
        modulo = int(modHex, 16)
        actualValue = polynomial_evaluate(lst, fx, modulo)
        assert polynomial_EvaluateFX.lstrip("0") == hex(actualValue).upper().lstrip("0X"), "Test failed"

def test_InitFromListDec():

    for x in range(10, 20):

        fx = str(randint(10, 100000))
        modulo = str(randint(1000, 100000))
        listCoefficients = []

        for i in range(x):
            # Generate random coefficients for the polynomial
            listCoefficients.append(str(randint(100000, 1000000)))

        # create a Polynomial from a list of coefficients
        allCoeffeicient = Nakasendo.Polynomial.initFromListDec(listCoefficients)
        assert len(allCoeffeicient.coefficients) == x, "Test failed"

        # Calling evaluate polynomial function
        allCoeffeicient.modulo = modulo
        polynomial_EvaluateFX = allCoeffeicient(fx)

        # convert list of coefficients from string to decimal
        lst = []
        for i in (allCoeffeicient.coefficients):
            lst.append(int(i))

        fx = int(fx)
        modulo = int(modulo)
        actualValue = polynomial_evaluate(lst, fx, modulo)
        assert polynomial_EvaluateFX == str(actualValue), "Test failed"

def test_InitFromListHex():

    for x in range(10, 20):

        fx = Nakasendo.BigNum().value
        modulo = str(randint(1000, 100000))
        listCoefficients = []

        for i in range(x):
            # Generate random coefficients for the polynomial
            listCoefficients.append(Nakasendo.BigNum().value)

        # create a Polynomial from a list of coefficients
        allCoeffeicient = Nakasendo.Polynomial.initFromListHex(listCoefficients)
        assert len(allCoeffeicient.coefficients) == x, "Test failed"

        # Calling evaluate polynomial function
        allCoeffeicient.modulo = modulo
        polynomial_EvaluateFX = allCoeffeicient(fx)

        # convert list of coefficients from string to decimal
        lst = []
        for i in (allCoeffeicient.coefficients):
            lst.append(int(i, 16))

        fx = int(fx, 16)
        modulo = int(modulo, 16)
        actualValue = polynomial_evaluate(lst, fx, modulo)
        assert polynomial_EvaluateFX.lstrip("0") == hex(actualValue).upper().lstrip("0X"), "Test failed"

def test_LGInterpolatorFullDec():

    listTupleObj = [(2, "10"), (3, "15")]
    modulo = "0"
    dec = 1

    for x in range(100, 101):
        randomX = randint(1000, 1000000)
        xValue = str(randomX)

        # LGInterpolator, full evaluation of the polynomial at xValue
        lgInterpolatorX = Nakasendo.LGInterpolator(listTupleObj, modulo, dec)
        lgInterpolatorX = lgInterpolatorX(xValue)
        TestVal = Nakasendo.BigNum(value=str(randomX*5), mod=modulo,isDec=dec)
        assert lgInterpolatorX == TestVal, "Test failed"

def test_LGInterpolatorFullHex():

    listTupleObj = [(2, "A"), (3, "F")]
    modulo = "0"
    hex_value = 0

    for x in range(100, 200):
        randomX = Nakasendo.BigNum()
        xValue = str(randomX.value)
        # LGInterpolator, full evaluation of the polynomial at xValue
        lgInterpolatorX = Nakasendo.LGInterpolator(listTupleObj, modulo, hex_value)
        lgInterpolatorX = lgInterpolatorX(xValue)
        TestVal = Nakasendo.BigNum(value=hex(int(randomX.value, 16) * 5).lstrip("0x").upper(),mod=modulo,isDec =hex_value)
        assert lgInterpolatorX  == TestVal, "Test failed"

def test_LGInterpolatorSingleDec():

    listTupleObj = [(1, "13"), (2, "4"), (3, "2"), (4, "5"), (5, "11"), (6, "1")]
    modulo = str(1166395137945795428855301183413)
    dec = 1
    xValue = str(randint(10, 100000))

    for x in range(1, 6):
        xPoint = str(x)

        # LGInterpolator, evaluate the ith basis polynomial at xValue
        lgInterpolatorX = Nakasendo.LGInterpolator(listTupleObj, modulo, dec)
        lgInterpolatorX = lgInterpolatorX(xValue, xPoint)
        assert type(lgInterpolatorX) == Nakasendo.BigNum, "Test failed"

def test_LGInterpolatorSingleHex():

    listTupleObj = [(1, "13"), (2, "4"), (3, "2"), (4, "5"), (5, "11"), (6, "1")]
    modulo = str(1238275102653497496038888277977)
    hex_value = 0
    xValue = str(randint(10, 100000))
    for x in range(1, 6):
        xPoint = str(x)
        # LGInterpolator, evaluate the ith basis polynomial at xValue
        lgInterpolatorX = Nakasendo.LGInterpolator(listTupleObj, modulo, hex_value)
        lgInterpolatorX = lgInterpolatorX(xValue, xPoint)
        assert type(lgInterpolatorX) == Nakasendo.BigNum, "Test failed"

def test_LGECInterpolatorFull():

    # modulo = PyBigNumbers.GenerateRandPrimeHex(1000)
    modulo = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141"
    xValue = Nakasendo.BigNum().value
    listTupleObj = []
    hex_value = False
    # Generating Random EC
    for x in range(10, 50):
        # Generate a Random EC Point with default NID ==> NID_secp256k1
        hexValue = Nakasendo.ECPoint()
        print(hexValue.value)
        # Check if the point is on the curve with the supplied NID default NID ==> NID_secp256k1
        assert hexValue.IsPointOnCurve(), "Test failed"

        x_Axis, y_axis = hexValue.GetAffineCoOrdinates()
        # EC Point GetAffineCoOrdinates_GFp with default NID => NID_secp256k1
        listTupleObj.append((str(x), x_Axis, y_axis))

    lgInterpolatorX = Nakasendo.LGECInterpolator(listTupleObj, modulo,hex_value)
    lgInterpolatorX = lgInterpolatorX(xValue)
    assert type(lgInterpolatorX) == Nakasendo.ECPoint, "Test failed"
