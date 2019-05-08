#include <Python.h>
#include <string>
#include <iostream>
#include <MessageHash/MessageHashAPI.h>
#include "MessageHash/Base64EncDec.h"


struct module_state {
    PyObject *error;
};

#if PY_MAJOR_VERSION >= 3
#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))
#else
#define GETSTATE(m) (&_state)
static struct module_state _state;
#endif


static PyObject* wrap_HashMsgSHA256(PyObject* self, PyObject *args)
{
    char * argA; 

    if (!PyArg_ParseTuple(args, "s", &argA))
        return NULL;

    std::string result = HashMsgSHA256 (argA);
    return Py_BuildValue("s",result.c_str());
}

static PyObject* wrap_HashMsg(PyObject* self, PyObject *args)
{
    char * argA; 
    char * argB; 

    if (!PyArg_ParseTuple(args, "ss", &argA, &argB))
        return NULL;

    std::string result = HashMsg(argA,argB);
    return Py_BuildValue("s",result.c_str());
}

static PyObject* wrap_ListHash(PyObject* self, PyObject *args)
{
	std::string result = ListHashFunc();
	std::cout << result << std::endl;
	return Py_BuildValue("s", result.c_str());
}

static PyObject* wrap_EncodeBase64(PyObject* self, PyObject *args){
    char * argA;
    if (!PyArg_ParseTuple(args, "s", &argA))
        return NULL;
    std::string result = EncodeBase64(argA);
    return Py_BuildValue("s",result.c_str());
}

static PyObject* wrap_DecodeBase64(PyObject* self, PyObject *args){
    char * argA;
    if (!PyArg_ParseTuple(args, "s", &argA))
        return NULL;

    std::string result = DecodeBase64(argA);
    return Py_BuildValue("s",result.c_str());
}

static PyObject* wrap_EncodeBase58(PyObject* self, PyObject *args){
    char * argA;
    if (!PyArg_ParseTuple(args, "s", &argA))
        return NULL;
    std::string result = EncodeBase58(argA);
    return Py_BuildValue("s",result.c_str());
}

static PyObject* wrap_DecodeBase58(PyObject* self, PyObject *args){
    char * argA;
    if (!PyArg_ParseTuple(args, "s", &argA))
        return NULL;

    std::string result = DecodeBase58(argA);
    return Py_BuildValue("s",result.c_str());
}

static PyObject* wrap_EncodeBase58Checked(PyObject* self, PyObject *args){
    char * argA;
    if (!PyArg_ParseTuple(args, "s", &argA))
        return NULL;
    std::string result = EncodeBase58Checked(argA);
    return Py_BuildValue("s",result.c_str());
}

static PyObject* wrap_DecodeBase58Checked(PyObject* self, PyObject *args){
    char * argA;
    if (!PyArg_ParseTuple(args, "s", &argA))
        return NULL;

    std::string result = DecodeBase58Checked(argA);
    return Py_BuildValue("s",result.c_str());
}

static PyMethodDef ModuleMethods[] =
{
    {"HashMsgSHA256",wrap_HashMsgSHA256,METH_VARARGS,"Generate SHA256 hash on a given input"},
    {"HashMsg",wrap_HashMsg,METH_VARARGS,"Generate a HASH on a given input using the hashing algorithim specified"},
    {"ListHash",wrap_ListHash, METH_VARARGS,"Return a list of available hash functions"},
    {"EncodeBase64",wrap_EncodeBase64,METH_VARARGS,"Encode given string in base64"},
    {"DecodeBase64",wrap_DecodeBase64,METH_VARARGS,"Decode given string in base64"},
    {"EncodeBase58",wrap_EncodeBase58,METH_VARARGS,"Encode given string in base58"},
    {"DecodeBase58",wrap_DecodeBase58,METH_VARARGS,"Decode given string in base58"},
    {"EncodeBase58Checked",wrap_EncodeBase58Checked,METH_VARARGS,"Encode given string in base58"},
    {"DecodeBase58Checked",wrap_DecodeBase58Checked,METH_VARARGS,"Decode given string in base58"},
    {NULL, NULL, 0, NULL},
};
 
#if PY_MAJOR_VERSION >= 3

static int PyMessageHash_traverse(PyObject *m, visitproc visit, void *arg) {
    Py_VISIT(GETSTATE(m)->error);
    return 0;
}

static int PyMessageHash_clear(PyObject *m) {
    Py_CLEAR(GETSTATE(m)->error);
    return 0;
}


static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "PyMessageHash",
        NULL,
        sizeof(struct module_state),
        ModuleMethods,
        NULL,
        PyMessageHash_traverse,
        PyMessageHash_clear,
        NULL
};

#define INITERROR return NULL

PyMODINIT_FUNC
PyInit_PyMessageHash(void)

#else
#define INITERROR return

void
initmyextension(void)
#endif
{
#if PY_MAJOR_VERSION >= 3
    PyObject *module = PyModule_Create(&moduledef);
#else
    PyObject *module = Py_InitModule("PyMessageHash", myextension_methods);
#endif

    if (module == NULL)
        INITERROR;
    struct module_state *st = GETSTATE(module);

    st->error = PyErr_NewException("PyMessageHash.Error", NULL, NULL);
    if (st->error == NULL) {
        Py_DECREF(module);
        INITERROR;
    }

#if PY_MAJOR_VERSION >= 3
    return module;
#endif
}
