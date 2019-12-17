#include <sstream>
#include <GroupMetaData.h>

#include <algorithm>


GroupMetadata::GroupMetadata(const std::string& id, const std::string& proposer, const int& m, const int& n, const int& t)
    : m_groupid (id), m_proposer(proposer), m_m(m), m_n(n), m_t(t),m_participantList(), m_GroupSet(false),m_calculationType()
{ 
    m_participantList.push_back(m_proposer);
    return;
}


GroupMetadata::GroupMetadata(const GroupMetadata& obj)
   : m_groupid (obj.m_groupid), m_proposer(obj.m_proposer), m_m(obj.m_m), m_n(obj.m_n), m_t(obj.m_t)
   , m_participantList(), m_GroupSet(obj.m_GroupSet),m_calculationType(obj.m_calculationType)
{  
    for(std::vector<std::string>::const_iterator iter = obj.m_participantList.begin(); 
            iter != obj.m_participantList.end(); ++iter){
                m_participantList.push_back(*iter);
    }   
    return; 
} 

GroupMetadata& GroupMetadata::operator= (const GroupMetadata& obj){
    if(this != &obj){
        m_groupid = obj.m_groupid;
        m_proposer = obj.m_proposer;
        m_m = obj.m_m;
        m_n = obj.m_n;
        m_t = obj.m_t;
        for(std::vector<std::string>::const_iterator iter = obj.m_participantList.begin(); 
            iter != obj.m_participantList.end(); ++iter){
                m_participantList.push_back(*iter);
        }
        m_GroupSet = obj.m_GroupSet; 
        m_calculationType = obj.m_calculationType;
    }
    return *this;
}

void GroupMetadata::addUserToGroup(const std::string& userid){
    // the assumption is made that a group proposer is always a member
    if (m_GroupInviteReplies < (m_n-1)){
        ++ m_GroupInviteReplies; 
        m_participantList.push_back(userid);
    }
    return ; 
}

bool GroupMetadata::removeUserFromparticipantList(const std::string& userToDelete){

    m_participantList.erase(std::remove(m_participantList.begin(), m_participantList.end(), userToDelete), m_participantList.end());
    
    if(std::find(m_participantList.begin(), m_participantList.end(), userToDelete) == m_participantList.end())
        return true;
    else
        return false;
} 

void GroupMetadata::addCollatedEvals(const std::string& ord, const std::string& evalOrd, const std::string& eval){

    BigNumber bn;
    bn.FromHex(eval);
    
    std::map<std::string, std::vector<std::pair<std::string, BigNumber> > >::iterator iter = m_CollatedEvals.find(ord);
    if(iter != m_CollatedEvals.end()){
        iter->second.push_back(std::make_pair(evalOrd,bn));
    }else{
        std::vector<std::pair<std::string, BigNumber> > evalsBN;
        evalsBN.push_back(std::make_pair(evalOrd,bn));
        m_CollatedEvals[ord] = evalsBN;
    }
}

void GroupMetadata::addCollatedHiddenPolys(const std::string& ord, const std::vector<std::string>& polyBN){
    
    std::vector<ECPoint> poly; 
    for(std::vector<std::string>::const_iterator iter = polyBN.begin(); iter != polyBN.end(); ++ iter){
        ECPoint pt;
        pt.FromHex(*iter);
        poly.push_back(pt);
    }
    std::map<std::string, std::vector<ECPoint> >::iterator iter = m_CollatedHiddenPolys.find(ord);
    if(iter != m_CollatedHiddenPolys.end()){
        iter->second.insert(iter->second.end(), poly.begin(), poly.end());
    }else{
        m_CollatedHiddenPolys[ord] = poly;
    }
}

void GroupMetadata::addCollatedHiddenEvals(const std::string& ord, const std::string& evalOrd, const std::string& eval){
    ECPoint pt;
    pt.FromHex(eval);
    std::map<std::string, std::vector<std::pair<std::string,ECPoint> > >::iterator iter = m_CollatedHiddenEvals.find(ord);
    if(iter != m_CollatedHiddenEvals.end()){
        iter->second.push_back(std::make_pair(evalOrd,pt));
    }else{
        std::vector<std::pair<std::string,ECPoint> > evals;
        evals.push_back(std::make_pair(evalOrd,pt));
        m_CollatedHiddenEvals[ord]=evals;
    }
    return ; 
}

void GroupMetadata::addCollatedVW (const std::string& ord, const std::string& kalpha, const std::string& alphag){
    BigNumber kalphabn;
    kalphabn.FromHex(kalpha);
    ECPoint alphagpt;
    alphagpt.FromHex(alphag);
    
    std::map<std::string, std::pair<BigNumber, ECPoint> >::const_iterator iter = m_CollatedVW.find(ord); 
    if(iter != m_CollatedVW.end()){
        // check if the values are equal .. throw if not
        if(!(iter->second.first == kalphabn) && iter->second.second != alphagpt){
            throw std::runtime_error ("collatedVW received two different inputs for ordinal " + ord );
        }
    }
    m_CollatedVW[ord] = std::make_pair(kalphabn,alphagpt);
}

