#ifndef __CONVERSIONS_H__
#define __CONVERSIONS_H__

#include <MessageHash/MessageHashConfig.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory>
#include <vector>


std::unique_ptr<unsigned char[]> MessageHash_API HexToBin(const std::unique_ptr<char[]>& input, size_t *l);

std::unique_ptr<unsigned char[]> MessageHash_API HexStrToBin(const std::string& input, size_t *l);
std::string MessageHash_API binTohexStr(const std::unique_ptr<unsigned char[]>& data, int len);


std::vector<uint8_t> MessageHash_API HexToUInt (const std::string&);
std::string MessageHash_API UintToHex(const std::vector<uint8_t>&);
#endif 
