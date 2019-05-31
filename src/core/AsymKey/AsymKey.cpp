#include <AsymKey/AsymKey.h>
#include <AsymKey/AsymKeyImpl.h>

AsymKey::~AsymKey() = default;
AsymKey::AsymKey(AsymKey&& obj) noexcept = default;
AsymKey& AsymKey::operator=(AsymKey&& obj) noexcept = default;
AsymKey::AsymKey(const AsymKey& obj)
: m_pImpl(new AsymKeyImpl(*obj.m_pImpl))
{
}

AsymKey& AsymKey::operator=(const AsymKey& obj)
{
    if (this != &obj)
    {
        m_pImpl.reset(new AsymKeyImpl(*obj.m_pImpl));
    }
    return *this;
}

AsymKey::AsymKey() : m_pImpl(new AsymKeyImpl())
{
}

int AsymKey::GroupNid()const
{
    return m_pImpl->GroupNid();
}

std::string AsymKey::getPublicKeyHEX() const
{
    return m_pImpl->getPublicKeyHEXStr();
}

std::string AsymKey::getPrivateKeyHEX() const
{
    return m_pImpl->getPrivateKeyHEXStr();
}

std::string AsymKey::getPublicKeyPEM()  const
{
    return m_pImpl->getPublicKeyPEMStr();
}

std::string AsymKey::getPrivateKeyPEM() const
{
    return m_pImpl->getPrivateKeyPEMStr();
}

void AsymKey::setPEMPrivateKey(const std::string& crPEMKey)
{
    m_pImpl->setPEMPrivateKey(crPEMKey);
}

std::string AsymKey::getSharedSecretHex(const std::string& crOtherPublicPEMKey) const
{
    return m_pImpl->getSharedSecretHex(crOtherPublicPEMKey);
}

std::pair<std::string, std::string> AsymKey::sign(const std::string& crMsg)const
{
    return m_pImpl->sign(crMsg);
}

bool AsymKey::verify(const std::string& crMsg, const std::string& crPublicKeyPEMStr, const std::pair<std::string, std::string>& rs)
{
    return AsymKeyImpl::verify(crMsg,crPublicKeyPEMStr,rs);
}