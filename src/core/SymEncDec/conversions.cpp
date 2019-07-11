#include <stdio.h>
//#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include <iostream>
#include <sstream>
#include <memory>
#define BYTES_PER_GROUP 4
#define GROUPS_PER_LINE 4

/* Don't change these */
#define BYTES_PER_LINE (BYTES_PER_GROUP * GROUPS_PER_LINE)

#define ERR_NOT_HEX  -1
#define ERR_BAD_SIZE -2
#define ERR_NO_MEM   -3



std::unique_ptr<char[]> spc_print_hex(char *prefix, const std::unique_ptr<unsigned char[]>& str, int len) {
  unsigned long i, j, preflen = 0;

  if (prefix) {
    printf("%s", prefix);
    preflen = strlen(prefix);
  }

    std::cout << "Len of buffer in spc_print_hex ... " << (len + preflen) * 2 << std::endl; 
  std::unique_ptr<char[]> buffer (new char [(len + preflen) * 2] );
  std::fill_n (buffer.get(), (len+preflen) * 2, 0x00);
  

  for (i = 0;  i < len;  i++) {
    printf("%02X ", str.get()[i]);
    sprintf(&buffer.get()[i*2], "%02X ", str.get()[i]);
 
    if (((i % BYTES_PER_LINE) == (BYTES_PER_LINE - 1)) && ((i + 1) != len)) {
      putchar('\n');
      for (j = 0;  j < preflen;  j++){ 
        putchar(' ');
      }
    }
    else if ((i % BYTES_PER_GROUP) == (BYTES_PER_GROUP - 1)){
      putchar(' ');
    }
  }
  putchar('\n');

  std::cout << "Via the stream: \n" <<  buffer.get() << std::endl;
  return std::move(buffer); 
}


std::unique_ptr<char[]> binToHex(const std::unique_ptr<unsigned char[]>& str, const int& len) {
  int i, j = 0;


  std::cout << "binToHex ... len: " << len << std::endl;
  std::unique_ptr<char[]> buffer (new char [(len*2)] );
  char * localPtr = new char[(len*2)];
  std::fill_n (buffer.get(), len*2, 0x0);
  
  int n(0);
  int wtf(0); 
  for (i = 0;  i < len;  ++i) {

    n += sprintf(&localPtr[i*2], "%02X", str.get()[i]);
    wtf += printf("%02X", str.get()[i]);
  
  }
  std::cout << "\nNumber of characters written on sprintf: " << n << "\nNumber of characters written on printf: " << wtf << std::endl;
  // Add the terminator character

  //buffer.get()[len*2]='0';

  for (int i=0;i<n;++i){
    buffer.get()[i] = localPtr[i]; 
  }
  //buffer.get()[(len*2)] = '0';
  std::cout << "Size of buffer in binToHex .. " << strlen(buffer.get()) << "\n" << buffer.get() << "\n" << std::addressof(buffer) << std::endl; 
  delete [] localPtr;
  return std::move(buffer); 
}

//std::unique_ptr<unsigned char[], std::default_delete<unsigned char[]>> pData(new unsigned char[size]);

std::unique_ptr<unsigned char[]> spc_hex2bin(const std::unique_ptr<char[]>& input, size_t *l) {
  unsigned char       shift = 4, value = 0;
  unsigned char       *r, *ret;
  const unsigned char *p;
  
  std::cout << "sizeof buffer for hex2bin:\t" << (strlen(input.get())/2) << std::endl;
  r = ret = new unsigned char[strlen(input.get()) / 2];
  if (ret == nullptr){
    *l = ERR_NO_MEM;
    return nullptr;
  }
  
  int lenInput = strlen(input.get()); 
  
  std::cout << "SIZE OF lenInput: " << lenInput << std::endl;
  unsigned char * ptr = new unsigned char[strlen(input.get())]; 
  if(ptr == nullptr){
    *l = ERR_NO_MEM;
    return nullptr;
  }
  std::fill_n(ptr, strlen(input.get()), 0x00);
  
 
  
  for (int i =0; i<lenInput; ++i){
    ptr[i] = input.get()[i]; 
  }

  //add the terminator
  ptr[lenInput] = '0';
  
  for (p = ptr;  isspace(*p);  p++);
  if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) p += 2;

  int index = 0; 
  while (p[0] && index < lenInput) {
    ++index; 
    switch (p[0]) {
      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9':
        value |= (*p++ - '0') << shift;
        break;
      case 'a': case 'b': case 'c':
      case 'd': case 'e': case 'f':
        value |= (*p++ - 'a' + 0xa) << shift;
        break;
      case 'A': case 'B': case 'C':
      case 'D': case 'E': case 'F':
        value |= (*p++ - 'A' + 0xa) << shift;
        break;
      case 0:
        if (!shift) { 
          *l = ERR_NOT_HEX;
          delete [] ptr;
          delete [] ret;
          return nullptr;
        }
        break;
      default:
        if (isspace(p[0])) p++;
        else {
          std::cout << "ERR_NOT HEX FAIL" << std::endl;
          *l = ERR_NOT_HEX;
          delete [] ptr;
          delete [] ret; 
          return 0;
        }
    }
    if ((shift = (shift + 4) % 8) != 0) {
      *r++ = value;
      value = 0;
    }
  }
  if (!shift) {
    std::cout << "ERR_BAD_SIZE FAIL" << std::endl;
    *l = ERR_BAD_SIZE;
    delete [] ptr;
    delete [] ret; 
    return nullptr;
  }
  *l = (r - ret);
  
  std::unique_ptr<unsigned char[]> retVal ( new unsigned char[strlen(input.get())/ 2]); 
  if (retVal == nullptr){
    *l = ERR_NO_MEM; 
    delete [] ptr; 
    delete [] ret; 
  }
  for(int i=0; i<*l;++i){
    retVal.get()[i] = ret[i] ; 
  }
  
  delete [] ptr; 
  delete [] ret ; 
  
  return retVal;
}


