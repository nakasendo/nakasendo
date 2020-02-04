#include <fstream>
#include <TSState.h>
#include <TSProtoBufHelper.h>


void WriteGroupMetaDataStateToFile(const GroupMetadata& grp, const std::string& filename){
    std::ofstream ostrm(filename, std::ios::binary);
    if (!ostrm.is_open()) {
        throw std::runtime_error("Unable to open the file for writing" + filename);
    }else{
        WriteGroupMetaDataState(grp,ostrm);
    }
}
void ReadGroupMetaDataStateFromFile(GroupMetadata& grp, const std::string& filename){
    std::ifstream istrm(filename,std::ios::binary);
    if(!istrm.is_open()){
        throw std::runtime_error("unable to open file for reading " + filename);
    }else{
        ReadGroupMetaDataState(grp,istrm);
    }
}

void WriteGroupMetaDataState(const GroupMetadata& grp, std::ostream& os){
    thresholdsignature::GroupMetaDataState grpstate;
    grpstate.set_groupid(grp.groupid());
    grpstate.set_proposer(grp.proposer());
    grpstate.set_groupsize(grp.m());
    grpstate.set_groupthreshold(grp.n());
    grpstate.set_grouppolydegree(grp.t());
    for(std::vector<playerCommsInfo>::const_iterator iter = grp.participantList().begin(); iter != grp.participantList().end(); ++iter){
        thresholdsignature::Player* p = grpstate.add_players();
        p->set_name(iter->m_userid);
        p->set_uri(iter->m_uri);
        p->set_addr(iter->m_ip);
        p->set_port(iter->m_port);
    }


    for(std::map<std::string, std::vector<ECPoint> >::const_iterator iter = grp.CollatedHiddenPolys().begin();iter != grp.CollatedHiddenPolys().end(); ++iter){
        thresholdsignature::hiddenPolynomial*  collatedPtr = grpstate.add_collatedhiddenpolys();
        collatedPtr->set_ordinal(std::stoi(iter->first));
        for(std::vector<ECPoint>::const_iterator innerIter = iter->second.begin();innerIter != iter->second.end(); ++ innerIter){
            collatedPtr->add_coefficients(innerIter->ToHex());
        }
    }

    for(std::map<std::string, std::vector<std::pair<std::string, ECPoint> > >::const_iterator iter = grp.CollatedHiddenEvals().begin(); iter != grp.CollatedHiddenEvals().end(); ++iter){
        thresholdsignature::listOfPolynomials* hiddenEvalPtr = grpstate.add_collatedhiddenevals();
        hiddenEvalPtr->set_ordinal(std::stoi(iter->first));
        for(std::vector<std::pair<std::string, ECPoint> >::const_iterator innerIter = iter->second.begin(); innerIter != iter->second.end(); ++innerIter){
            thresholdsignature::evaluatedPoly* evalInfoPtr = hiddenEvalPtr->add_ep();
            evalInfoPtr->set_ordinal(std::stoi(innerIter->first));
            evalInfoPtr->set_f_x(innerIter->second.ToHex());
        }
    }

    for(std::map<std::string, std::pair<BigNumber, ECPoint> >::const_iterator iter = grp.CollatedVW().begin(); iter != grp.CollatedVW().end(); ++ iter){
        thresholdsignature::VWData* collatedVWPtr = grpstate.add_collatedvw();
        collatedVWPtr->set_ordinal(std::stoi(iter->first));
        collatedVWPtr->set_v(iter->second.first.ToHex());
        collatedVWPtr->set_w(iter->second.second.ToHex());
    }


    for(std::vector<std::pair<std::string, BigNumber> >::const_iterator iter = grp.CollatedPartialSignatures().begin(); iter != grp.CollatedPartialSignatures().end(); ++ iter){
        thresholdsignature::SigData* sigPtr = grpstate.add_collatedpartialsignatures();
        sigPtr->set_ordinal(std::stoi(iter->first));
        sigPtr->set_signature(iter->second.ToHex());
    }
    if(!grp.calculationType().empty()){
        grpstate.set_calculation(string2enum(grp.calculationType()));
    }else{
        grpstate.set_calculation(thresholdsignature::UNSET);
    }

    grpstate.set_groupset(grp.GroupSet());
    grpstate.set_groupinvitereplies(grp.GroupInviteReplies());
    grpstate.set_groupsignaturereplies(grp.GroupSignatureReplies());
    if(!grpstate.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize GroupMetaDataState"); 
    }
    return;
}

