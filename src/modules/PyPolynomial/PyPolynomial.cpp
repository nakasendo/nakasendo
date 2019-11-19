#include <Python.h>
#include <sstream>
#include <string>
#include <iostream>
#include <Polynomial/Polynomial.h>
#include <Polynomial/LGInterpolator.h>

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


// Create Vector from python list 
std::vector< std::string > createVector( PyObject* obj )
{
    std::vector< std::string > tmpVec ;

    PyObject *iter = PyObject_GetIter( obj ) ;
    if ( !iter ) 
        return tmpVec;

    std::vector< std::string > strCoefficients ;
    int index_i ( 0 ) ;
    while ( true ) 
    {
        PyObject *next = PyIter_Next( iter ) ;
        if ( !next ) 
        {
            // nothing left in the iterator 
            break;
        }

        char * argA ;
        if ( !PyArg_Parse (next, "s", &argA ) )  
        {
            return tmpVec ;
        }
        tmpVec.push_back( argA ) ;
        
    }
    return tmpVec ;
}


// Create Vector of Pairs from python list 
std::vector<std::pair<BigNumber, BigNumber> > createVectorPairs( PyObject* obj, bool decimal )
{
    std::vector<std::pair<BigNumber, BigNumber> > tmpVec ;
    
    PyObject *iter = PyObject_GetIter( obj ) ;
    if ( !iter ) 
        return tmpVec;

    while ( true ) 
    {
        PyObject *next = PyIter_Next( iter ) ;
        if ( !next ) 
        {
            // nothing left in the iterator 
            break;
        }

        int argA ;
        char * argB ;
        if ( !PyArg_ParseTuple (next, "is", &argA, &argB ) )  
        {
            return tmpVec ;
        }
        BigNumber a, b ;
        if ( decimal )
        {
            a.FromDec( std::to_string( argA ) ) ;
            b.FromDec( argB ) ;
        }
        else
         {
            a.FromHex( std::to_string( argA ) ) ;
            b.FromHex( argB ) ;
        }           

        tmpVec.push_back( std::make_pair( a, b ) ) ;        
    }
    return tmpVec;
}

// Create python list from Polynomial object - Dec
PyObject * createList(const Polynomial& poly, bool decimal )
{
    int degree = poly.getDegree( ) ;

    int numberCoeffs    = degree + 1 ;
    Py_ssize_t len      = numberCoeffs ;

    PyObject *list = PyList_New( len ) ;
    if ( !list )
        return NULL ;

    if ( decimal )
    {
        for (auto i = 0; i < numberCoeffs; ++i  )
        {
            std::string  item = poly[i].ToDec() ;
            PyList_SET_ITEM(list, i, Py_BuildValue("s",item.c_str() ) ) ;
        }
    }
    else
    {
        for (auto i = 0; i < numberCoeffs; ++i  )
        {
            std::string  item = poly[i].ToHex() ;
            PyList_SET_ITEM(list, i, Py_BuildValue("s",item.c_str() ) ) ;
        }
    }
    return list ;     
}


/***********************************************************
 * Polynomial wrappers
 ************************************************************/

// RandomPolynomial with degree and modulo 
static PyObject* wrap_RandomPolynomial(PyObject* self, PyObject *args) 
{ 
    int degree ;
    char * argB ;
    int dec ; // decimal (1) or hexadecimal (0)

    if (!PyArg_ParseTuple(args, "isi", &degree, &argB, &dec ))
        return NULL;
    
    BigNumber modulo ;
    if ( dec )
    {    
        modulo.FromDec( argB ) ;
    }
    else
    {    
        modulo.FromHex( argB ) ;
    }
    
    
    // construct using degree and modulo 0     
    Polynomial poly = Polynomial( degree, modulo ) ;

    return createList(poly, dec) ;
}

// randomPolynomial with degree, modulo and fixed a_0term 
static PyObject* wrap_RandomPolynomialFixed_a_0(PyObject* self, PyObject *args) 
{ 
    int degree ;
    char * argB ;
    char * argC ;
    int dec ; // decimal (1) or hexadecimal (0)

    if (!PyArg_ParseTuple(args, "issi", &degree, &argB, &argC, &dec ))
        return NULL;

    BigNumber modulo, a_0 ;
    if ( dec )
    {
        modulo.FromDec( argB ) ;
        a_0.FromDec( argC ) ;
    }
    else
    {
        modulo.FromHex( argB ) ;
        a_0.FromHex( argC ) ;
    }
    

    // construct using degree, modulo and fixed a_0 term  
    Polynomial poly = Polynomial( degree, modulo, a_0 ) ;

    return createList(poly, dec) ;
}

