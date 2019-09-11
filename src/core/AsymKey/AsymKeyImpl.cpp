#include <AsymKey/AsymKeyImpl.h>
#include <MessageHash/MessageHash.h>
#include <BigNumbers/BigNumbers.h>
#include <Polynomial/Polynomial.h>
#include <SecretSplit/KeyShare.h>
#include <SecretSplit/SecretSplit.h>

#include <openssl/ec.h>      // for EC_GROUP_new_by_curve_name, EC_GROUP_free, EC_KEY_new, EC_KEY_set_group, EC_KEY_generate_key, EC_KEY_free
#include <openssl/ecdsa.h>   // for ECDSA_do_sign, ECDSA_do_verify
#include <openssl/obj_mac.h> // for 
#include <openssl/bn.h>
#include <openssl/crypto.h>

#include <stdexcept>
#include <cstring>

//// https://stackoverflow.com/questions/2228860/signing-a-message-using-ecdsa-in-openssl
using SIG_ptr = std::unique_ptr< ECDSA_SIG, decltype(&ECDSA_SIG_free)>;
using BN_ptr = std::unique_ptr<BIGNUM, decltype(&::BN_free)>;

using BIO_ptr = std::unique_ptr< BIO, decltype(&BIO_free_all)  >;
using EC_KEY_ptr = std::unique_ptr< EC_KEY, decltype(&EC_KEY_free)   >;

using BN_CTX_ptr = std::unique_ptr< BN_CTX, decltype(&BN_CTX_free) >;
using EC_GROUP_ptr = std::unique_ptr< EC_GROUP, decltype(&EC_GROUP_free) >;
using EC_POINT_ptr = std::unique_ptr< EC_POINT, decltype(&EC_POINT_free) >;
using BIGNUM_ptr = std::unique_ptr<BIGNUM, decltype(&BN_free)>;
inline void help_openssl_free_char(char* p) { OPENSSL_free(p); }
using STR_ptr = std::unique_ptr<char, decltype(&help_openssl_free_char)>;//

std::string _do_hash_msg(const std::string& crMsg)
{
    /*
    MessageHash hasher;
    hasher.HashSha256(crMsg);
    return std::move(hasher.HashHex());
    */

    /// This is a quick version hashing small string
    auto buff2hexstr_helper = [](unsigned char* buff, size_t buff_len)->std::string
    {
        /// OPENSSL_buf2hexstr doesn't work as expected, it returned hex string with parts separated by comma (file $OPENSSL_ROOT_DIR/crypto/o_str.c:OPENSSL_buf2hexstr)
        /// Use manual converter here could give flexibility https://stackoverflow.com/questions/10723403/char-array-to-hex-string-c
        constexpr char hexmap[] = "0123456789ABCDEF";
        std::string hexstr(2 * buff_len, ' ');

        for (size_t i = 0; i < buff_len; ++i)
        {
            hexstr[2 * i] = hexmap[(buff[i] & 0xF0) >> 4];
            hexstr[2 * i + 1] = hexmap[(buff[i] & 0x0F) >> 0];
        }
        return std::move(hexstr);
    };

    /// For longer string, need to use buffer and sha update mechanism
    unsigned char hash_buff[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(crMsg.c_str()), crMsg.size(), hash_buff);
    const std::string output = buff2hexstr_helper(hash_buff, SHA256_DIGEST_LENGTH);
    return std::move(output);
}

AsymKeyImpl::~AsymKeyImpl()
{
}

AsymKeyImpl::AsymKeyImpl()
: m_key(EC_KEY_new_by_curve_name(OBJ_txt2nid("secp256k1")), &EC_KEY_free)
{
    EC_KEY_set_asn1_flag(m_key.get(), OPENSSL_EC_NAMED_CURVE);
    if(!EC_KEY_generate_key(m_key.get()))
        throw std::runtime_error("Unable to generate EC Key");
}

AsymKeyImpl::AsymKeyImpl(int groupNID)
: m_key(EC_KEY_new_by_curve_name(groupNID), &EC_KEY_free)
{
    EC_KEY_set_asn1_flag(m_key.get(), OPENSSL_EC_NAMED_CURVE);
    if (!EC_KEY_generate_key(m_key.get()))
        throw std::runtime_error("Unable to generate EC Key");
}

AsymKeyImpl::AsymKeyImpl(const AsymKeyImpl& crOther)
: m_key(EC_KEY_new_by_curve_name(crOther.GroupNid()), &EC_KEY_free)
{
    if (!EC_KEY_copy(m_key.get(), crOther.m_key.get()))
        throw std::runtime_error("Error copying ec key");
    EC_KEY_set_asn1_flag(m_key.get(), OPENSSL_EC_NAMED_CURVE);
}

