#include <iostream>
#include <sstream>
#include <player.h>
#include <Polynomial/LGInterpolator.h>
#include <MessageHash/MessageHash.h>
#include <algorithm>

player::player(const std::string& id, const std::string& uri, const std::string& addr, const std::string& port)
    : m_userID(id), m_uri(uri), m_address(addr), m_port(port)
{ return ; }

 player::~player()
 { return ; }
        
player::player(const player& obj)
    : m_userID(obj.m_userID)
    , m_uri(obj.m_uri)
    , m_address(obj.m_address)
    , m_port(obj.m_port)
{ return ; }

player& player::operator=(const player& obj)
{
    if(this != &obj){
        m_userID = obj.m_userID;
        m_uri = obj.m_uri;
        m_address = obj.m_address;
        m_port = obj.m_port; 
    }
    return *this;
}

std::ostream& operator<< (std::ostream& out, const player& obj)
{

    out << obj.m_userID << " " << obj.m_uri << " " << obj.m_address << " " << obj.m_port;
    return out;
}

// methods with JVRSS

void jvrss::reset(){
    m_fx = GenerateZero();
    m_evals.clear();
    m_publicEvals.clear();
    m_hiddenEvals.clear();
    m_hiddenPolynomial.clear();
    m_allHiddenPolynomials.clear();
    m_allVWShares.clear();
}

bool jvrss::verifyCorrectness (const std::vector<int>& grpOrdList, const BigNumber& modulo){
    
    for(std::vector<int>::const_iterator iter = grpOrdList.begin(); iter != grpOrdList.end(); ++ iter){
        std::map<std::string, std::vector<std::pair<std::string, ECPoint> > >::const_iterator ecPtCurveIter = m_hiddenEvals.find(std::to_string(*iter));
        if(ecPtCurveIter == m_hiddenEvals.end()){
            throw std::runtime_error("Invalid ordinal supplied in grpOrdList");
        }
        std::vector<std::pair<std::string, ECPoint> > curvePts = ecPtCurveIter->second; 
        std::vector<std::pair<BigNumber, ECPoint> >  curvePtsBN; 
        for(std::vector<std::pair<std::string, ECPoint> >::const_iterator curveptIter = curvePts.begin(); curveptIter != curvePts.end(); ++ curveptIter){
            BigNumber xCord;
            xCord.FromHex(curveptIter->first);
            curvePtsBN.push_back(std::pair(xCord, curveptIter->second)); 
        }
        LGECInterpolator ecPointInterp (curvePtsBN, modulo);
        BigNumber zeroValue = GenerateZero(); 
        ECPoint interpolatedZeroPt = ecPointInterp(zeroValue);
        // Check the constant term of the hidden polynomail for the ordinal 
        std::map<std::string, std::vector<ECPoint> >::const_iterator polyIter = m_allHiddenPolynomials.find(std::to_string(*iter));
        if(polyIter == m_allHiddenPolynomials.end()){
            throw std::runtime_error("invalid ordinal supplied " + std::to_string(*iter) + " is not included in the shared HiddenPolynomial Data " ); 
        }
        if ( interpolatedZeroPt != polyIter->second[0]) {
            std::runtime_error ("Verification of Correctness "+ std::to_string(*iter) + " failed. " ); 
        }
    }
    
    return true;
}

