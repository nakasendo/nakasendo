import PyBigNumbers
import pytest
import pathlib

def test_AddFromDecWithBigNumApi(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir/"testData_AddDec", "r") as addDec_txt:
        for x in addDec_txt:

            decNumber = x.split()
            # Add too big numbers of arbitrary precision in dec
            actual_Value = PyBigNumbers.addFromDec(decNumber[0], decNumber[1])

            #Verifying the actual value with expected value
            assert actual_Value == decNumber[2], "Test failed"

def test_AddFromHexWithBigNumApi(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir/"testData_AddHex", "r") as addHex_txt:
        for x in addHex_txt:

            hexNumber = x.split()
            # Add too big numbers of arbitrary precision in Hex
            actual_Value = PyBigNumbers.addFromHex(hexNumber[0], hexNumber[1])

            #Verifying the actual value with expected value
            assert actual_Value.upper() == hexNumber[2].upper(), "Test failed"

def test_SubFromDecWithBigNumApi(test_data_dir):
    # Reading test data from the file
    with open(test_data_dir/"testData_SubDec", "r") as subDec_txt:
        for x in subDec_txt:

            decNumber = x.split()
            # Subtract too big numbers of arbitrary precision in dec
            actual_Value = PyBigNumbers.subFromDec(decNumber[0], decNumber[1])

            #Verifying the actual value with expected value
            assert actual_Value == decNumber[2], "Test failed"

def test_SubFromHexWithBigNumApi(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir/"testData_SubHex", "r") as subHex_txt:
        for x in subHex_txt:

            hexNumber = x.split()
            # Subtract too big numbers of arbitrary precision in hex
            actual_Value = PyBigNumbers.subFromHex(hexNumber[0], hexNumber[1])

            #Verifying the actual value with expected value
            assert actual_Value.upper() == hexNumber[2].upper(), "Test failed"

def test_GenRandDec(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir/"testData_GenBigNum", "r") as genDec_txt:

        for x in genDec_txt.readlines():

            decNumber = int(x)
            # Generate Random Number of arbitrary precision in dec
            actual_Value = PyBigNumbers.GenerateRandDec(decNumber)

            #Verifying the actual value as a string and not negative value
            assert type(actual_Value) is str and actual_Value != "-1", "Test failed"

def test_GenRandHex(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir/"testData_GenBigNum", "r") as genHex_txt:

        for x in genHex_txt.readlines():

            decNumber = int(x)
            # Generate Random Number of arbitrary precision in dec
            actual_Value = PyBigNumbers.GenerateRandHex(decNumber)

            #Verifying the actual value as a string and not negative value
            assert type(actual_Value) is str and actual_Value != "-1", "Test failed"

def test_genRandDecWithSeed(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir/"testData_SeedDec", "r") as seedDec_txt:
        for x in seedDec_txt:

            decNumber = x.split()
            # Generate Random Number of arbitrary precision in Dec with seed (specified as a string)
            actual_Value = PyBigNumbers.GenerateRandDecWithSeed(decNumber[0], int(decNumber[1]))

            #Verifying the actual value as a string with no negative sign
            assert type(actual_Value) is str and actual_Value != "-1", "Test failed"

def test_genRandHexWithSeed(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir/"testData_SeedDec", "r") as seedHex_txt:
        for x in seedHex_txt:

            decNumber = x.split()
            # Generate Random Number of arbitrary precision in hex with seed (specified as a string)
            actual_Value = PyBigNumbers.GenerateRandHexWithSeed(decNumber[0], int(decNumber[1]))

            #Verifying the actual value as a string with no negative sign
            assert type(actual_Value) is str and actual_Value != "-1", "Test failed"

def test_IsPrimeDec(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir/"testData_PrimeDec", "r") as primeDec_txt:
        for x in primeDec_txt:

            decNumber = x.split(",")
            # Check if Dec big number is a prime
            actual_Value = PyBigNumbers.isPrimeDec(decNumber[0].rstrip("\n"))

            # Verifying the actual value with expected value
            assert actual_Value == int(decNumber[1]), "Test failed"

