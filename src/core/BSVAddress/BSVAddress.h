#ifndef BSV_ADDRESS_H
#define BSV_ADDRESS_H

#include <memory> 
#include <BSVAddress/BSVAddressInfo.h>
#include <BSVAddress/BSVAddressConfig.h>


class BSVAddressImpl ;

class BSVAddress_API BSVAddress
{

    public: 

        // Create an address object from an existing address
        BSVAddress( const std::string& ) ;

        // Create an address from public key and version prefix
        BSVAddress( const std::string&,  VersionPrefix ) ;
        
        ~BSVAddress( ) ;

        // Returns the BSV address as a string
        std::string getAddress( ) const ;

        // Returns a boolean  indicating the address is valid 
        // This checks the checksum, and verifies version prefix is supported
        bool valid( ) const ;

        // Returns a pair of < prefix, networkType >
        // Example:  
        std::pair< std::string, std::string >  getVersion( ) const ;

        // Returns enum type of Version Prefix, example PUBLIC_KEY_ADDRESS
        VersionPrefix getVersionPrefix( ) const ;

        // Returns the decoded string (Base58 decoding)
        std::string decode( ) const  ;

        // output operator
        BSVAddress_API friend std::ostream&  operator<< ( std::ostream&, const BSVAddress& ) ;


    private:
        
        BSVAddress(const BSVAddress& ) ;
        BSVAddress& operator=(const BSVAddress& ) ;
        std::unique_ptr< BSVAddressImpl > m_pImpl ;

} ;


#endif // BSV_ADDRESS_H
