#include <vector>
#include <tuple>
#include <string>
#include <iostream>
#include <fstream>

#include "ECPoint/ECPoint.h"
#include "BigNumbers/BigNumbers.h"
#include "MessageHash/conversions.h"
#include "MessageHash/MessageHash.h"
#include "AsymKey/AsymKey.h"
#include "BSVAddress/BSVAddress.h"


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
                    
    std::cout << "Value of random ECPoint: " << ecPt.ToDec() << "\n"
                    << "Value of random ECPoint2: " << ecPt2.ToDec() << "\n"
                    << "Value of random ECPoint3: " << ecPt3.ToDec() << std::endl; 
        
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
    
    //std::cout << EnumAsUnderlying(VersionConfig::Instance()->getNetwork(VersionPrefix::TESTNET_PUBLIC_KEY_ADDRESS)) << std::endl;
    
    
    
    BigNumber rValue;
    rValue.FromHex ("8256CC314D0AB79CDB40D3E1909E654E012DC9AE460A994E72C8F1A0897011D0");
    BigNumber sValue;
    sValue.FromHex ("0714C2452997F71538541D376B37D1F5C626ED534008569C678CFFECFA98D07A");
    
    size_t lenSigDer(-1);
   
    std::unique_ptr<unsigned char[]>  sigDER = DEREncodedSignature(rValue, sValue,lenSigDer);
    std::cout << "I bet you £5 it's broken" << std::endl;
    std::string sigDerHex = binTohexStr (sigDER,lenSigDer);
    
    
   
    /*
    30450221008256cc314d0ab79cdb40d3e1909e654e012dc9ae460a994e72c8f1a0897011d002200714c2452997f71538541d376b37d1f5c626ed534008569c678cffecfa98d07a    
    */
    
    
    std::cout << sigDerHex << std::endl;
    
    const AsymKey randomKey;
    const std::string random_str = randomKey.exportPublicHEXStr();

    const AsymKey ecdsa;
    const std::string pubkey = ecdsa.exportPublicPEM();
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

    BigNumber TestNumber;
    TestNumber.generateRandHex();
    std::cout << TestNumber.ToHex() << std::endl;
    ECPoint genTest; 
    ECPoint GENPOINT = genTest.getGenerator();
    
    ECPoint hiddenPoint = GENPOINT.MulHex(TestNumber.ToHex(), std::string());
    std::cout << "Point in hex * G " << hiddenPoint.ToHex(false) << std::endl;
    std::pair<std::string, std::string> points = hiddenPoint.GetAffineCoords_GFp();

    std::cout << "Affine coords\n" << points.first << "\t" << points.second << std::endl;
    std::cout << "Point in PEM format\n" << pubkey_coordinates2pem(points.first, points.second) << std::endl;

    std::string PEM1 = pubkey_coordinates2pem(points.first, points.second) ;

    std::cout << "Private key/Public key PEM check" << std::endl;
    // Add the python interface to call x,y to PEM
    // verify the PEM format by converting back to x,y & comparing to the original
    // to try verify the signature
    // add tests for the new function in AsymKey


    AsymKey newKey; 
    newKey.importPrivateHEX(TestNumber.ToHex());
    std::cout << newKey.exportPublicHEXStr() << std::endl;
    std::cout << newKey.exportPublicPEM() << std::endl; 
    std::cout << newKey.exportPrivateHEX() << std::endl;
    std::string PEM2 = newKey.exportPublicPEM();
    if (PEM1 != PEM2){
        std::cout << "PANIC PEM KEYS NOT EQUAL" << std::endl;
        return -1; 
    }



    ECPoint pubKey; 
    BigNumber rValueTest;
    BigNumber sValueTest; 

    pubKey.FromHex("036351C7062588B27EB0ABC83093D9345E4C7692920A855FA397388C09BF8BA6C8");
    std::string pubKeyAsPEM = pubkey_coordinates2pem(pubKey.GetAffineCoords_GFp().first, pubKey.GetAffineCoords_GFp().second);
    rValueTest.FromHex("7872B5465A46C3858C75FB1FC7684621C3E17C096CF9A31A6CC480645BE20990");
    sValueTest.FromHex("B1164242138D07C2828CF4E71FECFFB414EA78DA828C352E3381773788065F6B"); 


    std::cout << pubKey.GetAffineCoords_GFp().first << " " <<  pubKey.GetAffineCoords_GFp().second << std::endl;
    std::cout << "Point in PEM format\n" << pubkey_coordinates2pem(pubKey.GetAffineCoords_GFp().first, pubKey.GetAffineCoords_GFp().second) << std::endl;



    //verify(const std::string& crMsg, const std::string& crPublicKeyPEMStr, const std::pair<std::string, std::string>& rs);

    const std::string message("I love deadlines. I love the whooshing noise they make as they go by.");
    std::pair<std::string, std::string> rsInfo = std::make_pair(rValueTest.ToHex(),sValueTest.ToHex());
    bool res = verify(message,pubKeyAsPEM, rsInfo );
    if(res)
        std::cout << "HURRAH" << std::endl;

    
    // hard coded test
