#ifndef BIGNUMBERS_CONFIG_H
#define BIGNUMBERS_CONFIG_H

#include <DYNAMIC_LIBRARY_API.hpp>

#ifdef EXPORT_BigNumbers
#    define BigNumbers_API EXPORT_DYNAMIC_LIBRARY
#else
#    define BigNumbers_API IMPORT_DYNAMIC_LIBRARY
#endif

#endif /* BIGNUMBERS_CONFIG_H */