#include <Python.h>
#include <string.h>
#include <string>
#include <iostream>
#include <MessageHash/MessageHashConfig.h>
#include <MessageHash/MessageHash.h>
#include <MessageHash/Base64EncDec.h>
#include <MessageHash/Base58EncDec.h>


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

    MessageHash hashMess;
    hashMess.Hash(argA, argB);
    std::string result = hashMess.HashHex ();
    return Py_BuildValue("s",result.c_str());
}

static PyObject* wrap_ListHash(PyObject* self, PyObject *args)
{
    MessageHash hashMess;
	const std::string result = hashMess.ListAvailableHash () ;
	return Py_BuildValue("s", result.c_str());
}

static PyObject* wrap_EncodeBase64(PyObject* self, PyObject *args){
    char * argA;
    int lenargA; 
    if (!PyArg_ParseTuple(args, "s#", &argA, &lenargA))
        return NULL;

    std::unique_ptr<unsigned char[]> msgValptr = std::make_unique<unsigned char[]>(lenargA + 1);

    for (int i = 0;i < lenargA; ++i){
        msgValptr.get()[i] = argA[i]; 
    }
    msgValptr.get()[lenargA]='\0';

    std::string encoded = EncodeBase64(msgValptr,lenargA);
    return Py_BuildValue("s#",encoded.c_str(),encoded.size());

}

static PyObject* wrap_DecodeBase64(PyObject* self, PyObject *args){
    char * argA;
    int lenargA; 
    if (!PyArg_ParseTuple(args, "s#", &argA, &lenargA))
        return NULL;
 
   
    int msgLen(0);
    std::string result;
    std::unique_ptr<unsigned char[]> resultAsBytes = DecodeBase64(argA,msgLen);
    for(int i=0;i<msgLen;++i){
        result.push_back(resultAsBytes.get()[i]);
    }

    return Py_BuildValue("s",result.c_str());
}

static PyObject* wrap_EncodeBase58(PyObject* self, PyObject *args){
    unsigned char * argA;
    int lenA;
    if (!PyArg_ParseTuple(args, "s#", &argA,&lenA))
        return NULL;
    std::string paramsToPass;     
    for (int i=0;i<lenA; ++ i){
        paramsToPass.push_back(argA[i]); 
    }
    std::string result = EncodeBase58(paramsToPass);
    return Py_BuildValue("s",result.c_str());
}

static PyObject* wrap_DecodeBase58(PyObject* self, PyObject *args){
    unsigned char * argA;
    int lenA;

    if (!PyArg_ParseTuple(args, "s#", &argA, &lenA))
        return NULL;

    std::string paramsToPass;     
    for (int i=0;i<lenA; ++ i){
        paramsToPass.push_back(argA[i]); 
    }
    std::string result = DecodeBase58(paramsToPass);
    return Py_BuildValue("s",result.c_str());
}

static PyObject* wrap_EncodeBase58Checked(PyObject* self, PyObject *args){
    unsigned char * argA;
    int lenA;
    if (!PyArg_ParseTuple(args, "s#", &argA,&lenA))
        return NULL;
    std::string paramsToPass;     
    for (int i=0;i<lenA; ++ i){
        paramsToPass.push_back(argA[i]); 
    }        
    std::string result = EncodeBase58Checked(paramsToPass);
    return Py_BuildValue("s",result.c_str());
}

static PyObject* wrap_DecodeBase58Checked(PyObject* self, PyObject *args){
    unsigned char * argA;
    int lenA;
    if (!PyArg_ParseTuple(args, "s#", &argA, &lenA))
        return NULL;

    std::string paramsToPass;     
    for (int i=0;i<lenA; ++ i){
        paramsToPass.push_back(argA[i]); 
    }
    std::string result = DecodeBase58Checked(paramsToPass);
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
