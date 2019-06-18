import PyECPoint
import pathlib
import PyBigNumbers
import ast

# using the integer value of NID secp256k1, which is 714
nid_Id = 714

def test_GenerateRandomECHex():

    # Generating Random EC Points ranging from 10 to 10000 in step 10
    for x in range(10, 1000, 10):

        # Generate a Random EC Point with default NID ==> NID_secp256k1
        actual_value = PyECPoint.GenerateRandomECHex()

        # Verifying the the length of actual value as 66
        assert len(actual_value) == 66, "Test failed"

def test_GenerateECHexFromHex():

    # Generating Random EC Points ranging from 10 to 10000 in step 10
    for x in range(10, 1000, 10):

        # Generate a Random EC Point with default NID ==> NID_secp256k1
        value_ECHex = PyECPoint.GenerateRandomECHex()

        #Generate a EC Point from hex string with default NID ==> NID_secp256k1
        actual_value = PyECPoint.GenerateECHexFromHex(value_ECHex)

        # Verifying the the length of actual value as 66
        assert len(actual_value) == 66 and actual_value == value_ECHex, "Test failed"

def test_GenerateRandomECHexOnCurve():

    # Generating Random EC Points ranging from 10 to 10000 in step 10
    for x in range(10, 1000, 10):

        #Generate a Random EC Point with supplied NID
        actual_value = PyECPoint.GenerateRandomECHexOnCurve(nid_Id)

        # Verifying the the length of actual value as 66
        assert len(actual_value) == 66, "Test failed"

def test_GenerateECHexFromHexOnCurve():

    # Generating Random EC Points ranging from 10 to 10000 in step 10
    for x in range(10, 1000, 10):

        # Generate a Random EC Point with default NID ==> NID_secp256k1
        value_ECHex = PyECPoint.GenerateRandomECHex()

        #Generate a EC Point from hex string with supplied NID
        actual_value = PyECPoint.GenerateECHexFromHexOnCurve(value_ECHex, nid_Id)

        # Verifying the the length of actual value as 66
        assert len(actual_value) == 66, "Test failed"

def test_CheckOnCurve():

    # Generating Random EC Points ranging from 10 to 10000 in step 10
    for x in range(10, 1000, 10):
        # Generate a Random EC Point with default NID ==> NID_secp256k1
        value_ECHex = PyECPoint.GenerateRandomECHex()

        # Generate a EC Point from hex string with supplied NID
        hexOnCurve_value = PyECPoint.GenerateECHexFromHexOnCurve(value_ECHex, nid_Id)
        #Check if the point is on the curve with the supplied NID default NID ==> NID_secp256k1
        actual_value = PyECPoint.CheckOnCurve(hexOnCurve_value)

        # Verifying the actual value as True or False
        assert actual_value, "Test failed"

def test_GetAffineCoOrdinates():

    # Generating Random EC Points ranging from 10 to 10000 in step 10
    for x in range(10, 1000, 10):

        # Generate a Random EC Point with default NID ==> NID_secp256k1
        hexValue = PyECPoint.GenerateRandomECHex()

        # Check if the point is on the curve with the supplied NID default NID ==> NID_secp256k1
        assert PyECPoint.CheckOnCurve(hexValue), "Test failed"

        # EC Point GetAffineCoOrdinates_GFp with default NID => NID_secp256k1
        x_axis, y_axis = PyECPoint.GetAffineCoOrdinates(hexValue)

        assert len(x_axis) == 62 or len(x_axis) == 64, "Test failed"

def test_GetAffineCoOrdinatesOnCurve():

    # Generating Random EC Points ranging from 10 to 10000 in step 10
    for x in range(10, 1000, 10):
        # Generate a Random EC Point with default NID ==> NID_secp256k1
        hexValue = PyECPoint.GenerateRandomECHex()

        # Check if the point is on the curve with the supplied NID default NID ==> NID_secp256k1
        assert PyECPoint.CheckOnCurve(hexValue), "Test failed"

        # EC Point GetAffineCoOrdinates_GFp with supplied curve
        x_axis, y_axis = PyECPoint.GetAffineCoOrdinatesOnCurve(hexValue, nid_Id)

        assert len(x_axis) == 62 or len(x_axis) == 64, "Test failed"