def test_IsPrimeHex(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir/"testData_PrimeDec", "r") as primeHex_txt:
        for x in primeHex_txt:

            decNumber = x.split(",")
            #converting decimal to hex-decimal
            j = int(decNumber[0])
            hex_Value = hex(j).lstrip("0x")

            # Check if hex big number is a prime
            actual_Value = PyBigNumbers.isPrimeHex(str(hex_Value))

            # Verifying the actual value with expected value
            assert actual_Value == int(decNumber[1]), "Test failed"

def test_IsPrimeFastDec(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir/"testData_PrimeDec", "r") as primeFastDec_txt:
        for x in primeFastDec_txt:

            decNumber = x.split(",")
            # Check if dec big number is a prime (Fasttest)
            actual_Value = PyBigNumbers.isPrimeFasttestDec(decNumber[0])

            # Verifying the actual value with expected value
            assert actual_Value == int(decNumber[1]), "Test failed"

def test_IsPrimeFastHex(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir/"testData_PrimeDec", "r") as primeFastHex_txt:
        for x in primeFastHex_txt:

            decNumber = x.split(",")
            #converting decimal to hex-decimal
            j = int(decNumber[0])
            hex_Value = hex(j).lstrip("0x")

            # Check if hex big number is a prime (Fasttest)
            actual_Value = PyBigNumbers.isPrimeFasttestHex(str(hex_Value))

            # Verifying the actual value with expected value
            assert actual_Value == int(decNumber[1]), "Test failed"

def test_MultiplyDec(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir/"testData_MultiplyDec", "r") as multiplyDec_txt:
        for x in multiplyDec_txt:

            decNumber = x.split(",")
            # multiply two big numbers of arbitrary precision in dec
            actual_Value = PyBigNumbers.multiplyFromDec(decNumber[0],decNumber[1])

            # Verifying the actual value with expected value
            assert actual_Value == decNumber[2].rstrip("\n"), "Test failed"

def test_MultiplyHex(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir/"testData_MultiplyDec", "r") as multiplyHex_txt:
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
            assert actual_Value == expected_Value.upper(), "Test failed"

def test_LeftShiftDec(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir/"testData_LeftRightShiftDec", "r") as leftDec_txt:
        for x in leftDec_txt:

            decNumber = x.split(",")
            # leftshit bitwise operation that moves bits of right big number to the left by left big number value in dec
            actual_Value = PyBigNumbers.leftShiftFromDec(decNumber[0],decNumber[1])

            # Verifying the actual value with expected value
            assert actual_Value == decNumber[2].rstrip("\n"), "Test failed"

def test_RightShiftDec(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir/"testData_LeftRightShiftDec", "r") as rightDec_txt:
        for x in rightDec_txt:

            decNumber = x.split(",")
            # rightshift bitwise operation that moves bits of right big number to the right by left big number value in dec
            actual_Value = PyBigNumbers.rightShiftFromDec(decNumber[2].rstrip("\n"),decNumber[1])

            # Verifying the actual value with expected value
            assert actual_Value == decNumber[0], "Test failed"

def test_LeftShiftHex(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir/"testData_LeftRightShiftHex", "r") as leftHex_txt:
        for x in leftHex_txt:

            decNumber = x.split(",")
            # leftshit bitwise operation that moves bits of right big number to the left by left big number value in hex
            actual_Value = PyBigNumbers.leftShiftFromHex(decNumber[0],decNumber[1])

            # Verifying the actual value with expected value
            assert actual_Value.lstrip("0") == decNumber[2].rstrip("\n"), "Test failed"

def test_RightShiftHex(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir/"testData_LeftRightShiftHex", "r") as rightHex_txt:
        for x in rightHex_txt:

            decNumber = x.split(",")
            # rightshift bitwise operation that moves bits of right big number to the right by left big number value in hex
            actual_Value = PyBigNumbers.rightShiftFromHex(decNumber[2].rstrip("\n"),decNumber[1])

            # Verifying the actual value with expected value
            assert actual_Value.lstrip("0") == decNumber[0].upper(), "Test failed"

def test_GenerateRandPrimeDec():

    #Generating prime decimal numbers with input parameter
    for x in range (10, 1000, 10):

        # Generate Random Prime Number of arbitary precision in dec
        primeDec_Value = PyBigNumbers.GenerateRandPrimeDec(x)

        # Verifying the actual value as prime number or not
        assert PyBigNumbers.isPrimeDec(primeDec_Value), "Test failed"

