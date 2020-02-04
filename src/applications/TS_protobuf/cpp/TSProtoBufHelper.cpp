#include <exception>
#include <sstream>
#include <TSProtoBufHelper.h>


std::string enum2string( const thresholdsignature::CalcType& ct){
    if( ct == thresholdsignature::PRIVATEKEYSHARE )
        return "PRIVATEKEYSHARE" ;
    else if( ct == thresholdsignature::LITTLEK )
        return  "LITTLEK" ;
    else if( ct == thresholdsignature::ALPHA )
        return "ALPHA" ;
    else if( ct == thresholdsignature::UNSET)
        return "UNSET";
    else{
        std::stringstream err;
        err << "ERROR: unknown calculation: " << ct << std::endl ;
        throw std::runtime_error (err.str());
    }
}

thresholdsignature::CalcType string2enum (const std::string& ctStr){
    if( ctStr == "PRIVATEKEYSHARE" )
    {
        return thresholdsignature::PRIVATEKEYSHARE ;
    }
    else if( ctStr ==  "LITTLEK" )
    {
        return thresholdsignature::LITTLEK ;
        
    }
    else if( ctStr ==  "ALPHA" )
    {
        return thresholdsignature::ALPHA ;
    }
    else if (ctStr == "UNSET"){
        return thresholdsignature::UNSET;
    }
    else{
        std::stringstream err;
        err <<"ERROR: unknown calculation: " << ctStr << std::endl ;
        throw std::runtime_error(err.str());
    }
}
