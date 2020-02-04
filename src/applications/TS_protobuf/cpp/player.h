#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include <map>
#include <string>
#include <memory>
#include <mutex>
#include <Polynomial/Polynomial.h>
#include <BigNumbers/BigNumbers.h>
#include <ECPoint/ECPoint.h>
#include <exportTSCore.h>

class TSCore_API player
{

	TSCore_API friend std::ostream& operator<< (std::ostream&, const player&);
    public:
        player() : m_userID(), m_uri(), m_address (), m_port() { return ; }
        player(const std::string&, const std::string&, const std::string&, const std::string&);
        ~player();
        
        player(const player& obj);
        player& operator=(const player& obj);
        
        const std::string& userID() const { return m_userID;}
        const std::string& uri() const { return m_uri; }
        const std::string& addr() const { return m_address; }
        const std::string& port() const { return m_port; }
        
        
    private:
        std::string m_userID;
        std::string m_uri;
        std::string m_address; 
        std::string m_port;
};


struct TSCore_API jvrss
{
	TSCore_API friend std::ostream& operator<< (std::ostream&, const jvrss&);
    jvrss() { return ; }
    ~jvrss() = default;
    
    jvrss(const jvrss& obj)
        : m_fx(obj.m_fx), m_evals(obj.m_evals), m_publicEvals(obj.m_publicEvals),m_hiddenEvals(obj.m_hiddenEvals) 
        , m_hiddenPolynomial(obj.m_hiddenPolynomial), m_allHiddenPolynomials(obj.m_allHiddenPolynomials), m_allVWShares(obj.m_allVWShares)
    { return ; }
    
    jvrss& operator=(const jvrss& obj){
        if(this != &obj){
            m_fx = obj.m_fx;
            m_evals = obj.m_evals;
            m_publicEvals = obj.m_publicEvals;
            m_hiddenEvals = obj.m_hiddenEvals;
            m_hiddenPolynomial = obj.m_hiddenPolynomial;
            m_allHiddenPolynomials = obj.m_allHiddenPolynomials;
            m_allVWShares = obj.m_allVWShares;
        }
        return *this;
    }
    
    void reset(); 
    bool verifyCorrectness (const std::vector<int>&, const BigNumber&);
    bool verifyHonesty(const std::vector<int>&, const BigNumber&);
    
    BigNumber m_fx;
    std::map<std::string, BigNumber> m_evals; // evaluated for each entry in the ordinallist (local player)
    std::vector<std::pair<std::string, BigNumber> > m_publicEvals; // local player's label evaluated by the other players polynomial and shared. 

    std::map<std::string, std::vector<std::pair<std::string, ECPoint> > > m_hiddenEvals; //Polynomial evaluations multiplied by generator point
    std::vector<ECPoint> m_hiddenPolynomial; // private polynonial encrypted
    std::map<std::string,std::vector<ECPoint> > m_allHiddenPolynomials; // all other users polynomials encrypted
    std::map<std::string,std::pair<BigNumber, ECPoint> > m_allVWShares; // V & W from all players
        
};

struct TSCore_API playerGroupMetaData
{
	TSCore_API friend std::ostream& operator<< (std::ostream&, const playerGroupMetaData&);
    
    
    
    playerGroupMetaData()=default;
    
    playerGroupMetaData(const int degree, const BigNumber& mod)
        : m_id(), m_ordinal(), m_ordinalAndPlayerList ()
        , m_degree(degree)
        , m_privateKeyPolynomial(degree, mod), m_privateKeyShare(), m_GroupPublicKey()
        , m_EmpheralKeyList(), m_littleK(), m_alpha()
        , m_PrivateKeyShares()
        , m_modulo(mod)
        , m_transientData()
    {return;}
    
    playerGroupMetaData(const playerGroupMetaData& obj) 
        : m_id(obj.m_id), m_ordinal(obj.m_ordinal), m_ordinalList(obj.m_ordinalList), m_ordinalAndPlayerList(obj.m_ordinalAndPlayerList)
        , m_degree(obj.m_degree), m_privateKeyPolynomial(obj.m_privateKeyPolynomial)
        , m_privateKeyShare(obj.m_privateKeyShare), m_GroupPublicKey(obj.m_GroupPublicKey)
        , m_EmpheralKeyList(obj.m_EmpheralKeyList), m_littleK(obj.m_littleK), m_alpha(obj.m_alpha)
        , m_PrivateKeyShares() , m_modulo(obj.m_modulo)
        , m_transientData(obj.m_transientData)
    { return ; }
    
