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