// randomPolynomial with degree, modulo and fixed a_0term 
static PyObject* wrap_RandomPolynomialMinMax(PyObject* self, PyObject *args) 
{ 
    int degree ;
    char * argB ;
    char * argC ;
    char * argD ;
    int dec ; // decimal (1) or hexadecimal (0)    

    if (!PyArg_ParseTuple(args, "isssi", &degree, &argB, &argC , &argD, &dec))
        return NULL;

    BigNumber modulo, min, max ;
    if ( dec )
    {    
        modulo.FromDec( argB ) ;
        min.FromDec( argC ) ;
        max.FromDec( argD ) ;
    }
    else
    {    
        modulo.FromHex( argB ) ;
        min.FromHex( argC ) ;
        max.FromHex( argD ) ;
    }


    // construct using degree, modulo and fixed a_0 term  
    Polynomial poly = Polynomial( degree, modulo, min, max ) ;

    return createList(poly, dec) ;
}

static PyObject* wrap_InitFromList(PyObject* self, PyObject *args)
{
    PyObject *obj ;
    int dec ; // decimal (1) or hexadecimal (0)    


    if ( !PyArg_ParseTuple( args, "Oi", &obj, &dec ) )
        return NULL;

    std::vector< std::string > strCoefficients = createVector( obj ) ;

    // setup vector of BigNumber    
    std::vector< BigNumber >  bnCoefficients ;  
    if ( dec )
    {      
        for ( auto & element : strCoefficients )
        {
            BigNumber big ;
            big.FromDec( element ) ;
            bnCoefficients.push_back( std::move( big ) ) ;
        }
    }
    else
    {      
        for ( auto & element : strCoefficients )
        {
            BigNumber big ;
            big.FromHex( element ) ;
            bnCoefficients.push_back( std::move( big ) ) ;
        }
    }

    // create the polynomial from vector of BigNumber
    Polynomial poly = Polynomial( bnCoefficients, GenerateZero( ) ) ;
   
    return createList(poly, dec) ;
}

static PyObject* wrap_InitFromListModulo(PyObject* self, PyObject *args)
{
    PyObject *obj ;
    char * argA ;
    int dec ; // decimal (1) or hexadecimal (0)

    if ( !PyArg_ParseTuple( args, "Osi", &obj, &argA, &dec ) )
        return NULL;
    std::vector< std::string > strCoefficients = createVector( obj ) ;
    BigNumber modulo ;

    // setup vector of BigNumber    
    std::vector< BigNumber >  bnCoefficients ;  
    if ( dec )
    {
        modulo.FromDec( argA ) ;      
        for ( auto & element : strCoefficients )
        {
            BigNumber big ;
            big.FromDec( element ) ;
            bnCoefficients.push_back( std::move( big ) ) ;
        }
    }
    else
    {   
        modulo.FromHex( argA ) ;   
        for ( auto & element : strCoefficients )
        {
            BigNumber big ;
            big.FromHex( element ) ;
            bnCoefficients.push_back( std::move( big ) ) ;
        }
    }   

    // create the polynomial from vector of strings, and modulo
    Polynomial poly = Polynomial( bnCoefficients, modulo ) ;
    
    return createList(poly, dec) ;
}


// Evaluate the polynomial for x value 
static PyObject* wrap_Evaluate(PyObject* self, PyObject *args) 
{ 
    PyObject *obj ;
    char * argA ;
    char * argB ;
    int dec ;

    if ( !PyArg_ParseTuple( args, "Ossi", &obj, &argA, &argB, &dec ) )
        return NULL;

    std::vector< std::string > strCoefficients = createVector( obj ) ;
 
    BigNumber fx, modulo, eval ;

    // create the polynomial from vector of BigNumber
    std::vector< BigNumber >  bnCoefficients ;   
    if ( dec )
    {
        fx.FromDec      ( argA ) ;
        modulo.FromDec  ( argB ) ;  
        for ( auto & element : strCoefficients )
        {
            BigNumber big ;
            big.FromDec( element ) ;
            bnCoefficients.push_back( std::move( big ) ) ;
        }
    }
    else
    {   
        fx.FromHex      ( argA ) ;
        modulo.FromHex  ( argB ) ; 
        for ( auto & element : strCoefficients )
        {
            BigNumber big ;
            big.FromHex( element ) ;
            bnCoefficients.push_back( std::move( big ) ) ;
        }
    }   
   

   // create the polynomial and evaluate for x
    Polynomial poly = Polynomial( bnCoefficients, modulo ) ;

    eval = poly( fx ) ;

    if ( dec )
        return Py_BuildValue( "s", eval.ToDec( ).c_str() ) ;
    else      
        return Py_BuildValue( "s", eval.ToHex( ).c_str() ) ;
}

/***********************************************************
 *  Interpolation wrappers
 ************************************************************/

