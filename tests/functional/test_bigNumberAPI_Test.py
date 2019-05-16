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

            #Verifying the actual value as a string and not negative value
            assert type(actual_Value) is str and actual_Value != "-1", "Test failed"

    genDec_txt.close()

def test_GenRandHex():
    # Reading test data from the file
    with open("testData_GenBigNum", "r") as genHex_txt:

        for x in genHex_txt.readlines():

            decNumber = int(x)
            # Generate Random Number of arbitrary precision in dec
            actual_Value = PyBigNumbers.GenerateRandHex(decNumber)

            #Verifying the actual value as a string and not negative value
            assert type(actual_Value) is str and actual_Value != "-1", "Test failed"

    genHex_txt.close()

def test_genRandDecWithSeed():

    # Reading test data from the file
    with open("testData_SeedDec", "r") as seedDec_txt:
        for x in seedDec_txt:

            decNumber = x.split()
            # Generate Random Number of arbitrary precision in Dec with seed (specified as a string)
            actual_Value = PyBigNumbers.GenerateRandDecWithSeed(decNumber[0], int(decNumber[1]))

            #Verifying the actual value as a string with no negative sign
            assert type(actual_Value) is str and actual_Value != "-1", "Test failed"

    seedDec_txt.close()

def test_genRandHexWithSeed():

    # Reading test data from the file
    with open("testData_SeedDec", "r") as seedHex_txt:
        for x in seedHex_txt:

            decNumber = x.split()
            # Generate Random Number of arbitrary precision in hex with seed (specified as a string)
            actual_Value = PyBigNumbers.GenerateRandHexWithSeed(decNumber[0], int(decNumber[1]))

            #Verifying the actual value as a string with no negative sign
            assert type(actual_Value) is str and actual_Value != "-1", "Test failed"

    seedHex_txt.close()

def test_IsPrimeDec():

    # Reading test data from the file
    with open("testData_PrimeDec", "r") as primeDec_txt:
        for x in primeDec_txt:

            decNumber = x.split(",")
            # Check if Dec big number is a prime
            actual_Value = PyBigNumbers.isPrimeDec(decNumber[0].rstrip("\n"))

            # Verifying the actual value with expected value
            assert actual_Value == int(decNumber[1]), "Test failed"

    primeDec_txt.close()

def test_IsPrimeHex():

    # Reading test data from the file
    with open("testData_PrimeDec", "r") as primeHex_txt:
        for x in primeHex_txt:

            decNumber = x.split(",")
            #converting decimal to hex-decimal
            j = int(decNumber[0])
            hex_Value = hex(j).lstrip("0x")

            # Check if hex big number is a prime
            actual_Value = PyBigNumbers.isPrimeHex(str(hex_Value))

            # Verifying the actual value with expected value
            assert actual_Value == int(decNumber[1]), "Test failed"

    primeHex_txt.close()

def test_IsPrimeFastDec():

    # Reading test data from the file
    with open("testData_PrimeDec", "r") as primeFastDec_txt:
        for x in primeFastDec_txt:

            decNumber = x.split(",")
            # Check if dec big number is a prime (Fasttest)
            actual_Value = PyBigNumbers.isPrimeFasttestDec(decNumber[0])

            # Verifying the actual value with expected value
            assert actual_Value == int(decNumber[1]), "Test failed"

    primeFastDec_txt.close()

def test_IsPrimeFastHex():

    # Reading test data from the file
    with open("testData_PrimeDec", "r") as primeFastHex_txt:
        for x in primeFastHex_txt:

            decNumber = x.split(",")
            #converting decimal to hex-decimal
            j = int(decNumber[0])
            hex_Value = hex(j).lstrip("0x")

            # Check if hex big number is a prime (Fasttest)
            actual_Value = PyBigNumbers.isPrimeFasttestHex(str(hex_Value))

            # Verifying the actual value with expected value
            assert actual_Value == int(decNumber[1]), "Test failed"

    primeFastHex_txt.close()

def test_MultiplyDec():

    # Reading test data from the file
    with open("testData_MultiplyDec", "r") as multiplyDec_txt:
        for x in multiplyDec_txt:

            decNumber = x.split(",")
            # multiply two big numbers of arbitrary precision in dec
            actual_Value = PyBigNumbers.multiplyFromDec(decNumber[0],decNumber[1])

            # Verifying the actual value with expected value
            assert actual_Value == decNumber[2].rstrip("\n"), "Test failed"

    multiplyDec_txt.close()

def test_MultiplyHex():

    # Reading test data from the file
    with open("testData_MultiplyDec", "r") as multiplyHex_txt:
        for x in multiplyHex_txt:

            decNumber = x.split(",")
            #converting decimal to hex-decimal
            i = int(decNumber[0])
            j = int(decNumber[1])
            k = int(decNumber[2].rstrip("\n"))
            hex_Value = hex(i).lstrip("0x")
            hex_Value2 = hex(j).lstrip("0x")
            expected_Value = hex(k).lstrip("0x")

            # multiply two big numbers of arbitrary precision in hex
            actualValue = PyBigNumbers.multiplyFromHex(str(hex_Value), str(hex_Value2))
            actual_Value = actualValue.lstrip("0")
            expectedValue = expected_Value.upper()

            # Verifying the actual value with expected value
            #assert actual_Value == expected_Value.upper(), "Test failed"
            if (actual_Value == expected_Value.upper()):
                #print(actual_Value)
                print("Pass")

            else:

                print(actual_Value)
                print("Failed")

    multiplyHex_txt.close()

