#ifndef __CONVERSIONS_H__
#define __CONVERSIONS_H__

#include <SymEncDec/SymEncDecConfig.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory>
#include <vector>


#include <DYNAMIC_LIBRARY_API.hpp>
#ifdef EXPORT_SymEncDec
#    define SymEncDec_API EXPORT_DYNAMIC_LIBRARY
#else
#    define SymEncDec_API IMPORT_DYNAMIC_LIBRARY
#endif


std::unique_ptr<unsigned char[]> SymEncDec_API HexToBin(const std::unique_ptr<char[]>& input, size_t *l);

std::unique_ptr<unsigned char[]> SymEncDec_API HexStrToBin(const std::string& input, size_t *l);
std::string SymEncDec_API binTohexStr(const std::unique_ptr<unsigned char[]>& data, int len);


std::vector<uint8_t> SymEncDec_API HexToUInt (const std::string&);
std::string SymEncDec_API UintToHex(const std::vector<uint8_t>&);
#endif 
