#ifndef SYMENCDEC_CONFIG_H
#define SYMENCDEC_CONFIG_H

#include <DYNAMIC_LIBRARY_API.hpp>

#ifdef EXPORT_SymEncDec
#    define SymEncDec_API EXPORT_DYNAMIC_LIBRARY
#else
#    define SymEncDec_API IMPORT_DYNAMIC_LIBRARY
#endif

#endif /* SYMENCDEC_CONFIG_H */