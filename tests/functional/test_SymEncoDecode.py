import PySymEncDec
import random
import string

bitSize = "256"

def randomString(stringLength):

    #Generate a random string of fixed length
    letters = string.ascii_lowercase
    return ''.join(random.choice(letters) for i in range(stringLength))

def test_GenerateKey():

    for x in range(100):

        nounceString = PySymEncDec.GenerateNounce();
        # Generate a random string of fixed length
        String = randomString(10)

        #Generate a key from random string of fixed length
        actual_value = PySymEncDec.GenerateKey(String, nounceString)

        # verifying the actual value with the expected value
        assert len(actual_value) == 64, "Test failed"
    

#def test_EncodeAES(test_data_dir):

    # Generate a key from random string of fixed length
#    genKey = PySymEncDec.GenerateKey("ghmgpipbmz", bitSize)

    # verifying the generated key
 #   assert len(genKey) == 64, "Test failed"

    # Reading test data from the file
#    with open(test_data_dir/"testData_EncDecode", "r") as strEncode_txt: # Test data were generated using our python interface to encode the input value and decode the output for same input value

#        for x in strEncode_txt:

#            strEncode = x.split(",")

#            actual_value = PySymEncDec.encodeAES(strEncode[0], genKey, bitSize)

#            assert actual_value == strEncode[1].rstrip("\n"), "Test failed"

#def test_DecodeAES(test_data_dir):

    # Generate a key from random string of fixed length
#    genKey = PySymEncDec.GenerateKey("ghmgpipbmz", bitSize)

    # verifying the generated key
 #   assert len(genKey) == 64, "Test failed"

    # Reading test data from the file
#    with open(test_data_dir/"testData_EncDecode", "r") as strEncode_txt: # Test data were generated using our python interface to encode the input value and decode the output for same input value

#        for x in strEncode_txt:

#            strEncode = x.split(",")

#            actual_value = PySymEncDec.decodeAES(strEncode[1].rstrip("\n"), genKey, bitSize)

#            assert actual_value == strEncode[0], "Test failed"

def test_EncodeDecodeAES(test_data_dir):

    # Generate a nounce value
    genNounce = PySymEncDec.GenerateNounce()
    # Generate a key from random string of fixed length
    genKey = PySymEncDec.GenerateKey("ghmgpipbmz", genNounce)

    # verifying the generated key
    assert len(genKey) == 64, "Test failed"

    # Reading test data from the file
    with open(test_data_dir/"testData_EncDecode", "r") as strEncode_txt: # Test data were generated using our python interface to encode the input value and decode the output for same input value

        for x in strEncode_txt:

            strEncode = x.split(",")

            encode_value = PySymEncDec.encodeAES(strEncode[0], genKey, genNounce)
            actual_value = PySymEncDec.decodeAES(encode_value, genKey, genNounce)

            assert actual_value == strEncode[0], "Test failed"
