#ifndef __POLYNOMIAL_API_H__
#define __POLYNOMIAL_API_H__

#ifdef EXPORT_Polynomial
#    define Polynomial_API EXPORT_DYNAMIC_LIBRARY
#else
#    define Polynomial_API IMPORT_DYNAMIC_LIBRARY
#endif

#endif //#ifndef __POLYNOMIAL_API_H__