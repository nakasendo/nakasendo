#ifndef __CONVERSIONS_H__
#define __CONVERSIONS_H__

#include <MessageHash/MessageHashConfig.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory>
#include <vector>


MessageHash_API std::unique_ptr<unsigned char[]> HexToBin(const std::unique_ptr<char[]>& input, size_t *l);

MessageHash_API std::unique_ptr<unsigned char[]> HexStrToBin(const std::string& input, size_t *l);
MessageHash_API std::string binTohexStr(const std::unique_ptr<unsigned char[]>& data, int len);

MessageHash_API std::vector<uint8_t> HexToUInt (const std::string&);
MessageHash_API std::string UintToHex(const std::vector<uint8_t>&);
#endif 
