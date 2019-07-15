#ifndef SYM_ENC_DEC_API_H
#define SYM_ENC_DEC_API_H

#include <SymEncDec/SymEncDec.h>

#include <string>
#include <vector>
#include <cstdint>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define SYMENCDEC_RETURN_TYPE EMSCRIPTEN_KEEPALIVE const char *
#define SYMENCDEC_C_API extern "C" SymEncDec_API
#else
#define SYMENCDEC_RETURN_TYPE std::string
#define SYMENCDEC_C_API SymEncDec_API
#endif

SYMENCDEC_C_API SYMENCDEC_RETURN_TYPE GenKeyAndEncode (const std::string&, const std::string&, std::string&,uint64_t keylen=32, uint64_t blocksize=16);
SYMENCDEC_C_API SYMENCDEC_RETURN_TYPE GenKeyAndDecode (const std::string&, const std::string&, const std::string&,uint64_t keylen=32, uint64_t blocksize=16);


SYMENCDEC_C_API SYMENCDEC_RETURN_TYPE Encode (const std::string&, const std::string&,const std::string&,uint64_t keylen=32, uint64_t blocksize=16);
SYMENCDEC_C_API SYMENCDEC_RETURN_TYPE Decode (const std::string&, const std::string&, const std::string&,uint64_t keylen=32, uint64_t blocksize=16);

SYMENCDEC_C_API SYMENCDEC_RETURN_TYPE GenerateNounce(const int blocksize=16);
SYMENCDEC_C_API SYMENCDEC_RETURN_TYPE GenerateKey256(const std::string&, const std::string&,  uint64_t keylen = 32, uint64_t blocksize= 16);



#endif /* SYM_ENC_DEC_API_H */
