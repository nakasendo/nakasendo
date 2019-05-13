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

        std::string getPublicKey()  const;
        std::string getPrivateKey() const;

    private:

        std::unique_ptr<AsymKeyImpl> m_pImpl;
};

#endif /* ASYM_KEY_H */


