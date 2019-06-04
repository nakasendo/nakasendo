#include <AsymKey/AsymKeyImpl.h>
#include <MessageHash/MessageHash.h>
#include <BigNumbers/BigNumbers.h>

#include <openssl/ec.h>      // for EC_GROUP_new_by_curve_name, EC_GROUP_free, EC_KEY_new, EC_KEY_set_group, EC_KEY_generate_key, EC_KEY_free
#include <openssl/ecdsa.h>   // for ECDSA_do_sign, ECDSA_do_verify
#include <openssl/obj_mac.h> // for 
#include <openssl/bn.h>

#include <stdexcept>
#include <cstring>

//// https://stackoverflow.com/questions/2228860/signing-a-message-using-ecdsa-in-openssl
using SIG_ptr = std::unique_ptr< ECDSA_SIG, decltype(&ECDSA_SIG_free)>;
using BN_ptr = std::unique_ptr<BIGNUM, decltype(&::BN_free)>;

using BN_CTX_ptr = std::unique_ptr< BN_CTX, decltype(&BN_CTX_free) >;
using EC_GROUP_ptr = std::unique_ptr< EC_GROUP, decltype(&EC_GROUP_free) >;
using EC_POINT_ptr = std::unique_ptr< EC_POINT, decltype(&EC_POINT_free) >;

constexpr char hexmap[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

std::string _do_hash_msg(const std::string& crMsg)
{
    MessageHash hasher;
    hasher.HashSha256(crMsg);
    return std::move(hasher.HashHex());
}

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

int AsymKeyImpl::GroupNid()const
{
    /// The curve name secp256k1 has to be consistent with the constructor
    return OBJ_txt2nid("secp256k1");
}

std::string AsymKeyImpl::getPublicKeyHEXStr()  const
{
    BN_CTX_ptr nb_ctx( BN_CTX_new() , &BN_CTX_free);
    EC_GROUP_ptr ec_group(EC_GROUP_new_by_curve_name(GroupNid()) , &EC_GROUP_free);
    const EC_POINT* pEC_POINT = EC_KEY_get0_public_key(p_eckey);
    const std::string pubkey_hex( EC_POINT_point2hex(ec_group.get(), pEC_POINT, POINT_CONVERSION_COMPRESSED, nb_ctx.get()));

    return std::move(pubkey_hex);
}

std::string AsymKeyImpl::getPrivateKeyHEXStr()  const
{
    using BN_CTX_ptr = std::unique_ptr< BN_CTX, decltype(&BN_CTX_free) >;
    using EC_GROUP_ptr = std::unique_ptr< EC_GROUP, decltype(&EC_GROUP_free) >;
    using EC_POINT_ptr = std::unique_ptr< EC_POINT, decltype(&EC_POINT_free) >;

    BN_CTX_ptr nb_ctx(BN_CTX_new(), &BN_CTX_free);
    EC_GROUP_ptr ec_group(EC_GROUP_new_by_curve_name(GroupNid()), &EC_GROUP_free);
    const BIGNUM * pBN = EC_KEY_get0_private_key(p_eckey);
    const std::string private_key_hex(BN_bn2hex(pBN));
    return std::move(private_key_hex);
}

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

std::string AsymKeyImpl::getSharedSecretHex(const std::string& crOtherPublicPEMKey) const
{
    BIO_ptr bio(BIO_new(BIO_s_mem()), &BIO_free_all);
    const int bio_write_ret = BIO_write(bio.get(), static_cast<const char*>(crOtherPublicPEMKey.c_str()), (int)crOtherPublicPEMKey.size());
    if (bio_write_ret <= 0)
        throw std::runtime_error("Error reading PEM string");

    EVP_PKEY* raw_tmp_pub_pkey=nullptr;
    if (!PEM_read_bio_PUBKEY(bio.get(), &raw_tmp_pub_pkey, NULL, NULL))
        throw std::runtime_error("Error reading public key");
    EVP_PKEY_ptr tmp_pub_key(raw_tmp_pub_pkey, &EVP_PKEY_free);

    using EVP_PKEY_CTX_ptr = std::unique_ptr< EVP_PKEY_CTX, decltype(&EVP_PKEY_CTX_free) >;
    EVP_PKEY_CTX_ptr ctx(EVP_PKEY_CTX_new(m_prikey.get(), nullptr), &EVP_PKEY_CTX_free);
    if (!ctx)
        throw std::runtime_error("Error creating key context");
    if (EVP_PKEY_derive_init(ctx.get()) <= 0)
        throw std::runtime_error("Error deriving key context");
    if (EVP_PKEY_derive_set_peer(ctx.get(), tmp_pub_key.get()) <= 0)
        throw std::runtime_error("Error setting peer public key");

    size_t shared_secret_len;
    if (EVP_PKEY_derive(ctx.get(), NULL, &shared_secret_len) <= 0)
        throw std::runtime_error("Error determining shared secret lenght");

    unsigned char* shared_secret = (unsigned char*)OPENSSL_malloc(shared_secret_len);
    if (!shared_secret)
        throw std::runtime_error("Error creating shared secret");

    if (EVP_PKEY_derive(ctx.get(), shared_secret, &shared_secret_len) <= 0)
        throw std::runtime_error("Error creating shared secret");

    std::string hexStr(shared_secret_len * 2, ' ');
    for (int i = 0; i < shared_secret_len; ++i) {
        hexStr[2 * i] = hexmap[(shared_secret[i] & 0xF0) >> 4];
        hexStr[2 * i + 1] = hexmap[shared_secret[i] & 0x0F];
    }
    return std::move(hexStr);
}

std::pair<std::string, std::string> AsymKeyImpl::sign(const std::string& crMsg)const
{
    const std::string msg_hash = _do_hash_msg(crMsg);
    SIG_ptr pSig (ECDSA_do_sign((const unsigned char*)msg_hash.c_str(), (int)strlen(msg_hash.c_str()), p_eckey), &ECDSA_SIG_free);
    if (pSig ==nullptr)
        throw std::runtime_error("error signing message");

    const BIGNUM* bnR = ECDSA_SIG_get0_r(pSig.get());
    const BIGNUM* bnS = ECDSA_SIG_get0_s(pSig.get());
    const std::string rHEX(BN_bn2hex(bnR));
    const std::string sHEX(BN_bn2hex(bnS));

    return std::make_pair(rHEX, sHEX);
}

bool impl_verify(const std::string& crMsg, const std::string& crPublicKeyPEMStr, const std::pair<std::string, std::string>& rs)
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
    EC_KEY_ptr pEC(raw_tmp_ec, &EC_KEY_free);// wrap to unique_ptr for safety
    EC_KEY_set_asn1_flag(pEC.get(), OPENSSL_EC_NAMED_CURVE);

    const std::string msg_hash = _do_hash_msg(crMsg);
    const int verify_status = ECDSA_do_verify((const unsigned char*)msg_hash.c_str(), (int)strlen(msg_hash.c_str()), pSig.get(), pEC.get());

    if(verify_status<0)
        throw std::runtime_error("error veryfying ECDSA signature r[" + rs.first + "] s[" + rs.second + "] msg_hash["+ msg_hash +"]");

    const bool verify_OK = (1== verify_status);
    return verify_OK;
}