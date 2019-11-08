#include <Python.h>
#include <string>
#include <sstream>
#include <iostream>
#include <memory>
#include <ECPoint/ECPoint.h>
#include <BigNumbers/BigNumbers.h>

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
namespace PY_ECPOINT
{
    // make an ECPoint according to curveID
    std::unique_ptr< ECPoint > makeECPoint( int curveID )
    {
        std::unique_ptr< ECPoint > pointPtr ;
        if ( curveID == 0 ) 
        {
            // use defualt constructor
            pointPtr =  std::unique_ptr< ECPoint> ( new ECPoint( ) ) ;
        }
        else
        {
            pointPtr =  std::unique_ptr< ECPoint> ( new ECPoint( curveID ) ) ; 
        }
        return pointPtr ;
    }
}


/***********************************************************
 * ECPoint wrappers
 ************************************************************/

/* ----------------------------------------------------------------
 * wrap_add 
 */
static PyObject* wrap_add(PyObject* self, PyObject *args)
{
    char *  argA ; 
    char *  argB ; 
    int     curveID     ( 0 ) ;
    int     dec         ( 0 ) ; 
    int     compressed  ( 0 ) ; 

    if (!PyArg_ParseTuple( args, "ssiii", &argA, &argB, &curveID, &dec, &compressed ))
        return NULL;

    bool bCompress ;

    if ( compressed )   bCompress = true ;
    else                bCompress = false ;

    std::unique_ptr< ECPoint > ecpointA = PY_ECPOINT::makeECPoint( curveID ) ;
    std::unique_ptr< ECPoint > ecpointB = PY_ECPOINT::makeECPoint( curveID ) ;

    if( dec )
    {
        ecpointA.get()->FromDec(argA);
        ecpointB.get()->FromDec(argB);
    }
    else
    {
        ecpointA.get()->FromHex(argA);
        ecpointB.get()->FromHex(argB);
    }

    ECPoint ecpointC = *ecpointA.get() + *ecpointB.get() ; 

    if ( dec )  return Py_BuildValue("s", ecpointC.ToDec(bCompress).c_str()) ;
    else        return Py_BuildValue("s", ecpointC.ToHex(bCompress).c_str()) ;
}


/* ----------------------------------------------------------------
 * wrap_compare 
 */
static PyObject* wrap_compare(PyObject* self, PyObject *args)
{
    char *  argA; 
    char *  argB; 
    int     curveID( 0 ) ; 
    int     dec( 0 ) ;

    if (!PyArg_ParseTuple( args, "ssii", &argA, &argB, &curveID, &dec ))
        return NULL;

    std::unique_ptr< ECPoint > ecpointA = PY_ECPOINT::makeECPoint( curveID ) ;
    std::unique_ptr< ECPoint > ecpointB = PY_ECPOINT::makeECPoint( curveID ) ;

    if( dec )
    {
        ecpointA.get()->FromDec(argA);
        ecpointB.get()->FromDec(argB);
    }
    else
    {
        ecpointA.get()->FromHex(argA);
        ecpointB.get()->FromHex(argB);
    }

    bool isSame = *ecpointA.get() == *ecpointB.get() ; 
    return Py_BuildValue("O", isSame == true ? Py_True: Py_False);
}


/* ----------------------------------------------------------------
 * wrap_double 
 */
static PyObject* wrap_double(PyObject* self, PyObject *args)
{
    char *  argA; 
    int     curveID     ( 0 ) ; 
    int     dec         ( 0 ) ; 
    int     compressed  ( 0 ) ;     

    if (!PyArg_ParseTuple( args, "siii", &argA, &curveID, &dec, &compressed ) )
        return NULL;

    bool bCompress ;

    if ( compressed )   bCompress = true ;
    else                bCompress = false ;  

    std::unique_ptr< ECPoint > ecpointA = PY_ECPOINT::makeECPoint( curveID ) ;

    if( dec )   { ecpointA.get()->FromDec(argA) ; }
    else        { ecpointA.get()->FromHex(argA) ; }

    ECPoint ecpointC = ecpointA.get()->Double() ;

    if ( dec )  return Py_BuildValue("s", ecpointC.ToDec(bCompress).c_str()) ;
    else        return Py_BuildValue("s", ecpointC.ToHex(bCompress).c_str()) ;    
}