bool jvrss::verifyHonesty(const std::vector<int>& grpOrdList, const BigNumber& modulo){
    for(std::vector<int>::const_iterator FromOrdIter = grpOrdList.begin(); FromOrdIter != grpOrdList.end(); ++FromOrdIter){
        for(std::vector<int>::const_iterator ToOrdIter = grpOrdList.begin(); ToOrdIter != grpOrdList.end(); ++ ToOrdIter){
            if(FromOrdIter != ToOrdIter){
                // create bignumbers for the ordinals
                BigNumber multiplier;
                BigNumber toOrdinal ; 
                toOrdinal.FromHex(std::to_string(*ToOrdIter)); 
                multiplier = toOrdinal;
                // evaluate the hidden-polynomial for the toOrdinal 
                std::map<std::string, std::vector<ECPoint> >::const_iterator ecPointPoly = m_allHiddenPolynomials.find(std::to_string(*FromOrdIter));
                if(ecPointPoly == m_allHiddenPolynomials.end()){
                    throw std::runtime_error("No Polynomails supplied for player " + std::to_string(*FromOrdIter) + " in jvrss::verifyhonesty" );
                }
                std::vector<ECPoint>::const_iterator ptsIter = ecPointPoly->second.begin();
                ECPoint resECPt = *ptsIter;
                ++ptsIter;
                for(;ptsIter != ecPointPoly->second.end(); ++ ptsIter){
                    ECPoint labelTimesPt = ptsIter->MulHex(multiplier.ToHex(), std::string());
                    resECPt = resECPt + labelTimesPt;
                    multiplier = Mul_mod (multiplier,toOrdinal,modulo);
                }
                // check the value against the evaluated point for this pair of ordinals
                std::map<std::string, std::vector<std::pair<std::string, ECPoint> > >::const_iterator hiddenEvalIter = m_hiddenEvals.find(std::to_string(*FromOrdIter)); 
                if(hiddenEvalIter == m_hiddenEvals.end()){
                    throw std::runtime_error("No Hidden Evaluations found for player " + std::to_string(*FromOrdIter) + " in jvrss::verifyHonesty " );
                }
                const std::vector<std::pair<std::string, ECPoint> >& vecPts = hiddenEvalIter->second; 
                
                std::string value = std::to_string(*ToOrdIter);
                std::vector<std::pair<std::string, ECPoint> >::const_iterator vecPtsIter 
                    = std::find_if(vecPts.begin(), vecPts.end(), [&value](std::pair<std::string, ECPoint> const& elem) {return elem.first == value;});
                if(vecPtsIter->second != resECPt){
                    throw std::runtime_error("Verification of honesty from " + std::to_string(*FromOrdIter) + " to " + std::to_string(*ToOrdIter) + " has failed.");
                }
            }
        }
    }
        
    return true;
}



std::ostream& operator<< (std::ostream& out, const jvrss& obj){
    out << "JVRSS :"
            << "\n\tf_x = " << obj.m_fx.ToHex() 
            << "\n\tevals = " 
            ;

    {
        std::map<std::string, BigNumber>::const_iterator iter = obj.m_evals.begin();
        for(; iter != obj.m_evals.end(); ++ iter){
            out << "[" << iter->first << " " << iter->second.ToHex() << "],";
        }
    }

    out << "\n\tpublicEvals = ";
    {
        std::vector<std::pair<std::string, BigNumber> >::const_iterator iter =  obj.m_publicEvals.begin();
        for(; iter != obj.m_publicEvals.end(); ++ iter){
            out << "[" << iter->first << "," << iter->second.ToHex() << "]" << "," ; 
        }
    }
    out << "\n\tthiddenEvals = "; 
    {
        std::map<std::string, std::vector<std::pair<std::string,ECPoint> > >::const_iterator iter = obj.m_hiddenEvals.begin();
        for(; iter != obj.m_hiddenEvals.end(); ++ iter){
            out << "[" << iter->first << " " ;
            for(std::vector<std::pair<std::string, ECPoint> >::const_iterator ecptIter = iter->second.begin(); ecptIter != iter->second.end(); ++ ecptIter){
                out << "[" << ecptIter->first << "," << ecptIter->second.ToHex() << "],";
            }
            out << "],";
        }
    }
    
    out << "\n\thiddenPolynomial = ";
    {
        std::vector<ECPoint>::const_iterator iter =  obj.m_hiddenPolynomial.begin();
        for(; iter != obj.m_hiddenPolynomial.end(); ++ iter){
            out << iter->ToHex() << ",";
        }
    }
    out << "\n\tallHiddenPolynomials =  ";
    {
        std::map<std::string,std::vector<ECPoint> >::const_iterator iter =  obj.m_allHiddenPolynomials.begin();
        
        for(;iter != obj.m_allHiddenPolynomials.end(); ++ iter){
            out << iter->first << ", [";
            std::vector<ECPoint>::const_iterator innerIter = iter->second.begin(); 
            for(;innerIter != iter->second.end(); ++ innerIter){
                out << innerIter->ToHex() << " " ; 
            }
            out << "]\n";
        }
    }
    out << "\n\tallVWshares = ";
    {
        std::map<std::string,std::pair<BigNumber, ECPoint> >::const_iterator iter = obj.m_allVWShares.begin();
        for(;iter != obj.m_allVWShares.end(); ++iter){
            out << "[" << iter->first << " , " << iter->second.first.ToHex() << " , " << iter->second.second.ToHex() << "],";
        }
    }
    return out;
}

