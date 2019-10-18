#include <Python.h>
#include <string>
#include <iostream>
#include <BigNumbers/BigNumbers.h>


struct module_state {
    PyObject *error;
};

#if PY_MAJOR_VERSION >= 3
#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))
#else
#define GETSTATE(m) (&_state)
static struct module_state _state;
#endif

static PyObject* wrap_addFromHex(PyObject* self, PyObject *args)
{
    char * argA; 
    char * argB; 

    if (!PyArg_ParseTuple(args, "ss", &argA, &argB))
        return NULL;

    BigNumber bnA, bnB; 
    bnA.FromHex (argA);
    bnB.FromHex (argB);
    const BigNumber res = bnA + bnB;

    return Py_BuildValue("s",res.ToHex().c_str());
}

static PyObject* wrap_addFromDec(PyObject* self, PyObject *args)
{
    char * argA; 
    char * argB; 

    if (!PyArg_ParseTuple(args, "ss", &argA, &argB))
        return NULL;

    BigNumber bnA, bnB; 
    bnA.FromDec (argA);
    bnB.FromDec (argB);
    const BigNumber res = bnA + bnB;

    return Py_BuildValue("s",res.ToDec().c_str());
}

static PyObject* wrap_subFromHex(PyObject* self, PyObject *args)
{
    char * argA; 
    char * argB; 

    if (!PyArg_ParseTuple(args, "ss", &argA, &argB))
        return NULL;

    BigNumber bnA, bnB; 
    bnA.FromHex (argA);
    bnB.FromHex (argB);
    const BigNumber res = bnA - bnB;

    return Py_BuildValue("s",res.ToHex().c_str());
}

static PyObject* wrap_subFromDec(PyObject* self, PyObject *args)
{
    char * argA; 
    char * argB; 

    if (!PyArg_ParseTuple(args, "ss", &argA, &argB))
        return NULL;

    BigNumber bnA, bnB; 
    bnA.FromDec (argA);
    bnB.FromDec (argB);
    const BigNumber res = bnA - bnB;

    return Py_BuildValue("s",res.ToDec().c_str());
}

static PyObject* wrap_multiplyFromHex(PyObject* self, PyObject *args)
{
    char * argA; 
    char * argB; 

    if (!PyArg_ParseTuple(args, "ss", &argA, &argB))
        return NULL;

    BigNumber bnA, bnB; 
    bnA.FromHex (argA);
    bnB.FromHex (argB);
    const BigNumber res = bnA * bnB;

    return Py_BuildValue("s",res.ToHex().c_str());
}

static PyObject* wrap_multiplyFromDec(PyObject* self, PyObject *args)
{
    char * argA; 
    char * argB; 

    if (!PyArg_ParseTuple(args, "ss", &argA, &argB))
        return NULL;


    BigNumber bnA, bnB; 
    bnA.FromDec (argA);
    bnB.FromDec (argB);
    const BigNumber res = bnA * bnB;

    return Py_BuildValue("s",res.ToDec().c_str());
}

static PyObject* wrap_divideFromHex(PyObject* self, PyObject *args)
{
    char * argA; 
    char * argB; 

    if (!PyArg_ParseTuple(args, "ss", &argA, &argB))
        return NULL;

    BigNumber bnA, bnB; 
    bnA.FromHex (argA);
    bnB.FromHex (argB);

    try{
        const BigNumber res = bnA / bnB;
        return Py_BuildValue("s",res.ToHex().c_str());
    }
    catch(std::runtime_error & re){
        PyErr_SetString(PyExc_ZeroDivisionError, "division or modulo by zero");
        return NULL;
    }
}

static PyObject* wrap_divideFromDec(PyObject* self, PyObject *args)
{
    char * argA; 
    char * argB; 

    if (!PyArg_ParseTuple(args, "ss", &argA, &argB))
        return NULL;

    BigNumber bnA, bnB; 
    bnA.FromDec (argA);
    bnB.FromDec (argB);

    try{
        const BigNumber res = bnA / bnB;
        return Py_BuildValue("s",res.ToDec().c_str());
    }
    catch(std::runtime_error & re){
        PyErr_SetString(PyExc_ZeroDivisionError, "division or modulo by zero");
        return NULL;
    }
}

static PyObject* wrap_Mod_Hex(PyObject* self, PyObject *args)
{
    char* pARG;
    char* pMOD;

    if (!PyArg_ParseTuple(args, "ss", &pARG, &pMOD))
        return NULL;

    BigNumber bnArg, bnMod;
    bnArg.FromHex (pARG);
    bnMod.FromHex (pMOD);
    const BigNumber res = bnArg % bnMod;

    return Py_BuildValue("s",res.ToHex().c_str());
}

