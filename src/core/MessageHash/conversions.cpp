#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <iostream>
#include <sstream>
#include <memory>
#include <MessageHash/conversions.h>

#define BYTES_PER_GROUP 4
#define GROUPS_PER_LINE 4

/* Don't change these */
#define BYTES_PER_LINE (BYTES_PER_GROUP * GROUPS_PER_LINE)

#define ERR_NOT_HEX  -1
#define ERR_BAD_SIZE -2
#define ERR_NO_MEM   -3

constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7','8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};


std::string binTohexStr(const std::unique_ptr<unsigned char[]>& data, int len)
{
  std::string s(len * 2, ' ');
  for (int i = 0; i < len; ++i) {
    s[2 * i]     = hexmap[(data.get()[i] & 0xF0) >> 4];
    s[2 * i + 1] = hexmap[data.get()[i] & 0x0F];
  }
  return s;
}

//std::unique_ptr<unsigned char[], std::default_delete<unsigned char[]>> pData(new unsigned char[size]);

std::unique_ptr<unsigned char[]> HexToBin(const std::unique_ptr<char[]>& input, size_t *l) {
  unsigned char       shift = 4, value = 0;
  unsigned char       *r, *ret;
  const unsigned char *p;
  
  r = ret = new unsigned char[strlen(input.get()) / 2];
  if (ret == nullptr){
    *l = ERR_NO_MEM;
    return nullptr;
  }
  
  int lenInput = strlen(input.get()); 
  
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
  //while (p[0] && index < lenInput) {
  while (index < lenInput){
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


std::unique_ptr<unsigned char[]> HexStrToBin(const std::string& input, size_t *l) {
  unsigned char       shift = 4, value = 0;
  unsigned char       *r, *ret;
  const unsigned char *p;
  

  r = ret = new unsigned char[input.length() / 2];
  if (ret == nullptr){
    *l = ERR_NO_MEM;
    return nullptr;
  }
  
  int lenInput = input.length(); 
  
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
  //while (p[0] && index < lenInput) {
  while(index < lenInput){
   ++index;
  //while (p[0]){
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

std::vector<uint8_t> HexToUInt (const std::string& hexStr){
  size_t len(0);
  std::unique_ptr<unsigned char[]> binChar =  HexStrToBin(hexStr, &len);
  std::vector<uint8_t> ret; 
  if(len != 0){
    for(int i = 0;i<len;++i){
      ret.push_back(binChar[i]);
    }
  }
  return ret;
}

std::string UintToHex(const std::vector<uint8_t>& UintRep){
  std::unique_ptr<unsigned char[]> uncharRep (new unsigned char[UintRep.size()]);
  int index(0);
  for(std::vector<uint8_t>::const_iterator iter = UintRep.begin(); iter != UintRep.end(); ++iter){
    uncharRep[index++] = *iter; 
  }
  return (binTohexStr(uncharRep, UintRep.size()));
}