AsymKeyImpl& AsymKeyImpl::operator=(const AsymKeyImpl& crOther)
{
    if (!EC_KEY_copy(m_key.get(), crOther.m_key.get()))
        throw std::runtime_error("Error copying ec key");
    return *this;
    EC_KEY_set_asn1_flag(m_key.get(), OPENSSL_EC_NAMED_CURVE);
}

bool AsymKeyImpl::is_valid() const
{
    return EC_KEY_check_key(m_key.get());
}

/// ECKey import export
/// https://stackoverflow.com/questions/50479284/openssl-read-an-ec-key-then-write-it-again-and-its-different
/// https://www.openssl.org/docs/man1.1.0/man3/PEM_read_bio.html
int AsymKeyImpl::GroupNid()const
{
    const EC_GROUP* pEC_GROUP = EC_KEY_get0_group(m_key.get());
    if (pEC_GROUP == nullptr)
        throw std::runtime_error("Unable to get group ID from EC key");
    return EC_GROUP_get_curve_name(pEC_GROUP);
}

std::string AsymKeyImpl::Group_G_x() const
{
    const EC_GROUP* pEC_GROUP = EC_KEY_get0_group(m_key.get());
    if (pEC_GROUP == nullptr)
        throw std::runtime_error("Unable to get group ID from EC key");
    const EC_POINT* pEC_GENERATOR = EC_GROUP_get0_generator(pEC_GROUP);
    if (pEC_GENERATOR == nullptr)
        throw std::runtime_error("Unable to get key group generator");

    BIGNUM_ptr G_x(BN_new(), &BN_free);
    BN_CTX_ptr pCTX_get(BN_CTX_new(), &BN_CTX_free);
    if (!EC_POINT_get_affine_coordinates_GFp(pEC_GROUP, pEC_GENERATOR, G_x.get(), nullptr, pCTX_get.get()))
        throw std::runtime_error("Unable to get x coordinate of shared secret");

    STR_ptr pStr(BN_bn2hex(G_x.get()), &help_openssl_free_char);
    const std::string hex_str(pStr.get());
    return std::move(hex_str);
}
std::string AsymKeyImpl::Group_G_y() const
{
    const EC_GROUP* pEC_GROUP = EC_KEY_get0_group(m_key.get());
    if (pEC_GROUP == nullptr)
        throw std::runtime_error("Unable to get group ID from EC key");
    const EC_POINT* pEC_GENERATOR = EC_GROUP_get0_generator(pEC_GROUP);
    if (pEC_GENERATOR == nullptr)
        throw std::runtime_error("Unable to get key group generator");

    BIGNUM_ptr G_y(BN_new(), &BN_free);
    BN_CTX_ptr pCTX_get(BN_CTX_new(), &BN_CTX_free);
    if (!EC_POINT_get_affine_coordinates_GFp(pEC_GROUP, pEC_GENERATOR, nullptr, G_y.get(), pCTX_get.get()))
        throw std::runtime_error("Unable to get y coordinate of shared secret");

    STR_ptr pStr(BN_bn2hex(G_y.get()), &help_openssl_free_char);
    const std::string hex_str(pStr.get());
    return std::move(hex_str);
}
std::string AsymKeyImpl::Group_p() const
{
    const EC_GROUP* pEC_GROUP = EC_KEY_get0_group(m_key.get());
    if (pEC_GROUP == nullptr)
        throw std::runtime_error("Unable to get group ID from EC key");
    BIGNUM_ptr p(BN_new(), ::BN_free);
    BIGNUM_ptr a(BN_new(), ::BN_free);
    BIGNUM_ptr b(BN_new(), ::BN_free);
    BN_CTX_ptr pCTX_get_curve(BN_CTX_new(), &BN_CTX_free);
    if (!EC_GROUP_get_curve_GFp(pEC_GROUP, p.get(), a.get(), b.get(), pCTX_get_curve.get()))
        throw std::runtime_error("Unable to get ec group information");

    STR_ptr pStr(BN_bn2hex(p.get()), &help_openssl_free_char);
    const std::string hex_str(pStr.get());
    return std::move(hex_str);
}
std::string AsymKeyImpl::Group_a() const
{
    const EC_GROUP* pEC_GROUP = EC_KEY_get0_group(m_key.get());
    if (pEC_GROUP == nullptr)
        throw std::runtime_error("Unable to get group ID from EC key");
    BIGNUM_ptr p(BN_new(), ::BN_free);
    BIGNUM_ptr a(BN_new(), ::BN_free);
    BIGNUM_ptr b(BN_new(), ::BN_free);
    BN_CTX_ptr pCTX_get_curve(BN_CTX_new(), &BN_CTX_free);
    if (!EC_GROUP_get_curve_GFp(pEC_GROUP, p.get(), a.get(), b.get(), pCTX_get_curve.get()))
        throw std::runtime_error("Unable to get ec group information");

    STR_ptr pStr ( BN_bn2hex(a.get()), &help_openssl_free_char);
    const std::string hex_str(pStr.get());
    return std::move(hex_str);
}
std::string AsymKeyImpl::Group_b() const
{
    const EC_GROUP* pEC_GROUP = EC_KEY_get0_group(m_key.get());
    if (pEC_GROUP == nullptr)
        throw std::runtime_error("Unable to get group ID from EC key");
    BIGNUM_ptr p(BN_new(), ::BN_free);
    BIGNUM_ptr a(BN_new(), ::BN_free);
    BIGNUM_ptr b(BN_new(), ::BN_free);
    BN_CTX_ptr pCTX_get_curve(BN_CTX_new(), &BN_CTX_free);
    if (!EC_GROUP_get_curve_GFp(pEC_GROUP, p.get(), a.get(), b.get(), pCTX_get_curve.get()))
        throw std::runtime_error("Unable to get ec group information");

    STR_ptr pStr(BN_bn2hex(b.get()), &help_openssl_free_char);
    const std::string hex_str(pStr.get());
    return std::move(hex_str);
}
std::string AsymKeyImpl::Group_n() const
{
    const EC_GROUP* pEC_GROUP = EC_KEY_get0_group(m_key.get());
    if (pEC_GROUP == nullptr)
        throw std::runtime_error("Unable to get group ID from EC key");

    BIGNUM_ptr n(BN_new(), ::BN_free);
    BN_CTX_ptr pCTX_get_order(BN_CTX_new(), &BN_CTX_free);
    if (!EC_GROUP_get_order(pEC_GROUP, n.get(), pCTX_get_order.get()))
        throw std::runtime_error("Unable to get key group order");

    STR_ptr pStr(BN_bn2hex(n.get()), &help_openssl_free_char);
    const std::string hex_str(pStr.get());
    return std::move(hex_str);
}

