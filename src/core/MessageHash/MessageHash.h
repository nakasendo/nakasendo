#ifndef __MESSAGE_HASH_H__
#define __MESSAGE_HASH_H__

#include <memory>
#include <string>

#include <DYNAMIC_LIBRARY_API.hpp>
#ifdef EXPORT_MessageHash
#    define MessageHash_API EXPORT_DYNAMIC_LIBRARY
#else
#    define MessageHash_API IMPORT_DYNAMIC_LIBRARY
#endif

class MessageHashImpl;

class MessageHash_API MessageHash
{
    public:
        explicit MessageHash () ;
        ~MessageHash ();

        void HashSha256(const std::string&);
        void Hash(const std::string&, const std::string&);
        std::string HashHex (); 
        std::unique_ptr<unsigned char> Value (); 


        void printHash () ; 
        std::string ListAvailableHash () ; 

    private:

        MessageHash (const MessageHash& ) ; 
        MessageHash& operator= (const MessageHash& ) ; 

        MessageHash (const MessageHash&& ) ; 
        MessageHash& operator= (const MessageHash&& ) ; 
        std::unique_ptr<MessageHashImpl> m_pImpl ; 
};

#endif //ifndef __MESSAGE_HASH_H__