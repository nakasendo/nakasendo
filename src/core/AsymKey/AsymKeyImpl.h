#ifndef ASYM_KEY_IMPL_H
#define ASYM_KEY_IMPL_H

#include<memory>
#include<string>
#include<utility>
#include<vector>

#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ec.h>
#include <openssl/pem.h>

class KeyShare; 

class AsymKeyImpl
{
public:

    AsymKeyImpl(); // Generate keys pair at construction
    AsymKeyImpl(int groupNID);

    ~AsymKeyImpl();

    AsymKeyImpl(const AsymKeyImpl& crOther);
    AsymKeyImpl& operator= (const AsymKeyImpl&  crOther);

    bool is_valid() const; // Check if the keys satisfy pubkey = privkey * G

    //// some usefull infomation about the EC group
    int GroupNid()const;
    std::string Group_G_x() const ;// big numb hex string
    std::string Group_G_y() const ;// big numb hex string
    std::string Group_p()   const ;// big numb hex string
    std::string Group_a()   const ;// big numb hex string
    std::string Group_b()   const ;// big numb hex string
    std::string Group_n()   const ;// order

    std::pair<std::string, std::string> getPublicKeyHEX()  const;
    std::string getPublicKeyHEXStr()  const;
    std::string getPrivateKeyHEXStr() const;
    std::string getPublicKeyPEMStr()  const;
    std::string getPrivateKeyPEMStr() const;
    void setPEMPrivateKey(const std::string&);// Import PEM private key
    void setHEXPrivateKey(const std::string&);// Import HEX private key, knowing it is the right group
    std::string  getSharedSecretHex(const std::string& crOtherPublicPEMKey) const ;// Calculate the shared secrete giving the public key from other

    AsymKeyImpl* derive_private(const std::string& crAdditiveMsg) const;

    /// Sign the message, return <r,s>  component
    std::pair<std::string, std::string> sign(const std::string& crMsg) const;

    // split the key into multiple parts
    std::vector<KeyShare> split (const int&, const int&);
    // recover a key from multiple shares
    void recover (const std::vector<KeyShare>& ); 

private:
    using EC_KEY_ptr = std::unique_ptr< EC_KEY, decltype(&EC_KEY_free)   >;
    EC_KEY_ptr m_key;
};

bool impl_verify(const std::string& crMsg, const std::string& crPublicKeyPEMStr, const std::pair<std::string, std::string>& rs);

std::string impl_derive_pubkey(const std::string& crPubPEMkey, const std::string& crRandomMsg);
std::pair<std::string, std::string> impl_pubkey_pem2hex(const std::string& crPubPEMkey);
#endif /* ASYM_KEY_IMPL_H */