std::pair<std::string, std::string> AsymKeyImpl::getPublicKeyHEX()  const
{
    const EC_GROUP* pEC_GROUP = EC_KEY_get0_group(m_key.get());
    if (pEC_GROUP == nullptr)
        throw std::runtime_error("Unable to get EC key group");
    const EC_POINT* pEC_POINT = EC_KEY_get0_public_key(m_key.get());
    if (pEC_POINT == nullptr)
        throw std::runtime_error("Unable to get EC point from public key");

    BIGNUM_ptr x(BN_new(), &BN_free);
    BIGNUM_ptr y(BN_new(), &BN_free);
    BN_CTX_ptr pCTX_get(BN_CTX_new(), &BN_CTX_free);
    if (!EC_POINT_get_affine_coordinates_GFp(pEC_GROUP, pEC_POINT, x.get(), y.get(), pCTX_get.get()))
        throw std::runtime_error("Unable to get x coordinate of shared secret");

    STR_ptr xStr(BN_bn2hex(x.get()), &help_openssl_free_char);
    STR_ptr yStr(BN_bn2hex(y.get()), &help_openssl_free_char);
    return std::move(std::make_pair( std::move(std::string(xStr.get())), std::move(std::string(yStr.get())) ));
}

std::string AsymKeyImpl::getPublicKeyHEXStr()  const
{
    BN_CTX_ptr nb_ctx(BN_CTX_new(), &BN_CTX_free);
    const EC_GROUP* pEC_GROUP = EC_KEY_get0_group(m_key.get());
    const EC_POINT* pEC_POINT = EC_KEY_get0_public_key(m_key.get());

    STR_ptr pStr(EC_POINT_point2hex(pEC_GROUP, pEC_POINT, POINT_CONVERSION_COMPRESSED, nb_ctx.get()), &help_openssl_free_char);
    const std::string pubkey_hex(pStr.get());
    return std::move(pubkey_hex);
}

std::string AsymKeyImpl::getPrivateKeyHEXStr()  const
{
    BN_CTX_ptr nb_ctx(BN_CTX_new(), &BN_CTX_free);
    const BIGNUM * pBN = EC_KEY_get0_private_key(m_key.get());

    STR_ptr pStr(BN_bn2hex(pBN), &help_openssl_free_char);
    const std::string priv_key_hex_str(pStr.get());
    return std::move(priv_key_hex_str);
}

