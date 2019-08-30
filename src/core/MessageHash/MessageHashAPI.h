#ifndef __MESSAGE_HASH_API__
#define __MESSAGE_HASH_API__

#include <MessageHash/MessageHash.h>
#include <vector>
#include <string>
#include <cstdint>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/bind.h>
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> EncodeBase64Ex (const std::vector<uint8_t>&);
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> DecodeBase64Ex (std::vector<uint8_t>);
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> EncodeBase58Ex (std::vector<uint8_t>);
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> DecodeBase58Ex (std::vector<uint8_t>);
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> EncodeBase58CheckEx (std::vector<uint8_t>);
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> DecodeBase58CheckEx (std::vector<uint8_t>);

#define MESSAGE_HASH_RETURN_TYPE EMSCRIPTEN_KEEPALIVE const char *
#define MessageHash_C_API MessageHash_API extern "C"
#else
#define MESSAGE_HASH_RETURN_TYPE std::string
#define MessageHash_C_API MessageHash_API
#endif

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE std::string
#else
MessageHash_C_API MESSAGE_HASH_RETURN_TYPE
#endif 
 HashMsgSHA256 (const std::string&);

MessageHash_C_API MESSAGE_HASH_RETURN_TYPE HashMsgSHA256Test (char * );

MessageHash_C_API MESSAGE_HASH_RETURN_TYPE HashMsg (const std::string&, const std::string&);

MessageHash_API std::unique_ptr<unsigned char> HashSha256 ( const std::string& );

#ifndef __EMSCRIPTEN__
MessageHash_C_API MESSAGE_HASH_RETURN_TYPE ListHashFunc();
#endif


MessageHash_C_API MESSAGE_HASH_RETURN_TYPE EncodeBase64(const std::unique_ptr<unsigned char[]>&, int size);


MessageHash_C_API std::unique_ptr<unsigned char[]> DecodeBase64(const std::string&, int& size);


//MessageHash_C_API std::vector<uint8_t> EncodeBase64Ex (const std::vector<uint8_t>&);
//MessageHash_C_API std::vector<uint8_t> DecodeBase64Ex (std::vector<uint8_t>);

MessageHash_C_API MESSAGE_HASH_RETURN_TYPE EncodeBase58(const std::string&);

MessageHash_C_API MESSAGE_HASH_RETURN_TYPE DecodeBase58(const std::string&);


MessageHash_C_API MESSAGE_HASH_RETURN_TYPE EncodeBase58Checked(const std::string&);

MessageHash_C_API MESSAGE_HASH_RETURN_TYPE DecodeBase58Checked(const std::string&);



#endif //ifndef __MESSAGE_HASH_API__