/* ----------------------------------------------------------------
 * wrap_invert 
 */
static PyObject* wrap_invert(PyObject* self, PyObject *args)
{
    char *  argA; 
    int     curveID     ( 0 ) ; 
    int     dec         ( 0 ) ; 
    int     compressed  ( 0 ) ;     

    if (!PyArg_ParseTuple( args, "siii", &argA, &curveID, &dec, &compressed ))
        return NULL;

    bool bCompress ;

    if ( compressed )   bCompress = true ;
    else                bCompress = false ;  

    std::unique_ptr< ECPoint > ecpointA = PY_ECPOINT::makeECPoint( curveID ) ;

    if( dec )   { ecpointA.get()->FromDec(argA) ; }
    else        { ecpointA.get()->FromHex(argA) ; }

    ecpointA.get()->Invert();
    
    if ( dec )  return Py_BuildValue("s", ecpointA.get()->ToDec(bCompress).c_str()) ;
    else        return Py_BuildValue("s", ecpointA.get()->ToHex(bCompress).c_str()) ; 
}


/* ----------------------------------------------------------------
 * wrap_checkInfinity 
 */
static PyObject* wrap_checkInfinity(PyObject* self, PyObject *args)
{
    char *  argA; 
    int     curveID ( 0 ) ; 
    int     dec     ( 0 ) ; ; 

    if (!PyArg_ParseTuple( args, "sii", &argA, &curveID, &dec ))
        return NULL;

    std::unique_ptr< ECPoint > ecpointA = PY_ECPOINT::makeECPoint( curveID ) ;

    if( dec )   { ecpointA.get()->FromDec(argA) ; }
    else        { ecpointA.get()->FromHex(argA) ; }

    return Py_BuildValue("O", ecpointA.get()->CheckInfinity()== true ? Py_True: Py_False);
}


/* ----------------------------------------------------------------
 * wrap_checkOnCurve 
 */
static PyObject* wrap_checkOnCurve(PyObject* self, PyObject *args)
{
    char *  argA; 
    int     curveID ( 0 ) ; 
    int     dec     ( 0 ) ; 

    if (!PyArg_ParseTuple(args, "sii", &argA, &curveID, &dec))
        return NULL;

    std::unique_ptr< ECPoint > ecpointA = PY_ECPOINT::makeECPoint( curveID ) ;

    if( dec )   { ecpointA.get()->FromDec(argA) ; }
    else        { ecpointA.get()->FromHex(argA) ; }

    return Py_BuildValue("O", ecpointA.get()->CheckOnCurve()== true ? Py_True: Py_False);
}


/* ----------------------------------------------------------------
 * wrap_GenerateRandomEC 
 */
static PyObject* wrap_GenerateRandomEC(PyObject* self, PyObject *args)
{
    int     curveID     ( 0 ) ;
    int     dec         ( 0 ) ; 
    int     compressed  ( 0 ) ; 


    if (!PyArg_ParseTuple(args, "iii", &curveID, &dec, &compressed))
        return NULL;

    bool bCompress ;

    if ( compressed )   bCompress = true ;
    else                bCompress = false ;        

    std::unique_ptr< ECPoint > ecpointA = PY_ECPOINT::makeECPoint( curveID ) ;

    ecpointA.get()->SetRandom();

    if ( dec )  return Py_BuildValue("s", ecpointA.get()->ToDec( bCompress ).c_str()) ;
    else        return Py_BuildValue("s", ecpointA.get()->ToHex( bCompress ).c_str()) ;
}


/* ----------------------------------------------------------------
 * wrap_GenerateEC 
 */