    playerGroupMetaData& operator= (const playerGroupMetaData& obj){
        if(this != &obj){
            m_id = obj.m_id;
            m_ordinal = obj.m_ordinal;
            m_ordinalList = obj.m_ordinalList;
            m_ordinalAndPlayerList = obj.m_ordinalAndPlayerList;
            m_degree = obj.m_degree;
            m_privateKeyPolynomial = obj.m_privateKeyPolynomial;
            m_privateKeyShare = obj.m_privateKeyShare;
            m_GroupPublicKey = obj.m_GroupPublicKey;
            m_EmpheralKeyList = obj.m_EmpheralKeyList;
            m_littleK = obj.m_littleK; 
            m_alpha = obj.m_alpha; 
            m_PrivateKeyShares = obj.m_PrivateKeyShares;
            m_modulo = obj.m_modulo;
            m_transientData = obj.m_transientData;
        }
        return *this;
    }
    ~playerGroupMetaData() = default;
    
    Polynomial createPolynomial(const int&, const BigNumber&); 
    
    void polynomialPreCalculation(const Polynomial&);
    void addPublicEvalsToJVRSS(const std::string&, const std::string&);
    void addHiddenEvalsToJVRSS(const std::string&, const std::vector<std::pair<std::string, std::string> >&);
    void addHiddenPolynomialToJVRSS(const std::string&, const std::vector<std::string>& ) ; 
    void addPreSignDataToJVRSS(const std::string&, const std::string&, const std::string&);
    
    
    bool verifyCorrectness();
    bool verifyHonesty();
    
    BigNumber createSecret();
    ECPoint createGroupPublicKey(); 
    
    bool CalculateEphemeralKey();

    BigNumber Signature(const std::string&, const std::pair<BigNumber,BigNumber>&);
    std::pair<std::string, BigNumber> CalcPartialSignature (const std::string&, const int& ); 
    std::pair<BigNumber, BigNumber> CalculateGroupSignature(const std::string& , const int&, const std::vector<std::pair<std::string, std::string> >& ) ; 
    void addPrivateKeyInfo(const std::string&, const std::string&);

    const BigNumber& privateKeyShare () const { return m_privateKeyShare; }
    BigNumber CalculateGroupPrivateKey ();
    bool ValidateGroupPrivateKey(const BigNumber&);

    std::string m_id; // Group ID
    int m_ordinal; // label assigned by the orchestrator
    std::vector<int> m_ordinalList; // labels of other participants in the group .. to be deleted
    std::vector<std::pair<int, player> > m_ordinalAndPlayerList; // lables and player info for the rest of the group
    int m_degree; // degree of the polynomial
    Polynomial m_privateKeyPolynomial; //key polynomial for this group
    BigNumber m_privateKeyShare; // calculated share of private key
    ECPoint m_GroupPublicKey; // Group Public key
    
    std::vector<std::pair<BigNumber, BigNumber> > m_EmpheralKeyList; 
    BigNumber m_littleK;
    BigNumber m_alpha; 
    
    // a vector to hold ordinals & private key shares when a reconstruction of a private key is required
    std::vector<std::pair<BigNumber, BigNumber> > m_PrivateKeyShares; 

    BigNumber m_modulo; 
    
    jvrss m_transientData;
};


// static version of player. which the client keeps private
using playerGroups = std::map<std::string, playerGroupMetaData>; 
class TSCore_API SinglePlayer
{
    public:
        
        static std::unique_ptr<SinglePlayer>& Instance(); 
        const player& publicInfo() const { return m_PublicInfo; }
        player& publicInfo() { return m_PublicInfo; }
                 
        // 
        
        const std::string& UserID() const  { return m_PublicInfo.userID(); }
        
        const playerGroups& PlayerGroup() const { return m_PlayerGroup;}
        
        void addPlayerGroup(const playerGroupMetaData&);
        std::vector<std::string> GroupIDS () const; 
        
        const playerGroupMetaData& groupdetails (const std::string&) const; 
        playerGroupMetaData& groupdetails(const std::string&);
        
        bool deleteGroup(const std::string&);
        
    private:
    
        SinglePlayer()=default;
        SinglePlayer(const SinglePlayer&) = delete;
        SinglePlayer& operator=(const SinglePlayer&) = delete; 
        static std::unique_ptr<SinglePlayer> m_Instance; 
        static std::once_flag m_onceFlag;
        
        player m_PublicInfo;
        playerGroups m_PlayerGroup;
    
};


// free functions
TSCore_API std::pair<std::string, bool>  acceptInvitationToGroup(const std::string&);
TSCore_API void  deletePlayerGroup(const std::string&, const std::string&);
TSCore_API const player& getPublicPlayerInfo();
TSCore_API const jvrss& PlayerSecretDataForGrp (const std::string&,const std::string& calctype);
TSCore_API playerGroupMetaData& PlayerGrpData (const std::string& );

TSCore_API const int& ordinalForGroup(const std::string&);

TSCore_API std::pair<BigNumber,ECPoint>  EphemeralKeyPlayerData(const std::string&);




#endif //ifndef __PLAYER_H__