static PyObject* wrap_Inv_mod_Hex(PyObject* self, PyObject *args)
{
    char* pARG;
    char* pMOD;

    if (!PyArg_ParseTuple(args, "ss", &pARG, &pMOD))
        return NULL;

    BigNumber bnArg, bnMod;
    bnArg.FromHex (pARG);
    bnMod.FromHex (pMOD);
    const BigNumber res = Inv_mod( bnArg , bnMod);

    return Py_BuildValue("s",res.ToHex().c_str());
}

static PyObject* wrap_Add_mod_Hex(PyObject* self, PyObject *args)
{
    char* pLHS;
    char* pRHS;
    char* pMOD;

    if (!PyArg_ParseTuple(args, "sss", &pLHS, &pRHS, &pMOD))
        return NULL;

    BigNumber bnLHS, bnRHS, bnMOD;
    bnLHS.FromHex (pLHS);
    bnRHS.FromHex (pRHS);
    bnMOD.FromHex (pMOD);
    const BigNumber res = Add_mod(bnLHS, bnRHS, bnMOD);

    return Py_BuildValue("s",res.ToHex().c_str());
}

static PyObject* wrap_Sub_mod_Hex(PyObject* self, PyObject *args)
{
    char* pLHS;
    char* pRHS;
    char* pMOD;

    if (!PyArg_ParseTuple(args, "sss", &pLHS, &pRHS, &pMOD))
        return NULL;

    BigNumber bnLHS, bnRHS, bnMOD;
    bnLHS.FromHex (pLHS);
    bnRHS.FromHex (pRHS);
    bnMOD.FromHex (pMOD);
    const BigNumber res = Sub_mod(bnLHS, bnRHS, bnMOD);

    return Py_BuildValue("s",res.ToHex().c_str());
}

static PyObject* wrap_Mul_mod_Hex(PyObject* self, PyObject *args)
{
    char* pLHS;
    char* pRHS;
    char* pMOD;

    if (!PyArg_ParseTuple(args, "sss", &pLHS, &pRHS, &pMOD))
        return NULL;

    BigNumber bnLHS, bnRHS, bnMOD;
    bnLHS.FromHex (pLHS);
    bnRHS.FromHex (pRHS);
    bnMOD.FromHex (pMOD);
    const BigNumber res = Mul_mod(bnLHS, bnRHS, bnMOD);

    return Py_BuildValue("s",res.ToHex().c_str());
}

static PyObject* wrap_Div_mod_Hex(PyObject* self, PyObject *args)
{
    char* pLHS;
    char* pRHS;
    char* pMOD;

    if (!PyArg_ParseTuple(args, "sss", &pLHS, &pRHS, &pMOD))
        return NULL;

    BigNumber bnLHS, bnRHS, bnMOD;
    bnLHS.FromHex (pLHS);
    bnRHS.FromHex (pRHS);
    bnMOD.FromHex (pMOD);
    const BigNumber res = Div_mod(bnLHS, bnRHS, bnMOD);

    return Py_BuildValue("s",res.ToHex().c_str());
}

static PyObject* wrap_Mod_Dec(PyObject* self, PyObject *args)
{
    char* pARG;
    char* pMOD;

    if (!PyArg_ParseTuple(args, "ss", &pARG, &pMOD))
        return NULL;

    BigNumber bnArg, bnMod;
    bnArg.FromDec (pARG);
    bnMod.FromDec (pMOD);
    const BigNumber res = bnArg % bnMod;

    return Py_BuildValue("s",res.ToDec().c_str());
}

static PyObject* wrap_Inv_mod_Dec(PyObject* self, PyObject *args)
{
    char* pARG;
    char* pMOD;

    if (!PyArg_ParseTuple(args, "ss", &pARG, &pMOD))
        return NULL;

    BigNumber bnArg, bnMod;
    bnArg.FromDec (pARG);
    bnMod.FromDec (pMOD);
    const BigNumber res = Inv_mod( bnArg , bnMod);

    return Py_BuildValue("s",res.ToDec().c_str());
}

