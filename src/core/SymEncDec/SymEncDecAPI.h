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

SYMENCDEC_C_API SYMENCDEC_RETURN_TYPE Encode (const std::string&, const std::string&,const std::string&);
SYMENCDEC_C_API SYMENCDEC_RETURN_TYPE Decode (const std::string&, const std::string&, const std::string& );

SYMENCDEC_C_API std::vector<uint8_t> EncodeEx (const std::string&, const std::string&,const std::string&);
SYMENCDEC_C_API std::string DecodeEx (const std::vector<uint8_t>&, const std::string&,const std::string&);

SYMENCDEC_C_API SYMENCDEC_RETURN_TYPE GenerateKey256(const std::string&, const std::string&);



#endif /* SYM_ENC_DEC_API_H */
