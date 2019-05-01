import pytest
import bigNumlib

def test_AddFromDecWithBigNumApi():

    # Reading test data from the file
    with open("testData_AddDec", "r") as addDec_txt:
        for x in addDec_txt:

            decNumber = x.split()
            actual_Value = bigNumlib.addFromDecWithBigNumApi(decNumber[0], decNumber[1])

            #Verifying the actual value with expected value
            assert actual_Value == decNumber[2], "Test failed"
            '''
            if (actual_Value == decNumber[2]):
                #print(actual_value)
                print("Pass")

            else:
                print(actual_Value)
                print("False")
            '''
    addDec_txt.close()

def test_AddFromHexWithBigNumApi():

    # Reading test data from the file
    with open("testData_AddHex", "r") as addHex_txt:
        for x in addHex_txt:

            hexNumber = x.split()
            actual_Value = bigNumlib.addFromHexWithBigNumApi(hexNumber[0], hexNumber[1])
            #Verifying the actual value with expected value
            assert actual_Value.upper() == hexNumber[2].upper(), "Test failed"
            '''
            if (actual_Value.upper() == hexNumber[2].upper()):
                # print(actual_value)
                print("Pass")

            else:
                print(actual_Value)
                print("False")
            '''
    addHex_txt.close()

def test_SubFromDecWithBigNumApi():

    # Reading test data from the file
    with open("testData_SubDec", "r") as subDec_txt:
        for x in subDec_txt:

            decNumber = x.split()
            actual_Value = bigNumlib.subFromDecWithBigNumApi(decNumber[0], decNumber[1])

            #Verifying the actual value with expected value
            assert actual_Value == decNumber[2], "Test failed"
            '''
            if (actual_Value == decNumber[2]):
                # print(actual_value)
                print("Pass")

            else:
                print(actual_Value)
                print("False")
            '''
    subDec_txt.close()

def test_SubFromHexWithBigNumApi():

    # Reading test data from the file
    with open("testData_SubHex", "r") as subHex_txt:
        for x in subHex_txt:

            hexNumber = x.split()
            actual_Value = bigNumlib.subFromHexWithBigNumApi(hexNumber[0], hexNumber[1])
            #Verifying the actual value with expected value
            assert actual_Value.upper() == hexNumber[2].upper(), "Test failed"
            '''
            if (actual_Value == hexNumber[2]):
                # print(actual_value)
                print("Pass")

            else:
                print(actual_Value)
                print("False")
            '''
    subHex_txt.close()

def test_GenRandDec():
    # Reading test data from the file
    with open("testData_GenBigNum", "r") as genDec_txt:

        for x in genDec_txt.readlines():

            decNumber = int(x)
            actual_Value = bigNumlib.genRandDec(decNumber)

            #Verifying the actual value with expected value
            assert type(actual_Value) is str and actual_Value != "-1", "Test failed"
            '''
            if (type(actual_Value) is str and actual_Value != "-1"):

                #print(actual_Value)
                print("Pass")

            else:
                print(actual_Value)
                print("False")
            '''
    genDec_txt.close()

def test_GenRandHex():
    # Reading test data from the file
    with open("testData_GenBigNum", "r") as genHex_txt:

        for x in genHex_txt.readlines():

            decNumber = int(x)
            actual_Value = bigNumlib.genRandHex(decNumber)

            #Verifying the actual value with expected value
            assert type(actual_Value) is str and actual_Value != "-1", "Test failed"
            '''
            if (type(actual_Value) is str and actual_Value != "-1"):
                # print(actual_value)
                print("Pass")

            else:
                print(actual_Value)
                print("False")
            '''
    genHex_txt.close()

def test_genRandDecWithSeed():

    # Reading test data from the file
    with open("testData_SeedDec", "r") as seedDec_txt:
        for x in seedDec_txt:

            decNumber = x.split()
            actual_Value = bigNumlib.genRandDecWithSeed(decNumber[0], int(decNumber[1]))

            #Verifying the actual value with expected value
            assert type(actual_Value) is str and actual_Value != "-1", "Test failed"
            '''
            if (type(actual_Value) is str and actual_Value != "-1"):
                # print(actual_value)
                print("Pass")

            else:
                print(actual_Value)
                print("False")
            '''
    seedDec_txt.close()

def test_genRandHexWithSeed():
    # Reading test data from the file
    with open("testData_SeedDec", "r") as seedHex_txt:
        for x in seedHex_txt:

            decNumber = x.split()
            actual_Value = bigNumlib.genRandHexWithSeed(decNumber[0], int(decNumber[1]))

            #Verifying the actual value with expected value
            assert type(actual_Value) is str and actual_Value != "-1", "Test failed"
            '''
            if (type(actual_Value) is str and actual_Value != "-1"):
                # print(actual_value)
                print("Pass")

            else:
                print(actual_Value)
                print("False")
            '''

    seedHex_txt.close()


test_AddFromDecWithBigNumApi()

test_AddFromHexWithBigNumApi()

test_SubFromDecWithBigNumApi()

test_SubFromHexWithBigNumApi()

test_GenRandDec()

test_GenRandHex()

test_genRandDecWithSeed()

test_genRandHexWithSeed()