def test_GetCurveList():

    #Get all the list of curves
    listOfCurve = PyECPoint.GetCurveList()

    #converting string to dictionary
    actual_value = ast.literal_eval(listOfCurve)

    #Verifying one of the keys of the dictionary with it's value
    assert actual_value[nid_Id] == "secp256k1", "Test failed"

def test_AddECFromHex(test_data_dir):

    #Reading a Random generated EC Point with default NID ==> NID_secp256k1 from file
    with open(test_data_dir/"testData_AddECFromHex", "r") as addEChex_txt:
        for x in addEChex_txt:

            hexNumber = x.split(",")

            # Check if the point is on the curve with the supplied NID default NID ==> NID_secp256k1
            assert PyECPoint.CheckOnCurve(hexNumber[0]), "Test failed"

            # Add two ECPoints in hex with the default NID ==> NID_secp256k1
            actual_value = PyECPoint.addFromHex(hexNumber[0], hexNumber[1])

            # Verifying the actual value with expected value
            assert actual_value == hexNumber[2].rstrip("\n") and len(actual_value) == 66, "Test failed"

def test_AddECFromHexWithCurveID(test_data_dir):

    #Reading a Random generated EC Point with default NID ==> NID_secp256k1 from file
    with open(test_data_dir/"testData_AddECFromHex", "r") as addEChex_txt:
        for x in addEChex_txt:

            hexNumber = x.split(",")

            # Check if the point is on the curve with the supplied NID default NID ==> NID_secp256k1
            assert PyECPoint.CheckOnCurve(hexNumber[0]), "Test failed"

            # Add two ECPoints in hex with the supplied curve IDs
            actual_value = PyECPoint.addFromHexWithCurveID(hexNumber[0], hexNumber[1], nid_Id)

            # Verifying the actual value with expected value
            assert actual_value == hexNumber[2].rstrip("\n") and len(actual_value) == 66, "Test failed"

def test_MultiplyScalarM():

    # Generating Random EC Points ranging from 10 to 10000 in step 10
    for x in range(10, 10000, 10):

        # Generate a Random EC Point with default NID ==> NID_secp256k1
        x = PyECPoint.GenerateRandomECHex()
        y = PyECPoint.GenerateRandomECHex()

        # Check if the point is on the curve with the supplied NID default NID ==> NID_secp256k1
        assert PyECPoint.CheckOnCurve(x) and PyECPoint.CheckOnCurve(y), "Test failed"

        # EC Point Scalar multiply with default NID => NID_secp256k1
        actual_value = PyECPoint.MultiplyScalarM(x, y)

        # Verifying the actual value with expected value
        assert len(actual_value) == 66, "Test failed"

def test_MultiplyScalarMN():

    # Generating Random EC Points ranging from 10 to 10000 in step 10
    for x in range(10, 1000, 10):
        # Generate a Random EC Point with default NID ==> NID_secp256k1
        ecPoint_value = PyECPoint.GenerateRandomECHex()
        # Check if the point is on the curve with the supplied NID default NID ==> NID_secp256k1
        assert PyECPoint.CheckOnCurve(ecPoint_value), "Test failed"

        #Generate a Random Big number M and N using BigNumberAPIs
        bigNumbM = PyBigNumbers.GenerateRandDec(257)
        bigNumbN = PyBigNumbers.GenerateRandDec(128)

        # EC Point Scalar multiply with default NID => NID_secp256k1
        actual_value = PyECPoint.MultiplyScalarMN(ecPoint_value, bigNumbM, bigNumbN)

        # Verifying the the length of actual value as 66
        assert len(actual_value) == 66, "Test failed"

