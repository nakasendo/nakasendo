#ifndef ASYM_KEY_IMPL_H
#define ASYM_KEY_IMPL_H

#include<memory>
#include<string>

#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ec.h>
#include <openssl/pem.h>


class AsymKeyImpl
{
public:

    AsymKeyImpl(); // Generate keys pair at construction
    ~AsymKeyImpl();

    AsymKeyImpl(const AsymKeyImpl& crOther);
    AsymKeyImpl& operator= (const AsymKeyImpl&  crOther);

    std::string getPublicKeyStr()  const;
    std::string getPrivateKeyStr() const;

private:

    using BIO_ptr    = std::unique_ptr< BIO     , decltype(&BIO_free_all)  >;
    using PubKey_ptr = std::unique_ptr< EC_KEY  , decltype(&EC_KEY_free)   >;
    using PriKey_ptr = std::unique_ptr< EVP_PKEY, decltype(&EVP_PKEY_free) >;

    PriKey_ptr m_prikey;
    EC_KEY* pPubKey;// Do not need to free pPubKey since all are own by m_prikey

    void genPrivKeyFromPubKey();
};

#endif /* ASYM_KEY_IMPL_H */