std::string AsymKeyImpl::getPublicKeyPEMStr()  const
{
    BIO_ptr outbio (BIO_new(BIO_s_mem()),&BIO_free_all);
    if (!PEM_write_bio_EC_PUBKEY(outbio.get(), m_key.get()))
        throw std::runtime_error("Error writting public key");
    
    const int pubKeyLen = BIO_pending(outbio.get());
    std::string pubkey_str(pubKeyLen, '0');
    BIO_read(outbio.get(), (void*)&(pubkey_str.front()), pubKeyLen);

    return std::move(pubkey_str);
}

std::string AsymKeyImpl::getPrivateKeyPEMStr() const
{
    BIO_ptr outbio(BIO_new(BIO_s_mem()), &BIO_free_all);
    if(!PEM_write_bio_ECPrivateKey(outbio.get(), m_key.get(), NULL, NULL, 0, 0,NULL))
        throw std::runtime_error("Error writting private key");

    const int privKeyLen = BIO_pending(outbio.get());
    std::string privkey_str(privKeyLen, '0');
    BIO_read(outbio.get(), (void*)&(privkey_str.front()), privKeyLen);

    return std::move(privkey_str);
}


// export key in PEM encrypted format
std::string AsymKeyImpl::getPrivateKeyPEMEncrypted( const std::string& passphrase ) const
{

    int length = passphrase.length( ) ;
    std::unique_ptr < unsigned char[] > passPhrasePtr ( new unsigned char [ length + 1 ] ) ;

    std::fill_n( passPhrasePtr.get(), length+1, 0x00 ) ;

    int index = 0;
    for ( 
            std::string::const_iterator iter = passphrase.begin() ;  
            iter != passphrase.end() ; 
            ++ iter, ++index
        )
    {
        passPhrasePtr.get()[ index ] = *iter ;
    }
    
    BIO_ptr outbio(BIO_new(BIO_s_mem()), &BIO_free_all);

    if  (
            !PEM_write_bio_ECPrivateKey
            ( 
                outbio.get(),           // pointer to the I/O structure
                m_key.get(),            // the key < publickey, EC_POINT >
                EVP_aes_256_cbc(),      // EVP_Cipher object
                passPhrasePtr.get(),
                length,                 // length of key buffer
                0,                      // function that obtains the password (callback)
                NULL                    // arguments for callback above
            ) 
        )
        throw std::runtime_error("Error exporting private key");

    const int privKeyLen = BIO_pending(outbio.get());
    std::string privkey_str(privKeyLen, '0');
    BIO_read(outbio.get(), (void*)&(privkey_str.front()), privKeyLen);

    return std::move(privkey_str);
}

// import key in PEM Encrypted format
void AsymKeyImpl::setPrivateKeyPEMEncrypted( const std::string& encryptedPEM, const std::string& passphrase )
{

    int length = passphrase.length( ) ;
    std::unique_ptr < unsigned char[] > passPhrasePtr ( new unsigned char [ length + 1 ] ) ;
    
    std::fill_n( passPhrasePtr.get(), length+1, 0x00 ) ;
    
    int index = 0;
    for ( 
            std::string::const_iterator iter = passphrase.begin() ;  
            iter != passphrase.end() ; 
            ++ iter, ++index
        )
    {
        passPhrasePtr.get()[ index ] = *iter ;
    }


    BIO_ptr bio(BIO_new(BIO_s_mem()), &BIO_free_all);
    const int bio_write_ret = BIO_write(bio.get(), static_cast<const char*>(encryptedPEM.c_str()), (int)encryptedPEM.size());
    if (bio_write_ret <= 0)
        throw std::runtime_error("Error reading PEM string");

    EC_KEY* imported_EC_KEY = nullptr;

    if  ( 
            !PEM_read_bio_ECPrivateKey
            (
                bio.get(), 
                &imported_EC_KEY, 
                NULL, 
                passPhrasePtr.get()
            )
        )

        throw std::runtime_error("Error importing private key");
    
    m_key.reset(imported_EC_KEY);

    /// Get private key
    const BIGNUM* pBN = EC_KEY_get0_private_key(m_key.get());
    if (pBN == nullptr)
        throw std::runtime_error("Unable to get private key");

    const EC_GROUP* pEC_GROUP = EC_KEY_get0_group(m_key.get());
    if (pEC_GROUP == nullptr)
        throw std::runtime_error("Unable to get EC key group");

    EC_POINT_ptr pEC_POINT(EC_POINT_new(pEC_GROUP), &EC_POINT_free);
    BN_CTX_ptr pCTX_mul(BN_CTX_new(), &BN_CTX_free);
    if (!EC_POINT_mul(pEC_GROUP, pEC_POINT.get(), pBN, nullptr, nullptr, pCTX_mul.get()))
        throw std::runtime_error("Unable to calculate public key");
    
    if (!EC_KEY_set_public_key(m_key.get(), pEC_POINT.get()))
        throw std::runtime_error("Unable to set public key");
}