def test_GenerateRandPrimeHex():

    #Generating prime decimal numbers with input parameter
    for x in range (10, 100, 10):

        # Generate Random Prime Number of arbitary precision in hex
        primeHex_Value = PyBigNumbers.GenerateRandPrimeHex(x)

        # Verifying the actual value as prime hex number or not
        assert PyBigNumbers.isPrimeHex(primeHex_Value), "Test failed"

def test_GenerateRandPrimeDecWithSeed():

    seed = "moKDVMpSuLxh3tS2baDvmM4XmfTugpctBX"

    #Generating prime decimal numbers with input parameter
    for x in range (10, 100, 10):

        # Generate Random Prime Number of arbitary precision in dec with seed (specified as a string)
        primeDec_Value = PyBigNumbers.GenerateRandPrimeDecWithSeed(seed, x)

        # Verifying the actual value as prime number or not
        assert PyBigNumbers.isPrimeDec(primeDec_Value), "Test failed"

def test_GenerateRandPrimeHexWithSeed():

    seed = "mhxdmxVS4qJWMyB7jsH9qfpS4qm1KHfY42"

    #Generating prime decimal numbers with input parameter
    for x in range (10, 100, 10):

        # Generate Random Prime Number of arbitary precision in hex with seed (specified as a string)
        primeHex_Value = PyBigNumbers.GenerateRandPrimeHexWithSeed(seed, x)

        # Verifying the actual value as prime hex number or not
        assert PyBigNumbers.isPrimeHex(primeHex_Value), "Test failed"

def test_DivideDec(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir/"testData_DivideDec", "r") as divideDec_txt:
        for x in divideDec_txt:

            decNumber = x.split(",")
            # Divide two big numbers of arbitrary precision in dec
            actual_Value = PyBigNumbers.divideFromDec(decNumber[0],decNumber[1])

            # Verifying the actual value with expected value
            assert actual_Value == decNumber[2].rstrip("\n"), "Test failed"

def test_DivideHex(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir/"testData_DivideHex", "r") as divideHex_txt:
        for x in divideHex_txt:

            hexNumber = x.split(",")

            # Divide two big numbers of arbitrary precision in hex
            actual_value = PyBigNumbers.divideFromHex(hexNumber[0], hexNumber[1])
            expected_value = hexNumber[2].rstrip("\n")

            # Verifying the actual value with expected value
            assert actual_value == expected_value.upper(), "Test failed"

def test_ModuloDec(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir/"testData_ModuloDec", "r") as moduloDec_txt:
        for x in moduloDec_txt:

            decNumber = x.split(",")

            # Modulo of big numbers of arbitrary precision in dec
            actual_Value = PyBigNumbers.Mod_Dec(decNumber[0],decNumber[1])

            # Verifying the actual value with expected value
            assert actual_Value == decNumber[2].rstrip("\n"), "Test failed"

def test_ModuloHex(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir/"testData_ModuloHex", "r") as moduloHex_txt:
        for x in moduloHex_txt:

            decNumber = x.split(",")

            # Modulo of big numbers of arbitrary precision in hex
            actual_Value = PyBigNumbers.Mod_Hex(decNumber[0], decNumber[1])
            expected_Value = decNumber[2].rstrip("\n")
            # Verifying the actual value with expected value
            assert actual_Value == expected_Value.upper(), "Test failed"

def test_AddModDec(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir/"testData_AddModDec", "r") as addModDec_txt:
        for x in addModDec_txt:
            decNumber = x.split(",")

            # Add modulo of big numbers of arbitrary precision in dec
            actual_Value = PyBigNumbers.Add_mod_Dec(decNumber[0], decNumber[1],decNumber[2])

            # Verifying the actual value with expected value
            assert actual_Value == decNumber[3].rstrip("\n"), "Test failed"

def test_AddModHex(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir/"testData_AddModHex", "r") as addModHex_txt:
        for x in addModHex_txt:
            decNumber = x.split(",")

            # Add modulo of big numbers of arbitrary precision in hex
            actual_Value = PyBigNumbers.Add_mod_Hex(decNumber[0], decNumber[1],decNumber[2])
            expected_Value = decNumber[3].rstrip("\n")

            # Verifying the actual value with expected value
            assert actual_Value.lstrip("0") == expected_Value.upper(), "Test failed"

