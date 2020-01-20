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

class BigNumber;

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

    std::pair<std::string, std::string> exportPublicHEX()  const;
    std::string exportPublicHEXStr()  const;
    std::string exportPrivateHEXStr() const;
    std::string exportPublicPEMStr()  const;
    std::string exportPrivatePEMStr() const;
    std::string exportPrivatePEMEncrypted( const std::string& ) const ;
    void importPrivatePEMEncrypted( const std::string&, const std::string& ) ; // Import PEM Encrypted private key
    void importPrivatePEM(const std::string&);// Import PEM private key
    void importPrivateHEX(const std::string&);// Import HEX private key, knowing it is the right group
    std::string  exportSharedSecretHex(const std::string& crOtherPublicPEMKey) const ;// Calculate the shared secrete giving the public key from other

    AsymKeyImpl* derive_private(const std::string& crAdditiveMsg) const;

    /// Sign the message, return <r,s>  component
    std::pair<std::string, std::string> impl_sign(const std::string& crMsg) const;

    /// Sign the message, return <r,s>  component with the provided inv_k and r
    std::pair<std::string, std::string> impl_sign_ex(const std::string& crMsg, const std::string& inv_k_hex, const std::string& r_hex) const;

    // split the key into multiple parts
    std::vector<KeyShare> split (const int&, const int&);
    // recover a key from multiple shares
    void recover (const std::vector<KeyShare>& ); 

private:
    using EC_KEY_ptr = std::unique_ptr< EC_KEY, decltype(&EC_KEY_free)   >;
    EC_KEY_ptr m_key;
};

bool impl_verify(const std::string& crMsg, const std::string& crPublicKeyPEMStr, const std::pair<std::string, std::string>& rs);
bool impl_verifyDER
(const std::string& crMsg, const std::string& crPublicKeyPEMStr, const std::unique_ptr<unsigned char[]>& derSIG, const size_t& lenDERSig);

std::string impl_derive_pubkey(const std::string& crPubPEMkey, const std::string& crRandomMsg);
std::pair<std::string, std::string> impl_pubkey_pem2hex(const std::string& crPubPEMkey);
std::string impl_pubkey_pem2Hex_point(const std::string& crPubPEMkey, const bool& compressed=true);

std::unique_ptr<unsigned char []> impl_DEREncodedSignature(const BigNumber&, const BigNumber&, size_t& len);

std::string impl_pubkey_coordinates2pem(const std::string& xval, const std::string& yval, const int nid=714);
#endif /* ASYM_KEY_IMPL_H */