static PyObject* wrap_Add_mod_Dec(PyObject* self, PyObject *args)
{
    char* pLHS;
    char* pRHS;
    char* pMOD;

    if (!PyArg_ParseTuple(args, "sss", &pLHS, &pRHS, &pMOD))
        return NULL;

    BigNumber bnLHS, bnRHS, bnMOD;
    bnLHS.FromDec(pLHS);
    bnRHS.FromDec(pRHS);
    bnMOD.FromDec(pMOD);
    const BigNumber res = Add_mod(bnLHS, bnRHS, bnMOD);

    return Py_BuildValue("s",res.ToDec().c_str());
}

static PyObject* wrap_Sub_mod_Dec(PyObject* self, PyObject *args)
{
    char* pLHS;
    char* pRHS;
    char* pMOD;

    if (!PyArg_ParseTuple(args, "sss", &pLHS, &pRHS, &pMOD))
        return NULL;

    BigNumber bnLHS, bnRHS, bnMOD;
    bnLHS.FromDec(pLHS);
    bnRHS.FromDec(pRHS);
    bnMOD.FromDec(pMOD);
    const BigNumber res = Sub_mod(bnLHS, bnRHS, bnMOD);

    return Py_BuildValue("s",res.ToDec().c_str());
}

static PyObject* wrap_Mul_mod_Dec(PyObject* self, PyObject *args)
{
    char* pLHS;
    char* pRHS;
    char* pMOD;

    if (!PyArg_ParseTuple(args, "sss", &pLHS, &pRHS, &pMOD))
        return NULL;

    BigNumber bnLHS, bnRHS, bnMOD;
    bnLHS.FromDec(pLHS);
    bnRHS.FromDec(pRHS);
    bnMOD.FromDec(pMOD);
    const BigNumber res = Mul_mod(bnLHS, bnRHS, bnMOD);

    return Py_BuildValue("s",res.ToDec().c_str());
}

static PyObject* wrap_Div_mod_Dec(PyObject* self, PyObject *args)
{
    char* pLHS;
    char* pRHS;
    char* pMOD;

    if (!PyArg_ParseTuple(args, "sss", &pLHS, &pRHS, &pMOD))
        return NULL;

    BigNumber bnLHS, bnRHS, bnMOD;
    bnLHS.FromDec(pLHS);
    bnRHS.FromDec(pRHS);
    bnMOD.FromDec(pMOD);
    const BigNumber res = Div_mod(bnLHS, bnRHS, bnMOD);

    return Py_BuildValue("s",res.ToDec().c_str());
}


static PyObject* wrap_leftShiftFromHex(PyObject* self, PyObject *args)
{
    char * argA; 
    char * argB; 

    if (!PyArg_ParseTuple(args, "ss", &argA, &argB))
        return NULL;

    BigNumber bnA, bnB; 
    bnA.FromHex (argA);
    bnB.FromHex (argB);

    try{
        const BigNumber res = bnA << bnB;
        return Py_BuildValue("s",res.ToHex().c_str());
    }
    catch(std::runtime_error & re){
        PyErr_SetString(PyExc_ValueError, "negative shift count");
        return NULL;
    }
}

static PyObject* wrap_leftShiftFromDec(PyObject* self, PyObject *args)
{
    char * argA; 
    char * argB; 

    if (!PyArg_ParseTuple(args, "ss", &argA, &argB))
        return NULL;

    BigNumber bnA, bnB; 
    bnA.FromDec(argA);
    bnB.FromDec(argB);

    try{
        const BigNumber res = bnA << bnB;
        return Py_BuildValue("s",res.ToDec().c_str());
    }
    catch(std::runtime_error & re){
        PyErr_SetString(PyExc_ValueError, "negative shift count");
        return NULL;
    }
}

static PyObject* wrap_rightShiftFromHex(PyObject* self, PyObject *args)
{
    char * argA; 
    char * argB; 

    if (!PyArg_ParseTuple(args, "ss", &argA, &argB))
        return NULL;

    BigNumber bnA, bnB; 
    bnA.FromHex (argA);
    bnB.FromHex (argB);

    try{
        const BigNumber res = bnA >> bnB;
        return Py_BuildValue("s",res.ToHex().c_str());
    }
    catch(std::runtime_error & re){
        PyErr_SetString(PyExc_ValueError, "negative shift count");
        return NULL;
    }
}

static PyObject* wrap_rightShiftFromDec(PyObject* self, PyObject *args)
{
    char * argA; 
    char * argB; 

    if (!PyArg_ParseTuple(args, "ss", &argA, &argB))
        return NULL;

    BigNumber bnA, bnB; 
    bnA.FromDec(argA);
    bnB.FromDec(argB);

    try{
        const BigNumber res = bnA >> bnB;
        return Py_BuildValue("s",res.ToDec().c_str());
    }
    catch(std::runtime_error & re){
        PyErr_SetString(PyExc_ValueError, "negative shift count");
        return NULL;
    }
}

