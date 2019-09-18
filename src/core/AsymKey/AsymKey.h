#ifndef ASYM_KEY_H
#define ASYM_KEY_H

#include <DYNAMIC_LIBRARY_API.hpp>

#ifdef EXPORT_AsymKey
#    define AsymKey_API EXPORT_DYNAMIC_LIBRARY
#else
#    define AsymKey_API IMPORT_DYNAMIC_LIBRARY
#endif

#include <memory>
#include <string>
#include <utility>
#include <vector>

class AsymKeyImpl; 
class KeyShare; 

class AsymKey_API AsymKey
{
    public:
        explicit AsymKey();
        explicit AsymKey(int groupNID);
        ~AsymKey();

        //moveable
        AsymKey(AsymKey&& obj) noexcept;
        AsymKey& operator=(AsymKey&& obj) noexcept;
        //copyable
        AsymKey(const AsymKey& obj);
        AsymKey& operator=(const AsymKey& obj);

        bool is_valid() const; // Check if the keys satisfy pubkey = privkey * G

        //// some usefull infomation about the EC group
        int GroupNid()const;
        std::string Group_G_x() const;// big numb hex string 
        std::string Group_G_y() const;// big numb hex string
        std::string Group_p()   const;// big numb hex string 
        std::string Group_a()   const;// big numb hex string 
        std::string Group_b()   const;// big numb hex string
        std::string Group_n()   const;// group order

        std::pair<std::string, std::string> getPublicKeyHEX()  const;
        std::string getPublicKeyHEXStr()  const; // Encoded
        std::string getPrivateKeyHEX() const;
        std::string getPublicKeyPEM()  const;
        std::string getPrivateKeyPEM() const;
        std::string getPrivateKeyPEMEncrypted( const std::string& ) const ;
        void setPrivateKeyPEMEncrypted( const std::string&, const std::string& ) ;
        void setPEMPrivateKey(const std::string&);// Import PEM private key
        void setHEXPrivateKey(const std::string&);// Import HEX private key, knowing it is the right group
        std::string getSharedSecretHex(const std::string& crOtherPublicPEMKey) const;// Calculate the shared secrete giving the public key from other

        AsymKey derive(const std::string& crAdditiveMsg) const;

        /// Sign the message, return <r,s>  component
        std::pair<std::string, std::string> sign(const std::string& crMsg) const;

        // split the key into multiple parts
        std::vector<KeyShare> split (const int&, const int&);
        // recover a key from multiple shares
        void recover (const std::vector<KeyShare>& ); 
        


    private:

        std::unique_ptr<AsymKeyImpl> m_pImpl;
};

bool AsymKey_API verify(const std::string& crMsg, const std::string& crPublicKeyPEMStr, const std::pair<std::string, std::string>& rs);
std::string AsymKey_API derive_pubkey(const std::string& crPubPEMkey, const std::string& crRandomMsg);
std::pair<std::string, std::string> AsymKey_API pubkey_pem2hex(const std::string& crPubPEMkey);

#endif /* ASYM_KEY_H */


