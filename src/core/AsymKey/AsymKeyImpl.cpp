#include <AsymKey/AsymKeyImpl.h>
#include <stdexcept>

void AsymKeyImpl::genPrivKeyFromPubKey()
{
    if (!EVP_PKEY_assign_EC_KEY(m_prikey.get(), pPubKey))
        throw std::runtime_error("Error generating private key from public key");
}

AsymKeyImpl::~AsymKeyImpl()
{
    pPubKey = nullptr;
}

AsymKeyImpl::AsymKeyImpl()
: m_prikey(EVP_PKEY_new(), &EVP_PKEY_free)
, pPubKey(EC_KEY_new_by_curve_name(OBJ_txt2nid("secp256k1")))
{
    EC_KEY_set_asn1_flag(pPubKey, OPENSSL_EC_NAMED_CURVE);
    if (!(EC_KEY_generate_key(pPubKey)))
        throw std::runtime_error("Error generate public key");

    genPrivKeyFromPubKey();
}

AsymKeyImpl::AsymKeyImpl(const AsymKeyImpl& crOther)
: m_prikey(EVP_PKEY_new(), ::EVP_PKEY_free)
, pPubKey(EC_KEY_new_by_curve_name(OBJ_txt2nid("secp256k1")))
{
    if (!EC_KEY_copy(pPubKey, crOther.pPubKey))
        throw std::runtime_error("Error copying public key");
    genPrivKeyFromPubKey();
}

AsymKeyImpl& AsymKeyImpl::operator=(const AsymKeyImpl& crOther)
{
    if (this != &crOther) {
        if (!EC_KEY_copy(pPubKey, crOther.pPubKey))
            throw std::runtime_error("Error copying public key");
        genPrivKeyFromPubKey();
    }
    return *this;
}

std::string AsymKeyImpl::getPublicKeyStr()  const
{
    //https://stackoverflow.com/questions/50479284/openssl-read-an-ec-key-then-write-it-again-and-its-different

    BIO_ptr outbio (BIO_new(BIO_s_mem()),&BIO_free_all);
    if (!PEM_write_bio_PUBKEY(outbio.get(), m_prikey.get()))
        throw std::runtime_error("Error writting public key");
    
    const int pubKeyLen = BIO_pending(outbio.get());
    std::string pubkey_str(pubKeyLen, '0');
    BIO_read(outbio.get(), (void*)&(pubkey_str.front()), pubKeyLen);

    return std::move(pubkey_str);
}

std::string AsymKeyImpl::getPrivateKeyStr() const
{
    BIO_ptr outbio(BIO_new(BIO_s_mem()), &BIO_free_all);
    if(!PEM_write_bio_PrivateKey(outbio.get(), m_prikey.get(), NULL, NULL, 0, 0,NULL))
        throw std::runtime_error("Error writting private key");

    const int privKeyLen = BIO_pending(outbio.get());
    std::string privkey_str(privKeyLen, '0');
    BIO_read(outbio.get(), (void*)&(privkey_str.front()), privKeyLen);

    return std::move(privkey_str);
}