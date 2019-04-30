#include <Python.h>
#include <string>
#include <iostream>
#include <BigNumbers/BigNumAPI.h>


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

    std::unique_ptr<char> result = addFromHex (argA,argB);
    return Py_BuildValue("s",result.get());
}

static PyObject* wrap_addFromDec(PyObject* self, PyObject *args)
{
    char * argA; 
    char * argB; 

    if (!PyArg_ParseTuple(args, "ss", &argA, &argB))
        return NULL;

    std::unique_ptr<char> result = addFromDec(argA,argB);
    return Py_BuildValue("s",result.get());
}

static PyObject* wrap_multiplyFromHex(PyObject* self, PyObject *args)
{
    char * argA; 
    char * argB; 

    if (!PyArg_ParseTuple(args, "ss", &argA, &argB))
        return NULL;

    std::unique_ptr<char> result = multiplyFromHex(argA,argB);
    return Py_BuildValue("s",result.get());
}

static PyObject* wrap_multiplyFromDec(PyObject* self, PyObject *args)
{
    char * argA; 
    char * argB; 

    if (!PyArg_ParseTuple(args, "ss", &argA, &argB))
        return NULL;

    std::unique_ptr<char> result = multiplyFromDec(argA,argB);
    return Py_BuildValue("s",result.get());
}

static PyObject* wrap_divideFromHex(PyObject* self, PyObject *args)
{
    char * argA; 
    char * argB; 

    if (!PyArg_ParseTuple(args, "ss", &argA, &argB))
        return NULL;

    std::unique_ptr<char> result;
    try{
       result = divideFromHex(argA,argB);
    }
    catch(std::runtime_error & re){
        PyErr_SetString(PyExc_ZeroDivisionError, "division or modulo by zero");
	return NULL;
    }
    return Py_BuildValue("s", result.get());
}

static PyObject* wrap_divideFromDec(PyObject* self, PyObject *args)
{
    char * argA; 
    char * argB; 

    if (!PyArg_ParseTuple(args, "ss", &argA, &argB))
        return NULL;

    std::unique_ptr<char> result;
    try{
       result = divideFromDec(argA,argB);
    }
    catch(std::runtime_error & re){
        PyErr_SetString(PyExc_ZeroDivisionError, "division or modulo by zero");
	return NULL;
    }
    return Py_BuildValue("s", result.get());
}

static PyObject* wrap_leftShiftFromHex(PyObject* self, PyObject *args)
{
    char * argA; 
    char * argB; 

    if (!PyArg_ParseTuple(args, "ss", &argA, &argB))
        return NULL;

    std::unique_ptr<char> result;
    try{
        result = leftShiftFromHex(argA,argB);
    }
    catch(std::runtime_error & re){
        PyErr_SetString(PyExc_ValueError, "negative shift count");
	return NULL;
    }
    return Py_BuildValue("s",result.get());
}

static PyObject* wrap_leftShiftFromDec(PyObject* self, PyObject *args)
{
    char * argA; 
    char * argB; 

    if (!PyArg_ParseTuple(args, "ss", &argA, &argB))
        return NULL;

    std::unique_ptr<char> result;
    try{
        result = leftShiftFromDec(argA,argB);
    }
    catch(std::runtime_error & re){
        PyErr_SetString(PyExc_ValueError, "negative shift count");
	return NULL;
    }
    return Py_BuildValue("s",result.get());
}

static PyObject* wrap_rightShiftFromHex(PyObject* self, PyObject *args)
{
    char * argA; 
    char * argB; 

    if (!PyArg_ParseTuple(args, "ss", &argA, &argB))
        return NULL;

    std::unique_ptr<char> result;
    try{
        result = rightShiftFromHex(argA,argB);
    }
    catch(std::runtime_error & re){
        PyErr_SetString(PyExc_ValueError, "negative shift count");
	return NULL;
    }
    return Py_BuildValue("s",result.get());
}

static PyObject* wrap_rightShiftFromDec(PyObject* self, PyObject *args)
{
    char * argA; 
    char * argB; 

    if (!PyArg_ParseTuple(args, "ss", &argA, &argB))
        return NULL;

    std::unique_ptr<char> result;
    try{
        result = rightShiftFromDec(argA,argB);
    }
    catch(std::runtime_error & re){
        PyErr_SetString(PyExc_ValueError, "negative shift count");
	return NULL;
    }
    return Py_BuildValue("s",result.get());
}

