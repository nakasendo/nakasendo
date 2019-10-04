#ifndef SECRETSPLIT_CONFIG_H
#define SECRETSPLIT_CONFIG_H

#include <DYNAMIC_LIBRARY_API.hpp>

#ifdef EXPORT_SecretSplit
#    define SecretSplit_API EXPORT_DYNAMIC_LIBRARY
#else
#    define SecretSplit_API IMPORT_DYNAMIC_LIBRARY
#endif

#endif /* SECRETSPLIT_CONFIG_H */