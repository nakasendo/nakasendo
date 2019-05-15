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

        std::string getPublicKeyPEM()  const;
        std::string getPrivateKeyPEM() const;
        void setPEMPrivateKey(const std::string&);// Import PEM private key

    private:

        std::unique_ptr<AsymKeyImpl> m_pImpl;
};

#endif /* ASYM_KEY_H */


