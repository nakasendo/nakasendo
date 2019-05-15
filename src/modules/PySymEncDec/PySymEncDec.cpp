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
    unsigned char * argA;
    char * argB; 
    char * argC; 
    int lenA; 
    //int lenB;
    //int lenC; 

     
    //if (!PyArg_ParseTuple(args, "u#s#s#", &argA,&lenA,&argB,&lenB,&argC,&lenC))
    if (!PyArg_ParseTuple(args, "u#ss", &argA,&lenA,&argB,&argC))
        return NULL;

    printf("key: %s:", argB);
    std::string argAStr,argBStr (argB), argCStr(argC);
    for(int i=0;i<lenA;++i){
        argAStr.push_back(argA[i]);
    }
#if 0    
    for(int i=0;i<lenB;++i){
        argBStr.push_back(argB[i]);
    }
    for(int i=0;i<lenC;++i){
        argCStr.push_back(argC[i]);
    }
#endif
    
    
    std::cout << "Key & IV parameters .. wrap_encodeAES" << std::endl; 
    //std::cout << "key length: " << lenB << "\tiv length: " << lenC << std::endl; 
    std::cout << "key length: " << argBStr.length() << "\tiv length: " << argCStr.length() << std::endl; 
    std::cout << "\nKey->: " << argBStr << "\tiv: " << argCStr << std::endl;     
    std::vector<uint8_t> encMsg = EncodeEx(argAStr, argBStr, argCStr);

    std::unique_ptr<unsigned char> msgPtr (new unsigned char[encMsg.size()]);
    std::unique_ptr<char> msgPtrChar (new char[encMsg.size()]);
    int index(0);
    int index1(0);
    std::fill_n(msgPtr.get(),encMsg.size(),0x00);
    for (std::vector<uint8_t>::const_iterator iter = encMsg.begin(); iter != encMsg.end(); ++ iter){
        printf("%02x", *iter);
        msgPtr.get()[index++] = *iter; 
        msgPtrChar.get()[index1++] = (char)*iter; 
    } 

    std::cout << "Did I make it here" << std::endl; 

    //PyObject* obj = PyBytes_FromStringAndSize(msgPtrChar.get(),encMsg.size());
    Py_buffer* obj = (Py_buffer)malloc(sizeof(unsigned char) * encMsg.size()); 
    memcpy (msgPtr.get(),obj->buf, obj->len);
    //return Py_BuildValue("s#", encMsg.c_str(), encMsg.length());
    std::cout << "\nEncoded length: " << encMsg.size() << std::endl; 
    //return Py_BuildValue("y#",encMsg.c_str(), encMsg.length());
    //return Py_BuildValue("S",encMsg.c_str());
    //return Py_BuildValue("y#",msgPtr.get(),encMsg.size() );
    return Py_BuildValue("S",obj );
    
}

static PyObject* wrap_decodeAES(PyObject* self, PyObject *args){
    //PyByteArrayObject * argA;
    unsigned char * argA;
    unsigned char * argB;
    unsigned char * argC;
    int argALen; 
    int argBLen; 
    int argCLen; 
    
    
    //if (!PyArg_ParseTuple(args, "y#ss",&argA,&argALen, &argB,&argC))
    if (!PyArg_ParseTuple(args, "s#s#s#",&argA,&argALen, &argB,&argBLen, &argC, &argCLen))
        return NULL;

    
    
    std::vector<uint8_t> argABytes;
    std::cout << "Lenght of encoded data: "<< argALen << std::endl; 
    for (int i=0; i<argALen; ++i){
        printf("%02x", argA[i]);
        argABytes.push_back(argA[i]);
    }
    std::string argBStr,argCStr; 
    for (int i=0;i<argBLen;++i){
        argBStr.push_back(argB[i]);
    }
    for(int i=0;i<argCLen;++i){
        argCStr.push_back(argC[i]);
    }
    std::cout << "Key & IV parameters .. wrap_decodAES" << std::endl; 
    std::cout << "key length: " << argBLen << "\tiv length: " << argCLen << std::endl; 
    std::cout << "\nKey->: " << argBStr << "\tiv: " << argCStr << std::endl; 
    

    std::cout << "\nSizeof argABytes: " << argABytes.size() << std::endl;
    std::string decMsg = DecodeEx(argABytes, argBStr, argCStr);
    std::cout << "decoded message in PySymEncDec: " << decMsg << std::endl;
    return Py_BuildValue("s#",decMsg.c_str(), decMsg.length());
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

