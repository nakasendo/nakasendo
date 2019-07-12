#ifndef __CONVERSIONS_H__
#define __CONVERSIONS_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory>

std::unique_ptr<unsigned char[]> spc_hex2bin(const std::unique_ptr<char[]>& input, size_t *l);
std::unique_ptr<unsigned char[]> spc_hex2binTwo(const std::string& input, size_t *l);
std::string binTohexStr(const std::unique_ptr<unsigned char[]>& data, int len);


#endif 
