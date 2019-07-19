import PySymEncDec
import random
import string

def randomString(stringLength):

    #Generate a random string of fixed length
    letters = string.ascii_lowercase
    return ''.join(random.choice(letters) for i in range(stringLength))

def test_Generate_Nonce():

    for x in range(100):

        # Generate a nonce value of fixed length.
        nonce_value = PySymEncDec.GenerateNounce()

        # verifying the length of nonce value as 32
        assert len(nonce_value) == 32, "Test failed"

def test_GenerateKey():

    for x in range(100):

        nounceString = PySymEncDec.GenerateNounce();
        # Generate a random string of fixed length
        String = randomString(10)

        # Generate a nonce value of fixed length.
        nonce_value = PySymEncDec.GenerateNounce()

        #Generate a key from random generated nonce value
        actual_value = PySymEncDec.GenerateKey(String, nonce_value)

        # verifying the actual value with the expected value
        assert len(actual_value) == 64, "Test failed"
    

#def test_EncodeAES(test_data_dir):

    nonce_value = "c280f8553e93485876e423b1d3f2e3a7"

    # Generate a key from random string of fixed length
    genKey = PySymEncDec.GenerateKey("ghmgpipbmz", nonce_value)

    # verifying the generated key
 #   assert len(genKey) == 64, "Test failed"

    # Reading test data from the file
#    with open(test_data_dir/"testData_EncDecode", "r") as strEncode_txt: # Test data were generated using our python interface to encode the input value and decode the output for same input value

#        for x in strEncode_txt:

#            strEncode = x.split(",")

            actual_value = PySymEncDec.encodeAES(strEncode[0], genKey, nonce_value)

#            assert actual_value == strEncode[1].rstrip("\n"), "Test failed"

#def test_DecodeAES(test_data_dir):

    nonce_value = "c280f8553e93485876e423b1d3f2e3a7"

    # Generate a key from random string of fixed length
    genKey = PySymEncDec.GenerateKey("ghmgpipbmz", nonce_value)

    # verifying the generated key
 #   assert len(genKey) == 64, "Test failed"

    # Reading test data from the file
#    with open(test_data_dir/"testData_EncDecode", "r") as strEncode_txt: # Test data were generated using our python interface to encode the input value and decode the output for same input value

#        for x in strEncode_txt:

#            strEncode = x.split(",")

            actual_value = PySymEncDec.decodeAES(strEncode[1].rstrip("\n"), genKey, nonce_value)

#            assert actual_value == strEncode[0], "Test failed"

def test_EncodeDecodeAES(test_data_dir):

    nonce_value = "c280f8553e93485876e423b1d3f2e3a7"

    # Generate a key from random string of fixed length
    genKey = PySymEncDec.GenerateKey("ghmgpipbmz", nonce_value)

    # verifying the generated key
    assert len(genKey) == 64, "Test failed"

    # Reading test data from the file
    with open(test_data_dir/"testData_EncDecode", "r") as strEncode_txt: # Test data were generated using our python interface to encode the input value and decode the output for same input value

        for x in strEncode_txt:

            strEncode = x.split(",")

            encode_value = PySymEncDec.encodeAES(strEncode[0], genKey, nonce_value)
            actual_value = PySymEncDec.decodeAES(encode_value, genKey, nonce_value)

            assert actual_value == strEncode[0], "Test failed"