static PyObject* wrap_BNRandomHex(PyObject* self, PyObject *args)
{
    int nSize;     
    if (!PyArg_ParseTuple(args, "i", &nSize))
        return NULL;

    const BigNumber res = GenerateRand (nSize); 
    return Py_BuildValue("s",res.ToHex().c_str());
}

static PyObject* wrap_BNRandomDec(PyObject* self, PyObject *args)
{
    int nSize; 
    if (!PyArg_ParseTuple(args, "i", &nSize))
        return NULL;

    const BigNumber res = GenerateRand (nSize); 
    return Py_BuildValue("s",res.ToDec().c_str());
}

static PyObject* wrap_BNRandomHexWithSeed(PyObject* self, PyObject *args)
{
    char * argSeed;
    int nSize;

    if (!PyArg_ParseTuple(args,"si",&argSeed,&nSize))
        return NULL;

    BigNumber res;
    res.generateRandHexWithSeed(argSeed, nSize);

    return Py_BuildValue("s",res.ToHex().c_str());
}

static PyObject* wrap_BNRandomDecWithSeed(PyObject* self, PyObject *args)
{
    char * argSeed;
    int nSize;

    if (!PyArg_ParseTuple(args,"si",&argSeed,&nSize))
        return NULL;

    BigNumber res;
    res.generateRandDecWithSeed(argSeed, nSize);

    return Py_BuildValue("s",res.ToDec().c_str());
}

static PyObject* wrap_BNRandomPrimeHex(PyObject* self, PyObject *pyargs)
{
    int nSize;
    if (!PyArg_ParseTuple(pyargs, "i", &nSize))
        return NULL;

    const BigNumber res = GenerateRandPrime (nSize); 
    return Py_BuildValue("s",res.ToHex().c_str());
}

static PyObject* wrap_BNRandomPrimeDec(PyObject* self, PyObject *pyargs)
{
    int nSize;
    if (!PyArg_ParseTuple(pyargs, "i", &nSize))
        return NULL;

    const BigNumber res = GenerateRandPrime (nSize); 
    return Py_BuildValue("s",res.ToDec().c_str());
}

static PyObject* wrap_BNRandomPrimeHexWithSeed(PyObject* self, PyObject *pyargs)
{
    char * argSeed;
    int nSize;

    if (!PyArg_ParseTuple(pyargs, "si", &argSeed, &nSize))
        return NULL;

    BigNumber res;
    res.generateRandPrimeHexWithSeed(argSeed, nSize);

    return Py_BuildValue("s",res.ToHex().c_str());
}

static PyObject* wrap_BNRandomPrimeDecWithSeed(PyObject* self, PyObject *pyargs)
{
    char * argSeed;
    int nSize;

    if (!PyArg_ParseTuple(pyargs, "si", &argSeed, &nSize))
        return NULL;

    BigNumber res;
    res.generateRandPrimeDecWithSeed(argSeed, nSize);

    return Py_BuildValue("s",res.ToDec().c_str());
}

static PyObject* wrap_isPrimeHex(PyObject* self, PyObject *pyargs)
{
    char * carg;
    if (!PyArg_ParseTuple(pyargs, "s", &carg))
        return NULL;

    BigNumber bnArg;
    bnArg.FromHex(carg);
    const int isPrime = (int) bnArg.isPrime();
    return Py_BuildValue("i", isPrime);
}

static PyObject* wrap_isPrimeFasttestHex(PyObject* self, PyObject *pyargs)
{
    char * carg;
    if (!PyArg_ParseTuple(pyargs, "s", &carg))
        return NULL;

    BigNumber bnArg;
    bnArg.FromHex(carg);
    const int isPrime = (int) bnArg.isPrimeFasttest();
    return Py_BuildValue("i", isPrime);
}

static PyObject* wrap_isPrimeDec(PyObject* self, PyObject *pyargs)
{
    char * carg;
    if (!PyArg_ParseTuple(pyargs, "s", &carg))
        return NULL;

    BigNumber bnArg;
    bnArg.FromDec(carg);
    const int isPrime = (int) bnArg.isPrime();
    return Py_BuildValue("i", isPrime);
}

