#include <vector>
#include <tuple>
#include <string>
#include <iostream>
#include "ECPoint/ECPoint.h"
#include "BigNumbers/BigNumbers.h"
#include "SymEncDec/conversions.h"
#include "MessageHash/MessageHash.h"
#include "AsymKey/AsymKey.h"
#include <BCHAddressInfo.h>

/*
g++ -o KeyTest -I. -I /Users/j.murphy/nChain/SDK-CLEAN/sdklibraries-john/src/core KeyTest.cpp cashaddr.cpp BCHAddressInfo.cpp -I /Users/j.murphy/nChain/SDK-CLEAN/build/generated_hpp -L /Users/j.murphy/nChain/SDK-CLEAN/build/x64/release -lBigNumbers -lECPoint -lPolynomial -lAsymKey -lSymEncDec -lMessageHash -lssl -lcrypto -ldl -pthread -std=c++17
*/

int main(int argc,char * argv[]){

    ECPoint ecPt; 
    ECPoint ecPt2; 
    ECPoint ecPt3; 

    std::vector<std::tuple<int, std::string, std::string>> curveList = getCurveList();
    for (const auto &curveTuple : curveList)
    {
        std::cout << "Curve ID: " << std::get<0>(curveTuple) << "\tCurve String: " <<  std::get<1>(curveTuple) << std::endl;
    }
    std::cout << std::endl;


    ecPt.SetRandom(); 
    ecPt2.SetRandom(); 
    ecPt3.SetRandom(); 

    
    std::cout << "Value of random ECPoint: " << ecPt.ToHex() << "\n"
                    << "Value of random ECPoint2: " << ecPt2.ToHex() << "\n"
                    << "Value of random ECPoint3: " << ecPt3.ToHex () << std::endl; 
        
    std::cout << "Random Elliptic Curve Point P: " << std::endl; 
    
    std::pair<std::string, std::string> coord = ecPt.GetAffineCoords_GFp(); 
    std::cout << "x = " << coord.first << "\n" << "y = " << coord.second << std::endl;
    

    std::cout << "Try adding: " << std::endl; 

    ECPoint ecPtRes = ecPt + ecPt2 ; 

    std::cout << ecPtRes.ToHex() << std:: endl;

    std::cout << "Try doubling..." << std::endl; 
    ECPoint doublePtrRes = ecPtRes.Double () ; 
   
    std::cout <<  doublePtrRes.ToHex () << std::endl ; 

    std::cout << "Try inverting..." << std::endl; 
    doublePtrRes.Invert () ; 
    std::cout << doublePtrRes.ToHex () << std::endl; 

    bool isOnCurve = doublePtrRes.CheckOnCurve() ; 
    if (isOnCurve){
        std::cout << "Inverted Point on curve" << std::endl ; 
    }else{
        std::cout << "Inverted Point NOT on curve" << std::endl ; 
    }

    std::cout << "Multiply.." << std::endl; 


    BigNumber bnValm ; 
    bnValm.generateRandHex(256) ;
 
    BigNumber bnValn ; 
    bnValn.generateRandHex(256) ;
    
    
    ECPoint mulres = doublePtrRes.MulHex (bnValm.ToHex (), bnValn.ToHex () ); 
    std::cout << "Multiply result: " << mulres.ToHex () << std::endl; 

    bool isMulResOnCurve = doublePtrRes.CheckOnCurve () ; 
    if (isMulResOnCurve){
        std::cout << "Result of multiplication on the curve " << std::endl ; 
    }else{
        std::cout << "Result of multiplication  NOT on curve" << std::endl ; 
    }
    
    std::string ecPointasHexStr = ecPt3.ToHex() ; 
    ECPoint newValFromStr  ; 
    std::cout << "Constructing a new ECPoint from ECPoint 3 above ... the x & y coordinates should match" << std::endl;
    newValFromStr.FromHex (ecPointasHexStr) ; 

    std::pair<std::string, std::string> coord3 = ecPt3.GetAffineCoords_GFp(); 
    std::cout << "x = " << coord3.first << "\n" << "y = " << coord3.second << std::endl;

    std::pair<std::string, std::string> coordNewVal = newValFromStr.GetAffineCoords_GFp(); 
    std::cout << "x = " << coordNewVal.first << "\n" << "y = " << coordNewVal.second << std::endl; 

    // testing out the compare fucntion

    if ( ecPt3 == newValFromStr ) {
        std::cout << "Points are equal " << std::endl ; 
    }  

    if ( ecPt3 != newValFromStr ){
        std::cout << "These points are definately the same...shoud never see this" << std::endl ; 
    }

    if (ecPt3 != ecPt2){
        std::cout << "Randomly generated points should not be equal ..." << std::endl; 
    }
   
	
	ECPoint nonDefaultCurvePt(704);
	
	
	
	std::cout << "Working with Keys" << std::endl;
	
	
	
	ECPoint myPubkey;
	myPubkey.SetRandom();
	std::cout << "Compressed public key is " << myPubkey.ToHex() << std::endl;
	std::cout << "Uncompressed public key is " << myPubkey.ToHex(false) << std::endl; 
	std::cout << "Compressed publci key in DEC is " << myPubkey.ToDec() << std::endl;
	std::cout << "Unompressed public key in DEC is " << myPubkey.ToDec(false) << std::endl;
	
	std::cout << myPubkey.GetAffineCoords_GFp().first << " " << myPubkey.GetAffineCoords_GFp().second << std::endl;
	
	std::cout << "Calling convert to bin" << std::endl;
	std::vector<uint8_t> UnCompressedPubAsBin = HexToUInt(myPubkey.ToHex(false));
	
	MessageHash mh; 
	mh.Hash(UnCompressedPubAsBin,std::string("sha256"));
	
	std::vector<uint8_t> HashedUNCompPub = mh.HashValueBytes();
	
	
	std::cout << "Size of HashedUNCompPub " << HashedUNCompPub.size() << std::endl;
	
	MessageHash mh1; 
	mh1.Hash(HashedUNCompPub,std::string("ripemd160"));
	std::vector<uint8_t> HashedHashedUNcompPub = mh1.HashValueBytes();
	std::cout << "Size of HashedUNCompPub " << HashedHashedUNcompPub.size() << std::endl;
	
	
    std::string pubKeyX = UintToHex(UnCompressedPubAsBin);
    std::cout << pubKeyX << std::endl;
    
    ECPoint recoverKey;
    recoverKey.FromHex(pubKeyX);
    
    if (recoverKey == myPubkey){
        std::cout << "SUCCESS" << std::endl;
    }
    
    std::cout << EnumAsUnderlying(VersionConfig::Instance()->getNetwork(VersionPrefix::TESTNET_PUBLIC_KEY_ADDRESS)) << std::endl;
    
    
    
    BigNumber rValue;
    rValue.FromHex ("8256CC314D0AB79CDB40D3E1909E654E012DC9AE460A994E72C8F1A0897011D0");
    BigNumber sValue;
    sValue.FromHex ("0714C2452997F71538541D376B37D1F5C626ED534008569C678CFFECFA98D07A");
    
    size_t lenSigDer(-1);
    std::unique_ptr<unsigned char[]>  sigDER = DEREncodedSignature(rValue, sValue,lenSigDer);
    std::string sigDerHex = binTohexStr (sigDER,lenSigDer);
    
    
   
    /*
    30450221008256cc314d0ab79cdb40d3e1909e654e012dc9ae460a994e72c8f1a0897011d002200714c2452997f71538541d376b37d1f5c626ed534008569c678cffecfa98d07a    
    */
    
    
    std::cout << sigDerHex << std::endl;
    
    const AsymKey randomKey;
    const std::string random_str = randomKey.getPublicKeyHEXStr();

    const AsymKey ecdsa;
    const std::string pubkey = ecdsa.getPublicKeyPEM();
    const std::pair<std::string, std::string> rs = ecdsa.sign(random_str);
    
    BigNumber rTest;
    rTest.FromHex(rs.first);
    BigNumber sTest;
    sTest.FromHex(rs.second);
    
    size_t len(-1); 
    std::unique_ptr<unsigned char[]>  sigDERTest = DEREncodedSignature(rTest,sTest,len);
    
    std::string sigAsHex = binTohexStr(sigDERTest,len);
    std::cout << sigAsHex << std::endl;

    
    
    
    
     const bool verify_ok = verify(random_str, pubkey, rs);
    const bool verify_ok_der = verifyDER(random_str, pubkey, sigDERTest, len);
    
    if(verify_ok)
        std::cout << "Message verifed via r&s " << std::endl;
        
    if(verify_ok_der)
        std::cout << "Message verified via DER format " << std::endl;
    
    
    return 0; 
}
