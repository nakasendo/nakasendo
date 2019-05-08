import PyBigNumbers

def test_AddFromDecWithBigNumApi():

    # Reading test data from the file
    with open("testData_AddDec", "r") as addDec_txt:
        for x in addDec_txt:

            decNumber = x.split()
            # Add too big numbers of arbitrary precision in dec
            actual_Value = PyBigNumbers.addFromDec(decNumber[0], decNumber[1])

            #Verifying the actual value with expected value
            assert actual_Value == decNumber[2], "Test failed"

    addDec_txt.close()

def test_AddFromHexWithBigNumApi():

    # Reading test data from the file
    with open("testData_AddHex", "r") as addHex_txt:
        for x in addHex_txt:

            hexNumber = x.split()
            # Add too big numbers of arbitrary precision in Hex
            actual_Value = PyBigNumbers.addFromHex(hexNumber[0], hexNumber[1])

            #Verifying the actual value with expected value
            assert actual_Value.upper() == hexNumber[2].upper(), "Test failed"

    addHex_txt.close()

def test_SubFromDecWithBigNumApi():

    # Reading test data from the file
    with open("testData_SubDec", "r") as subDec_txt:
        for x in subDec_txt:

            decNumber = x.split()
            # Subtract too big numbers of arbitrary precision in dec
            actual_Value = PyBigNumbers.subFromDec(decNumber[0], decNumber[1])

            #Verifying the actual value with expected value
            assert actual_Value == decNumber[2], "Test failed"

    subDec_txt.close()

def test_SubFromHexWithBigNumApi():

    # Reading test data from the file
    with open("testData_SubHex", "r") as subHex_txt:
        for x in subHex_txt:

            hexNumber = x.split()
            # Subtract too big numbers of arbitrary precision in hex
            actual_Value = PyBigNumbers.subFromHex(hexNumber[0], hexNumber[1])

            #Verifying the actual value with expected value
            assert actual_Value.upper() == hexNumber[2].upper(), "Test failed"

    subHex_txt.close()

def test_GenRandDec():
    # Reading test data from the file
    with open("testData_GenBigNum", "r") as genDec_txt:

        for x in genDec_txt.readlines():

            decNumber = int(x)
            # Generate Random Number of arbitrary precision in dec
            actual_Value = PyBigNumbers.GenerateRandDec(decNumber)

            #Verifying the actual value with expected value
            assert type(actual_Value) is str and actual_Value != "-1", "Test failed"

    genDec_txt.close()

def test_GenRandHex():
    # Reading test data from the file
    with open("testData_GenBigNum", "r") as genHex_txt:

        for x in genHex_txt.readlines():

            decNumber = int(x)
            # Generate Random Number of arbitrary precision in dec
            actual_Value = PyBigNumbers.GenerateRandHex(decNumber)

            #Verifying the actual value with expected value
            assert type(actual_Value) is str and actual_Value != "-1", "Test failed"

    genHex_txt.close()

def test_genRandDecWithSeed():

    # Reading test data from the file
    with open("testData_SeedDec", "r") as seedDec_txt:
        for x in seedDec_txt:

            decNumber = x.split()
            # Generate Random Number of arbitrary precision in Dec with seed (specified as a string)
            actual_Value = PyBigNumbers.GenerateRandDecWithSeed(decNumber[0], int(decNumber[1]))

            #Verifying the actual value with expected value
            assert type(actual_Value) is str and actual_Value != "-1", "Test failed"

    seedDec_txt.close()

def test_genRandHexWithSeed():
    # Reading test data from the file
    with open("testData_SeedDec", "r") as seedHex_txt:
        for x in seedHex_txt:

            decNumber = x.split()
            # Generate Random Number of arbitrary precision in Dec with seed (specified as a string)
            actual_Value = PyBigNumbers.GenerateRandHexWithSeed(decNumber[0], int(decNumber[1]))

            #Verifying the actual value with expected value
            assert type(actual_Value) is str and actual_Value != "-1", "Test failed"

    seedHex_txt.close()


test_AddFromDecWithBigNumApi()

test_AddFromHexWithBigNumApi()

test_SubFromDecWithBigNumApi()

test_SubFromHexWithBigNumApi()

test_GenRandDec()

test_GenRandHex()

test_genRandDecWithSeed()

test_genRandHexWithSeed()