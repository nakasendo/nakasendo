#include <Python.h>
#include <string>
#include <iostream>
#include <SymEncDec/SymEncDecAPI.h>
#include <MessageHash/MessageHashAPI.h>

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
    if (!PyArg_ParseTuple(args, "sss", &argA, &argB, &argC))
        return NULL;

    std::cout << "AM I HERE?" << std::endl;  
    std::string encMsg = Encode(argA, argB, argC);

    std::string result = EncodeBase64(encMsg.c_str());

#if 0     
    std::unique_ptr<char> tmpBuffer (new char[encMsg.length()*2+1]); 
    for (unsigned int i=0;i<encMsg.length(); ++i)
    {
        sprintf(&tmpBuffer.get()[i*2],"%02x", encMsg[i]);
    }
    //return std::string (tmpBuffer.get());

    std::cout << tmpBuffer.get() << std::endl ;
#endif    
    return Py_BuildValue("s",result.c_str());
}

static PyObject* wrap_decodeAES(PyObject* self, PyObject *args){
    char * argA;
    char * argB;
    char * argC;
    if (!PyArg_ParseTuple(args, "sss", &argA,&argB,&argC))
        return NULL;

    std::string decMsg = Decode(argA, argB, argC);
    return Py_BuildValue("s",decMsg.c_str());
}

static PyMethodDef ModuleMethods[] =
{
    {"encodeAES",wrap_encodeAES,METH_VARARGS,"encode AES"},
    {"decodeAES",wrap_decodeAES,METH_VARARGS,"decode AES"},
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