def test_SubModDec(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir/"testData_SubModDec", "r") as subModDec_txt:
        for x in subModDec_txt:
            decNumber = x.split(",")

            # Subtract modulo of big numbers of arbitrary precision in dec
            actual_Value = PyBigNumbers.Sub_mod_Dec(decNumber[0], decNumber[1],decNumber[2])

            # Verifying the actual value with expected value
            assert actual_Value == decNumber[3].rstrip("\n"), "Test failed"

def test_SubModHex(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir/"testData_SubModHex", "r") as subModHex_txt:
        for x in subModHex_txt:
            decNumber = x.split(",")

            # Subtract modulo of big numbers of arbitrary precision in hex
            actual_Value = PyBigNumbers.Sub_mod_Hex(decNumber[0], decNumber[1],decNumber[2])
            expected_Value = decNumber[3].rstrip("\n")

            # Verifying the actual value with expected value
            assert actual_Value.lstrip("0") == expected_Value.upper(), "Test failed"

def test_MulModDec(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir/"testData_MulModDec", "r") as mulModDec_txt:
        for x in mulModDec_txt:
            decNumber = x.split(",")

            # Multiply modulo of big numbers of arbitrary precision in dec
            actual_Value = PyBigNumbers.Mul_mod_Dec(decNumber[0], decNumber[1],decNumber[2])

            # Verifying the actual value with expected value
            assert actual_Value == decNumber[3].rstrip("\n"), "Test failed"

def test_MulModHex(test_data_dir):

    # Reading test data from the file
    with open(test_data_dir/"testData_MulModHex", "r") as mulModHex_txt:
        for x in mulModHex_txt:
            hexNumber = x.split(",")

            # Multiply modulo of big numbers of arbitrary precision in hex
            actual_Value = PyBigNumbers.Mul_mod_Hex(hexNumber[0], hexNumber[1],hexNumber[2])
            expected_Value = hexNumber[3].rstrip("\n")

            # Verifying the actual value with expected value
            assert actual_Value.lstrip("0") == expected_Value.upper(), "Test failed"

def test_DivModDec(test_data_dir):

    #Reading test data from the file
    with open(test_data_dir/"testData_DivModDec", "r") as divModDec_txt:

        for x in divModDec_txt:

            decNumber = x.split(",")

            #Divide modulo of big numbers of arbitrary precision in dec
            actual_value = PyBigNumbers.Div_mod_Dec(decNumber[0], decNumber[1], decNumber[2])

            #verifying the actual value with the expected value
            assert actual_value == decNumber[3].rstrip("\n"), "Test failed"

def test_DivModHex(test_data_dir):

    #Reading test data from the file
    with open(test_data_dir/"testData_DivModHex", "r") as divModHex_txt:

        for x in divModHex_txt:

            hexNumber = x.split(",")

            #Divide modulo of big numbers of arbitrary precision in hex
            actual_value = PyBigNumbers.Div_mod_Hex(hexNumber[0], hexNumber[1], hexNumber[2])
            expected_value = hexNumber[3].rstrip("\n")
            #verifying the actual value with the expected value
            assert actual_value.lstrip("0") == expected_value.upper(), "Test failed"

def test_InvModDec(test_data_dir):

    #Reading test data from the file
    with open(test_data_dir/"testData_InvModDec", "r") as invModDec_txt:

        for x in invModDec_txt:

            decNumber = x.split(",")

            #Inverse modulo of big numbers of arbitrary precision in dec
            actual_value = PyBigNumbers.Inv_mod_Dec(decNumber[0], decNumber[1])

            #verifying the actual value with the expected value
            assert actual_value == decNumber[2].rstrip("\n"), "Test failed"

def test_InvModHex(test_data_dir):

    #Reading test data from the file
    with open(test_data_dir/"testData_InvModHex", "r") as invModHex_txt:

        for x in invModHex_txt:

            hexNumber = x.split(",")

            # Inverse modulo of big numbers of arbitrary precision in hex
            actual_value = PyBigNumbers.Inv_mod_Hex(hexNumber[0], hexNumber[1])
            expected_value = hexNumber[2].rstrip("\n")

            #verifying the actual value with the expected value
            assert actual_value.lstrip("0") == expected_value.upper(), "Test failed"