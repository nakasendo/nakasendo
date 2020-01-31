#ifndef ASYM_KEY_H
#define ASYM_KEY_H

#include <AsymKey/AsymKeyConfig.h>

#include <memory>
#include <string>
#include <utility>
#include <vector>

class AsymKeyImpl; 
class KeyShare; 
class BigNumber;

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

        std::pair<std::string, std::string> exportPublicHEX()  const;
        std::string exportPublicHEXStr()  const; // Encoded
        std::string exportPrivateHEX() const;
        std::string exportPublicPEM()  const;
        std::string exportPrivatePEM() const;
        std::string exportPrivatePEMEncrypted( const std::string& ) const ;
        void importPrivatePEMEncrypted( const std::string&, const std::string& ) ;
        void importPrivatePEM(const std::string&);// Import PEM private key
        void importPrivateHEX(const std::string&);// Import HEX private key, knowing it is the right group
        std::string exportSharedSecretHex(const std::string& crOtherPublicPEMKey) const;// Calculate the shared secrete giving the public key from other

        AsymKey derive(const std::string& crAdditiveMsg) const;

        /// Sign the message, return <r,s>  component
        std::pair<std::string, std::string> sign(const std::string& crMsg) const;
        /// Sign the message, return <r,s>  component with the provided inv_k and r
        std::pair<std::string, std::string> sign_ex(const std::string& crMsg, const std::string& inv_k_hex, const std::string& r_hex) const;

        // split the key into multiple parts
        std::vector<KeyShare> split (const int&, const int&);
        // recover a key from multiple shares
        void recover (const std::vector<KeyShare>& ); 
        


    private:

        std::unique_ptr<AsymKeyImpl> m_pImpl;
};

AsymKey_API bool verify(const std::string& crMsg, const std::string& crPublicKeyPEMStr, const std::pair<std::string, std::string>& rs);
AsymKey_API bool verifyDER(const std::string& crMsg, const std::string& crPublicKeyPEMStr, const std::unique_ptr<unsigned char[]>&, const size_t&);
AsymKey_API std::string derive_pubkey(const std::string& crPubPEMkey, const std::string& crRandomMsg);
AsymKey_API std::pair<std::string, std::string> pubkey_pem2hex(const std::string& crPubPEMkey);
AsymKey_API std::string pubkey_pem2Hex_point(const std::string& crPubPEMkey, const bool& compressed=true);
AsymKey_API std::string pubkey_coordinates2pem(const std::string&, const std::string&, const int nid = 714);
AsymKey_API std::unique_ptr<unsigned char[]> DEREncodedSignature(const BigNumber&, const BigNumber&, size_t& len);
#endif /* ASYM_KEY_H */


