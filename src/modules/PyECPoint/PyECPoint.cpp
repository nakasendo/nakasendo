#include <Python.h>
#include <string>
#include <iostream>
#include <ECPoint/ECPoint.h>


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

    ECPoint ecpointA, ecpointB;
    ecpointA.FromHex(argA);
    ecpointB.FromHex(argB);
    ECPoint ecpointC = ecpointA + ecpointB; 
    return Py_BuildValue("s", ecpointC.ToHex());
}

static PyObject* wrap_comapre(PyObject* self, PyObject *args)
{
    char * argA; 
    char * argB; 

    if (!PyArg_ParseTuple(args, "ss", &argA, &argB))
        return NULL;

    ECPoint ecpointA, ecpointB;
    ecpointA.FromHex(argA);
    ecpointB.FromHex(argB);
    bool isSame = ecpointA == ecpointB;
    return Py_BuildValue("O", isSame == true ? Py_True: Py_False);
}

static PyObject* wrap_doubleFromHex(PyObject* self, PyObject *args)
{
    char * argA; 

    if (!PyArg_ParseTuple(args, "s", &argA))
        return NULL;

    ECPoint ecpointA;
    ecpointA.FromHex(argA);
    ECPoint ecpointC = ecpointA.Double();
    return Py_BuildValue("s", ecpointC.ToHex());
}

static PyObject* wrap_invertFromHex(PyObject* self, PyObject *args)
{
    char * argA; 

    if (!PyArg_ParseTuple(args, "s", &argA))
        return NULL;

    ECPoint ecpointA;
    ecpointA.FromHex(argA);
    ecpointA.Invert();
    return Py_BuildValue("s", ecpointA.ToHex());
}

static PyObject* wrap_checkInfinityFromHex(PyObject* self, PyObject *args)
{
    char * argA; 

    if (!PyArg_ParseTuple(args, "s", &argA))
        return NULL;

    ECPoint ecpointA;
    ecpointA.FromHex(argA);
    return Py_BuildValue("O", ecpointA.CheckInfinity()== true ? Py_True: Py_False);
}


static PyObject* wrap_checkOnCurveFromHex(PyObject* self, PyObject *args)
{
    char * argA; 

    if (!PyArg_ParseTuple(args, "s", &argA))
        return NULL;

    ECPoint ecpointA;
    ecpointA.FromHex(argA);
    return Py_BuildValue("O", ecpointA.CheckOnCurve()== true ? Py_True: Py_False);
}

static PyMethodDef ModuleMethods[] =
{
    {"addFromHex", wrap_addFromHex, METH_VARARGS, "Add two ECPoints in hex with the default NID ==> NID_secp256k1"},
    {"comapre", wrap_comapre, METH_VARARGS, "Compare two given ECPoints "},
    {"doubleFromHex", wrap_doubleFromHex, METH_VARARGS, "Double the ECPoint in hex with the default NID ==> NID_secp256k1"},
    {"invertFromHex", wrap_invertFromHex, METH_VARARGS, "Invert the ECPoint in hex with the default NID ==> NID_secp256k1"},
    {NULL, NULL, 0, NULL},
};
 
#if PY_MAJOR_VERSION >= 3

static int PyECPoint_traverse(PyObject *m, visitproc visit, void *arg) {
    Py_VISIT(GETSTATE(m)->error);
    return 0;
}

static int PyECPoint_clear(PyObject *m) {
    Py_CLEAR(GETSTATE(m)->error);
    return 0;
}


static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "PyECPoint",
        NULL,
        sizeof(struct module_state),
        ModuleMethods,
        NULL,
        PyECPoint_traverse,
        PyECPoint_clear,
        NULL
};

#define INITERROR return NULL

PyMODINIT_FUNC
PyInit_PyECPoint(void)

#else
#define INITERROR return

void
initmyextension(void)
#endif
{
#if PY_MAJOR_VERSION >= 3
    PyObject *module = PyModule_Create(&moduledef);
#else
    PyObject *module = Py_InitModule("ECPointextension", myextension_methods);
#endif

    if (module == NULL)
        INITERROR;
    struct module_state *st = GETSTATE(module);

    st->error = PyErr_NewException("PyECPointextension.Error", NULL, NULL);
    if (st->error == NULL) {
        Py_DECREF(module);
        INITERROR;
    }

#if PY_MAJOR_VERSION >= 3
    return module;
#endif
}
