#ifndef BCH_ADDRESS_IMPL_H
#define BCH_ADDRESS_IMPL_H

#include <string>
#include <BCHAddress/BCHAddressInfo.h>

class BCHAddressImpl
{
    public: 

        BCHAddressImpl ( const std::string& ) ;
        BCHAddressImpl ( const std::string&, VersionPrefix  ) ;
        ~BCHAddressImpl ( ) = default ;


        bool operator==( const BCHAddressImpl& rhs ) const 
        { return m_address == rhs.m_address ; }


        std::string getAddress( ) const { return m_address ; }

        bool valid( ) const ;

        std::pair< std::string, std::string > getVersion( ) const ;

        VersionPrefix getVersionPrefix( ) const ;        
        std::string decode( ) const ;

        friend std::ostream& operator<< ( std::ostream&, const BCHAddressImpl& ) ;

    private:
        
        BCHAddressImpl( ) ;
        
        std::string m_address ; 

} ;

#endif // BCH_ADDRESS_IMPL_H