static PyObject* wrap_isPrimeFasttestDec(PyObject* self, PyObject *pyargs)
{
    char * carg;
    if (!PyArg_ParseTuple(pyargs, "s", &carg))
        return NULL;

    BigNumber bnArg;
    bnArg.FromDec(carg);
    const int isPrime = (int) bnArg.isPrimeFasttest();
    return Py_BuildValue("i", isPrime);
}

static PyObject* wrap_isGreater(PyObject* self, PyObject *pyargs)
{
    char * aArg;
    char * bArg; 
    int dec(0);
    if (!PyArg_ParseTuple(pyargs, "ssi", &aArg,&bArg,&dec))
        return NULL;

    BigNumber bnValA ;
    BigNumber bnValB;
    if(dec){
        bnValA.FromDec(aArg);
        bnValB.FromDec(bArg);
    }else{
        bnValA.FromHex(aArg);
        bnValB.FromHex(bArg);
    }

    const int isGreater = bnValA > bnValB;
    return Py_BuildValue("i", isGreater);
}

static PyObject* wrap_isEqual(PyObject* self, PyObject *pyargs)
{
    char * aArg;
    char * bArg; 
    int dec(0);
    if (!PyArg_ParseTuple(pyargs, "ssi", &aArg,&bArg,&dec))
        return NULL;

    BigNumber bnValA ;
    BigNumber bnValB;
    if(dec){
        bnValA.FromDec(aArg);
        bnValB.FromDec(bArg);
    }else{
        bnValA.FromHex(aArg);
        bnValB.FromHex(bArg);
    }

    const int isGreater = bnValA == bnValB;
    return Py_BuildValue("i", isGreater);
}