void AsymKeyImpl::setPEMPrivateKey(const std::string& crPEMStr)
{
    BIO_ptr bio(BIO_new(BIO_s_mem()), &BIO_free_all);
    const int bio_write_ret = BIO_write(bio.get(), static_cast<const char*>(crPEMStr.c_str()), (int)crPEMStr.size());
    if (bio_write_ret <= 0)
        throw std::runtime_error("Error reading PEM string");

    EC_KEY* imported_EC_KEY = nullptr;

    if (!PEM_read_bio_ECPrivateKey(bio.get(), &imported_EC_KEY, NULL, NULL))
        throw std::runtime_error("Error reading private key");
    m_key.reset(imported_EC_KEY);

    /// Get private key
    const BIGNUM* pBN = EC_KEY_get0_private_key(m_key.get());
    if (pBN == nullptr)
        throw std::runtime_error("Unable to get private key");

    const EC_GROUP* pEC_GROUP = EC_KEY_get0_group(m_key.get());
    if (pEC_GROUP == nullptr)
        throw std::runtime_error("Unable to get EC key group");

    EC_POINT_ptr pEC_POINT(EC_POINT_new(pEC_GROUP), &EC_POINT_free);
    BN_CTX_ptr pCTX_mul(BN_CTX_new(), &BN_CTX_free);
    if (!EC_POINT_mul(pEC_GROUP, pEC_POINT.get(), pBN, nullptr, nullptr, pCTX_mul.get()))
        throw std::runtime_error("Unable to calculate public key");
    
    if (!EC_KEY_set_public_key(m_key.get(), pEC_POINT.get()))
        throw std::runtime_error("Unable to set public key");
}

void AsymKeyImpl::setHEXPrivateKey(const std::string& crHEXKey)
{
    BIGNUM* pBN = nullptr;
    if(!BN_hex2bn(&pBN, crHEXKey.c_str()))
        throw std::runtime_error("Error importing private key from HEX");
    BIGNUM_ptr priv_key(pBN, ::BN_free);

    if (!EC_KEY_set_private_key(m_key.get(), priv_key.get()))
        throw std::runtime_error("Unable to set private key from HEX");

    const EC_GROUP* pEC_GROUP = EC_KEY_get0_group(m_key.get());
    if (pEC_GROUP == nullptr)
        throw std::runtime_error("Unable to get EC key group");

    EC_POINT_ptr pEC_POINT(EC_POINT_new(pEC_GROUP), &EC_POINT_free);
    BN_CTX_ptr pCTX_mul(BN_CTX_new(), &BN_CTX_free);
    if (!EC_POINT_mul(pEC_GROUP, pEC_POINT.get(), pBN, nullptr, nullptr, pCTX_mul.get()))
        throw std::runtime_error("Unable to calculate public key");

    if (!EC_KEY_set_public_key(m_key.get(), pEC_POINT.get()))
        throw std::runtime_error("Unable to set public key");
}

