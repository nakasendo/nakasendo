#ifndef __MESSAGE_HASH_IMPL__
#define __MESSAGE_HASH_IMPL__

#include <string>
#include <memory>

using messagePtr = std::unique_ptr<unsigned char[]> ; 
using charPtr = std::unique_ptr<char>;

class MessageHashImpl
{
    public:        
        explicit MessageHashImpl();
        ~MessageHashImpl () = default; 

        void HashSha256 (const std::string&);
        void Hash(const std::string&, const std::string&); 
        void Hash(const std::vector<uint8_t>&, const std::string&); 
        void Hash(const messagePtr&, size_t length, const std::string&); 
        messagePtr HashVal ();
        std::string hashValHex () ; 
        void printHash (); 
        std::string ListAvailableHash(); 
        
        unsigned int HashLength() const { return m_MessageHashLength ;}

    private:
        MessageHashImpl (const MessageHashImpl&);
        MessageHashImpl& operator= (const MessageHashImpl&);
        MessageHashImpl(const MessageHashImpl&&);
        MessageHashImpl& operator= (const MessageHashImpl&&);
        

        messagePtr m_mPtr ; 
        unsigned int m_MessageHashLength;         
        
        charPtr m_Hashes ; 

};

#endif //ifndef __MESSAGE_HASH_IMPL