#if 0 
    AsymKey priKeyHC; 
    priKeyHC.importPrivateHEX("B0BD078D3A70DA8407398E764712680B762EDC0AE417B71AC29DB1DB19E6135F");
    std::cout << "Public Key PEM format " << priKeyHC.exportPublicPEM() << std::endl;
    std::cout << "Public Key Hex format " << priKeyHC.exportPublicHEXStr() << std::endl;
    std::string PEM3 = priKeyHC.exportPublicPEM() ;

    if (pubKeyAsPEM != PEM3){
        std::cout <<"PANIC KEYS NOT EQUAL" << std::endl;
        return -1;
    }
    if(verify(message, PEM3, rsInfo)){
        std::cout << "HURRAH HERE" << std::endl;
        return -1;
    }



    const std::pair<std::string, std::string> rsSigTest = priKeyHC.sign(message);
    std::cout << rsSigTest.first << " " << rsSigTest.second << std::endl;
    if(verify(message,PEM3,rsSigTest )){
        std::cout << "Thats expected" << std::endl;
    }

    std::cout << "verify again with a different version of the same public key" << std::endl;
    if(verify(message,pubKeyAsPEM,rsSigTest )){
        std::cout << "Thats blown my mind" << std::endl;
        return -1;
    }
#endif
    AsymKey priKeyHCB; 
    const std::pair<std::string, std::string> rsSigTestB = priKeyHCB.sign(message);
    std::cout << rsSigTestB.first << " " << rsSigTestB.second << std::endl;
    std::cout << "Private Key Hex " << priKeyHCB.exportPrivateHEX() << std::endl;
    std::cout << "Public Key Hex " << priKeyHCB.exportPublicHEXStr() << std::endl;
    if(verify(message,priKeyHCB.exportPublicPEM(),rsSigTestB )){
        std::cout << "Message verified with raw r & s" << std::endl;
    }

    size_t derlen = 0;
    // create two bignumbers for r & s
    BigNumber rBN, sBN;
    rBN.FromHex(rsSigTestB.first);
    sBN.FromHex(rsSigTestB.second);
    std::unique_ptr<unsigned char[]> derSig = DEREncodedSignature(rBN, sBN, derlen);


    std::cout << "creating output files" << std::endl;
    if(derlen >0){
        std::ofstream outfile;
        outfile.open("sig.der",std::fstream::out | std::fstream::binary);
        if(outfile.is_open()){
            for(int index=0; index<derlen;++index){
                outfile << derSig[index] ;
            }
            outfile.close();
        }
        std::cout << "writing PEM public key" << std::endl;
        std::ofstream pemFile;
        pemFile.open("pubkey.pem",std::fstream::out | std::fstream::binary);
        if(pemFile.is_open()){
            pemFile << priKeyHCB.exportPublicPEM() ;
        }
        pemFile.close();
    }

    std::cout << "Verifiy the message with DER format" << std::endl;

    if(verifyDER(message,priKeyHCB.exportPublicPEM(),derSig,derlen)){
        std::cout << "DER format verified" << std::endl;
    }

    std::cout << "Export Pubkey in PEM format" << std::endl;
    std::ofstream prikeyOut;
    prikeyOut.open("prikey.pem",std::fstream::out | std::fstream::binary);
    if(prikeyOut.is_open()){
        prikeyOut << priKeyHCB.exportPrivatePEM() ;
    }
    prikeyOut.close();


    return 0; 
}
