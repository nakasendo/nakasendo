#ifndef DYNAMIC_LIBRARY_API_H
#define DYNAMIC_LIBRARY_API_H

//// Do not modify this file
//// This file is generated from DYNAMIC_LIBRARY_API.hpp.in
////

#if !defined WIN32  || defined __EMSCRIPTEN__ || defined BUILD_ALL_STATIC_LIBS
#  define EXPORT_DYNAMIC_LIBRARY
#  define IMPORT_DYNAMIC_LIBRARY
#else
#  define EXPORT_DYNAMIC_LIBRARY __declspec( dllexport )
#  define IMPORT_DYNAMIC_LIBRARY __declspec( dllimport )
#  ifdef _MSC_VER
#    pragma warning(disable : 4231)
#    pragma warning(disable : 4910)
#  endif
#endif

#endif /// DYNAMIC_LIBRARY_API_H