void ReadGroupMetaDataState(GroupMetadata& grp, std::istream& is){
    thresholdsignature::GroupMetaDataState grpstate;
    if(!grpstate.ParseFromIstream(&is)){
        throw std::runtime_error("Could not deserialize GroupMetaDataState");
    }

    grp.groupid() = grpstate.groupid();
    grp.proposer() = grpstate.proposer();
    grp.m() = grpstate.groupsize();
    grp.n() = grpstate.groupthreshold();
    grp.t() = grpstate.grouppolydegree();

    for(int i=0;i<grpstate.players_size();++i){
        playerCommsInfo p;
        p.m_userid = grpstate.players(i).name();
        p.m_uri = grpstate.players(i).uri();
        p.m_ip = grpstate.players(i).addr();
        p.m_port = grpstate.players(i).port();
        grp.participantList().push_back(p);
    }

    for(int i=0;i<grpstate.collatedhiddenpolys_size(); ++i){
        const std::string& ord = std::to_string(grpstate.collatedhiddenpolys(i).ordinal());
        std::vector<std::string> poly;

        for(int j=0; j<grpstate.collatedhiddenpolys(i).coefficients_size();++j){
            std::string ecptStr = grpstate.collatedhiddenpolys(i).coefficients(j);
            poly.push_back(ecptStr);
        }
        grp.addCollatedHiddenPolys(ord,poly); 
    }


    for(int i=0;i<grpstate.collatedhiddenevals_size(); ++i){
        std::string fromord = std::to_string(grpstate.collatedhiddenevals(i).ordinal());
        std::vector<std::pair<std::string, ECPoint> > hiddenevals;
        for(int j=0;j<grpstate.collatedhiddenevals(i).ep_size();++j){
            std::string toord = std::to_string(grpstate.collatedhiddenevals(i).ep(j).ordinal());
            std::string evalStr = grpstate.collatedhiddenevals(i).ep(j).f_x();
            grp.addCollatedHiddenEvals(fromord, toord, evalStr);
        }
    }

    for(int i=0;i<grpstate.collatedvw_size();++i){
        std::string fromord = std::to_string(grpstate.collatedvw(i).ordinal());
        std::string vVal = grpstate.collatedvw(i).v();
        std::string wVal = grpstate.collatedvw(i).w();
        grp.addCollatedVW(fromord, vVal, wVal);
    }

    for(int i=0;i<grpstate.collatedpartialsignatures_size();++i){
        std::string ord = std::to_string(grpstate.collatedpartialsignatures(i).ordinal());
        std::string sig = grpstate.collatedpartialsignatures(i).signature();
        grp.addCollatedPartialSignautre(ord, sig);
    }

    std::string ct = enum2string(grpstate.calculation());
    if (ct != "UNSET")
        grp.calculationType() = enum2string(grpstate.calculation());
    grp.GroupSet() = grpstate.groupset();
    grp.GroupInviteReplies() = grpstate.groupinvitereplies();
    grp.GroupSignatureReplies() = grpstate.groupsignaturereplies();
    
    return;
}

    //repeated VWData allvwshares=7;

