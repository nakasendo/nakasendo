#include <Python.h>
#include <string>
#include <iostream>
#include <AsymKey/AsymKeyAPI.h>
#include <AsymKey/AsymKey.h>


struct module_state {
    PyObject *error;
};

#if PY_MAJOR_VERSION >= 3
#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))
#else
#define GETSTATE(m) (&_state)
static struct module_state _state;
#endif

static PyObject* wrap_GenerateKeyPairPEM(PyObject* self, PyObject *args)
{
    const std::pair<std::string, std::string> keyPairPEM = GenerateKeyPairPEM();
    return Py_BuildValue("ss", keyPairPEM.first.c_str(), keyPairPEM.second.c_str());
}

static PyObject* wrap_GenerateKeyPairHEX(PyObject* self, PyObject *args)
{
    const std::pair<std::string, std::string> keyPairHEX = GenerateKeyPairHEX();
    return Py_BuildValue("ss", keyPairHEX.first.c_str(), keyPairHEX.second.c_str());
}

static PyObject* wrap_GetKeyPairHEX(PyObject* self, PyObject *args)
{
    char* cPrivKeyPEM = nullptr;

    if (!PyArg_ParseTuple(args, "s", &cPrivKeyPEM))
        return NULL;

    const std::string privPEMKey(cPrivKeyPEM);

    const std::pair<std::string, std::string> keyPairHEX = GetKeyPairHEX(privPEMKey);
    return Py_BuildValue("ss", keyPairHEX.first.c_str(), keyPairHEX.second.c_str());
}

static PyObject* wrap_Sign(PyObject* self, PyObject *args)
{
    char* cPrivKey = nullptr;
    char* cMsg = nullptr;

    if (!PyArg_ParseTuple(args, "ss", &cPrivKey, &cMsg))
        return NULL;

    const std::string privPEMKey(cPrivKey);
    const std::string msg(cMsg);

    AsymKey key;
    key.setPEMPrivateKey(privPEMKey);
    const std::pair<std::string, std::string> sign = key.sign(msg);

    return Py_BuildValue("ss", sign.first, sign.second);
}

static PyObject* wrap_Verify(PyObject* self, PyObject *args)
{
    char* cMsg = nullptr;
    char* cPubKey = nullptr;
    char* cSigR = nullptr;
    char* cSigS = nullptr;

    if (!PyArg_ParseTuple(args, "ssss", &cMsg, &cPubKey, &cSigR, &cSigS))
        return NULL;

    const std::string msg(cMsg);
    const std::string pubKey(cPubKey);
    const std::string sigR(cSigR);
    const std::string sigS(cSigS);

    const bool verifyOK = verify(msg, pubKey, std::make_pair(sigR, sigS));

    return Py_BuildValue("i", verifyOK);
}


static PyMethodDef ModuleMethods[] =
{
    // {"test_get_data_nulls", wrap_test_get_data_nulls, METH_NOARGS, "Get a string of fixed length with embedded nulls"},
    {"GenerateKeyPairPEM",wrap_GenerateKeyPairPEM,METH_VARARGS,"Generate pair of keys in pem format"},
    {"GenerateKeyPairHEX",wrap_GenerateKeyPairHEX,METH_VARARGS,"Generate pair of keys in hex format"},
    {"GetKeyPairHEX",wrap_GetKeyPairHEX,METH_VARARGS,"Get pair of keys in hex format from a private PEM key"},
    {"Sign",wrap_Sign,METH_VARARGS,"Sign message with private Key"},
    {"Verify",wrap_Verify,METH_VARARGS,"Verify message's signature with public key"},
    {NULL, NULL, 0, NULL},
};
 
#if PY_MAJOR_VERSION >= 3

static int PyAsymKey_traverse(PyObject *m, visitproc visit, void *arg) {
    Py_VISIT(GETSTATE(m)->error);
    return 0;
}

static int PyAsymKey_clear(PyObject *m) {
    Py_CLEAR(GETSTATE(m)->error);
    return 0;
}


static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "PyAsymKey",
        NULL,
        sizeof(struct module_state),
        ModuleMethods,
        NULL,
        PyAsymKey_traverse,
        PyAsymKey_clear,
        NULL
};

#define INITERROR return NULL

PyMODINIT_FUNC
PyInit_PyAsymKey(void)

#else
#define INITERROR return

void
initmyextension(void)
#endif
{
#if PY_MAJOR_VERSION >= 3
    PyObject *module = PyModule_Create(&moduledef);
#else
    PyObject *module = Py_InitModule("PyAsymKeyExtension", myextension_methods);
#endif

    if (module == NULL)
        INITERROR;
    struct module_state *st = GETSTATE(module);

    st->error = PyErr_NewException("PyAsymKeyExtension.Error", NULL, NULL);
    if (st->error == NULL) {
        Py_DECREF(module);
        INITERROR;
    }

#if PY_MAJOR_VERSION >= 3
    return module;
#endif
}