//methods associated with playerGroupMetaData


std::ostream& operator<< (std::ostream& out, const playerGroupMetaData& obj){
    std::stringstream strout;
    
    out << "\n\tGroup ID: " << obj.m_id
        << "\n\tPlayer ordinal: " << obj.m_ordinal
        << "\n\tList of players ordinals: " 
        ;
    for(std::vector<int>::const_iterator iter = obj.m_ordinalList.begin(); iter != obj.m_ordinalList.end(); ++ iter){
        out << *iter << " ";
    }

    out << "\n\tList of player ordinals and comms info: ";
    for(std::vector<std::pair<int, player> >::const_iterator iter = obj.m_ordinalAndPlayerList.begin(); iter != obj.m_ordinalAndPlayerList.end(); ++iter){
        out << iter->first << " " << iter->second << "\n" ;
    }
    
    out << "\n\tDegree of polynomial: " << obj.m_degree 
        << "\n\tprivate polynomail: " << obj.m_privateKeyPolynomial
        << "\n\tprivate key share: " << obj.m_privateKeyShare.ToHex()
        << "\n\tGroup Public Key: " << obj.m_GroupPublicKey.ToHex()
        << "\n\tEmpheralKeyList: "; 
        
    for(std::vector<std::pair<BigNumber, BigNumber> >::const_iterator iter = obj.m_EmpheralKeyList.begin(); iter != obj.m_EmpheralKeyList.end(); ++ iter){
        out << "[" << iter->first.ToHex() << "," << iter->second.ToHex() << "]" << "\n";
    }
    out << "\n\tlittleK: " << obj.m_littleK.ToHex()
        << "\n\talpha: " << obj.m_alpha.ToHex()
        << "\n\tmodulo: " << obj.m_modulo.ToHex()
        << "\n\ttranient data: " << obj.m_transientData
        ;    
    
    return out; 
    
}

bool playerGroupMetaData::verifyCorrectness(){
    std::vector<int> grpOrds;
    grpOrds.push_back(m_ordinal);
    grpOrds.insert(grpOrds.end(),m_ordinalList.begin(), m_ordinalList.end());
    // check no duplicates;
    return (m_transientData.verifyCorrectness(grpOrds,m_modulo));
}

bool playerGroupMetaData::verifyHonesty(){
    std::vector<int> grpOrds;
    grpOrds.push_back(m_ordinal);
    grpOrds.insert(grpOrds.end(),m_ordinalList.begin(), m_ordinalList.end());
    // check for duplicates
    return (m_transientData.verifyHonesty(grpOrds,m_modulo));
}

Polynomial playerGroupMetaData::createPolynomial(const int& degree, const BigNumber& mod){
    return Polynomial(degree, mod);
}

