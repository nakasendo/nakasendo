#ifndef __MESSAGE_HASH_H__
#define __MESSAGE_HASH_H__

#include <memory>
#include <string>
#include <vector>

#include <MessageHash/MessageHashConfig.h>

class MessageHashImpl;

class MessageHash_API MessageHash
{
    public:
        explicit MessageHash () ;
        ~MessageHash ();

        void HashSha256(const std::string&);
        void Hash(const std::string&, const std::string&);
        void Hash(const std::vector<uint8_t>&, const std::string&);
        std::string HashHex ();     
        std::unique_ptr<unsigned char[]> Value (); 
        std::vector<uint8_t> HashValueBytes();

        void printHash () ; 
        std::string ListAvailableHash () ;

    private:

        MessageHash (const MessageHash& ) ; 
        MessageHash& operator= (const MessageHash& ) ; 

        MessageHash (const MessageHash&& ) ; 
        MessageHash& operator= (const MessageHash&& ) ; 
        std::unique_ptr<MessageHashImpl> m_pImpl ; 
};


MessageHash_API std::vector<uint8_t>  Hash
    (
        std::vector<uint8_t>,  
        const std::string& 
    ) ;    

MessageHash_API std::string HashMsgSHA256(const std::string& crMsg);

MessageHash_API std::unique_ptr<unsigned char[]> HashSha256 ( const std::string& );

#endif //ifndef __MESSAGE_HASH_H__