std::string AsymKeyImpl::getSharedSecretHex(const std::string& crOtherPublicPEMKey) const
{
    BIO_ptr bio(BIO_new(BIO_s_mem()), &BIO_free_all);
    const int bio_write_ret = BIO_write(bio.get(), static_cast<const char*>(crOtherPublicPEMKey.c_str()), (int)crOtherPublicPEMKey.size());
    if (bio_write_ret <= 0)
        throw std::runtime_error("Error reading PEM public key");

    /// Import the public key
    EC_KEY* raw_tmp_ec = nullptr;
    if (!PEM_read_bio_EC_PUBKEY(bio.get(), &raw_tmp_ec, NULL, NULL))
        throw std::runtime_error("Error reading public key when verifying signature");
    EC_KEY_ptr pEC_KEY(raw_tmp_ec, &EC_KEY_free);// wrap to unique_ptr for safety
    EC_KEY_set_asn1_flag(pEC_KEY.get(), OPENSSL_EC_NAMED_CURVE);

    /// Get group generator point
    const EC_GROUP* pEC_GROUP = EC_KEY_get0_group(pEC_KEY.get());
    if (pEC_GROUP == nullptr)
        throw std::runtime_error("Unable to import EC key group");
    /// Get group generator point
    const EC_GROUP* mEC_GROUP = EC_KEY_get0_group(m_key.get());
    if (mEC_GROUP == nullptr)
        throw std::runtime_error("Unable to get EC key group");

    {/// Check EC Group are compatible
        BN_CTX_ptr pCTX(BN_CTX_new(), &BN_CTX_free);
        if (0 != EC_GROUP_cmp(pEC_GROUP, mEC_GROUP, pCTX.get()))
            throw std::runtime_error("Error calculating shared secret, incompatible elliptic curve group");;
    }

    const EC_POINT* pEC_GENERATOR = EC_GROUP_get0_generator(pEC_GROUP);
    if (pEC_GENERATOR == nullptr)
        throw std::runtime_error("Unable to get key group generator");

    /// Get public key
    const EC_POINT* their_pub_key = EC_KEY_get0_public_key(pEC_KEY.get());
    if (their_pub_key == nullptr)
        throw std::runtime_error("Unable to import public EC key");

    /// Get private key
    const BIGNUM* my_private_key = EC_KEY_get0_private_key(m_key.get());
    if (my_private_key == nullptr)
        throw std::runtime_error("Unable to get private key");

    EC_POINT_ptr shared_secret_point(EC_POINT_new(pEC_GROUP), &EC_POINT_free);
    BN_CTX_ptr pCTX_mul(BN_CTX_new(), &BN_CTX_free);
    if (!EC_POINT_mul(pEC_GROUP, shared_secret_point.get(), nullptr, their_pub_key, my_private_key, pCTX_mul.get()))
        throw std::runtime_error("Unable to calculate shared secret");

    BIGNUM_ptr shared_secret_x(BN_new(),&BN_free);
    BN_CTX_ptr pCTX_get(BN_CTX_new(), &BN_CTX_free);
    if(!EC_POINT_get_affine_coordinates_GFp(pEC_GROUP, shared_secret_point.get(), shared_secret_x.get(), nullptr, pCTX_get.get()))
        throw std::runtime_error("Unable to get x coordinate of shared secret");

    STR_ptr pStr(BN_bn2hex(shared_secret_x.get()), &help_openssl_free_char);
    const std::string shared_secret_x_str(pStr.get());
    return std::move(shared_secret_x_str);
}

AsymKeyImpl* AsymKeyImpl::derive_private(const std::string& crAdditiveMsg) const
{
    const EC_GROUP* pEC_GROUP = EC_KEY_get0_group(m_key.get());
    if (pEC_GROUP == nullptr)
        throw std::runtime_error("Unable to get EC key group");

    const EC_POINT* pEC_GENERATOR = EC_GROUP_get0_generator(pEC_GROUP);
    if (pEC_GENERATOR == nullptr)
        throw std::runtime_error("Unable to get key group generator");

    BIGNUM_ptr n(BN_new(), ::BN_free);// group order
    BN_CTX_ptr pCTX_get_order(BN_CTX_new(), &BN_CTX_free);
    if(!EC_GROUP_get_order(pEC_GROUP, n.get(), pCTX_get_order.get()))
        throw std::runtime_error("Unable to get key group order");

    const std::string hashed_msg = _do_hash_msg(crAdditiveMsg);
    BIGNUM* pBN = nullptr;
    if (!BN_hex2bn(&pBN, hashed_msg.c_str()))
        throw std::runtime_error("Unable to hash message as additive big number");
    BIGNUM_ptr additive_bn(pBN, &BN_free);

    /// Get private key
    const BIGNUM* my_private_key = EC_KEY_get0_private_key(m_key.get());
    if (my_private_key == nullptr)
        throw std::runtime_error("Unable to get private key");

    BIGNUM_ptr calc_private_key(BN_new(), ::BN_free);
    BN_CTX_ptr pCTX_add(BN_CTX_new(), &BN_CTX_free);
    //if (!BN_add(calc_private_key.get(), my_private_key, additive_bn.get()))
    if (!BN_mod_add(calc_private_key.get(), my_private_key, additive_bn.get(), n.get(), pCTX_add.get()))
        throw std::runtime_error("Unable to add big number on ec curve");

    EC_POINT_ptr calc_public_key(EC_POINT_new(pEC_GROUP), &EC_POINT_free);
    BN_CTX_ptr pCTX_mul(BN_CTX_new(), &BN_CTX_free);
    if (!EC_POINT_mul(pEC_GROUP, calc_public_key.get(), calc_private_key.get(), nullptr, nullptr, pCTX_mul.get()))
        throw std::runtime_error("Unable to calculate public key");

    //// Setup the new key structure =====================================================
    AsymKeyImpl* new_key = new AsymKeyImpl(GroupNid());

    EC_KEY_set_asn1_flag(new_key->m_key.get(), OPENSSL_EC_NAMED_CURVE);
    if (!EC_KEY_set_public_key(new_key->m_key.get(), calc_public_key.get()))
        throw std::runtime_error("Error assining public ec key");

    if (!EC_KEY_set_private_key(new_key->m_key.get(), calc_private_key.get()))
        throw std::runtime_error("Unable to set private key for the new key");

    return new_key;
}

