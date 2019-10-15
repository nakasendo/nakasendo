#include <Python.h>
#include <sstream>
#include <string>
#include <iostream>

#include <SymEncDec/SymEncDec.h>

struct module_state {
    PyObject *error;
};

#if PY_MAJOR_VERSION >= 3
#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))
#else
#define GETSTATE(m) (&_state)
static struct module_state _state;
#endif

static PyObject* wrap_encodeAES(PyObject* self, PyObject *args){
    char * argA;
    char * argB; 
    char * argC;

         
    if (!PyArg_ParseTuple(args, "sss", &argA,&argB,&argC))
        return NULL;       
    
    std::string msg(argA);
    std::string key(argB);
    std::string iv(argC);

    std::cout << msg << "\t" << key <<"\t" << iv << std::endl; 

    const std::string encMsgHex = Encode(msg, key, iv);
    return Py_BuildValue("s#",encMsgHex.c_str(), encMsgHex.length());      

    
}

static PyObject* wrap_decodeAES(PyObject* self, PyObject *args){
    
    char * argA;
    char * argB;
    char * argC;
    
    
    
    //if (!PyArg_ParseTuple(args, "y#ss",&argA,&argALen, &argB,&argC))
    if (!PyArg_ParseTuple(args, "sss",&argA, &argB, &argC))
        return NULL;


    std::string decMsg = Decode (argA, argB, argC);
    return Py_BuildValue("s#",decMsg.c_str(), decMsg.length());
}

static PyObject* wrap_generateKey(PyObject* self, PyObject *args){
    char * argA;
    int lenArgA;
    char * argB;
    int lenArgB; 
    if (!PyArg_ParseTuple(args, "s#s#",&argA, &lenArgA, &argB, &lenArgB))
        return NULL;

    std::unique_ptr<unsigned char> argAConv (new unsigned char[lenArgA+1]);
    std::fill_n(argAConv.get(), lenArgA+1, 0x00);
    for(int i=0;i<lenArgA;++i){
        argAConv.get()[i] = argA[i];
    }
    std::unique_ptr<unsigned char> argBConv (new unsigned char[lenArgB+1]);
    std::fill_n(argBConv.get(), lenArgB+1, 0x00);
    for(int i=0;i<lenArgB;++i){
        argBConv.get()[i] = argB[i]; 
    }

    std::string keyHex = GenerateKey256(argA, argB);

    return Py_BuildValue("s#",keyHex.c_str(), keyHex.length());
}

static PyObject* wrap_GenerateNounce(PyObject* self, PyObject *args){
    std::string nounceAsHex = GenerateNounce (); 
    return Py_BuildValue ("s#", nounceAsHex.c_str(), nounceAsHex.size());
}

static PyMethodDef ModuleMethods[] =
{
    {"encodeAES",wrap_encodeAES,METH_VARARGS,"encode AES"},
    {"decodeAES",wrap_decodeAES,METH_VARARGS,"decode AES"},
    {"GenerateNounce", wrap_GenerateNounce,METH_NOARGS,"generate a nounce value"},
    {"GenerateKey", wrap_generateKey, METH_VARARGS,"generate a key"},
    {NULL, NULL, 0, NULL},
};
 
#if PY_MAJOR_VERSION >= 3

static int PySymEncDec_traverse(PyObject *m, visitproc visit, void *arg) {
    Py_VISIT(GETSTATE(m)->error);
    return 0;
}

static int PySymEncDec_clear(PyObject *m) {
    Py_CLEAR(GETSTATE(m)->error);
    return 0;
}


static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "PySymEncDec",
        NULL,
        sizeof(struct module_state),
        ModuleMethods,
        NULL,
        PySymEncDec_traverse,
        PySymEncDec_clear,
        NULL
};

#define INITERROR return NULL

PyMODINIT_FUNC
PyInit_PySymEncDec(void)

#else
#define INITERROR return

void
initmyextension(void)
#endif
{
#if PY_MAJOR_VERSION >= 3
    PyObject *module = PyModule_Create(&moduledef);
#else
    PyObject *module = Py_InitModule("PySymEncDec", myextension_methods);
#endif

    if (module == NULL)
        INITERROR;
    struct module_state *st = GETSTATE(module);

    st->error = PyErr_NewException("PySymEncDec.Error", NULL, NULL);
    if (st->error == NULL) {
        Py_DECREF(module);
        INITERROR;
    }

#if PY_MAJOR_VERSION >= 3
    return module;
#endif
}