void playerGroupMetaData::polynomialPreCalculation(const Polynomial& poly){
    BigNumber bnOrdinal;
    bnOrdinal.FromHex(std::to_string(m_ordinal));
    m_transientData.m_fx = poly(bnOrdinal);
    std::vector<std::pair<std::string, ECPoint> > hiddenvals;
    hiddenvals.push_back(std::make_pair(std::to_string(m_ordinal),MultiplyByGeneratorPt(m_transientData.m_fx)));
    for (std::vector<int>::const_iterator iter =  m_ordinalList.begin(); iter != m_ordinalList.end(); ++ iter){
        BigNumber bnOrd; 
        bnOrd.FromHex(std::to_string(*iter));
        BigNumber playereval = poly(bnOrd); 
        m_transientData.m_evals[std::to_string(*iter)] = playereval;
        hiddenvals.push_back(std::make_pair(std::to_string(*iter),MultiplyByGeneratorPt(playereval)));
    }
    
    m_transientData.m_hiddenEvals[std::to_string(m_ordinal)] = hiddenvals;
    m_transientData.m_hiddenPolynomial = poly.hideAsPoint();
    return; 
}

void playerGroupMetaData::addPublicEvalsToJVRSS(const std::string& ord, const std::string& evalAsStr){
    BigNumber eval; 
    eval.FromHex(evalAsStr);

    std::vector<std::pair<std::string, BigNumber> >::const_iterator pubEvalIter
                = std::find_if(m_transientData.m_publicEvals.begin(), m_transientData.m_publicEvals.end(), 
                                [&ord](std::pair<std::string, BigNumber> const& elem) {return elem.first == ord;});
                
        if(pubEvalIter != m_transientData.m_publicEvals.end()){
            if( !(pubEvalIter->second == eval)){
                throw std::runtime_error("Public eval received two different values for the same player within a group");
            }
        }else{
            m_transientData.m_publicEvals.push_back(std::make_pair(ord, eval));
        }

}

void playerGroupMetaData::addHiddenEvalsToJVRSS(const std::string& ordinal, const std::vector<std::pair<std::string, std::string> >& hiddenevals){
    std::vector<std::pair<std::string, ECPoint> > hiddenEvalPts;
    std::map<std::string, std::vector<std::pair<std::string, ECPoint> > >::iterator hiddenEvalIter = m_transientData.m_hiddenEvals.find(ordinal);
    if(hiddenEvalIter != m_transientData.m_hiddenEvals.end()){
        // This should be the players own data returning back ... verify
        
    }else{
        for(std::vector<std::pair<std::string, std::string> >::const_iterator iter = hiddenevals.begin(); iter != hiddenevals.end(); ++ iter){
            ECPoint pt;
            pt.FromHex(iter->second);
            hiddenEvalPts.push_back(std::make_pair(iter->first,pt));
        }
        m_transientData.m_hiddenEvals[ordinal] = hiddenEvalPts;
    }
    return;
}


void playerGroupMetaData::addHiddenPolynomialToJVRSS(const std::string& ordinal, const std::vector<std::string>& poly){
    
    std::vector<ECPoint> polyPts;
    for(std::vector<std::string>::const_iterator iter = poly.begin(); iter != poly.end(); ++ iter){
        ECPoint pt;
        pt.FromHex(*iter); 
        polyPts.push_back(pt);
    }
    // verify we haven't received a polynomial from this player
    std::map<std::string, std::vector<ECPoint> >::iterator polyIter = m_transientData.m_allHiddenPolynomials.find(ordinal);
    if(polyIter != m_transientData.m_allHiddenPolynomials.end()){
        // check if the polys are the same. if different throw an error
        throw std::runtime_error("Received different polynomials from the same player for the same jvrss");
    }
    m_transientData.m_allHiddenPolynomials[ordinal] = polyPts;
    return;
}

 void playerGroupMetaData::addPreSignDataToJVRSS(const std::string& ord, const std::string& v, const std::string& w){
    BigNumber vBN;
    vBN.FromHex(v);
    ECPoint wPt;
    wPt.FromHex(w);
    std::map<std::string, std::pair<BigNumber, ECPoint> >::iterator iter = m_transientData.m_allVWShares.find(ord);
    if(iter != m_transientData.m_allVWShares.end()){
        // check the values from a previous received ordinal are the same
        if(!(iter->second.first == vBN) && wPt != iter->second.second){
            throw std::runtime_error ("received different v or w from the same player twice for the same jvrss");
        }
    }
    m_transientData.m_allVWShares[ord] = std::make_pair(vBN,wPt);
    return;
 }