static PyObject* wrap_GenerateEC(PyObject* self, PyObject *args)
{
    char *  argA; 
    int     curveID     ( 0 ) ; 
    int     dec         ( 0 ) ; 
    int     compressed  ( 0 ) ; 

    if (!PyArg_ParseTuple(args, "siii", &argA, &curveID, &dec, &compressed ))
        return NULL;

    bool bCompress ;

    if ( compressed )   bCompress = true ;
    else                bCompress = false ;  

    std::unique_ptr< ECPoint > ecpointA = PY_ECPOINT::makeECPoint( curveID ) ;

    if( dec )   { ecpointA.get()->FromDec(argA) ; }
    else        { ecpointA.get()->FromHex(argA) ; }

    if ( dec )  return Py_BuildValue("s", ecpointA.get()->ToDec(bCompress).c_str()) ;
    else        return Py_BuildValue("s", ecpointA.get()->ToHex(bCompress).c_str()) ; 
}


/* ----------------------------------------------------------------
 * wrap_MultiplyScalarM 
 */
static PyObject*  wrap_MultiplyScalarM(PyObject* self, PyObject *args)
{
    char *  ecPoint ; 
    char *  bigNumM ; 
    int     curveID     ( 0 ) ; 
    int     dec         ( 0 ) ;
    int     compressed  ( 0 ) ;     

     if (!PyArg_ParseTuple(args, "ssiii", &ecPoint, &bigNumM, &curveID, &dec, &compressed))
        return NULL;

    bool bCompress ;

    if ( compressed )   bCompress = true ;
    else                bCompress = false ;  

    std::unique_ptr< ECPoint > ecpointA = PY_ECPOINT::makeECPoint( curveID ) ;

    ECPoint resECPt;

    if( dec )
    {
        ecpointA.get()->FromDec(ecPoint);
        resECPt = ecpointA->MulDec( bigNumM, std::string() ) ;
    }
    else
    {
        ecpointA.get()->FromHex(ecPoint);
        resECPt = ecpointA->MulHex( bigNumM, std::string() ) ;
    }

    if ( dec )  return Py_BuildValue("s", resECPt.ToDec(bCompress).c_str()) ;
    else        return Py_BuildValue("s", resECPt.ToHex(bCompress).c_str()) ;
}


/* ----------------------------------------------------------------
 * wrap_MultiplyScalarMN 
 */
static PyObject*  wrap_MultiplyScalarMN(PyObject* self, PyObject *args)
{
    char *  ecPoint ; 
    char *  bigNumM ; 
    char *  bigNumN ; 
    int     curveID     ( 0 ) ;  
    int     dec         ( 0 ) ;
    int     compressed  ( 0 ) ;      

    if (!PyArg_ParseTuple(args, "sssiii", &ecPoint, &bigNumM, &bigNumN, &curveID, &dec, &compressed))
        return NULL;
    
    bool bCompress ;

    if ( compressed )   bCompress = true ;
    else                bCompress = false ;  
      
    std::unique_ptr< ECPoint > ecpointA = PY_ECPOINT::makeECPoint( curveID ) ;
    
    ECPoint resECPt;

    if( dec )
    {
        ecpointA.get()->FromDec(ecPoint);
        resECPt = ecpointA->MulDec(bigNumM, bigNumN) ; 
    }
    else
    {
        ecpointA.get()->FromHex(ecPoint);
        resECPt = ecpointA->MulHex(bigNumM, bigNumN) ; 
    }

    if (dec)    return Py_BuildValue( "s", resECPt.ToDec (bCompress).c_str() ) ;
    else        return Py_BuildValue( "s", resECPt.ToHex (bCompress).c_str() ) ;
}


/* ----------------------------------------------------------------
 * wrap_GetAffineCoOrdinates 
 */