// LGInterpolator, evaluate the ith basis polynomial at xValue
static PyObject* wrap_LGInterpolatorSingle(PyObject* self, PyObject *args) 
{ 
    PyObject *obj ;
    char * argA ;
    char * argB ;
    char * basisPoint ;
    int dec ;

    if ( !PyArg_ParseTuple( args, "Osssi", &obj, &argA, &argB, &basisPoint, &dec ) )
        return NULL;

    BigNumber xValue, modulo  ;

    if ( dec )
    {
        modulo.FromDec      ( argA ) ;
        xValue.FromDec      ( argB ) ;
    }
    else
   {
        modulo.FromHex      ( argA ) ;
        xValue.FromHex      ( argB ) ;
   } 

    std::vector<std::pair<BigNumber, BigNumber> > xfx = createVectorPairs( obj, dec ) ;

    LGInterpolator lgInterpolator ( xfx, modulo ) ;
    BigNumber val = lgInterpolator( std::stoi(basisPoint), xValue ) ;


    if ( dec )
        return Py_BuildValue( "s", val.ToDec( ).c_str() ) ;  
    else
        return Py_BuildValue( "s", val.ToHex( ).c_str() ) ;  
}

// LGInterpolator with degree and modulo 
static PyObject* wrap_LGInterpolatorFull(PyObject* self, PyObject *args) 
{ 
    PyObject *obj ;
    char * argA ;
    char * argB ;
    int dec ;


    if ( !PyArg_ParseTuple( args, "Ossi", &obj, &argA, &argB, &dec ) )
        return NULL;

    BigNumber xValue, modulo ;
    if (dec )
    {
        modulo.FromDec      ( argA ) ;
        xValue.FromDec      ( argB ) ;
    }
    else
   {
        modulo.FromHex      ( argA ) ;
        xValue.FromHex      ( argB ) ;
   }     

    std::vector<std::pair<BigNumber, BigNumber> > xfx = createVectorPairs( obj, dec ) ;

    LGInterpolator lgInterpolator ( xfx, modulo ) ;
    BigNumber val = lgInterpolator( xValue ) ;

    if ( dec )
        return Py_BuildValue( "s", val.ToDec( ).c_str() ) ;  
    else
        return Py_BuildValue( "s", val.ToHex( ).c_str() ) ;  
}

//
static PyObject* wrap_LGECInterpolatorFull(PyObject* self, PyObject *args) 
{ 
    PyObject *obj ;
    char * argA ;
    char * argB ;
    int dec(0);

    if ( !PyArg_ParseTuple( args, "Ossi", &obj, &argA, &argB, &dec ) )
        return NULL;

    BigNumber xValue, modulo ;
    if(dec){
        modulo.FromDec( argA ) ;
        xValue.FromDec( argB ) ;
    }else{
        modulo.FromHex( argA ) ;
        xValue.FromHex( argB ) ;
    }
//////////
    std::vector<std::pair<BigNumber, ECPoint> > tmpVec ;
    
    PyObject *iter = PyObject_GetIter(obj) ;

    while (true) 
    {
        PyObject *next = PyIter_Next( iter ) ;
        if (!next){
            // nothing left in the iterator 
            break;
        }

        char * argA ;
        char * argB ;
        char * argC ;
        if (!PyArg_ParseTuple (next, "sss", &argA, &argB,&argC )){
            break;
        }
        BigNumber a ;
        BigNumber y1; 
        BigNumber y2; 
        if(dec){
            a.FromDec(argA);
            y1.FromDec(argB);
            y2.FromDec(argC);
        }else{
            a.FromHex( argA ) ;
            y1.FromHex(argB);
            y2.FromHex(argC);
        }
        ECPoint b ( y1,y2 ) ;

        tmpVec.push_back( std::make_pair( a, b ) ) ;        
    }

/////////

    LGECInterpolator lgInterpolator (tmpVec, modulo) ;
    ECPoint val = lgInterpolator( xValue ) ;

    if(dec)
        return Py_BuildValue("s", val.ToDec().c_str());
    else
        return Py_BuildValue( "s", val.ToHex( ).c_str() ) ;  
}


/***********************************************************
 * Python glue
 ************************************************************/

static PyMethodDef ModuleMethods[] =
{
    {"randomPolynomial",wrap_RandomPolynomial,METH_VARARGS,"create a random polynomial"},    
    {"randomPolynomialFixed_a_0",wrap_RandomPolynomialFixed_a_0,METH_VARARGS,  
        "create a random polynomial with fixed a_0"},    
    {"randomPolynomialMinMax",wrap_RandomPolynomialMinMax,METH_VARARGS,  
        "create a random polynomial with range of min..max"},      
    {"evaluate",wrap_Evaluate,METH_VARARGS,"evaluate polynomial for x"},     
    {"initFromList",wrap_InitFromList, METH_VARARGS,"create a Polynomial from a list"}, 
    {"initFromListModulo",wrap_InitFromListModulo, METH_VARARGS,
        "create a Polynomial from a list with modulo"}, 
    {"LGInterpolatorSingle",wrap_LGInterpolatorSingle, METH_VARARGS,
        "Lagrange Interpolator full evaluation"}, 
    {"LGInterpolatorFull",wrap_LGInterpolatorFull, METH_VARARGS,
        "Lagrange Interpolator for a single point"},         
    {"LGECInterpolatorFull",wrap_LGECInterpolatorFull, METH_VARARGS,
        "Lagrange Interpolator for a single point"}, 
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