static PyMethodDef ModuleMethods[] =
{
    // {"test_get_data_nulls", wrap_test_get_data_nulls, METH_NOARGS, "Get a string of fixed length with embedded nulls"},
    {"addFromHex",wrap_addFromHex,METH_VARARGS,"Add two big numbers of arbitrary precision in hex"},
    {"addFromDec",wrap_addFromDec,METH_VARARGS,"Add two big numbers of arbitrary precision in dec"},
    {"subFromHex",wrap_subFromHex,METH_VARARGS,"Sub two big numbers of arbitrary precision in hex"},
    {"subFromDec",wrap_subFromDec,METH_VARARGS,"Sub two big numbers of arbitrary precision in dec"},
    {"multiplyFromHex", wrap_multiplyFromHex, METH_VARARGS, "Multiply two big numbers of arbitrary precision in hex"},
    {"multiplyFromDec", wrap_multiplyFromDec, METH_VARARGS, "Multiply two big numbers of arbitrary precision in dec"},
    {"divideFromHex", wrap_divideFromHex, METH_VARARGS,"Divide two big numbers of arbitrary precision in hex"},
    {"divideFromDec", wrap_divideFromDec, METH_VARARGS,"Divide two big numbers of arbitrary precision in dec "},
    {"Mod_Hex",wrap_Mod_Hex,METH_VARARGS,"Modulo of big numbers of arbitrary precision in hex"},
    {"Inv_mod_Hex",wrap_Inv_mod_Hex,METH_VARARGS,"Inverse modulo of big numbers of arbitrary precision in hex"},
    {"Add_mod_Hex",wrap_Add_mod_Hex,METH_VARARGS,"Add modulo of big numbers of arbitrary precision in hex"},
    {"Sub_mod_Hex",wrap_Sub_mod_Hex,METH_VARARGS,"Sub modulo of big numbers of arbitrary precision in hex"},
    {"Mul_mod_Hex",wrap_Mul_mod_Hex,METH_VARARGS,"Mul modulo of big numbers of arbitrary precision in hex"},
    {"Div_mod_Hex",wrap_Div_mod_Hex,METH_VARARGS,"Div modulo of big numbers of arbitrary precision in hex"},
    {"Mod_Dec",wrap_Mod_Dec,METH_VARARGS,"Modulo of big numbers of arbitrary precision in dec"},
    {"Inv_mod_Dec",wrap_Inv_mod_Dec,METH_VARARGS,"Inverse modulo of big numbers of arbitrary precision in dec"},
    {"Add_mod_Dec",wrap_Add_mod_Dec,METH_VARARGS,"Add modulo of big numbers of arbitrary precision in dec"},
    {"Sub_mod_Dec",wrap_Sub_mod_Dec,METH_VARARGS,"Sub modulo of big numbers of arbitrary precision in dec"},
    {"Mul_mod_Dec",wrap_Mul_mod_Dec,METH_VARARGS,"Mul modulo of big numbers of arbitrary precision in dec"},
    {"Div_mod_Dec",wrap_Div_mod_Dec,METH_VARARGS,"Div modulo of big numbers of arbitrary precision in dec"},
    {"GenerateRandHex",wrap_BNRandomHex,METH_VARARGS,"Generate Random Number of arbitrary precision in hex"},
    {"GenerateRandDec",wrap_BNRandomDec,METH_VARARGS,"Generate Random Number of arbitrary precision in dec"},
    {"GenerateRandHexWithSeed", wrap_BNRandomHexWithSeed, METH_VARARGS,"Generate Random Number of arbitrary precision in hex with seed (specified as a string)"},
    {"GenerateRandDecWithSeed", wrap_BNRandomDecWithSeed, METH_VARARGS,"Generate Random Number of arbitrary precision in hex with seed (specified as a string)"},
    {"leftShiftFromHex", wrap_leftShiftFromHex, METH_VARARGS, "leftshit bitwise operation that moves bits of right big number to the left by left big number value in hex"},
    {"leftShiftFromDec", wrap_leftShiftFromDec, METH_VARARGS, "leftshit bitwise operation that moves bits of right big number to the left by left big number value in dec"},
    {"rightShiftFromHex", wrap_rightShiftFromHex, METH_VARARGS," rightshift bitwise operation that moves bits of right big number to the right by left big number value in hex"},
    {"rightShiftFromDec", wrap_rightShiftFromDec, METH_VARARGS," rightshift bitwise operation that moves bits of right big number to the right by left big number value in dec"},
    {"GenerateRandPrimeHex",wrap_BNRandomPrimeHex,METH_VARARGS,"Generate Random Prime Number of arbitrary precision in hex"},
    {"GenerateRandPrimeDec",wrap_BNRandomPrimeDec,METH_VARARGS,"Generate Random Prime Number of arbitrary precision in dec"},
    {"GenerateRandPrimeHexWithSeed",wrap_BNRandomPrimeHexWithSeed,METH_VARARGS,"Generate Random Prime Number of arbitrary precision in hex with seed (specified as a string)"},
    {"GenerateRandPrimeDecWithSeed",wrap_BNRandomPrimeDecWithSeed,METH_VARARGS,"Generate Random Prime Number of arbitrary precision in dec with seed (specified as a string)"},
    {"isPrimeHex",wrap_isPrimeHex,METH_VARARGS,"Check if hex big number is prime"},
    {"isPrimeFasttestHex",wrap_isPrimeFasttestHex,METH_VARARGS,"Check if hex big number is prime (fasttest)"},
    {"isPrimeDec",wrap_isPrimeDec,METH_VARARGS,"Check if dec big number is prime"},
    {"isPrimeFasttestDec",wrap_isPrimeFasttestDec,METH_VARARGS,"Check if dec big number is prime (fasttest)"},
    {"isGreater", wrap_isGreater,METH_VARARGS,"Check if  big number a > bignumber b"},
    {"isEqual", wrap_isEqual,METH_VARARGS,"Check if  big number a == bignumber b"},
    {NULL, NULL, 0, NULL},
};
 
#if PY_MAJOR_VERSION >= 3

static int PyBigNumbers_traverse(PyObject *m, visitproc visit, void *arg) {
    Py_VISIT(GETSTATE(m)->error);
    return 0;
}

static int PyBigNumbers_clear(PyObject *m) {
    Py_CLEAR(GETSTATE(m)->error);
    return 0;
}


static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "PyBigNumbers",
        NULL,
        sizeof(struct module_state),
        ModuleMethods,
        NULL,
        PyBigNumbers_traverse,
        PyBigNumbers_clear,
        NULL
};

#define INITERROR return NULL

PyMODINIT_FUNC
PyInit_PyBigNumbers(void)

#else
#define INITERROR return

void
initmyextension(void)
#endif
{
#if PY_MAJOR_VERSION >= 3
    PyObject *module = PyModule_Create(&moduledef);
#else
    PyObject *module = Py_InitModule("BigNumextension", myextension_methods);
#endif

    if (module == NULL)
        INITERROR;
    struct module_state *st = GETSTATE(module);

    st->error = PyErr_NewException("PyBigNumbersextension.Error", NULL, NULL);
    if (st->error == NULL) {
        Py_DECREF(module);
        INITERROR;
    }

#if PY_MAJOR_VERSION >= 3
    return module;
#endif
}
