#ifndef __BIGNUM_API__
#define __BIGNUM_API__

#include <memory>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif


extern "C" {

#ifdef __EMSCRIPTEN__    
    EMSCRIPTEN_KEEPALIVE const char * addFromHex(char*, char*);
    EMSCRIPTEN_KEEPALIVE const char * addFromDec(char *, char*);
    EMSCRIPTEN_KEEPALIVE const char * subFromHex (char*, char*);
    EMSCRIPTEN_KEEPALIVE const char * subFromDec(char*, char*);
    EMSCRIPTEN_KEEPALIVE const char * BNRandomHex(const int size);
    EMSCRIPTEN_KEEPALIVE const char * BNRandomDec(const int size);
    EMSCRIPTEN_KEEPALIVE const char *  BNRandomHexWithSeed(const char *, const int size );
#else
    std::unique_ptr<char> addFromHex(char *, char *);
    std::unique_ptr<char> addFromDec(char *, char*);
    std::unique_ptr<char> subFromHex (char*, char*);
    std::unique_ptr<char> subFromDec(char*, char*);
    std::unique_ptr<char>  BNRandomHex (const int size) ;  
    std::unique_ptr<char>  BNRandomDec (const int size) ;
    std::unique_ptr<char>  BNRandomHexWithSeed(const char *, const int size );
    std::unique_ptr<char>  BNRandomDecWithSeed(const char *, const int size );
#endif //ifdef __EMSCRIPTEN__
};


#endif //ifndef __BIGNUM_API__