std::unique_ptr<unsigned char[]> spc_hex2binTwo(const std::string& input, size_t *l) {
  unsigned char       shift = 4, value = 0;
  unsigned char       *r, *ret;
  const unsigned char *p;
  
  r = ret = new unsigned char[input.length() / 2];
  if (ret == nullptr){
    *l = ERR_NO_MEM;
    return nullptr;
  }
  
  int lenInput = input.length(); 
  
  std::cout << "SIZE OF lenInput: " << lenInput << std::endl;
  unsigned char * ptr = new unsigned char[lenInput]; 
  if(ptr == nullptr){
    *l = ERR_NO_MEM;
    return nullptr;
  }
  std::fill_n(ptr, lenInput, 0x00);
  
    int i (0);
    for(std::string::const_iterator iter = input.begin(); iter != input.end(); ++iter){
        ptr[i++] = *iter ; 
    }
  
  for (p = ptr;  isspace(*p);  p++);
  if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) p += 2;

  int index = 0; 
  while (p[0] && index < lenInput) {
    ++index; 
    switch (p[0]) {
      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9':
        value |= (*p++ - '0') << shift;
        break;
      case 'a': case 'b': case 'c':
      case 'd': case 'e': case 'f':
        value |= (*p++ - 'a' + 0xa) << shift;
        break;
      case 'A': case 'B': case 'C':
      case 'D': case 'E': case 'F':
        value |= (*p++ - 'A' + 0xa) << shift;
        break;
      case 0:
        if (!shift) { 
          *l = ERR_NOT_HEX;
          delete [] ptr;
          delete [] ret;
          return nullptr;
        }
        break;
      default:
        if (isspace(p[0])) p++;
        else {
          std::cout << "ERR_NOT HEX FAIL" << std::endl;
          *l = ERR_NOT_HEX;
          delete [] ptr;
          delete [] ret; 
          return 0;
        }
    }
    if ((shift = (shift + 4) % 8) != 0) {
      *r++ = value;
      value = 0;
    }
  }
  if (!shift) {
    std::cout << "ERR_BAD_SIZE FAIL" << std::endl;
    *l = ERR_BAD_SIZE;
    delete [] ptr;
    delete [] ret; 
    return nullptr;
  }
  *l = (r - ret);
  
  std::unique_ptr<unsigned char[]> retVal ( new unsigned char[input.length()/ 2]); 
  if (retVal == nullptr){
    *l = ERR_NO_MEM; 
    delete [] ptr; 
    delete [] ret; 
  }
  for(int i=0; i<*l;++i){
    retVal.get()[i] = ret[i] ; 
  }
  
  delete [] ptr; 
  delete [] ret ; 
  
  return retVal;
}


#if 0 
unsigned char *spc_hex2bin_orig(const unsigned char *input, size_t *l) {
  unsigned char       shift = 4, value = 0;
  unsigned char       *r, *ret;
  const unsigned char *p;

  if (!(r = ret = (unsigned char *)malloc(strlen(input) / 2))) {
    *l = ERR_NO_MEM;
    return 0;
  }
  for (p = input;  isspace(*p);  p++);
  if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) p += 2;

  while (p[0]) {
    switch (p[0]) {
      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9':
        value |= (*p++ - '0') << shift;
        break;
      case 'a': case 'b': case 'c':
      case 'd': case 'e': case 'f':
        value |= (*p++ - 'a' + 0xa) << shift;
        break;
      case 'A': case 'B': case 'C':
      case 'D': case 'E': case 'F':
        value |= (*p++ - 'A' + 0xa) << shift;
        break;
      case 0:
        if (!shift) {
          *l = ERR_NOT_HEX;
          free(ret);
          return 0;
        }
        break;
      default:
        if (isspace(p[0])) p++;
        else {
          *l = ERR_NOT_HEX;
          free(ret);
          return 0;
        }
    }
    if ((shift = (shift + 4) % 8) != 0) {
      *r++ = value;
      value = 0;
    }
  }
  if (!shift) {
    *l = ERR_BAD_SIZE;
    free(ret);
    return 0;
  }
  *l = (r - ret);
  return (unsigned char *)realloc(ret, *l);
}
#endif