def test_MultiplyScalarMOnCurve():

    # Generating Random EC Points ranging from 10 to 10000 in step 10
    for x in range(10, 10000, 10):

        # Generate a Random EC Point with default NID ==> NID_secp256k1
        x = PyECPoint.GenerateRandomECHex()
        y = PyECPoint.GenerateRandomECHex()

        # Check if the point is on the curve with the supplied NID default NID ==> NID_secp256k1
        assert PyECPoint.CheckOnCurve(x) and PyECPoint.CheckOnCurve(y) , "Test failed"

        # EC Point Scalar multiply on curve with supplied ID
        actual_value = PyECPoint.MultiplyScalarMOnCurve(x, y, nid_Id)

        # Verifying the actual value with expected value
        assert len(actual_value) == 66, "Test failed"

def test_MultiplyScalarMNOnCurve():

    # Generating Random EC Points ranging from 10 to 10000 in step 10
    for x in range(10, 10000, 10):
        # Generate a Random EC Point with default NID ==> NID_secp256k1
        ecPoint_value = PyECPoint.GenerateRandomECHex()
        # Check if the point is on the curve with the supplied NID default NID ==> NID_secp256k1
        assert PyECPoint.CheckOnCurve(ecPoint_value), "Test failed"

        #Generate a Random Big number M and N using BigNumberAPIs
        bigNumbM = PyBigNumbers.GenerateRandDec(257)
        bigNumbN = PyBigNumbers.GenerateRandDec(128)

        # EC Point Scalar multiply with supplied curve ID
        actual_value = PyECPoint.MultiplyScalarMNOnCurve(ecPoint_value, bigNumbM, bigNumbN, nid_Id)

        # Verifying the the length of actual value as 66
        assert len(actual_value) == 66, "Test failed"

def test_CheckInfinityFromHex():

    # Generating Random EC Points ranging from 10 to 10000 in step 10
    for x in range(10, 10000, 10):
        # Generate a Random EC Point with default NID ==> NID_secp256k1
        ecPoint_value = PyECPoint.GenerateRandomECHex()
        # Check if the point is on the curve with the supplied NID default NID ==> NID_secp256k1
        assert PyECPoint.CheckOnCurve(ecPoint_value), "Test failed"

        # Check if the given point is at infinity with default NID ==> NID_secp256k1
        actual_value = PyECPoint.GenerateECHexFromHex(ecPoint_value)
        assert PyECPoint.checkInfinityFromHex(actual_value) is False, "Test failed"

def test_CheckInfinityFromHexCurve():

    # Generating Random EC Points ranging from 10 to 10000 in step 10
    for x in range(10, 10000, 10):
        # Generate a Random EC Point with default NID ==> NID_secp256k1
        ecPoint_value = PyECPoint.GenerateRandomECHex()
        # Check if the point is on the curve with the supplied NID default NID ==> NID_secp256k1
        assert PyECPoint.CheckOnCurve(ecPoint_value), "Test failed"

        # Check if the given point is at infinity on the given curve ID
        actual_value = PyECPoint.GenerateECHexFromHex(ecPoint_value)
        assert PyECPoint.checkInfinityFromHexCurve(actual_value,  nid_Id) is False, "Test failed"

def test_CheckOnCurveFromHexOnCurve():

    # Generating Random EC Points ranging from 10 to 10000 in step 10
    for x in range(10, 10000, 10):
        # Generate a Random EC Point with default NID ==> NID_secp256k1
        ecPoint_value = PyECPoint.GenerateRandomECHex()
        # Check if the point is on the curve with the supplied NID default NID ==> NID_secp256k1
        assert PyECPoint.CheckOnCurve(ecPoint_value), "Test failed"

        # Check if the given point is at infinity on the given curve ID
        actual_value = PyECPoint.GenerateECHexFromHex(ecPoint_value)
        assert PyECPoint.checkInfinityFromHexCurve(actual_value,  nid_Id) is False, "Test failed"

        #Check if the point is on the curve with supplied curve ID
        assert PyECPoint.checkOnCurveFromHexOnCurve(actual_value,  nid_Id) is True, "Test failed"

