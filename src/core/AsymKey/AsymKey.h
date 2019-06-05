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

class AsymKeyImpl; 

class AsymKey_API AsymKey
{
    public:
        explicit AsymKey();

        ~AsymKey();

        //moveable
        AsymKey(AsymKey&& obj) noexcept;
        AsymKey& operator=(AsymKey&& obj) noexcept;
        //copyable
        AsymKey(const AsymKey& obj);
        AsymKey& operator=(const AsymKey& obj);

        int GroupNid()const;
        std::string getPublicKeyHEX()  const;
        std::string getPrivateKeyHEX() const;
        std::string getPublicKeyPEM()  const;
        std::string getPrivateKeyPEM() const;
        void setPEMPrivateKey(const std::string&);// Import PEM private key
        std::string getSharedSecretHex(const std::string& crOtherPublicPEMKey) const;// Calculate the shared secrete giving the public key from other

        /// Sign the message, return <r,s>  component
        std::pair<std::string, std::string> sign(const std::string& crMsg) const;

    private:

        std::unique_ptr<AsymKeyImpl> m_pImpl;
};

bool AsymKey_API verify(const std::string& crMsg, const std::string& crPublicKeyPEMStr, const std::pair<std::string, std::string>& rs);

#endif /* ASYM_KEY_H */


