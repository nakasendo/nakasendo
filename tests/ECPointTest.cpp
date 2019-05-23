#include <vector>
#include <string>
#include <iostream>
#include "ECPoint/ECPoint.h"
#include "BigNumbers/BigNumbers.h"


int main(int argc,char * argv[]){

    ECPoint ecPt; 
    ECPoint ecPt2; 
    ECPoint ecPt3; 

    std::vector<std::pair<int, std::string>> curveList = getCurveList();
    for(std::vector<std::pair<int, std::string>>::const_iterator iter = curveList.begin(); iter != curveList.end(); ++ iter){
        std::cout << "Curve ID: " << iter->first << "\tCurve String: " << iter->second << std::endl;
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
    return 0; 
}
