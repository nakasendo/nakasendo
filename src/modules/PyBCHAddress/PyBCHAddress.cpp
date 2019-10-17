#include <Python.h>
#include <sstream>
#include <string>
#include <iostream>
#include <BCHAddress/BCHAddress.h>
#include <BCHAddress/BCHAddressInfo.h>


struct module_state {
    PyObject *error;
};

#if PY_MAJOR_VERSION >= 3
#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))
#else
#define GETSTATE(m) (&_state)
static struct module_state _state;
#endif

/***********************************************************
 * Helper functions
 ************************************************************/



/***********************************************************
 * BCHAddress wrappers
 ************************************************************/

// Create a BCHAddress from public key and network type 
static PyObject* wrap_BCHAddress(PyObject* self, PyObject *args) 
{ 
    char * key ;
    int version ;

    if (!PyArg_ParseTuple(args, "si", &key, &version ))
        return NULL;
    
    VersionPrefix versionPrefix { version } ; 


    // construct using key and version prefix    
    BCHAddress address = BCHAddress( key, versionPrefix ) ;

    Py_ssize_t len      = 5 ;

    PyObject *list = PyList_New( len ) ;
    if ( !list )
        return NULL ;


    int valid ;
    if ( address.valid() ) { valid = 1 ; } else { valid = 0 ; } 
    std::pair< std::string, std::string > versionPair = address.getVersion( ) ;
    PyList_SET_ITEM(list, 0, Py_BuildValue("s", address.getAddress().c_str() ) ) ;
    PyList_SET_ITEM(list, 1, Py_BuildValue("i", valid) ) ;
    PyList_SET_ITEM(list, 2, Py_BuildValue("s", address.decode().c_str() ) ) ;
    PyList_SET_ITEM(list, 3, Py_BuildValue("s", versionPair.first.c_str() ) ) ;
    PyList_SET_ITEM(list, 4, Py_BuildValue("s", versionPair.second.c_str() ) ) ;

    return list ;

}

// Create a BCHAddress from public key and network type 
static PyObject* wrap_importBCHAddress(PyObject* self, PyObject *args) 
{ 
    char * addressStr ;

    if (!PyArg_ParseTuple(args, "s", &addressStr ))
        return NULL;


    // construct using the address string
    try
    {     
        BCHAddress address = BCHAddress( addressStr ) ;

        Py_ssize_t len = 5 ;

        PyObject *list = PyList_New( len ) ;
        if ( !list )
            return NULL ;
        
        int valid ;

        if ( address.valid() ) { valid = 1 ; } else { valid = 0 ; } 

        std::pair< std::string, std::string > versionPair = address.getVersion( ) ;
        PyList_SET_ITEM(list, 0, Py_BuildValue("s", address.getAddress().c_str() ) ) ;
        PyList_SET_ITEM(list, 1, Py_BuildValue("i", valid) ) ;
        PyList_SET_ITEM(list, 2, Py_BuildValue("s", address.decode().c_str() ) ) ;
        PyList_SET_ITEM(list, 3, Py_BuildValue("s", versionPair.first.c_str() ) ) ;
        PyList_SET_ITEM(list, 4, Py_BuildValue("s", versionPair.second.c_str() ) ) ;

        return list ;
    }
    catch (...)
    {
        Py_RETURN_NONE;
    }
}

// Create a BCHAddress from public key and network type 
static PyObject* wrap_BCHAddress_outputOperator(PyObject* self, PyObject *args) 
{ 
    char * argA ;

    if (!PyArg_ParseTuple(args, "s", &argA ))
        return NULL;

    std::string addressStr( argA ) ;
    BCHAddress address = BCHAddress( addressStr ) ;

    
    std::stringstream ss ;
    ss << address << std::endl ;
    auto x = ss.str() ;

    return Py_BuildValue( "s", x.c_str() ) ;
}

/***********************************************************
 * Python glue
 ************************************************************/

static PyMethodDef ModuleMethods[] =
{
    {"createAddress",wrap_BCHAddress,METH_VARARGS,"create a BCH Address from public key"},    
    {"importAddress",wrap_importBCHAddress,METH_VARARGS,"create a BCH Address a string representaiton of an existing address"},    
    {"print",wrap_BCHAddress_outputOperator,METH_VARARGS,"get the printable output from BCHAddress"},    
    {NULL, NULL, 0, NULL},
};
 
#if PY_MAJOR_VERSION >= 3

static int PyBCHAddress_traverse(PyObject *m, visitproc visit, void *arg) {
    Py_VISIT(GETSTATE(m)->error);
    return 0;
}

static int PyBCHAddress_clear(PyObject *m) {
    Py_CLEAR(GETSTATE(m)->error);
    return 0;
}


static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "PyBCHAddress",
        NULL,
        sizeof(struct module_state),
        ModuleMethods,
        NULL,
        PyBCHAddress_traverse,
        PyBCHAddress_clear,
        NULL
};

#define INITERROR return NULL

PyMODINIT_FUNC
PyInit_PyBCHAddress(void)

#else
#define INITERROR return

void
initmyextension(void)
#endif
{
#if PY_MAJOR_VERSION >= 3
    PyObject *module = PyModule_Create(&moduledef);
#else
    PyObject *module = Py_InitModule("PyBCHAddress", myextension_methods);
#endif

    if (module == NULL)
        INITERROR;
    struct module_state *st = GETSTATE(module);

    st->error = PyErr_NewException("PyBCHAddress.Error", NULL, NULL);
    if (st->error == NULL) {
        Py_DECREF(module);
        INITERROR;
    }

#if PY_MAJOR_VERSION >= 3
    return module;
#endif
}