std::pair<std::string, std::string> AsymKeyImpl::sign(const std::string& crMsg)const
{
    const std::string msg_hash = _do_hash_msg(crMsg);
    SIG_ptr pSig (ECDSA_do_sign((const unsigned char*)msg_hash.c_str(), (int)strlen(msg_hash.c_str()), m_key.get()), &ECDSA_SIG_free);
    if (pSig ==nullptr)
        throw std::runtime_error("error signing message");

    const BIGNUM* bnR = ECDSA_SIG_get0_r(pSig.get());
    const BIGNUM* bnS = ECDSA_SIG_get0_s(pSig.get());

    STR_ptr rStr(BN_bn2hex(bnR), &help_openssl_free_char);
    STR_ptr sStr(BN_bn2hex(bnS), &help_openssl_free_char);
    const std::string r_hex_str(rStr.get());
    const std::string s_hex_str(sStr.get());

    return std::move(std::make_pair(std::move(r_hex_str), std::move(s_hex_str)));
}

// split the key into multiple parts
std::vector<KeyShare> AsymKeyImpl::split (const int& threshold, const int& maxshares){
    BigNumber mod; 
    // from secp256k1 curve
    mod.FromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141"); 
/// Get private key
    const BIGNUM* my_private_key = EC_KEY_get0_private_key(m_key.get());  
    if (my_private_key == nullptr)
        throw std::runtime_error("Unable to get private key");

    char *charVal= BN_bn2hex(my_private_key);
    std::string val = charVal;
    OPENSSL_free(charVal);
    BigNumber bignum;
    bignum.FromHex(val); 
    

    int degree = threshold-1;
    Polynomial poly(degree, mod,bignum);
    std::vector<KeyShare> shares = make_shared_secret (poly, threshold,maxshares);
    
    return shares; 
}
// recover a key from multiple shares
void AsymKeyImpl::recover (const std::vector<KeyShare>& shares){
    BigNumber mod; 
    // from secp256k1 curve
    mod.FromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141");
    BigNumber secret; 
    secret.Zero(); 
    try
    {
        secret = RecoverSecret(shares, mod); 
        setHEXPrivateKey (secret.ToHex());
    }
    catch(std::exception& err){
        throw;
    }
    return ;
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

std::string impl_derive_pubkey(const std::string& crPubPEMkey, const std::string& crRandomMsg)
{
    //// Import the key structure from PEM =====================================================
    BIO_ptr bio(BIO_new(BIO_s_mem()), &BIO_free_all);
    const int bio_write_ret = BIO_write(bio.get(), static_cast<const char*>(crPubPEMkey.c_str()), (int)crPubPEMkey.size());
    if (bio_write_ret <= 0)
        throw std::runtime_error("Error reading PEM string");

    /// Import the public EC key
    EC_KEY* raw_tmp_ec = nullptr;
    if (!PEM_read_bio_EC_PUBKEY(bio.get(), &raw_tmp_ec, NULL, NULL))
        throw std::runtime_error("Error reading public key when verifying signature");
    EC_KEY_ptr imported_ec_key(raw_tmp_ec, &EC_KEY_free);// wrap to unique_ptr for safety
    EC_KEY_set_asn1_flag(imported_ec_key.get(), OPENSSL_EC_NAMED_CURVE);

    //// Calculate shared secret using elliptic curve ===========================================
    using EC_POINT_ptr = std::unique_ptr< EC_POINT, decltype(&EC_POINT_free) >;
    using BN_CTX_ptr = std::unique_ptr<BN_CTX, decltype(&BN_CTX_free)>;
    using BIGNUM_ptr = std::unique_ptr<BIGNUM, decltype(&BN_free)>;

    /// Get group generator point
    const EC_GROUP* imported_group = EC_KEY_get0_group(imported_ec_key.get());
    if (imported_group == nullptr)
        throw std::runtime_error("Unable to import EC key group");

    /// Get public key
    const EC_POINT* imported_pub_key = EC_KEY_get0_public_key(imported_ec_key.get());
    if (imported_pub_key == nullptr)
        throw std::runtime_error("Unable to import public EC key");

    const EC_POINT* imported_generator = EC_GROUP_get0_generator(imported_group);
    if (imported_generator == nullptr)
        throw std::runtime_error("Unable to get public key group generator");

    const std::string hashed_msg = _do_hash_msg(crRandomMsg);
    BIGNUM* pBN = nullptr;
    if(!BN_hex2bn(&pBN, hashed_msg.c_str()))
        throw std::runtime_error("Unable to hash message as additive bignumber");
    BIGNUM_ptr additive_bn(pBN, &BN_free);

    EC_POINT_ptr additive_point(EC_POINT_new(imported_group), &EC_POINT_free);
    BN_CTX_ptr pCTX_mul(BN_CTX_new(), &BN_CTX_free);
    if (!EC_POINT_mul(imported_group, additive_point.get(), additive_bn.get(), nullptr, nullptr, pCTX_mul.get()))
        throw std::runtime_error("Unable to calculate additive point");

    EC_POINT_ptr ret_point(EC_POINT_new(imported_group), &EC_POINT_free);
    BN_CTX_ptr pCTX_add(BN_CTX_new(), &BN_CTX_free);
    if (!EC_POINT_add(imported_group, ret_point.get(), imported_pub_key, additive_point.get(), pCTX_add.get()))
        throw std::runtime_error("Unable to add point");


    //// Setup the new key structure and output it as PEM =====================================================
    const int imported_nid = EC_GROUP_get_curve_name(imported_group);
    EC_KEY_ptr ret_ec_key(EC_KEY_new_by_curve_name(imported_nid), &EC_KEY_free);
    if(ret_ec_key==nullptr)
        throw std::runtime_error("Error creating ec key");

    EC_KEY_set_asn1_flag(ret_ec_key.get(), OPENSSL_EC_NAMED_CURVE);
    if(!EC_KEY_set_public_key(ret_ec_key.get(), ret_point.get()))
        throw std::runtime_error("Error assining public ec key");

    BIO_ptr outbio(BIO_new(BIO_s_mem()), &BIO_free_all);
    if (!PEM_write_bio_EC_PUBKEY(outbio.get(), ret_ec_key.get()))
        throw std::runtime_error("Error writting public key");

    const int pubKeyLen = BIO_pending(outbio.get());
    std::string pubkey_str(pubKeyLen, '0');
    BIO_read(outbio.get(), (void*)&(pubkey_str.front()), pubKeyLen);

    return std::move(pubkey_str);
}

std::pair<std::string, std::string> impl_pubkey_pem2hex(const std::string& crPubPEMkey)
{
    /// Import public key
    BIO_ptr bio(BIO_new(BIO_s_mem()), &BIO_free_all);
    const int bio_write_ret = BIO_write(bio.get(), static_cast<const char*>(crPubPEMkey.c_str()), (int)crPubPEMkey.size());
    if (bio_write_ret <= 0)
        throw std::runtime_error("Error reading PEM string when verifying signature");

    /// Import the public key
    EC_KEY* raw_tmp_ec = nullptr;
    if (!PEM_read_bio_EC_PUBKEY(bio.get(), &raw_tmp_ec, NULL, NULL))
        throw std::runtime_error("Error reading public key when verifying signature");
    EC_KEY_ptr pEC(raw_tmp_ec, &EC_KEY_free);// wrap to unique_ptr for safety
    EC_KEY_set_asn1_flag(pEC.get(), OPENSSL_EC_NAMED_CURVE);

    /// Get group generator point
    const EC_GROUP* pEC_GROUP = EC_KEY_get0_group(pEC.get());
    if (pEC_GROUP == nullptr)
        throw std::runtime_error("Unable to import EC key group");

    const EC_POINT* pEC_POINT = EC_KEY_get0_public_key(pEC.get());
    if (pEC_POINT == nullptr)
        throw std::runtime_error("Unable to import EC key point");

    BIGNUM_ptr x(BN_new(), &BN_free);
    BIGNUM_ptr y(BN_new(), &BN_free);
    BN_CTX_ptr pCTX_get(BN_CTX_new(), &BN_CTX_free);
    if (!EC_POINT_get_affine_coordinates_GFp(pEC_GROUP, pEC_POINT, x.get(), y.get(), pCTX_get.get()))
        throw std::runtime_error("Unable to get x coordinate of shared secret");

    STR_ptr xStr(BN_bn2hex(x.get()), &help_openssl_free_char);
    STR_ptr yStr(BN_bn2hex(y.get()), &help_openssl_free_char);
    const std::string x_hex_str(xStr.get());
    const std::string y_hex_str(yStr.get());

    return std::move(std::make_pair(std::move(x_hex_str), std::move(y_hex_str)));
}
