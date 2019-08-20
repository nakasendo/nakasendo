#ifndef __SECRET_SPLIT_API_H__
#define __SECRET_SPLIT_API_H__

#ifdef EXPORT_SecretSplit
#    define SecretSplit_API EXPORT_DYNAMIC_LIBRARY
#else
#    define SecretSplit_API IMPORT_DYNAMIC_LIBRARY
#endif

#endif //#ifndef __SECRET_SPLIT_API_H__