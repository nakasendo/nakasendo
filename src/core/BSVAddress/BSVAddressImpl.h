#ifndef BSV_ADDRESS_IMPL_H
#define BSV_ADDRESS_IMPL_H

#include <string>
#include <BSVAddress/BSVAddressInfo.h>

class BSVAddressImpl
{
    public: 

        BSVAddressImpl ( const std::string& ) ;
        BSVAddressImpl ( const std::string&, VersionPrefix  ) ;
        ~BSVAddressImpl ( ) = default ;


        bool operator==( const BSVAddressImpl& rhs ) const 
        { return m_address == rhs.m_address ; }


        std::string getAddress( ) const { return m_address ; }

        bool valid( ) const ;

        std::pair< std::string, std::string > getVersion( ) const ;

        VersionPrefix getVersionPrefix( ) const ;        
        std::string decode( ) const ;

        friend std::ostream& operator<< ( std::ostream&, const BSVAddressImpl& ) ;

    private:
        
        BSVAddressImpl( ) ;
        
        std::string m_address ; 

} ;

#endif // BSV_ADDRESS_IMPL_H
