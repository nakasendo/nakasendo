#ifndef __EXPORT_TSCORE_H__
#define __EXPORT_TSCORE_H__


#include <DYNAMIC_LIBRARY_API.hpp>

#ifdef EXPORT_TSCore
#    define TSCore_API EXPORT_DYNAMIC_LIBRARY
#else
#    define TSCore_API IMPORT_DYNAMIC_LIBRARY
#endif

#endif // ifndef __EXPORT_TSCORE_H__
