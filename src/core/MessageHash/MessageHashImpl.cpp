#include <openssl/evp.h>
#include <openssl/objects.h>
#include <functional>
#include <string.h>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <MessageHash/MessageHashImpl.h>

using md_ptr = std::unique_ptr<EVP_MD_CTX,decltype(&::EVP_MD_CTX_free)> ;

namespace {
    std::vector<std::string> g_PtrHashFuncList; 

void ListHashCallback(const OBJ_NAME *obj, void *arg)
{
    printf("Digest: %s\n", obj->name);
    std::unique_ptr<char>  PtrHashFuncName (new char[strlen(obj->name)+1]);
    sprintf(PtrHashFuncName.get(),"%s", obj->name);
    g_PtrHashFuncList.push_back(std::string(PtrHashFuncName.get()));
}
};

MessageHashImpl::MessageHashImpl() : m_mPtr (new unsigned char[EVP_MAX_MD_SIZE])
{
    return; 
}


void MessageHashImpl::HashSha256 (const std::string& msg)
{
    md_ptr mdctx (EVP_MD_CTX_create(),::EVP_MD_CTX_free);      
    EVP_DigestInit_ex (mdctx.get(), EVP_sha256(), NULL);
    EVP_DigestUpdate(mdctx.get(), msg.c_str(), msg.size());
    EVP_DigestFinal_ex(mdctx.get(), m_mPtr.get(), &m_MessageHashLength);
}

void MessageHashImpl::printHash()
{
    for (unsigned int i=0;i<this->m_MessageHashLength; ++i)
    {        
        printf("%02x", m_mPtr.get()[i]);  
    }
}

void MessageHashImpl::Hash(const std::string& msg, const std::string& hashfunc)
{
  // check for available hashfunc    
    md_ptr mdctx (EVP_MD_CTX_create(),::EVP_MD_CTX_free); 
    const EVP_MD* md = nullptr;
    md = EVP_get_digestbyname (hashfunc.c_str());
    if (md == nullptr)
        throw std::invalid_argument("Unknown message digest: " + hashfunc);
    EVP_DigestInit_ex (mdctx.get(),md,NULL);
    EVP_DigestUpdate(mdctx.get(), msg.c_str(), msg.size());
    EVP_DigestFinal_ex(mdctx.get(), m_mPtr.get(), &m_MessageHashLength);
}

messagePtr MessageHashImpl::HashVal ()
{
    return std::move(m_mPtr);
}

std::string MessageHashImpl::hashValHex ()
{
    std::unique_ptr<char>  tmpBuffer (new char[m_MessageHashLength*2+1]); 
    for (unsigned int i=0;i<this->m_MessageHashLength; ++i)
    {
        sprintf(&tmpBuffer.get()[i*2],"%02x", m_mPtr.get()[i]);
    }
    return std::string (tmpBuffer.get());
}
std::string MessageHashImpl::ListAvailableHash()
{
    void *my_arg = nullptr; 
    if ( !g_PtrHashFuncList.empty ()){
        g_PtrHashFuncList.clear (); 
    } 
#ifndef __EMSCRIPTEN__         
    OpenSSL_add_all_digests();
    
    OBJ_NAME_do_all (OBJ_NAME_TYPE_MD_METH,ListHashCallback, my_arg);
#endif

    std::stringstream strOp; 
#ifndef __EMSCRIPTEN__
    for (std::vector<std::string>::const_iterator iter = g_PtrHashFuncList.begin (); iter != g_PtrHashFuncList.end (); ++ iter)
    {
        strOp << *iter << "\n";
    }
    EVP_cleanup();
#endif        
    return std::string (strOp.str());
}




