#ifndef ASYM_KEY_IMPL_H
#define ASYM_KEY_IMPL_H

#include<memory>
#include<string>
#include<utility>

#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ec.h>
#include <openssl/pem.h>

using BIO_ptr = std::unique_ptr< BIO, decltype(&BIO_free_all)  >;
using EC_KEY_ptr = std::unique_ptr< EC_KEY, decltype(&EC_KEY_free)   >;
using EVP_PKEY_ptr = std::unique_ptr< EVP_PKEY, decltype(&EVP_PKEY_free) >;

class AsymKeyImpl
{
public:

    AsymKeyImpl(); // Generate keys pair at construction
    ~AsymKeyImpl();

    AsymKeyImpl(const AsymKeyImpl& crOther);
    AsymKeyImpl& operator= (const AsymKeyImpl&  crOther);

    int GroupNid()const;
    std::string getPublicKeyHEXStr()  const;
    std::string getPrivateKeyHEXStr() const;
    std::string getPublicKeyPEMStr()  const;
    std::string getPrivateKeyPEMStr() const;
    void setPEMPrivateKey(const std::string&);// Import PEM private key
    std::string  getSharedSecretHex(const std::string& crOtherPublicPEMKey) const ;// Calculate the shared secrete giving the public key from other

    /// Sign the message, return <r,s>  component
    std::pair<std::string, std::string> sign(const std::string& crMsg) const;

private:

    EVP_PKEY_ptr m_prikey;
    EC_KEY* p_eckey;// Do not need to free p_eckey since all are own by m_prikey

    void _assign_privat_key();// transfer all ownership to m_prikey

};

bool impl_verify(const std::string& crMsg, const std::string& crPublicKeyPEMStr, const std::pair<std::string, std::string>& rs);

#endif /* ASYM_KEY_IMPL_H */