static PyObject* wrap_GetAffineCoOrdinates(PyObject* self, PyObject *args){
    
    char *  ecPoint ; 
    int     curveID ( 0 ) ; 
    int     dec     ( 0 ) ; 

     if (!PyArg_ParseTuple(args, "sii", &ecPoint, &curveID, &dec) )
        return NULL;
    
    std::unique_ptr< ECPoint > ecpointA = PY_ECPOINT::makeECPoint( curveID ) ;
    
    std::pair<std::string,std::string> coords ;
    
    if( dec )
    {
        ecpointA.get()->FromDec(ecPoint);
        coords = ecpointA->GetAffineCoords_GFp_Dec();
    }
    else
    {
        ecpointA.get()->FromHex(ecPoint);
        coords = ecpointA->GetAffineCoords_GFp();     
    }

    return  Py_BuildValue("ss",coords.first.c_str(), coords.second.c_str());
}



/* ----------------------------------------------------------------
 * wrap_GetCurveList 
 */
static PyObject* wrap_GetCurveList(PyObject* self, PyObject *args)
{
    std::vector< std::tuple< int, std::string, std::string > > curveList = getCurveList();
    std::stringstream ss;
    ss << "{" << '\n';
    bool firstIter = true;

    for(auto&e : curveList)
    {
       if (firstIter)
           firstIter = false;
       else
           ss << "," << '\n';
        ss << std::get<0>(e)<< ":\""<<std::get<1>(e) << "\"";
    }
    ss <<'\n'<< "}";
    return  Py_BuildValue("s", ss.str().c_str());
}


/* ----------------------------------------------------------------
 * wrap_GetGenerator 
 */
static PyObject* wrap_GetGenerator(PyObject* self, PyObject *args)
{
    char *  ecPoint ; 
    int     curveID     ( 0 ) ; 
    int     dec         ( 0 ) ; 
    int     compressed  ( 0 ) ;     

    if ( !PyArg_ParseTuple(args, "siii", &ecPoint, &curveID, &dec, &compressed) )
        return NULL;
    
    bool bCompress ;

    if ( compressed )   bCompress = true ;
    else                bCompress = false ;  

    std::unique_ptr< ECPoint > ecpointA = PY_ECPOINT::makeECPoint( curveID ) ;

    if( dec )   { ecpointA.get()->FromDec( ecPoint ) ; }
    else        { ecpointA.get()->FromHex( ecPoint ) ; }

    ECPoint ecPointG = ecpointA->getGenerator();

    if ( dec )  return Py_BuildValue("s", ecPointG.ToDec(bCompress).c_str()) ;
    else        return Py_BuildValue("s", ecPointG.ToHex(bCompress).c_str()) ; 
}


/* ----------------------------------------------------------------
 * wrap_GetGroupDegree 
 */
static PyObject* wrap_GetGroupDegree(PyObject* self, PyObject *args)
{
    char *  ecPoint ; 
    int     curveID ( 0 ) ; 
    int     dec     ( 0 ) ; 

    if (!PyArg_ParseTuple( args, "sii", &ecPoint, &curveID, &dec ))
        return NULL;

    std::unique_ptr< ECPoint > ecpointA = PY_ECPOINT::makeECPoint( curveID ) ;

    if( dec )   { ecpointA.get()->FromDec( ecPoint ) ; }
    else        { ecpointA.get()->FromHex( ecPoint ) ; }

    return Py_BuildValue("i", ecpointA.get()->getECGroupDegree( ) );
}


/* ----------------------------------------------------------------
 * wrap_GetGroupOrder 
 */
static PyObject* wrap_GetGroupOrder(PyObject* self, PyObject *args)
{
    char *  ecPoint ; 
    int     curveID ( 0 ) ; 
    int     dec     ( 0 ) ; 

    if (!PyArg_ParseTuple( args, "sii", &ecPoint, &curveID, &dec ))
        return NULL;

    std::unique_ptr< ECPoint > ecpointA = PY_ECPOINT::makeECPoint( curveID ) ;

    if( dec )   { ecpointA.get()->FromDec( ecPoint ) ; }
    else        { ecpointA.get()->FromHex( ecPoint ) ; }

    BigNumber bnVal = ecpointA.get()->getECGroupOrder();

    if ( dec )  return Py_BuildValue("s", bnVal.ToDec().c_str()) ;
    else        return Py_BuildValue("s", bnVal.ToHex().c_str()) ;  
}

