#include <openssl/crypto.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>

#include "CoreModuleFile1.hpp"

struct hex_upper
{
    static constexpr char hexdig[] = "0123456789ABCDEF";
};
struct hex_lower
{
    static constexpr char hexdig[] = "0123456789abcdef";
};

template<typename THEX>
std::string test_help_buf2hexstr(unsigned char* buff, size_t len)
{
    std::string hexstr(2*len, ' ');

    for (size_t i = 0; i < len; ++i)
    {
        hexstr[2*i]   = THEX::hexdig[(buff[i] & 0xF0) >> 4];
        hexstr[2*i+1] = THEX::hexdig[(buff[i] & 0x0F) >> 0];
    }
    return std::move(hexstr);
}

template<typename THEX = hex_upper>
std::string helper_sha256_hex(const std::string& crIn)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(crIn.c_str()), crIn.size(), hash);
    const std::string output= test_help_buf2hexstr<THEX>(hash, SHA256_DIGEST_LENGTH);
    return std::move(output);
}

std::string hash_sha256_1(const std::string& crIn)
{
    return helper_sha256_hex(crIn);
}