void WriteJVRSSState(const jvrss& jvrssObj, std::ostream& os){
    thresholdsignature::JVRSSState jvrssState;
    jvrssState.set_fx(jvrssObj.m_fx.ToHex());
    for(std::map<std::string, BigNumber>::const_iterator iter = jvrssObj.m_evals.begin(); iter != jvrssObj.m_evals.end(); ++ iter){
        thresholdsignature::evaluatedPoly* ep = jvrssState.add_evals();
        ep->set_ordinal(std::stoi(iter->first));
        ep->set_f_x(iter->second.ToHex());
    }

    for(std::vector<std::pair<std::string, BigNumber> >::const_iterator iter = jvrssObj.m_publicEvals.begin(); iter != jvrssObj.m_publicEvals.end(); ++iter){
        thresholdsignature::evaluatedPoly* ep = jvrssState.add_publicevals();
        ep->set_ordinal(std::stoi(iter->first));
        ep->set_f_x(iter->second.ToHex());
    }

    for(std::map<std::string, std::vector<std::pair<std::string, ECPoint> > >::const_iterator iter = jvrssObj.m_hiddenEvals.begin(); iter != jvrssObj.m_hiddenEvals.end(); ++iter){
        thresholdsignature::listOfPolynomials* hiddenevalsPtr = jvrssState.add_hiddenevals();
        hiddenevalsPtr->set_ordinal(std::stoi(iter->first));
        for(std::vector<std::pair<std::string,ECPoint> >::const_iterator innerIter = iter->second.begin(); innerIter != iter->second.end(); ++innerIter){
            thresholdsignature::evaluatedPoly* epPtr = hiddenevalsPtr->add_ep();
            epPtr->set_ordinal(std::stoi(innerIter->first));
            epPtr->set_f_x(innerIter->second.ToHex());
        }
    }

    for(std::vector<ECPoint>::const_iterator hiddenPolyIter = jvrssObj.m_hiddenPolynomial.begin();hiddenPolyIter != jvrssObj.m_hiddenPolynomial.end(); ++hiddenPolyIter){
        jvrssState.add_hiddenpoly( hiddenPolyIter->ToHex() ) ; 
    }

    for(std::map<std::string,std::vector<ECPoint> >::const_iterator iter = jvrssObj.m_allHiddenPolynomials.begin();iter != jvrssObj.m_allHiddenPolynomials.end(); ++iter){
        thresholdsignature::hiddenPolynomial* hiddenPolyPtr = jvrssState.add_allhiddenpolys();
        hiddenPolyPtr->set_ordinal(std::stoi(iter->first));
        for(std::vector<ECPoint>::const_iterator innerIter = iter->second.begin(); innerIter != iter->second.end(); ++ innerIter){
            hiddenPolyPtr->add_coefficients(innerIter->ToHex());
        }
    }

    for(std::map<std::string,std::pair<BigNumber, ECPoint> >::const_iterator iter = jvrssObj.m_allVWShares.begin(); iter != jvrssObj.m_allVWShares.end(); ++iter){
        thresholdsignature::VWData* vwSharesPtr = jvrssState.add_allvwshares();
        vwSharesPtr->set_ordinal(std::stoi(iter->first));
        vwSharesPtr->set_v(iter->second.first.ToHex());
        vwSharesPtr->set_w(iter->second.second.ToHex());
    }
    if(!jvrssState.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize GroupMetaDataState"); 
    }
    return;
}

void ReadJVRSS(jvrss& jvrssobj, const thresholdsignature::JVRSSState& jvrssstate){
       BigNumber fx;
    fx.FromHex(jvrssstate.fx());
    jvrssobj.m_fx = fx; 

    for(int i=0;i<jvrssstate.evals_size();++i){
        std::string ord = std::to_string(jvrssstate.evals(i).ordinal());
        BigNumber fxEval;
        fxEval.FromHex(jvrssstate.evals(i).f_x());
        jvrssobj.m_evals[ord] = fxEval;
    }
    
    for(int i=0;i<jvrssstate.publicevals_size();++i){
        std::string ord = std::to_string(jvrssstate.publicevals(i).ordinal());
        BigNumber fxeval;
        fxeval.FromHex(jvrssstate.publicevals(i).f_x());
        jvrssobj.m_publicEvals.push_back(std::make_pair(ord,fxeval));
    }

    for(int i=0;i<jvrssstate.hiddenevals_size();++i){
        std::string ord = std::to_string(jvrssstate.hiddenevals(i).ordinal());
        std::vector<std::pair<std::string,ECPoint> > hevalsVec;
        for(int j=0;j<jvrssstate.hiddenevals(i).ep_size();++j){
            std::string fromOrd = std::to_string(jvrssstate.hiddenevals(i).ep(j).ordinal());
            ECPoint pt; 
            pt.FromHex(jvrssstate.hiddenevals(i).ep(j).f_x());
            hevalsVec.push_back(std::make_pair(fromOrd,pt));
        }
         jvrssobj.m_hiddenEvals[ord] = hevalsVec;
    }

    for(int i=0;i<jvrssstate.hiddenpoly_size();++i){
        ECPoint pt;
        pt.FromHex(jvrssstate.hiddenpoly(i));
        jvrssobj.m_hiddenPolynomial.push_back(pt);
    }

    for(int i=0;i<jvrssstate.allhiddenpolys_size();++i){
        std::string ord = std::to_string(jvrssstate.allhiddenpolys(i).ordinal());
        std::vector<ECPoint> vecPts;
        for(int j=0;j<jvrssstate.allhiddenpolys(i).coefficients_size();++j){
            ECPoint pt;
            pt.FromHex(jvrssstate.allhiddenpolys(i).coefficients(j));
            vecPts.push_back(pt);
        }
        jvrssobj.m_allHiddenPolynomials[ord]= vecPts;
    }

    for(int i=0;i<jvrssstate.allvwshares_size();++i){
        std::string ord = std::to_string(jvrssstate.allvwshares(i).ordinal());
        BigNumber vVal;
        vVal.FromHex(jvrssstate.allvwshares(i).v());
        ECPoint wVal;
        wVal.FromHex(jvrssstate.allvwshares(i).w());
        jvrssobj.m_allVWShares[ord] = std::make_pair(vVal,wVal);
    }
    return ; 
}
void ReadJVRSSState(jvrss& jvrssobj, std::istream& is){
    thresholdsignature::JVRSSState jvrssstate;
    if(!jvrssstate.ParseFromIstream(&is)){
        throw std::runtime_error("Could not deserialize JVRSS");
    }
    ReadJVRSS(jvrssobj,jvrssstate);
    return;
}
void WriteJVRSSStateToFile(const jvrss& obj, const std::string& filename){
    std::ofstream ostrm(filename, std::ios::binary);
    if (!ostrm.is_open()) {
        throw std::runtime_error("Unable to open the file for writing" + filename);
    }else{
         WriteJVRSSState(obj,ostrm);
    }
}
void ReadJVRSSStateFromFile(jvrss& obj, const std::string& filename){
    std::ifstream istrm(filename, std::ios::binary);
    if (!istrm.is_open()) {
        throw std::runtime_error("Unable to open the file for writing" + filename);
    }else{
         ReadJVRSSState(obj,istrm);
    }
}