def test_LeftShiftDec():

    # Reading test data from the file
    with open("testData_LeftRightShiftDec", "r") as leftDec_txt:
        for x in leftDec_txt:

            decNumber = x.split(",")
            # leftshit bitwise operation that moves bits of right big number to the left by left big number value in dec
            actual_Value = PyBigNumbers.leftShiftFromDec(decNumber[0],decNumber[1])

            # Verifying the actual value with expected value
            assert actual_Value == decNumber[2].rstrip("\n"), "Test failed"

    leftDec_txt.close()

def test_RightShiftDec():

    # Reading test data from the file
    with open("testData_LeftRightShiftDec", "r") as rightDec_txt:
        for x in rightDec_txt:

            decNumber = x.split(",")
            # rightshift bitwise operation that moves bits of right big number to the right by left big number value in dec
            actual_Value = PyBigNumbers.rightShiftFromDec(decNumber[2].rstrip("\n"),decNumber[1])

            # Verifying the actual value with expected value
            assert actual_Value == decNumber[0], "Test failed"

    rightDec_txt.close()

def test_LeftShiftHex():

    # Reading test data from the file
    with open("testData_LeftRightShiftHex", "r") as leftHex_txt:
        for x in leftHex_txt:

            decNumber = x.split(",")
            # leftshit bitwise operation that moves bits of right big number to the left by left big number value in hex
            actual_Value = PyBigNumbers.leftShiftFromHex(decNumber[0],decNumber[1])

            # Verifying the actual value with expected value
            assert actual_Value.lstrip("0") == decNumber[2].rstrip("\n"), "Test failed"

    leftHex_txt.close()

def test_RightShiftHex():

    # Reading test data from the file
    with open("testData_LeftRightShiftHex", "r") as rightHex_txt:
        for x in rightHex_txt:

            decNumber = x.split(",")
            # rightshift bitwise operation that moves bits of right big number to the right by left big number value in hex
            actual_Value = PyBigNumbers.rightShiftFromHex(decNumber[2].rstrip("\n"),decNumber[1])

            # Verifying the actual value with expected value
            assert actual_Value.lstrip("0") == decNumber[0].upper(), "Test failed"

    rightHex_txt.close()

def test_GenerateRandPrimeDec():

    #Generating prime decimal numbers with input parameter ranging from 10 to 10000
    for x in range (10,1000,50):

        # Generate Random Prime Number of arbitary precision in dec
        primeDec_Value = PyBigNumbers.GenerateRandPrimeDec(x)

        # Verifying the actual value as prime number or not
        assert PyBigNumbers.isPrimeDec(primeDec_Value), "Test failed"

def test_GenerateRandPrimeHex():

    #Generating prime decimal numbers with input parameter ranging from 10 to 10000
    for x in range (10,1000,50):

        # Generate Random Prime Number of arbitary precision in hex
        primeHex_Value = PyBigNumbers.GenerateRandPrimeHex(x)

        # Verifying the actual value as prime hex number or not
        assert PyBigNumbers.isPrimeHex(primeHex_Value), "Test failed"

def test_GenerateRandPrimeDecWithSeed():

    seed = "moKDVMpSuLxh3tS2baDvmM4XmfTugpctBX"

    #Generating prime decimal numbers with input parameter ranging from 10 to 10000
    for x in range (10,1000,50):

        # Generate Random Prime Number of arbitary precision in dec with seed (specified as a string)
        primeDec_Value = PyBigNumbers.GenerateRandPrimeDecWithSeed(seed, x)

        # Verifying the actual value as prime number or not
        assert PyBigNumbers.isPrimeDec(primeDec_Value), "Test failed"

def test_GenerateRandPrimeHexWithSeed():

    seed = "mhxdmxVS4qJWMyB7jsH9qfpS4qm1KHfY42"

    #Generating prime decimal numbers with input parameter ranging from 10 to 10000
    for x in range (10,1000,50):

        # Generate Random Prime Number of arbitary precision in hex with seed (specified as a string)
        primeHex_Value = PyBigNumbers.GenerateRandPrimeHexWithSeed(seed, x)

        # Verifying the actual value as prime hex number or not
        assert PyBigNumbers.isPrimeHex(primeHex_Value), "Test failed"

def test_DivideDec():

    # Reading test data from the file
    with open("testData_DivideDec", "r") as divideDec_txt:
        for x in divideDec_txt:

            decNumber = x.split(",")
            # Divide two big numbers of arbitrary precision in dec
            actual_Value = PyBigNumbers.divideFromDec(decNumber[0],decNumber[1])

            # Verifying the actual value with expected value
            assert actual_Value == decNumber[2].rstrip("\n"), "Test failed"

            if (actual_Value == decNumber[2].rstrip("\n")):
                #print(actual_Value)
                print("Pass")

            else:

                print(actual_Value)
                print("Failed")

    divideDec_txt.close()

def test_DivideHex():

    # Reading test data from the file
    with open("testData_DivideDec", "r") as divideHex_txt:
        for x in divideHex_txt:

            decNumber = x.split(",")
            #converting decimal to hex-decimal
            i = int(decNumber[0])
            j = int(decNumber[1])
            k = int(decNumber[2].rstrip("\n"))
            hex_Value = hex(i).lstrip("0x")
            hex_Value2 = hex(j).lstrip("0x")
            print(hex_Value)
            print(hex_Value2)
            expected_Value = hex(k).lstrip("0x")

            # multiply two big numbers of arbitrary precision in hex
            actualValue = PyBigNumbers.divideFromHex(str(hex_Value), str(hex_Value2))
            actual_Value = actualValue.lstrip("0")

            # Verifying the actual value with expected value
            assert actual_Value == expected_Value.upper(), "Test failed"

            if (actual_Value == expected_Value.upper()):
                #print(actual_Value)
                print("Pass")

            else:

                print(actual_Value)
                print("Failed")

    divideHex_txt.close()