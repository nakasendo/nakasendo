#include <BCHAddress/BCHAddress.h>
#include <BCHAddress/BCHAddressImpl.h>


BCHAddress::BCHAddress( const std::string& address ) 
    : m_pImpl( new BCHAddressImpl( address ) )
{
}

BCHAddress::BCHAddress( const std::string& publicKey,  VersionPrefix version ) 
    : m_pImpl( new BCHAddressImpl(  publicKey, version) )
{
}

BCHAddress::~BCHAddress( ) 
{
}


bool BCHAddress::valid( ) const 
{
    return m_pImpl->valid( ) ;
}

VersionPrefix BCHAddress::getVersionPrefix( ) const 
{
    return m_pImpl->getVersionPrefix( ) ;
}

std::string BCHAddress::getAddress( ) const 
{
    return m_pImpl->getAddress( ) ; 
}

std::pair< std::string, std::string >  BCHAddress::getVersion( ) const
{
    return m_pImpl->getVersion( ) ; 
}

std::string BCHAddress::decode( ) const 
{
    return m_pImpl->decode( ) ; 
}

std::ostream& operator<< ( std::ostream& out, const BCHAddress& address )
{
    out <<  *address.m_pImpl.get() ;

    return out ;
}