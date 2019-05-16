#include <AsymKey/AsymKeyImpl.h>
#include <MessageHash/MessageHash.h>
#include <BigNumbers/BigNumbers.h>

#include <openssl/ec.h>      // for EC_GROUP_new_by_curve_name, EC_GROUP_free, EC_KEY_new, EC_KEY_set_group, EC_KEY_generate_key, EC_KEY_free
#include <openssl/ecdsa.h>   // for ECDSA_do_sign, ECDSA_do_verify
#include <openssl/obj_mac.h> // for 
#include <openssl/bn.h>

#include <stdexcept>
#include <cstring>

void AsymKeyImpl::_assign_privat_key()
{
    if (!EVP_PKEY_assign_EC_KEY(m_prikey.get(), p_eckey))
        throw std::runtime_error("Error generating assiging private key to public key");
}

AsymKeyImpl::~AsymKeyImpl()
{
    p_eckey = nullptr;
}

AsymKeyImpl::AsymKeyImpl()
: m_prikey(EVP_PKEY_new(), &EVP_PKEY_free)
, p_eckey(EC_KEY_new_by_curve_name(OBJ_txt2nid("secp256k1")))
{
    EC_KEY_set_asn1_flag(p_eckey, OPENSSL_EC_NAMED_CURVE);
    if (!(EC_KEY_generate_key(p_eckey)))
        throw std::runtime_error("Error generate public key");

    _assign_privat_key();
}

AsymKeyImpl::AsymKeyImpl(const AsymKeyImpl& crOther)
: m_prikey(EVP_PKEY_new(), ::EVP_PKEY_free)
, p_eckey(EC_KEY_new_by_curve_name(OBJ_txt2nid("secp256k1")))
{
    if (!EC_KEY_copy(p_eckey, crOther.p_eckey))
        throw std::runtime_error("Error copying public key");
    _assign_privat_key();
}

AsymKeyImpl& AsymKeyImpl::operator=(const AsymKeyImpl& crOther)
{
    if (this != &crOther) {
        if (!EC_KEY_copy(p_eckey, crOther.p_eckey))
            throw std::runtime_error("Error copying public key");
        _assign_privat_key();
    }
    return *this;
}


/// ECKey import export
/// https://stackoverflow.com/questions/50479284/openssl-read-an-ec-key-then-write-it-again-and-its-different
/// https://www.openssl.org/docs/man1.1.0/man3/PEM_read_bio.html

std::string AsymKeyImpl::getPublicKeyPEMStr()  const
{
    BIO_ptr outbio (BIO_new(BIO_s_mem()),&BIO_free_all);
    if (!PEM_write_bio_PUBKEY(outbio.get(), m_prikey.get()))
        throw std::runtime_error("Error writting public key");
    
    const int pubKeyLen = BIO_pending(outbio.get());
    std::string pubkey_str(pubKeyLen, '0');
    BIO_read(outbio.get(), (void*)&(pubkey_str.front()), pubKeyLen);

    return std::move(pubkey_str);
}

std::string AsymKeyImpl::getPrivateKeyPEMStr() const
{
    BIO_ptr outbio(BIO_new(BIO_s_mem()), &BIO_free_all);
    if(!PEM_write_bio_PrivateKey(outbio.get(), m_prikey.get(), NULL, NULL, 0, 0,NULL))
        throw std::runtime_error("Error writting private key");

    const int privKeyLen = BIO_pending(outbio.get());
    std::string privkey_str(privKeyLen, '0');
    BIO_read(outbio.get(), (void*)&(privkey_str.front()), privKeyLen);

    return std::move(privkey_str);
}

void AsymKeyImpl::setPEMPrivateKey(const std::string& crPEMStr)
{
    BIO_ptr bio(BIO_new(BIO_s_mem()), &BIO_free_all);
    const int bio_write_ret = BIO_write(bio.get(), static_cast<const char*>(crPEMStr.c_str()), (int)crPEMStr.size());
    if (bio_write_ret <= 0)
        throw std::runtime_error("Error reading PEM string");

    EVP_PKEY* raw_tmp_priv_pkey = EVP_PKEY_new();
    if (!PEM_read_bio_PrivateKey(bio.get(), &raw_tmp_priv_pkey, NULL, NULL))
        throw std::runtime_error("Error reading private key");

    p_eckey = EVP_PKEY_get1_EC_KEY(raw_tmp_priv_pkey);
    if(!p_eckey)
        throw std::runtime_error("Error importing ec key from private key");

    EC_KEY_set_asn1_flag(p_eckey, OPENSSL_EC_NAMED_CURVE);

    _assign_privat_key();
}

