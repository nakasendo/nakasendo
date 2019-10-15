#ifndef MESSAGE_HASH_CONFIG_H
#define MESSAGE_HASH_CONFIG_H

#include <DYNAMIC_LIBRARY_API.hpp>

#ifdef EXPORT_MessageHash
#    define MessageHash_API EXPORT_DYNAMIC_LIBRARY
#else
#    define MessageHash_API IMPORT_DYNAMIC_LIBRARY
#endif

#endif /* MESSAGE_HASH_CONFIG_H */