/* ----------------------------------------------------------------
 * wrap_mulByGen
 */
static PyObject* wrap_mulByGen(PyObject* self, PyObject *args)
{
    char *  bnPoint ; 
    int     curveID     ( 0 ) ; 
    int     dec         ( 0 ) ; 
    int     compressed  ( 0 ) ;     

    if (!PyArg_ParseTuple(args, "siii", &bnPoint, &curveID, &dec, &compressed ))
        return NULL;
    
    bool bCompress ;

    if ( compressed )   bCompress = true ;
    else                bCompress = false ;   

    std::unique_ptr< ECPoint > gen = PY_ECPOINT::makeECPoint( curveID ) ;

    
    ECPoint GEN     = gen.get()->getGenerator();

    if ( dec )  
    {
        ECPoint result  = GEN.MulDec( bnPoint, std::string( ) );
        return Py_BuildValue("s", result.ToDec(bCompress).c_str()) ;
    }
    else
    {
        ECPoint result  = GEN.MulHex( bnPoint, std::string( ) );
        return Py_BuildValue("s", result.ToHex(bCompress).c_str()) ;   
    }
}



/***********************************************************
 * Python glue
 ************************************************************/
static PyMethodDef ModuleMethods[] =
{
    {"Add", wrap_add, METH_VARARGS, "Add two ECPoints, Specify CurveID [ 0 ==> secp256k1],  Hex=0 | Dec=1 "},
    {"Compare", wrap_compare, METH_VARARGS, "Compare two given ECPoints, Specify CurveID [ 0 ==> secp256k1],  Hex=0 | Dec=1 "},
    {"Double", wrap_double, METH_VARARGS, "Double the ECPoint, Specify CurveID [ 0 ==> secp256k1],  Hex=0 | Dec=1 "},
    {"Invert", wrap_invert, METH_VARARGS, "Invert the ECPoint, Specify CurveID [ 0 ==> secp256k1],  Hex=0 | Dec=1"},
    {"CheckInfinity", wrap_checkInfinity, METH_VARARGS, "Check if the given point is at infinity, Specify CurveID [ 0 ==> secp256k1],  Hex=0 | Dec=1"},
    {"CheckOnCurve", wrap_checkOnCurve, METH_VARARGS, "Check if the point is on the curve with the supplied NID default NID ==> NID_secp256k1"},
    {"GenerateRandomEC", wrap_GenerateRandomEC, METH_VARARGS ,"Generate a Random EC Point, Specify CurveID [ 0 ==> secp256k1],  Hex=0 | Dec=1"},
    {"GenerateEC", wrap_GenerateEC, METH_VARARGS ,"Generate an EC Point, Specify CurveID [ 0 ==> secp256k1],  Hex=0 | Dec=1"},
    {"MultiplyScalarM", wrap_MultiplyScalarM, METH_VARARGS, "EC Point Scalar multiply, Specify CurveID [ 0 ==> secp256k1],  Hex=0 | Dec=1"},
    {"MultiplyScalarMN", wrap_MultiplyScalarMN, METH_VARARGS, "EC Point Scalar multiply, Specify CurveID [ 0 ==> secp256k1],  Hex=0 | Dec=1"},
    {"GetAffineCoOrdinates", wrap_GetAffineCoOrdinates, METH_VARARGS, "EC Point GetAffineCoOrdinates_GFp, Specify CurveID [ 0 ==> secp256k1],  Hex=0 | Dec=1"},
    {"GetCurveList", wrap_GetCurveList, METH_NOARGS, "Get list of all curves"},
    {"GetGenerator", wrap_GetGenerator, METH_VARARGS, "EC Point Generator with supplied curve"},
    {"GetGroupDegree", wrap_GetGroupDegree, METH_VARARGS, "EC Point Group Degree with supplied curve"},
    {"GetGroupOrder", wrap_GetGroupOrder, METH_VARARGS, "EC Point Group Order with supplied curve"},
    {"MultiplyByGenerator", wrap_mulByGen,METH_VARARGS, "EC Point Group Order with supplied curve"},
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
