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

test_MsgHash256API()
test_HashMsgAPI()