BigNumber playerGroupMetaData::createSecret(){

    BigNumber res = m_transientData.m_fx;
    for(std::vector<std::pair<std::string, BigNumber> >::const_iterator iter = m_transientData.m_publicEvals.begin(); iter != m_transientData.m_publicEvals.end(); ++ iter){
        res = Add_mod(res, iter->second, m_modulo);
    }
    return res ; 
}

ECPoint playerGroupMetaData::createGroupPublicKey(){
    std::map<std::string,std::vector<ECPoint> >::const_iterator iter = m_transientData.m_allHiddenPolynomials.begin(); 
    ECPoint res = iter->second[0]; 
    ++ iter;
    for(;iter != m_transientData.m_allHiddenPolynomials.end(); ++iter){
        res = res + iter->second[0];
    }
    return res;
}

bool playerGroupMetaData::CalculateEphemeralKey(){
    std::vector<std::pair<BigNumber, BigNumber> > vCurve;
    std::vector<std::pair<BigNumber, ECPoint> > wCurve;
    for(std::map<std::string,std::pair<BigNumber, ECPoint> >::const_iterator iter = m_transientData.m_allVWShares.begin(); iter != m_transientData.m_allVWShares.end(); ++ iter){
        BigNumber ordBN;
        ordBN.FromHex(iter->first);
        vCurve.push_back(std::make_pair(ordBN, iter->second.first));
        wCurve.push_back(std::make_pair(ordBN, iter->second.second));
    }
    
    BigNumber zeroPoint = GenerateZero();
    LGInterpolator interp (vCurve, m_modulo);
    BigNumber vzero = interp(zeroPoint);
    LGECInterpolator interpW (wCurve, m_modulo);
    ECPoint wzero = interpW(zeroPoint);
    
    BigNumber vzeroInverse = Inv_mod(vzero, m_modulo);
    ECPoint ephemPt = wzero.MulHex(vzeroInverse.ToHex(), std::string());
    std::pair<std::string, std::string> interpolated_r_points = ephemPt.GetAffineCoords_GFp();
    BigNumber rzero;
    rzero.FromHex(interpolated_r_points.first);
    
    // check m_littleK is not zero. 
    std::pair<BigNumber, BigNumber> ephemeralKey = std::make_pair(m_littleK,rzero);
    m_EmpheralKeyList.push_back(ephemeralKey);
    
    return true;
    
}


BigNumber playerGroupMetaData::Signature(const std::string& msg, const std::pair<BigNumber,BigNumber>& ephKey ){
    // s = k*(Hash(msg) + (keyshare*r));
    std::string hashMsg = HashMsgSHA256(msg); 
    BigNumber hashMsgBN;
    hashMsgBN.FromHex(hashMsg);
    BigNumber partialSig = ephKey.first * (hashMsgBN + (m_privateKeyShare * ephKey.second));
    return partialSig;
    
}

