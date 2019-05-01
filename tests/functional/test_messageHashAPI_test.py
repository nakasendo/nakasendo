import msgHashlib

def test_MsgHash256API():

    # Reading test data from the file
    with open("testData_MsgSHA256", "r") as msgHashFile_txt:

        for x in msgHashFile_txt.readlines():

            msg_Hash_value = x.split(",")
            actual_Value = msgHashlib.hashMsgSHA256_method(msg_Hash_value[0])

            # Verifying the actual value with expected value
            assert actual_Value == msg_Hash_value[1].rstrip("\n"), "Test failed"
            assert len(actual_Value) == 64, "Test Failed"
            '''
            if (actual_Value == msg_Hash_value[1].rstrip("\n") and len(actual_Value) == 64):
                #print(actual_Value)
                print("Pass")

            else:
                print(actual_Value)
                print("False")
            '''
    msgHashFile_txt.close()

def test_HashMsgAPI():

    # Reading test data from the file
    with open("testData_HashMsg", "r") as hashMsgFile_txt:

        for x in hashMsgFile_txt.readlines():

            hashMsg_value = x.split(",")
            actual_Value = msgHashlib.hashMsg_method(hashMsg_value[0], hashMsg_value[1])

            # Verifying the actual value with expected value
            assert actual_Value == hashMsg_value[2].rstrip("\n"), "Test failed"
            '''
            if (actual_Value == hashMsg_value[2].rstrip("\n")):
                #print(actual_Value)
                print("Pass")

            else:
                print(actual_Value)
                print("False")
                '''
        hashMsgFile_txt.close()

test_MsgHash256API()
test_HashMsgAPI()