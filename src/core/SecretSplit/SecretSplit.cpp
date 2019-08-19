#include <string>
#include <sstream>
#include <Polynomial/Polynomial.h>
#include <BigNumbers/BigNumbers.h>
#include <MessageHash/MessageHash.h>
#include <Polynomial/LGInterpolator.h>
#include <SecretSplit/KeyShare.h>
#include <SecretSplit/SecretSplit.h>

std::string CreateUUID (){
    // Generate random value
    BigNumber ranVal;
    std::string uuidVal = ranVal.generateRandHex(32 * 8);
    MessageHash hashMess;
    hashMess.HashSha256 (uuidVal);
    return( hashMess.HashHex());
}

std::vector<KeyShare> make_shared_secret (const Polynomial& poly, const int& minimum, const int& shares){
    //    Generates a random shamir pool, returns the secret and the share points.
    
    if (minimum > shares){
        std::stringstream strerr;
        strerr << "pool secret would be irrecoverable";
        std::runtime_error err (strerr.str());
        throw err;
    }
   
    std::string uuid = CreateUUID(); 
     
    std::vector<KeyShare> shareValues; 
    for (int i = 1; i < (shares + 1); ++i){
        BigNumber xValue;
        xValue.FromDec ( std::to_string(i));
        BigNumber val = poly(xValue) ; 
        KeyShare share;
        share.k() = minimum;
        share.n() = shares; 
        share.publicID () = uuid;
        share.Index() = xValue ; 
        share.Share() = val; 
        shareValues.push_back (share);
    }
    
    
    return shareValues ; 
}

BigNumber RecoverSecret ( const std::vector<KeyShare>& shares , const BigNumber& mod){
    if (shares.size() < 2){
        std::stringstream strerr;
        strerr << "At least two shares are required to recover a secret" << std::endl;
        std::runtime_error err (strerr.str()); 
        throw err; 
    }
    // we need to build an std::vector<std::pair<BigNumber, BigNumber> > from the shares 
    std::vector<std::pair<BigNumber, BigNumber> > curvePoints;
    int k(0), n(0); 
    std::string uuid; 
    for (std::vector<KeyShare>::const_iterator iter = shares.begin(); iter != shares.end(); ++ iter){
        // first time around .. populate the values
        if(k == 0 && n == 0 && uuid.empty()){
            k = iter->k(); 
            n = iter->n();
            uuid = iter->publicID();
        }else{
            if ( k != iter->k() || n != iter->n() || uuid != iter->publicID()){
                std::stringstream errMsg;
                errMsg << "Invalid share provided for share group" ; 
                std::runtime_error err (errMsg.str()); 
                throw err; 
            }
        }
        curvePoints.push_back(std::make_pair(iter->Index(), iter->Share()));
    }
    if(curvePoints.size() < k){
        std::stringstream errmsg;
        errmsg << "inconsistant number of shares supplied: " << curvePoints.size() << " less than " << k << std::endl;
        std::runtime_error err(errmsg.str());
        throw err;
    }
    LGInterpolator interpFunc(curvePoints, mod);
    BigNumber zero;
    zero.Zero();
    BigNumber interpValViaFunc = interpFunc(zero); 
    return interpValViaFunc ; 
}


