import PyMessageHash
import pytest
import pathlib

##################################################################################################
#       All test data are generated using third parties online web application.                  #
#       The web sites used are as follow:-                                                       #
#       https://emn178.github.io/online-tools/base64_encode.html                                 #
#       https://incoherency.co.uk/base58/                                                        #
#       https://emn178.github.io/online-tools/sha256.html                                        #
##################################################################################################

def test_HashMsgAPI(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir/"testData_HashMsg", "r") as hashMsgFile_txt:

        for x in hashMsgFile_txt.readlines():

            hashMsg_value = x.split(",")
            # Generate a Hash on a given input using the hashing algorithim specified
            actual_Value = PyMessageHash.HashMsg(hashMsg_value[0], hashMsg_value[1])

            # Verifying the actual value with expected value
            assert actual_Value == hashMsg_value[2].rstrip("\n"), "Test failed"

def test_MsgHash256API(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir/"testData_MsgSHA256", "r") as msgHashFile_txt:

        for x in msgHashFile_txt.readlines():

            msg_Hash_value = x.split(",")
            # Generate SHA256 hash on a given input
            actual_Value = PyMessageHash.HashMsgSHA256(msg_Hash_value[0])

            # Verifying the actual value with expected value
            assert actual_Value == msg_Hash_value[1].rstrip("\n"), "Test failed"
            assert len(actual_Value) == 64, "Test Failed"

def test_EncodeBase58API(test_data_dir):

    #Reading test data from the file
    with open(test_data_dir/"testData_Encode58Decode", "r") as encodeFile_txt:

        for x in encodeFile_txt.readlines():

            encode_Value = x.split(",")
            # Encode given string in base58
            actual_Value = PyMessageHash.EncodeBase58(encode_Value[0])

            # Verifying the actual value with expected value
            assert actual_Value == encode_Value[1].rstrip("\n"), "Test failed"

def test_DecodeBase58API(test_data_dir):

    #Reading test data from the file
    with open(test_data_dir/"testData_Encode58Decode", "r") as decodeFile_txt:

        for x in decodeFile_txt.readlines():

            decode_Value = x.split(",")
            # Decode given string in base58
            actual_Value = PyMessageHash.DecodeBase58(decode_Value[1].rstrip("\n"))

            #Verifying the actual value with expected value
            assert actual_Value == decode_Value[0], "Test failed"

def test_EncodeBase58CheckedAPI(test_data_dir):

    #Reading test data from the file
    with open(test_data_dir/"testData_Encode58CheckedDecode", "r") as encode58File_txt: # Test data were generated using our python interface to encode the input value and decode the output for same input value

        for x in encode58File_txt.readlines():

            encode58_Value = x.split(",")
            # Encode given string in base58
            actual_Value = PyMessageHash.EncodeBase58Checked(encode58_Value[0])

            #Verifying the actual value with expected value
            assert actual_Value == encode58_Value[1].rstrip("\n"), "Test failed"

def test_DecodeBase58Checked58API(test_data_dir):

    #Reading test data from the file
    with open(test_data_dir/"testData_Encode58CheckedDecode", "r") as decode58File_txt:# Test data were generated using our python interface to encode the input value and decode the output for same input value

        for x in decode58File_txt.readlines():

            decode58_Value = x.split(",")
            # Decode given string in base58
            actual_Value = PyMessageHash.DecodeBase58Checked(decode58_Value[1].rstrip("\n"))

            #Verifying the actual value with expected value
            assert actual_Value == decode58_Value[0], "Test failed"

def test_EncodeBase64API(test_data_dir):

    #Reading test data from the file
    with open(test_data_dir/"testData_Encode64Decode", "r") as encodeFile_txt:

        for x in encodeFile_txt.readlines():

            encode_Value = x.split(",")
            #Encode given string in base64
            actual_Value = PyMessageHash.EncodeBase64(encode_Value[0])

            # Verifying the actual value with expected value
            assert actual_Value == encode_Value[1].rstrip("\n"), "Test failed"

def test_DecodeBase64API(test_data_dir):

    #Reading test data from the file
    with open(test_data_dir/"testData_Encode64Decode", "r") as decodeFile_txt:

        for x in decodeFile_txt.readlines():

            decode_Value = x.split(",")
            # Decode given string in base64
            actual_Value = PyMessageHash.DecodeBase64(decode_Value[1].rstrip("\n"))

            #Verifying the actual value with expected value
            assert actual_Value == decode_Value[0], "Test failed"