def test_CompareECPoint():

    # Generating Random EC Points ranging from 10 to 10000 in step 10
    for x in range(10, 10000, 10):

        # Generate a Random EC Point with default NID ==> NID_secp256k1
        ecPoint_value = PyECPoint.GenerateRandomECHex()
        ecPoint_value2 = PyECPoint.GenerateRandomECHex()
        # Check if the point is on the curve with the supplied NID default NID ==> NID_secp256k1
        assert PyECPoint.CheckOnCurve(ecPoint_value), "Test failed"
        assert PyECPoint.CheckOnCurve(ecPoint_value2), "Test failed"

        #Compare two given ECPoints
        assert PyECPoint.compare(ecPoint_value, ecPoint_value2) is False, "Test failed"

def test_CompareCurveECPoint():

    # Generating Random EC Points ranging from 10 to 10000 in step 10
    for x in range(10, 10000, 10):

        # Generate a Random EC Point with default NID ==> NID_secp256k1
        ecPoint_value = PyECPoint.GenerateRandomECHex()
        ecPoint_value2 = PyECPoint.GenerateRandomECHex()
        # Check if the point is on the curve with the supplied NID default NID ==> NID_secp256k1
        assert PyECPoint.CheckOnCurve(ecPoint_value), "Test failed"
        assert PyECPoint.CheckOnCurve(ecPoint_value2), "Test failed"

        #Compare two given ECPoints
        assert PyECPoint.compare(ecPoint_value, ecPoint_value2) is False, "Test failed"

        #Compare two given ECPoints with a Curve ID
        assert PyECPoint.compareCurve(ecPoint_value, ecPoint_value2, nid_Id) is False, "Test failed"

def test_DoubleFromHex():

    # Generating Random EC Points ranging from 10 to 10000 in step 10
    for x in range(10, 10000, 10):

        # Generate a Random EC Point with default NID ==> NID_secp256k1
        ecPoint_value = PyECPoint.GenerateRandomECHex()

        # Check if the point is on the curve with the supplied NID default NID ==> NID_secp256k1
        assert PyECPoint.CheckOnCurve(ecPoint_value), "Test failed"

        #Double the ECPoint in hex with the default NID ==> NID_secp256k1
        actual_value = PyECPoint.doubleFromHex(ecPoint_value)

        # Verifying the the length of actual value as 66
        assert len(actual_value) == 66, "Test failed"

def test_DoubleFromHexCurve():

    # Generating Random EC Points ranging from 10 to 10000 in step 10
    for x in range(10, 10000, 10):

        # Generate a Random EC Point with default NID ==> NID_secp256k1
        ecPoint_value = PyECPoint.GenerateRandomECHex()

        # Check if the point is on the curve with the supplied NID default NID ==> NID_secp256k1
        assert PyECPoint.CheckOnCurve(ecPoint_value), "Test failed"

        #Double the ECPoint in hex with the default NID ==> NID_secp256k1
        doubleECPoint_value = PyECPoint.doubleFromHex(ecPoint_value)

        #Double the ECPoint in hex with the given curve ID
        actual_value = PyECPoint.doubleFomHexCurve(doubleECPoint_value, nid_Id)

        # Verifying the the length of actual value as 66
        assert len(actual_value) == 66, "Test failed"

def test_InvertFromHex():

    # Generating Random EC Points ranging from 10 to 10000 in step 10
    for x in range(10, 10000, 10):

        # Generate a Random EC Point with default NID ==> NID_secp256k1
        ecPoint_value = PyECPoint.GenerateRandomECHex()

        # Check if the point is on the curve with the supplied NID default NID ==> NID_secp256k1
        assert PyECPoint.CheckOnCurve(ecPoint_value), "Test failed"

        #Invert the ECPoint in hex with the default NID ==> NID_secp256k1
        actual_value = PyECPoint.invertFromHex(ecPoint_value)

        # Verifying the the length of actual value as 66
        assert len(actual_value) == 66, "Test failed"

