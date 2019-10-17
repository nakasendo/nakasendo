#include <stdlib.h>
#include <string.h>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include <MessageHash/Base58EncDecImpl.h>
#include <MessageHash/MessageHash.h>

// Encode our raw data as a Base58 string.
// Functionality lifted from Bitcoin-SV.

namespace Base58{
    char b58table[] = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
};

Base58EncDecImpl::Base58EncDecImpl(){
    return ; 
}

std::string Base58EncDecImpl::encode (const messageVec& vch){
    return (EncodeBase58(&vch[0], &vch[0] + vch.size()));
}
std::string Base58EncDecImpl::encodeCheck (const messageVec& vchIn){
    // add 4-byte hash check to the end
    std::vector<uint8_t> vch(vchIn);
    std::string hashInput;
    for (messageVec::const_iterator iter = vch.begin();iter != vch.end(); ++iter){
        hashInput.push_back(*iter);
    }
    // hash-twice for SV
    std::string hashVal = HashMsgSHA256( HashMsgSHA256( hashInput ) ) ;
    
    // add the the first 4 bytes.
    for (int i=0; i<4; ++i)
    { 
        vch.push_back(hashVal[i]); 
    }
    
    return encode(vch);
}

std::string Base58EncDecImpl::EncodeBase58(const uint8_t* pbegin, const uint8_t* pend)
{
    // Skip & count leading zeroes.
    int zeroes = 0;
    int length = 0;
    while (pbegin != pend && *pbegin == 0) {
        pbegin++;
        zeroes++;
    }
    // Allocate enough space in big-endian base58 representation.
    // log(256) / log(58), rounded up.
    int size = (pend - pbegin) * 138 / 100 + 1;
    std::vector<uint8_t> b58(size);
    // Process the bytes.
    while (pbegin != pend) {
        int carry = *pbegin;
        int i = 0;
        // Apply "b58 = b58 * 256 + ch".
        for (std::vector<uint8_t>::reverse_iterator it = b58.rbegin();
             (carry != 0 || i < length) && (it != b58.rend()); it++, i++) {
            carry += 256 * (*it);
            *it = carry % 58;
            carry /= 58;
        }

        assert(carry == 0);
        length = i;
        pbegin++;
    }
    // Skip leading zeroes in base58 result.
    std::vector<uint8_t>::iterator it = b58.begin() + (size - length);
    while (it != b58.end() && *it == 0) {
        it++;
    }
    std::string str;
    str.reserve(zeroes + (b58.end() - it));
    str.assign(zeroes, '1');
    while (it != b58.end()) {
        //str += pszBase58[*(it++)];
        str += Base58::b58table[*(it++)];
    }
    return str;
}

messageVec Base58EncDecImpl::DecodeBase58(const char *psz){
   // Skip leading spaces.
   messageVec vch; 
    while (*psz && isspace(*psz)) {
        psz++;
    }
    // Skip and count leading '1's.
    int zeroes = 0;
    int length = 0;
    while (*psz == '1') {
        zeroes++;
        psz++;
    }
    // Allocate enough space in big-endian base256 representation.
    // log(58) / log(256), rounded up.
    int size = strlen(psz) * 733 / 1000 + 1;
    std::vector<uint8_t> b256(size);
    // Process the characters.
    while (*psz && !isspace(*psz)) {
        // Decode base58 character
        const char *ch = strchr(&Base58::b58table[0], *psz);
        if (ch == nullptr) {
            return messageVec();
        }
        // Apply "b256 = b256 * 58 + ch".
        int carry = ch - Base58::b58table;
        int i = 0;
        for (std::vector<uint8_t>::reverse_iterator it = b256.rbegin();
             (carry != 0 || i < length) && (it != b256.rend()); ++it, ++i) {
            carry += 58 * (*it);
            *it = carry % 256;
            carry /= 256;
        }
        assert(carry == 0);
        length = i;
        psz++;
    }
    // Skip trailing spaces.
    while (isspace(*psz)) {
        psz++;
    }
    if (*psz != 0) {
        return messageVec();
    }
    // Skip leading zeroes in b256.
    std::vector<uint8_t>::iterator it = b256.begin() + (size - length);
    while (it != b256.end() && *it == 0)
        it++;
    // Copy result into output vector.
    vch.reserve(zeroes + (b256.end() - it));
    vch.assign(zeroes, 0x00);
    while (it != b256.end()) {
        vch.push_back(*(it++));
    }
    return vch;
}

messageVec Base58EncDecImpl::decode (const std::string& msg){
    return DecodeBase58(msg.c_str());
}
messageVec Base58EncDecImpl::decodeCheck(const std::string& msg){
    messageVec retVal = DecodeBase58(msg.c_str());
    // Hash the msg twice (minus the last 4 bytes)
    // hash-twice for SV
    std::string hashInput;
    for(messageVec::const_iterator iter = retVal.begin(); iter!=retVal.end()-4; ++ iter){
        hashInput.push_back(*iter);
    }

    std::string hashVal = HashMsgSHA256( HashMsgSHA256( hashInput ) ) ;

    messageVec::iterator iter = retVal.end() - 4;     
    for (int i = 0 ; i<4; ++i ){
        if (*iter != hashVal[i] ){
            std::cout << "Values not equal" << std::endl; 
        }
        ++ iter ; 
    }
    retVal.resize(retVal.size() - 4);
    return retVal; 
}
