#include <Python.h>
#include <sstream>
#include <string>
#include <iostream>
#include <Polynomial/Polynomial.h>

struct module_state {
    PyObject *error;
};

#if PY_MAJOR_VERSION >= 3
#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))
#else
#define GETSTATE(m) (&_state)
static struct module_state _state;
#endif

static PyObject* wrap_getDegree(PyObject* self, PyObject *args){

    int dummySize ( 99 ) ; 

    return Py_BuildValue( "i", dummySize ) ;
}


// RandomPolynomial with degree and modulo 
static PyObject* wrap_RandomPolynomial(PyObject* self, PyObject *args) 
{ 

    int degree ;
    char * argB ;

    if (!PyArg_ParseTuple(args, "is", &degree, &argB ))
        return NULL;

    // constuct a polynomial
    BigNumber modulo ;
    modulo.FromDec( argB ) ;


    // construct using degree and modulo 0     
    Polynomial poly = Polynomial( degree, modulo ) ;

    // <TODO> remove this (here for debugging)
    std::cout << "poly = " << poly << std::endl; 

    int numberCoeffs    = degree + 1 ;
    Py_ssize_t len      = numberCoeffs ;

    PyObject *list = PyList_New( len ) ;
    if ( !list )
        return NULL ;

    for (auto i = 0; i < numberCoeffs; ++i  )
    {
        std::string  item = poly[i].ToDec() ;
        PyList_SET_ITEM(list, i, Py_BuildValue("s",item.c_str() ) ) ;
    }
    return list ;
}

// randomPolynomial with degree, modulo and fixed a_0term 
static PyObject* wrap_RandomPolynomialFixed_a_0(PyObject* self, PyObject *args) 
{ 

    int degree ;
    char * argB ;
    char * argC ;

    if (!PyArg_ParseTuple(args, "iss", &degree, &argB, &argC ))
        return NULL;

    // constuct a polynomial
    BigNumber modulo, a_0 ;
    modulo.FromDec( argB ) ;
    a_0.FromDec( argC ) ;


    // construct using degree, modulo and fixed a_0 term  
    Polynomial poly = Polynomial( degree, modulo, a_0 ) ;

    // <TODO> remove this (here for debugging)
    std::cout << "poly = " << poly << std::endl; 

    int numberCoeffs    = degree + 1 ;
    Py_ssize_t len      = numberCoeffs ;

    PyObject *list = PyList_New( len ) ;
    if ( !list )
        return NULL ;

    for (auto i = 0; i < numberCoeffs; ++i  )
    {
        std::string  item = poly[i].ToDec() ;
        PyList_SET_ITEM(list, i, Py_BuildValue("s",item.c_str() ) ) ;
    }
    return list ;
}

// randomPolynomial with degree, modulo and fixed a_0term 
static PyObject* wrap_RandomPolynomialMinMax(PyObject* self, PyObject *args) 
{ 

    int degree ;
    char * argB ;
    char * argC ;
    char * argD ;

    if (!PyArg_ParseTuple(args, "isss", &degree, &argB, &argC , &argD))
        return NULL;

    // constuct a polynomial
    BigNumber modulo, min, max ;
    modulo.FromDec( argB ) ;
    min.FromDec( argC ) ;
    max.FromDec( argD ) ;


    // construct using degree, modulo and fixed a_0 term  
    Polynomial poly = Polynomial( degree, modulo, min, max ) ;

    // <TODO> remove this (here for debugging)
    std::cout << "poly = " << poly << std::endl; 

    int numberCoeffs    = degree + 1 ;
    Py_ssize_t len      = numberCoeffs ;

    PyObject *list = PyList_New( len ) ;
    if ( !list )
        return NULL ;

    for (auto i = 0; i < numberCoeffs; ++i  )
    {
        std::string  item = poly[i].ToDec() ;
        PyList_SET_ITEM(list, i, Py_BuildValue("s",item.c_str() ) ) ;
    }
    return list ;
}


static PyMethodDef ModuleMethods[] =
{
    {"getDegree",wrap_getDegree,METH_VARARGS,"degree of polynomial"},    
    {"randomPolynomial",wrap_RandomPolynomial,METH_VARARGS,"create a random polynomial"},    
    {"randomPolynomialFixed_a_0",wrap_RandomPolynomialFixed_a_0,METH_VARARGS,  
        "create a random polynomial with fixed a_0"},    
    {"randomPolynomialMinMax",wrap_RandomPolynomialMinMax,METH_VARARGS,  
        "create a random polynomial with range of min..max"},         
    {NULL, NULL, 0, NULL},
};
 
#if PY_MAJOR_VERSION >= 3

static int PyPolynomial_traverse(PyObject *m, visitproc visit, void *arg) {
    Py_VISIT(GETSTATE(m)->error);
    return 0;
}

static int PyPolynomial_clear(PyObject *m) {
    Py_CLEAR(GETSTATE(m)->error);
    return 0;
}


static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "PyPolynomial",
        NULL,
        sizeof(struct module_state),
        ModuleMethods,
        NULL,
        PyPolynomial_traverse,
        PyPolynomial_clear,
        NULL
};

#define INITERROR return NULL

PyMODINIT_FUNC
PyInit_PyPolynomial(void)

#else
#define INITERROR return

void
initmyextension(void)
#endif
{
#if PY_MAJOR_VERSION >= 3
    PyObject *module = PyModule_Create(&moduledef);
#else
    PyObject *module = Py_InitModule("PyPolynomial", myextension_methods);
#endif

    if (module == NULL)
        INITERROR;
    struct module_state *st = GETSTATE(module);

    st->error = PyErr_NewException("PyPolynomial.Error", NULL, NULL);
    if (st->error == NULL) {
        Py_DECREF(module);
        INITERROR;
    }

#if PY_MAJOR_VERSION >= 3
    return module;
#endif
}