//// https://stackoverflow.com/questions/2228860/signing-a-message-using-ecdsa-in-openssl
using SIG_ptr = std::unique_ptr< ECDSA_SIG, decltype(&ECDSA_SIG_free)>;
using BN_ptr = std::unique_ptr<BIGNUM, decltype(&::BN_free)>;

std::pair<std::string, std::string> AsymKeyImpl::sign(const std::string& crMsg)const
{
    const std::string msg_hash = AsymKeyImpl::_hash(crMsg);
    SIG_ptr pSig (ECDSA_do_sign((const unsigned char*)msg_hash.c_str(), (int)strlen(msg_hash.c_str()), p_eckey), &ECDSA_SIG_free);
    if (pSig ==nullptr)
        throw std::runtime_error("error signing message");

    const BIGNUM* bnR = ECDSA_SIG_get0_r(pSig.get());
    const BIGNUM* bnS = ECDSA_SIG_get0_s(pSig.get());
    const std::string rHEX(BN_bn2hex(bnR));
    const std::string sHEX(BN_bn2hex(bnS));

    return std::make_pair(rHEX, sHEX);
}

bool AsymKeyImpl::verify(const std::string& crMsg, const std::string& crPublicKeyPEMStr, const std::pair<std::string, std::string>& rs)
{
    /// Create new ECDSA_SIG
    SIG_ptr pSig(ECDSA_SIG_new(), &ECDSA_SIG_free);
    const std::string& rHEX(rs.first);
    const std::string& sHEX(rs.second);
    
    /// Import <r,s> into BIGNUM
    BIGNUM* raw_r = BN_new();// Will be own by pSig
    BIGNUM* raw_s = BN_new();// Will be own by pSig
    BN_hex2bn(&raw_r, rHEX.c_str());
    BN_hex2bn(&raw_s, sHEX.c_str());

    if (!ECDSA_SIG_set0(pSig.get(), raw_r, raw_s))
        throw std::runtime_error("error importing <r,s> big numbers for ECDSA r["+ rs.first +"] s["+ rs.second+"]");

    /// Import public key
    BIO_ptr bio(BIO_new(BIO_s_mem()), &BIO_free_all);
    const int bio_write_ret = BIO_write(bio.get(), static_cast<const char*>(crPublicKeyPEMStr.c_str()), (int)crPublicKeyPEMStr.size());
    if (bio_write_ret <= 0)
        throw std::runtime_error("Error reading PEM string when verifying signature");

    /// Import the public key
    EC_KEY* raw_tmp_ec = nullptr;
    if (!PEM_read_bio_EC_PUBKEY(bio.get(), &raw_tmp_ec, NULL, NULL))
        throw std::runtime_error("Error reading public key when verifying signature");
    PubKey_ptr pEC(raw_tmp_ec, &EC_KEY_free);// wrap to unique_ptr for safety
    EC_KEY_set_asn1_flag(pEC.get(), OPENSSL_EC_NAMED_CURVE);

    const std::string msg_hash = AsymKeyImpl::_hash(crMsg);
    const int verify_status = ECDSA_do_verify((const unsigned char*)msg_hash.c_str(), (int)strlen(msg_hash.c_str()), pSig.get(), pEC.get());

    if(verify_status<0)
        throw std::runtime_error("error veryfying ECDSA signature r[" + rs.first + "] s[" + rs.second + "] msg_hash["+ msg_hash +"]");

    const bool verify_OK = (1== verify_status);
    return verify_OK;
}

std::string AsymKeyImpl::_hash(const std::string& crMsg)
{
    MessageHash hasher;
    hasher.HashSha256(crMsg);
    return std::move(hasher.HashHex());
}