def test_InvertFromHexCurve():

    # Generating Random EC Points ranging from 10 to 10000 in step 10
    for x in range(10, 10000, 10):

        # Generate a Random EC Point with default NID ==> NID_secp256k1
        ecPoint_value = PyECPoint.GenerateRandomECHex()

        # Check if the point is on the curve with the supplied NID default NID ==> NID_secp256k1
        assert PyECPoint.CheckOnCurve(ecPoint_value), "Test failed"

        #Double the ECPoint in hex with the default NID ==> NID_secp256k1
        doubleECPoint_value = PyECPoint.invertFromHex(ecPoint_value)

        #Double the ECPoint in hex with the given curve ID
        actual_value = PyECPoint.invertFromHexCurve(doubleECPoint_value, nid_Id)

        # Verifying the the length of actual value as 66
        assert len(actual_value) == 66, "Test failed"

def test_GetGenerator():

    # Generating Random EC Points ranging from 10 to 10000 in step 10
    for x in range(10, 10000, 10):

        # Generate a Random EC Point with default NID ==> NID_secp256k1
        ecPoint_value = PyECPoint.GenerateRandomECHex()

        # Check if the point is on the curve with the supplied NID default NID ==> NID_secp256k1
        assert PyECPoint.CheckOnCurve(ecPoint_value), "Test failed"

        #EC Point Generator with the supplied curve ID
        actual_value = PyECPoint.GetGenerator(ecPoint_value, nid_Id)

        # Verifying the the length of actual value as 66
        assert len(actual_value) == 66, "Test failed"

def test_GetGroupDegreeFromHex(test_data_dir):

    # Generate a Random EC Point with default NID ==> NID_secp256k1
    ecPoint_value = PyECPoint.GenerateRandomECHex()

    # Check if the point is on the curve with the supplied NID default NID ==> NID_secp256k1
    assert PyECPoint.CheckOnCurve(ecPoint_value), "Test failed"

    # EC Point Generator with the supplied curve ID
    generator_Point = PyECPoint.GetGenerator(ecPoint_value, nid_Id)

    # Reading a Random generated EC Point with default NID ==> NID_secp256k1 from file
    with open(test_data_dir/"testData_GetGroupDegree", "r") as getGrpDegree_txt:#Test data are generated from https://svn.python.org/projects/external/openssl-0.9.8a/crypto/ec/ec_curve.c
        for x in getGrpDegree_txt:

            # Reading the line of the file as string and splitting into list
            nidID_Degree_Value = x.split(",")

            #EC Point Group Degree with supplied curve
            actual_value = PyECPoint.GetGroupDegreeFromHex(generator_Point, int(nidID_Degree_Value[0]))

            # Verifying the actual value with the expected value.
            assert actual_value == int(nidID_Degree_Value[1]), "Test failed"

def test_GetGroupOrderFromHex(test_data_dir):

    # Generate a Random EC Point with default NID ==> NID_secp256k1
    ecPoint_value = PyECPoint.GenerateRandomECHex()

    # Check if the point is on the curve with the supplied NID default NID ==> NID_secp256k1
    assert PyECPoint.CheckOnCurve(ecPoint_value), "Test failed"

    # EC Point Generator with the supplied curve ID
    generator_Point = PyECPoint.GetGenerator(ecPoint_value, nid_Id)

    # Reading a Random generated EC Point with default NID ==> NID_secp256k1 from file
    with open(test_data_dir/"testData_GetGroupDegree", "r") as getGrpDegree_txt: #Test data are generated from https://svn.python.org/projects/external/openssl-0.9.8a/crypto/ec/ec_curve.c
        for x in getGrpDegree_txt:

            # Reading the line of the file as string and splitting into list
            nidID_Degree_Value = x.split(",")

            #EC Point Group Degree with supplied curve
            grpDegreeHex = PyECPoint.GetGroupDegreeFromHex(generator_Point, int(nidID_Degree_Value[0]))

            # Verifying the actual value with the expected value.
            assert grpDegreeHex == int(nidID_Degree_Value[1]), "Test failed"

            #EC Point Group Order with supplied curve
            actual_value = PyECPoint.GetGroupOrderFromHex(generator_Point, int(nidID_Degree_Value[0]))
            assert actual_value == nidID_Degree_Value[2].rstrip("\n"), "Test failed"