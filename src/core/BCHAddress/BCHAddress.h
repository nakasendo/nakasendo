#ifndef BCH_ADDRESS_H
#define BCH_ADDRESS_H

#include <memory> 
#include <BCHAddress/BCHAddressInfo.h>
#include <BCHAddress/BCHAddressConfig.h>


class BCHAddressImpl ;

class BCHAddress_API BCHAddress
{

    public: 

        // Create an address object from an existing address
        BCHAddress( const std::string& ) ;

        // Create an address from public key and version prefix
        BCHAddress( const std::string&,  VersionPrefix ) ;
        
        ~BCHAddress( ) ;

        // Returns the BCH address as a string
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
        friend BCHAddress_API std::ostream&  operator<< ( std::ostream&, const BCHAddress& ) ;


    private:
        
        BCHAddress(const BCHAddress& ) ;
        BCHAddress& operator=(const BCHAddress& ) ;
        std::unique_ptr< BCHAddressImpl > m_pImpl ;

} ;


#endif // BCH_ADDRESS_H
