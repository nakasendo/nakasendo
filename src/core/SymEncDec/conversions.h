#ifndef __CONVERSIONS_H__
#define __CONVERSIONS_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory>

std::unique_ptr<char> spc_print_hex(char *prefix, const std::unique_ptr<unsigned char[]>& str, int len);
std::unique_ptr<unsigned char[]> spc_hex2bin(const std::unique_ptr<char[]>& input, size_t *l);

std::unique_ptr<char[]> binToHex(const std::unique_ptr<unsigned char[]>& str, const int& len);
std::unique_ptr<unsigned char[]> spc_hex2binTwo(const std::string& input, size_t *l);


#endif 
