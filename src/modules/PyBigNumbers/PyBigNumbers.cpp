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
    { "GenerateRandHexWithSeed", wrap_BNRandomHexWithSeed, METH_VARARGS,"Generate Random Number of arbitrary precision in hex with seed (specified as a string)"},
    { "GenerateRandDecWithSeed", wrap_BNRandomDecWithSeed, METH_VARARGS,"Generate Random Number of arbitrary precision in hex with seed (specified as a string)"},
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
