#include <BSVAddress/BSVAddressImpl.h>
#include <BSVAddress/BSVAddressInfo.h>
#include <MessageHash/Base58EncDec.h>
#include <MessageHash/conversions.h>

#include <MessageHash/MessageHash.h>


#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <regex>

namespace BSVTypes
{
    typedef std::vector<uint8_t> byteVecType;
}

/* 
*  Construct a BSV Address from and exisiting address string, throw if not valid 
*  @param   Address
*/
BSVAddressImpl::BSVAddressImpl( const std::string& address )
{
    m_address = address ;

    if ( valid( ) )
        return ;

    std::runtime_error err ("BSVAddressImp::constructor - address is not valid ");
    throw err;
}


/* 
*  Construct a BSV Address from public key, and version prefix
*  @param   public key as a string representation of Hex
*  @param   enum type for version, e.g. PUBLIC_KEY_ADDRESS
*/
BSVAddressImpl::BSVAddressImpl( const std::string& publicKey, VersionPrefix version  ) 
{
    // convert to binary, do sha256 and ripemd160 
    BSVTypes::byteVecType publicKeyVec = HexToUInt( publicKey ) ;
    publicKeyVec = Hash( Hash( publicKeyVec, "sha256" ) , "ripemd160" );

    // prepend network ID, and save as used later in process
    // reserve space: add 6 to allow for prefix and checksum
    BSVTypes::byteVecType addressByteVec ;
    addressByteVec.reserve(  publicKeyVec.size() + 6 ) ;

    addressByteVec = VersionConfig::Instance()->getVersionBytes( version ) ;
    addressByteVec.insert( addressByteVec.end(), publicKeyVec.begin(), publicKeyVec.end() ) ;

    // create a new working vector with copy of stored data
    BSVTypes::byteVecType tempVec ;
    tempVec.reserve( addressByteVec.size() ) ;
    copy 
        ( 
            addressByteVec.begin(), 
            addressByteVec.end(), 
            back_inserter( tempVec )
        ) ;

    // do double sha256, take the first 4 bytes (this is the checksum) and append to the address
    tempVec = Hash( Hash( tempVec, "sha256" ) , "sha256" );
    
    addressByteVec.insert( addressByteVec.end(), tempVec.begin(), tempVec.begin()+4 ) ;

    // base58 encoding
    Base58EncDec encoder ;
    std::string encodedString ( encoder.encode( addressByteVec ) ) ;

    m_address = encodedString ;    
}


/* 
*  Get the version prefix of a BSV Address
*  @return enum representing Version Prefix, e.g. TESTNET_SCRIPT_ADDRESS
*/
VersionPrefix BSVAddressImpl::getVersionPrefix( ) const 
{
    Base58EncDec encoder ;
    BSVTypes::byteVecType bytes ( encoder.decode( m_address ) );

    // create a new vector with the verson bytes 
    BSVTypes::byteVecType tempVec ;
    tempVec.push_back( bytes[0]) ;

    return VersionConfig::Instance()->getVersionConfig ( tempVec ) ;
}


/* 
*  Get the string representation of the network prefix e.g. '00'
*  and the string representation of the prefix version, e.g. 'Public Key Address' 
*  @return pair of strings, e.g. <'00', 'Public Key Address'>
*/
std::pair< std::string, std::string > BSVAddressImpl::getVersion( ) const
{ 

    Base58EncDec encoder ;
    BSVTypes::byteVecType bytes ( encoder.decode( m_address ) ) ;

    // create a new vector with the verson bytes 
    BSVTypes::byteVecType tempVec ;
    tempVec.push_back( bytes[0]) ;

    std::string prefix =  UintToHex( tempVec ) ; 
    std::string version =  VersionConfig::Instance()->getReadableNetworkType( tempVec ); //("boo") ;

    return ( std::make_pair( prefix, version ) ) ;
}


/* 
*  Decode the BSV Address string (via base58 decoder) 
*  @return string representation of decoded address
*/
std::string BSVAddressImpl::decode( ) const 
{
    Base58EncDec encoder ;

    return UintToHex( encoder.decode( m_address ) ) ;
}

/* 
*  Verify the address is valid based on RegEx, Checksum and Version
*  @return boolean (valid = true; invalid = false)
*/
bool BSVAddressImpl::valid( ) const
{

    // first check the address contains valid characters and in length 14-74
    std::regex allowedChars( "^[a-km-zA-HJ-NP-Z1-9]{14,74}$" ) ;
    if ( !regex_match( m_address, allowedChars ) )
    {
        return false ;
    }

    int checksumSize ( 4 ) ;
    Base58EncDec encoder ;
    BSVTypes::byteVecType bytes ( encoder.decode( m_address ) ) ;

    if (bytes.size() < checksumSize )
    {
        return false ;
    }

    // create a new vector with the verson bytes 
    BSVTypes::byteVecType checkSumBytes ;
    
    copy 
        ( 
            bytes.end() - checksumSize, 
            bytes.end() , 
            back_inserter( checkSumBytes )
        ) ;

    // create a new vector with the bytes for which to find the checksum
    BSVTypes::byteVecType intermediateBytes ;
    intermediateBytes.reserve( bytes.size() ) ;
    copy 
        ( 
            bytes.begin(), 
            bytes.end() - checksumSize, 
            back_inserter( intermediateBytes )
        ) ;   


    // do double sha256 
    intermediateBytes = Hash( Hash( intermediateBytes, "sha256" ) , "sha256" );    

    // check the first 4 bytes are the same as the checksum
    BSVTypes::byteVecType::const_iterator check1, check2 ;
    check1 = intermediateBytes.begin() ;
    check2 = checkSumBytes.begin( ) ; 
    for ( 
            int i = 0 ;
            i < 4; 
            ++i, ++check1, ++check2
        )
    {
        if ( *check1 != *check2 )
            return false ;
    }
     
    // If we have got this far, then the checksum check is valid. 
    
    // Now check the versionByte is recognised
    BSVTypes::byteVecType versionByte ;
    versionByte.push_back( bytes[0]) ;
    try
    {
        VersionConfig::Instance()->getVersionConfig ( versionByte ) ;
    }
    catch ( std::runtime_error )
    {
        return false ;
    }

    
    return true ;
}



/* 
*  Output Operator - pretty-print the BSV Address, and derived information
*/
std::ostream& operator<< ( std::ostream& out, const BSVAddressImpl& address )
{

    std::pair< std::string, std::string > version = address.getVersion( ) ;

    address.getAddress( ) ;
    out << "BSVAddress:\t\t\t" << address.m_address  
    
        << "\n\tValid: "  ;
    if ( address.valid() )   out << "\t\t\tTrue " ; 
    else  out << "\t\t\tFalse" ; 
    out << "\n\tDecoded adddress:\t"    << address.decode( ) 
        << "\n\tNetwork prefix:\t\t"    << version.first 
        << "\n\tNetwork type:\t\t"      << version.second  
        << std::endl ;

    return out ;
}
