import PyBigNumbers

def addFromDecWithBigNumApi(firstDec, secondDec):
    ##Add too big numbers of arbitrary precision in dec
    return PyBigNumbers.addFromDec(firstDec, secondDec)

def addFromHexWithBigNumApi(firstDec, secondDec):
    ##Add too big numbers of arbitrary precision in Hex
    return PyBigNumbers.addFromHex(firstDec, secondDec)

def subFromDecWithBigNumApi(firstDec, secondDec):
    ##Subtract too big numbers of arbitrary precision in dec
    return PyBigNumbers.subFromDec(firstDec, secondDec)

def subFromHexWithBigNumApi(firstDec, secondDec):
    ##Subtract too big numbers of arbitrary precision in hex
    return PyBigNumbers.subFromHex(firstDec, secondDec)

def genRandDec(dec_Number):
    #Generate Random Number of arbitrary precision in dec
    return PyBigNumbers.GenerateRandDec(dec_Number)

def genRandHex(dec_Number):
    #Generate Random Number of arbitrary precision in dec
    return PyBigNumbers.GenerateRandHex(dec_Number)

def genRandDecWithSeed(seed, dec_Num):
    #Generate Random Number of arbitrary precision in Dec with seed (specified as a string)
     return  PyBigNumbers.GenerateRandDecWithSeed(seed, dec_Num)

def genRandHexWithSeed(seed, dec_Num):
    #Generate Random Number of arbitrary precision in Dec with seed (specified as a string)
    return PyBigNumbers.GenerateRandHexWithSeed(seed, dec_Num)
