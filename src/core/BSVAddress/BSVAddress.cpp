#include <BSVAddress/BSVAddress.h>
#include <BSVAddress/BSVAddressImpl.h>


BSVAddress::BSVAddress( const std::string& address ) 
    : m_pImpl( new BSVAddressImpl( address ) )
{
}

BSVAddress::BSVAddress( const std::string& publicKey,  VersionPrefix version ) 
    : m_pImpl( new BSVAddressImpl(  publicKey, version) )
{
}

BSVAddress::~BSVAddress( ) 
{
}


bool BSVAddress::valid( ) const 
{
    return m_pImpl->valid( ) ;
}

VersionPrefix BSVAddress::getVersionPrefix( ) const 
{
    return m_pImpl->getVersionPrefix( ) ;
}

std::string BSVAddress::getAddress( ) const 
{
    return m_pImpl->getAddress( ) ; 
}

std::pair< std::string, std::string >  BSVAddress::getVersion( ) const
{
    return m_pImpl->getVersion( ) ; 
}

std::string BSVAddress::decode( ) const 
{
    return m_pImpl->decode( ) ; 
}

std::ostream& operator<< ( std::ostream& out, const BSVAddress& address )
{
    out <<  *address.m_pImpl.get() ;

    return out ;
}