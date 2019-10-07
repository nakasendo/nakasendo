#ifndef POLYNOMIAL_CONFIG_H
#define POLYNOMIAL_CONFIG_H

#include <DYNAMIC_LIBRARY_API.hpp>

#ifdef EXPORT_Polynomial
#    define Polynomial_API EXPORT_DYNAMIC_LIBRARY
#else
#    define Polynomial_API IMPORT_DYNAMIC_LIBRARY
#endif

#endif /* POLYNOMIAL_CONFIG_H */