std::pair<std::string, BigNumber> playerGroupMetaData::CalcPartialSignature (const std::string& msg, const int& keyindex){
    if(m_EmpheralKeyList.empty()){
        throw std::runtime_error("No empheral keys available for use. Please generate some");
    }
    // remember size is one greater than an index i.e index =0 => size = 1
    // so if a user requests index =1 when size =1 that will return an error. 
    if(m_EmpheralKeyList.size() <= keyindex){
        throw std::runtime_error("No emphemeral keys available for use at the supplied index: " + std::to_string(keyindex));
    }
    const std::pair<BigNumber,BigNumber>& ephKey = m_EmpheralKeyList[keyindex];
    BigNumber partialSig = Signature(msg, ephKey);

    m_EmpheralKeyList.erase( m_EmpheralKeyList.begin() + keyindex);
    
    return std::make_pair(std::to_string(m_ordinal), partialSig);
    
}

std::pair<BigNumber, BigNumber> playerGroupMetaData::CalculateGroupSignature(const std::string& msg, const int& ekeyindex, const std::vector<std::pair<std::string, std::string> >& sig){
    std::vector<std::pair<BigNumber, BigNumber> > sigCurve;
    for(std::vector<std::pair<std::string, std::string> >::const_iterator iter = sig.begin(); iter != sig.end(); ++iter){
        BigNumber ord;
        ord.FromHex(iter->first);
        BigNumber sigvalue;
        sigvalue.FromHex(iter->second);
        sigCurve.push_back(std::make_pair(ord,sigvalue));
    }
    
    if(m_EmpheralKeyList.empty()){
       throw std::runtime_error ("No empheral keys available to use. Please generate some");
    }
    if(m_EmpheralKeyList.size() <= ekeyindex){
        throw std::runtime_error("No empheral keys available to use at the index " + std::to_string(ekeyindex));
    }
    // calculate own share (this is done here because we need the r value. It's burned when used)
    const std::pair<BigNumber,BigNumber>& ephKey = m_EmpheralKeyList[ekeyindex];
    BigNumber  playerSigData = Signature(msg,ephKey);
    BigNumber playerOrd; 
    playerOrd.FromHex(std::to_string(m_ordinal));
    sigCurve.push_back(std::make_pair(playerOrd,playerSigData));
     
    BigNumber zeroPoint = GenerateZero();
    LGInterpolator interp (sigCurve, m_modulo);
    BigNumber sigzero = interp(zeroPoint);
    
    std::pair<BigNumber, BigNumber> groupSig = std::make_pair(ephKey.second , sigzero);
    m_EmpheralKeyList.erase( m_EmpheralKeyList.begin() + ekeyindex);
    
    return groupSig;
}

void playerGroupMetaData::addPrivateKeyInfo(const std::string& ord, const std::string& priKeyShare){
    BigNumber ordBN;
    ordBN.FromHex(ord);
    BigNumber ks;
    ks.FromHex(priKeyShare);
    m_PrivateKeyShares.push_back(std::make_pair(ordBN,ks));
}

BigNumber playerGroupMetaData::CalculateGroupPrivateKey (){
    if(m_PrivateKeyShares.empty())
        std::runtime_error("No private key shares gathered. Not possible to calculate the group private key");
    // add the players own keyshare
    BigNumber ordBN;
    ordBN.FromHex(std::to_string(m_ordinal));
    m_PrivateKeyShares.push_back(std::make_pair(ordBN,m_privateKeyShare));

    BigNumber zeroPoint = GenerateZero();
    LGInterpolator interp (m_PrivateKeyShares, m_modulo);
    int index(0);
    BigNumber KeyValSum = GenerateZero();
    for(std::vector<std::pair<BigNumber, BigNumber> >::const_iterator iter =m_PrivateKeyShares.begin(); iter != m_PrivateKeyShares.end(); ++ iter ){
        BigNumber interpVal = interp(index++,zeroPoint);
        BigNumber KVAL = Mul_mod(interpVal,iter->second,m_modulo);
        KeyValSum = Add_mod(KVAL,KeyValSum,m_modulo);
    }
    // delete the private keyshares once the calculation is complete
    m_PrivateKeyShares.clear();
    return KeyValSum;
}

