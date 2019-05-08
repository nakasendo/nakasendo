#include "MessageHash.h"
#include "MessageHashImpl.h"

MessageHash::MessageHash(): m_pImpl(new MessageHashImpl)
{
    return ; 
}

MessageHash::~MessageHash ()
{
    return ; 
}

void MessageHash::HashSha256(const std::string& msg)
{
    m_pImpl->HashSha256 (msg);
}

void MessageHash::Hash(const std::string& msg, const std::string& hashfunc)
{
    m_pImpl->Hash (msg, hashfunc);
}

std::unique_ptr<unsigned char> MessageHash::Value(){
    return m_pImpl->HashVal();
}

std::string MessageHash::HashHex ()
{
    return (m_pImpl->hashValHex());
}

void MessageHash::printHash ()
{
    m_pImpl->printHash() ; 
}


std::string  MessageHash::ListAvailableHash ()
{
    return (m_pImpl->ListAvailableHash());
}