void WritePlayerGroupState(const playerGroupMetaData& pgrp, std::ostream& os){
    thresholdsignature::PlayerGroupState playergroupstate;
    playergroupstate.set_groupid (pgrp.m_id);
    playergroupstate.set_ordinal(pgrp.m_ordinal);
    for(std::vector<int>::const_iterator iter = pgrp.m_ordinalList.begin(); iter != pgrp.m_ordinalList.end(); ++ iter){
        playergroupstate.add_ordinallist(*iter);
    } 
    for(std::vector<std::pair<int, player> >::const_iterator iter = pgrp.m_ordinalAndPlayerList.begin(); iter != pgrp.m_ordinalAndPlayerList.end(); ++ iter){
        thresholdsignature::ordinalAndPlayer* ordP = playergroupstate.add_ordinalandplayerlist();
        ordP->set_ordinal(iter->first);
        thresholdsignature::Player* allocp = new thresholdsignature::Player( );
        allocp->set_name(iter->second.userID());
        allocp->set_uri(iter->second.uri());
        allocp->set_addr(iter->second.addr());
        allocp->set_port(iter->second.port());
        ordP->set_allocated_p(allocp);
    }

    playergroupstate.set_degree(pgrp.m_degree);
    playergroupstate.set_mod(pgrp.m_modulo.ToHex());

    const std::vector<BigNumber>& coeffs = pgrp.m_privateKeyPolynomial.getCoefficients();
    for(std::vector<BigNumber>::const_iterator iter = coeffs.begin();iter != coeffs.end(); ++ iter){
        playergroupstate.add_privatekeypoly(iter->ToHex());
    }

    playergroupstate.set_privatekeyshare (pgrp.m_privateKeyShare.ToHex());
    playergroupstate.set_grouppublickey(pgrp.m_GroupPublicKey.ToHex());

    for(std::vector<std::pair<BigNumber, BigNumber> >::const_iterator iter = pgrp.m_EmpheralKeyList.begin(); iter != pgrp.m_EmpheralKeyList.end(); ++iter){
        thresholdsignature::ephemeralkeylist* ekPtr = playergroupstate.add_ephemeralkeys();
        ekPtr->set_k(iter->first.ToHex());
        ekPtr->set_r(iter->second.ToHex());
    }

    playergroupstate.set_k(pgrp.m_littleK.ToHex());
    playergroupstate.set_alpha(pgrp.m_alpha.ToHex());


    for(std::vector<std::pair<BigNumber, BigNumber> >::const_iterator iter =  pgrp.m_PrivateKeyShares.begin(); iter != pgrp.m_PrivateKeyShares.end(); ++iter){
        thresholdsignature::privatekeyshares* ksptr = playergroupstate.add_keyshares();
        ksptr->set_ordinal(std::stoi(iter->first.ToHex()));
        ksptr->set_keyshare(iter->second.ToHex());
    }

    WriteJVRSSState(pgrp.m_transientData,os);
    if(!playergroupstate.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize GroupMetaDataState"); 
    }
    return ; 
}
void ReadPlayerGroupState(playerGroupMetaData& pgrp, std::istream& is){
    thresholdsignature::PlayerGroupState playergroupstate;
    if(!playergroupstate.ParseFromIstream(&is)){
        throw std::runtime_error("Could not deserialize GroupMetaDataState");
    }

    pgrp.m_id = playergroupstate.groupid();
    pgrp.m_ordinal = playergroupstate.ordinal();
    for(int i=0;i<playergroupstate.ordinallist_size();++i){
        pgrp.m_ordinalList.push_back(playergroupstate.ordinallist(i));
    }

    
    for(int i=0;i<playergroupstate.ordinalandplayerlist_size();++i){
        int ord = playergroupstate.ordinalandplayerlist(i).ordinal();
        player grpplayer(playergroupstate.ordinalandplayerlist(i).p().name(),playergroupstate.ordinalandplayerlist(i).p().uri(),
                    playergroupstate.ordinalandplayerlist(i).p().addr(),playergroupstate.ordinalandplayerlist(i).p().port());
        pgrp.m_ordinalAndPlayerList.push_back(std::make_pair(ord,grpplayer));
    }
    
    pgrp.m_degree = playergroupstate.degree();

    BigNumber bnMod;
    bnMod.FromHex(playergroupstate.mod());
    pgrp.m_modulo = bnMod;

    std::vector<BigNumber> polycoeffs;
    for(int i=0;i<playergroupstate.privatekeypoly_size();++i){
        BigNumber bn;
        bn.FromHex(playergroupstate.privatekeypoly(i));
        polycoeffs.push_back(bn);
    }

    pgrp.m_privateKeyPolynomial = Polynomial(polycoeffs,pgrp.m_modulo);
    
    BigNumber bnPK;
    bnPK.FromHex(playergroupstate.privatekeyshare());
    pgrp.m_privateKeyShare = bnPK;

    ECPoint ptgk;
    if(playergroupstate.grouppublickey() != "00"){
        ptgk.FromHex(playergroupstate.grouppublickey());
        pgrp.m_GroupPublicKey = ptgk;
    }

    for(int i=0;i<playergroupstate.ephemeralkeys_size();++i){
        BigNumber kVal;
        kVal.FromHex(playergroupstate.ephemeralkeys(i).k());
        BigNumber rVal;
        rVal.FromHex(playergroupstate.ephemeralkeys(i).r());
        pgrp.m_EmpheralKeyList.push_back(std::make_pair(kVal,rVal));
    }
    
    BigNumber bnLK;
    bnLK.FromHex(playergroupstate.k());
    pgrp.m_littleK = bnLK;

    BigNumber bnA;
    bnA.FromHex(playergroupstate.alpha());
    pgrp.m_alpha = bnA;
    
    for(int i=0;i<playergroupstate.keyshares_size();++i){
        BigNumber ord;
        ord.FromHex(std::to_string(playergroupstate.keyshares(i).ordinal()));
        BigNumber keyShare;
        keyShare.FromHex(playergroupstate.keyshares(i).keyshare());
        pgrp.m_PrivateKeyShares.push_back(std::make_pair(ord,keyShare));
    }
    
    ReadJVRSS(pgrp.m_transientData, playergroupstate.jvrss());

    return;
}

void WritePlayerGroupStateToFile(const playerGroupMetaData& pgrp, const std::string& filename){
    std::ofstream ostrm(filename, std::ios::binary);
    if (!ostrm.is_open()) {
        throw std::runtime_error("Unable to open the file for writing" + filename);
    }else{
         WritePlayerGroupState(pgrp,ostrm);
    }
}
void ReadPlayerGroupStateFromFile(playerGroupMetaData& pgrp, const std::string& filename ){
   std::ifstream istrm(filename, std::ios::binary);
    if (!istrm.is_open()) {
        throw std::runtime_error("Unable to open the file for writing" + filename);
    }else{
         ReadPlayerGroupState(pgrp,istrm);
    }
}