static PyObject* wrap_subFromHex(PyObject* self, PyObject *args)
{
    char * argA; 
    char * argB; 

    if (!PyArg_ParseTuple(args, "ss", &argA, &argB))
        return NULL;

    std::unique_ptr<char> result = subFromHex(argA,argB);
    return Py_BuildValue("s",result.get());
}

static PyObject* wrap_subFromDec(PyObject* self, PyObject *args)
{
    char * argA; 
    char * argB; 

    if (!PyArg_ParseTuple(args, "ss", &argA, &argB))
        return NULL;

    std::unique_ptr<char> result = subFromDec(argA,argB);
    return Py_BuildValue("s",result.get());
}

static PyObject* wrap_BNRandomHex(PyObject* self, PyObject *args)
{
    int nSize;     
    if (!PyArg_ParseTuple(args, "i", &nSize))
        return NULL;

    std::unique_ptr<char> result = BNRandomHex(nSize);
    return Py_BuildValue("s",result.get());
}

static PyObject* wrap_BNRandomDec(PyObject* self, PyObject *args)
{
    int nSize; 
    if (!PyArg_ParseTuple(args, "i", &nSize))
        return NULL;

    std::unique_ptr<char> result = BNRandomDec(nSize);
    return Py_BuildValue("s",result.get());
}

static PyObject* wrap_BNRandomHexWithSeed(PyObject* self, PyObject *args)
{
    char * argSeed;
    int nSize;

    if (!PyArg_ParseTuple(args,"si",&argSeed,&nSize))
        return NULL;
    std::unique_ptr<char> result = BNRandomHexWithSeed(argSeed, nSize);
    return Py_BuildValue ("s", result.get()); 
}

static PyObject* wrap_BNRandomDecWithSeed(PyObject* self, PyObject *args)
{
    char * argSeed;
    int nSize;

    if (!PyArg_ParseTuple(args,"si",&argSeed,&nSize))
        return NULL;
    std::unique_ptr<char> result = BNRandomDecWithSeed(argSeed, nSize);
    return Py_BuildValue ("s", result.get()); 
}


static PyMethodDef ModuleMethods[] =
{
    // {"test_get_data_nulls", wrap_test_get_data_nulls, METH_NOARGS, "Get a string of fixed length with embedded nulls"},
    {"addFromHex",wrap_addFromHex,METH_VARARGS,"Add too big numbers of arbitrary precision in hex"},
    {"addFromDec",wrap_addFromDec,METH_VARARGS,"Add too big numbers of arbitrary precision in dec"},
    {"subFromHex",wrap_subFromHex,METH_VARARGS,"Sub too big numbers of arbitrary precision in hex"},
    {"subFromDec",wrap_subFromDec,METH_VARARGS,"Sub too big numbers of arbitrary precision in dec"},
    {"GenerateRandHex",wrap_BNRandomHex,METH_VARARGS,"Generate Random Number of arbitrary precision in hex"},
    {"GenerateRandDec",wrap_BNRandomDec,METH_VARARGS,"Generate Random Number of arbitrary precision in dec"},
    {"GenerateRandHexWithSeed", wrap_BNRandomHexWithSeed, METH_VARARGS,"Generate Random Number of arbitrary precision in hex with seed (specified as a string)"},
    {"GenerateRandDecWithSeed", wrap_BNRandomDecWithSeed, METH_VARARGS,"Generate Random Number of arbitrary precision in hex with seed (specified as a string)"},
    {"leftShiftFromHex", wrap_leftShiftFromHex, METH_VARARGS, "leftshit bitwise operation that moves bits of right big number to the left by left big number value in hex"},
    {"leftShiftFromDec", wrap_leftShiftFromDec, METH_VARARGS, "leftshit bitwise operation that moves bits of right big number to the left by left big number value in dec"},
    {"rightShiftFromHex", wrap_rightShiftFromHex, METH_VARARGS," rightshift bitwise operation that moves bits of right big number to the right by left big number value in hex"},
    {"rightShiftFromDec", wrap_rightShiftFromDec, METH_VARARGS," rightshift bitwise operation that moves bits of right big number to the right by left big number value in dec"},
    {"multiplyFromHex", wrap_multiplyFromHex, METH_VARARGS, "Multiply two big numbers of arbitrary precision in hex"},
    {"multiplyFromDec", wrap_multiplyFromDec, METH_VARARGS, "Multiply two big numbers of arbitrary precision in dec"},
    {"divideFromHex", wrap_divideFromHex, METH_VARARGS,"Divide two big numbers of arbitrary precision in hex"},
    {"divideFromDec", wrap_divideFromDec, METH_VARARGS,"Divide two big numbers of arbitrary precision in dec "},
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