bool playerGroupMetaData::ValidateGroupPrivateKey(const BigNumber& testKeyVal){
    ECPoint TestVal = MultiplyByGeneratorPt(testKeyVal);
    return (TestVal == m_GroupPublicKey);
}

std::unique_ptr<SinglePlayer> SinglePlayer::m_Instance;
std::once_flag SinglePlayer::m_onceFlag;

std::unique_ptr<SinglePlayer>& SinglePlayer::Instance(){
    std::call_once(m_onceFlag, []{m_Instance.reset(new SinglePlayer);}); 
    return m_Instance; 
 }
 
 void SinglePlayer::addPlayerGroup(const playerGroupMetaData& obj){
    m_PlayerGroup[obj.m_id] = obj;
}

std::vector<std::string> SinglePlayer::GroupIDS() const {
    std::vector<std::string> ids;
    for(playerGroups::const_iterator iter = m_PlayerGroup.begin(); iter != m_PlayerGroup.end(); ++ iter){
        ids.push_back(iter->first);
    }
    return ids;
}

const playerGroupMetaData& SinglePlayer::groupdetails (const std::string& grpid) const{
    playerGroups::const_iterator iter = m_PlayerGroup.find(grpid);
    if(iter == m_PlayerGroup.end())
        throw std::runtime_error("No group with listed with id " + grpid); 
    return iter->second; 
}

playerGroupMetaData& SinglePlayer::groupdetails (const std::string& grpid){
    playerGroups::iterator iter = m_PlayerGroup.find(grpid);
    if(iter == m_PlayerGroup.end())
        throw std::runtime_error("No group with listed with id " + grpid); 
    return iter->second; 
}

bool SinglePlayer::deleteGroup(const std::string& grpid){
    playerGroups::const_iterator iter = m_PlayerGroup.find(grpid);
    if(iter == m_PlayerGroup.end())
        return false;

    size_t numElems = m_PlayerGroup.erase(grpid);
    if(numElems == 0)  
        return false;
    return true;
 }


// free functions
std::pair<std::string, bool> acceptInvitationToGroup(const std::string& grpid){
    // This function is a place-holder. It's application specific how 
    // group membership is rejected or accepted
    // The default implementation is to accept all requests
    return (std::make_pair(SinglePlayer::Instance()->UserID(), true)); 
}

void deletePlayerGroup(const std::string& userid, const std::string& grpid){
    if(!(SinglePlayer::Instance()->deleteGroup(grpid)))
        throw std::runtime_error("Unable to delete group with id : " + grpid );
}

const player& getPublicPlayerInfo(){
    return (SinglePlayer::Instance()->publicInfo());
}


const jvrss& PlayerSecretDataForGrp (const std::string& grpid,const std::string& calctype){
    if(calctype != "PRIVATEKEYSHARE"){
        playerGroupMetaData& grp = SinglePlayer::Instance()->groupdetails(grpid);
        Polynomial poly(grp.m_degree, grp.m_modulo); 
        grp.polynomialPreCalculation(poly);
        return grp.m_transientData;
    }else{
        return SinglePlayer::Instance()->groupdetails(grpid).m_transientData;
    }
}

playerGroupMetaData& PlayerGrpData (const std::string& grpid){
    return SinglePlayer::Instance()->groupdetails(grpid);
}

const int& ordinalForGroup(const std::string& grpid){
    return SinglePlayer::Instance()->groupdetails(grpid).m_ordinal;
}

std::pair<BigNumber,ECPoint> EphemeralKeyPlayerData(const std::string& grpid){
    const playerGroupMetaData& grp = SinglePlayer::Instance()->groupdetails(grpid);
    BigNumber kalpha = grp.m_littleK * grp.m_alpha ;
    ECPoint alphaG = MultiplyByGeneratorPt(grp.m_alpha);
    return std::make_pair(kalpha, alphaG);
}


 