void GroupMetadata::addCollatedPartialSignautre(const std::string& ord, const std::string& partialsig){

    // The threshold for signature creataion is 2t+1, however it is assumed that the player requesting 
    // the signature will contribute it's share meaning the code only needs to check for 2t replies
    // otherwise the code would be GroupSignatureReplies < (2*m_t +1 - 1)
    if(m_GroupSignatureReplies < (2*m_t)){
        ++ m_GroupSignatureReplies;
        BigNumber partialSigBN; 
        partialSigBN.FromHex(partialsig);
        

        std::vector<std::pair<std::string, BigNumber> >::const_iterator partSigIter
                = std::find_if(m_CollatedPartialSignatures.begin(), m_CollatedPartialSignatures.end(), [&ord](std::pair<std::string, BigNumber> const& elem) {return elem.first == ord;});
                
        if(partSigIter != m_CollatedPartialSignatures.end()){
            if( !(partSigIter->second == partialSigBN)){
                throw std::runtime_error("CollatedPartialSignautre received two different signatures for the same group and player ordinal");
            }
        }else{
            m_CollatedPartialSignatures.push_back(std::make_pair(ord, partialSigBN));
        }
    }
    return; 
}

void GroupMetadata::clearSharedDataContainers(){
    m_CollatedEvals.clear();
    m_CollatedHiddenPolys.clear();
    m_CollatedHiddenEvals.clear();
}

void GroupMetadata::clearSignatureContainers(){
    // reset the signature replies
    m_GroupSignatureReplies = 0;
    m_CollatedPartialSignatures.clear();

}

void GroupMetadata::clearPreSignatureContainers(){
    m_CollatedVW.clear();
}

std::ostream& operator<< (std::ostream& os,const GroupMetadata& grp){
    os << "Group Metadata for " << grp.m_groupid
        << "\n\tm = " << grp.m_m
        << "\n\tn = " << grp.m_n
        << "\n\tt = " << grp.m_t
        << "\n\tproposer = " << grp.m_proposer
        << "\n\tnumReplies = " << grp.m_GroupInviteReplies
        << "\n\tGroup size = " << grp.m_participantList.size()
        << "\n\tGroup set = "
        ;
    (grp.m_GroupSet) ? os << "True\n" : os << "False\n" ;
    std::vector<std::string>::const_iterator iter = grp.m_participantList.begin();
    for (; iter != grp.m_participantList.end(); ++ iter){
        os << *iter << " " << std::endl;
    }
    
    os << "\n\tCollated Public Evals = \n" ;
    for(std::map<std::string, std::vector<std::pair<std::string, BigNumber> > >::const_iterator iter = grp.m_CollatedEvals.begin();
            iter != grp.m_CollatedEvals.end(); ++ iter){
            os << iter->first << "\t";
            for(std::vector<std::pair<std::string, BigNumber> >::const_iterator innerIter = iter->second.begin(); innerIter != iter->second.end(); ++ innerIter){
                os << "[" << innerIter->first << "," << innerIter->second.ToHex() << "],";
            }
            os << "\n";
    }
    
    os << "\n\tCollated Hidden Polynomials = \n";
    for(std::map<std::string, std::vector<ECPoint> >::const_iterator iter =  grp.m_CollatedHiddenPolys.begin(); iter != grp.m_CollatedHiddenPolys.end(); ++ iter ){
        os << iter->first << "\t[";
        for(std::vector<ECPoint>::const_iterator innerIter = iter->second.begin(); innerIter != iter->second.end(); ++ innerIter){
            os << innerIter->ToHex() << " " ;
        }
        os << "]\n";
    };
    
    os << "\n\tCollated Hidden Evals = \n" ; 
    for(std::map<std::string, std::vector<std::pair<std::string, ECPoint> > >::const_iterator iter = grp.m_CollatedHiddenEvals.begin();
            iter != grp.m_CollatedHiddenEvals.end(); ++ iter){
        os << iter->first << "\t";
        for(std::vector<std::pair<std::string, ECPoint> >::const_iterator innerIter = iter->second.begin(); innerIter != iter->second.end(); ++ innerIter){
            os << "[" << innerIter->first << "," << innerIter->second.ToHex() << "],";
        }
        os << "\n";
    }
    
    os << "\n\tCollated VW shares = \n";
    for(std::map<std::string, std::pair<BigNumber, ECPoint> >::const_iterator iter = grp.m_CollatedVW.begin(); iter != grp.m_CollatedVW.end(); ++iter){
        os << iter->first << "\t[" << iter->second.first.ToHex() << "],[" << iter->second.second.ToHex() << "],";
    }
    
    os << "\n\tCollated Partial Signatures = \n";
    for(std::vector<std::pair<std::string, BigNumber> >::const_iterator iter = grp.m_CollatedPartialSignatures.begin(); iter != grp.m_CollatedPartialSignatures.end(); ++iter){
        os << "[" << iter->first << "," << iter->second.ToHex() << "]" << std::endl;
    }
    os << "\n";
    return os;
}
