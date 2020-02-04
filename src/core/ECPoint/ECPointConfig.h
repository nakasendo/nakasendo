#ifndef ECPOINT_CONFIG_H
#define ECPOINT_CONFIG_H

#include <DYNAMIC_LIBRARY_API.hpp>

#ifdef EXPORT_ECPoint
#    define ECPoint_API EXPORT_DYNAMIC_LIBRARY
#else
#    define ECPoint_API IMPORT_DYNAMIC_LIBRARY
#endif

#endif /* ECPOINT_CONFIG_H */