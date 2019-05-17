import PyMessageHash

def test_MsgHash256API():

    # Reading test data from the file
    with open("testData_MsgSHA256", "r") as msgHashFile_txt:

        for x in msgHashFile_txt.readlines():

            msg_Hash_value = x.split(",")
            # Generate SHA256 hash on a given input
            actual_Value = PyMessageHash.HashMsgSHA256(msg_Hash_value[0])

            # Verifying the actual value with expected value
            assert actual_Value == msg_Hash_value[1].rstrip("\n"), "Test failed"
            assert len(actual_Value) == 64, "Test Failed"

    msgHashFile_txt.close()

def test_HashMsgAPI():

    # Reading test data from the file
    with open("testData_HashMsg", "r") as hashMsgFile_txt:

        for x in hashMsgFile_txt.readlines():

            hashMsg_value = x.split(",")
            # Generate a Hash on a given input using the hashing algorithim specified
            actual_Value = PyMessageHash.HashMsg(hashMsg_value[0], hashMsg_value[1])

            # Verifying the actual value with expected value
            assert actual_Value == hashMsg_value[2].rstrip("\n"), "Test failed"

    hashMsgFile_txt.close()

def test_EncodeBase64API():

    #Reading test data from the file
    with open("testData_EncodeDecode", "r") as encodeFile_txt:

        for x in encodeFile_txt.readlines():

            encode_Value = x.split(",")
            actual_Value = PyMessageHash.EncodeBase64(encode_Value[0])
            expected_Value = encode_Value[1].rstrip("\n")

            # Verifying the actual value with expected value
            assert actual_Value == encode_Value[1].rstrip("\n"), "Test failed"

    encodeFile_txt.close()

def test_DecodeBase64API():

    #Reading test data from the file
    with open("testData_EncodeDecode", "r") as decodeFile_txt:

        for x in decodeFile_txt.readlines():

            decode_Value = x.split(",")
            actual_Value = PyMessageHash.DecodeBase64(decode_Value[1].rstrip("\n"))

            #Verifying the actual value with expected value
            assert actual_Value == decode_Value[0], "Test failed"

    decodeFile_txt.close()