/// Define test module name with debug postfix
#ifdef NDEBUG 
#  define BOOST_TEST_MODULE test_TSPlayerGroup
#else
#  define BOOST_TEST_MODULE test_TSPlayerGroup
#endif

#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <BigNumbers/BigNumbers.h>
#include <Polynomial/Polynomial.h>
#include <Polynomial/LGInterpolator.h>
#include <SecretSplit/SecretSplit.h>
#include <SecretSplit/KeyShare.h>
#include <GroupMetaData.h>
#include <player.h>
#include <orchestrator.h>
#include <TSState.h>
#include <random>
#include <ctime>
#include <string>
#include <iostream>
#include <set>
#include <fstream>


std::string random_string( std::size_t length )
{
    static const std::string alphabet = "abcdefghijklmnopqrstuvwxyz" ;
    static std::default_random_engine rng( std::time(nullptr) ) ;
    static std::uniform_int_distribution<std::size_t> distribution( 0, alphabet.size() - 1 ) ;

    std::string str ;
    while( str.size() < length ) str += alphabet[ distribution(rng) ] ;
    return str ;
}

using playerGroups = std::map<std::string, playerGroupMetaData>; 

class ExtendedPlayer : public player
{
    public:
        ExtendedPlayer(const std::string& pid, const std::string& uri, const std::string& addr, const std::string& port)
            : player(pid,uri,addr,port)
        { return ;}
        
        const playerGroups& PlayerGroup() const { return m_PlayerGroup;}
        void addPlayerGroup(const playerGroupMetaData& obj){
            m_PlayerGroup[obj.m_id] = obj;
        }
        
    private:
        playerGroups m_PlayerGroup;
};

BOOST_AUTO_TEST_SUITE( test_suite_TSPlayerGroup )


BOOST_AUTO_TEST_CASE( test_TS_PG )
{
    
    int tt = rand () % 10 + 1;
    int margin = rand () % 6;
    int nn = 2*tt+1 + margin;
    
    BigNumber mod; 
    mod.FromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141"); 
    
    std::cout << "Creating a group with parameters tt = " << tt << " nn = " << nn << std::endl;
    std::vector<ExtendedPlayer> players; 
    for(int i=0;i<nn;++i){
        ExtendedPlayer p (random_string(10), "","127.0.0.1", "800" + std::to_string(i));
        players.push_back(p);
    }
    
    std::string grpID = generateUUID();
    std::cout << grpID << std::endl;
    for (int i = 0;i<players.size(); ++i){
        playerGroupMetaData pgrp (tt,mod);
        pgrp.m_id = grpID;
        int ordinal=1;
        for(int j=0;j<players.size();++j){
            if(players[i].userID() == players[j].userID()){
                pgrp.m_ordinal = ordinal++;
            }else{
                player p(players[j].userID(),players[j].uri(),players[j].addr(),players[j].port());
                pgrp.m_ordinalAndPlayerList.push_back(std::make_pair(ordinal++,p));
            }
        }
        pgrp.polynomialPreCalculation(pgrp.m_privateKeyPolynomial);
        players[i].addPlayerGroup(pgrp);
    }
    
    // write each group state to disk. 
    // read it back in. 
    // should be equal.
    
    for(std::vector<ExtendedPlayer>::const_iterator iter = players.begin(); iter != players.end(); ++iter){
        const playerGroups& pg = iter->PlayerGroup();
        if( pg.find(grpID) == pg.end()){
            // no group
        }
        const playerGroupMetaData& pgrp = pg.find(grpID)->second;
        std::string filename = iter->userID() + std::to_string(pgrp.m_ordinal) + ".txt";
        std::cout << filename << std::endl;
        WritePlayerGroupStateToFile(pgrp,filename);
    }
    
    for(std::vector<ExtendedPlayer>::const_iterator iter = players.begin(); iter != players.end(); ++iter){
        const playerGroups& pg = iter->PlayerGroup();
        if( pg.find(grpID) == pg.end()){
            std::cout << "No group found" << std::endl;
        }
        const playerGroupMetaData& pgrp = pg.find(grpID)->second;
        std::string filename = iter->userID() + std::to_string(pgrp.m_ordinal) + ".txt";
        playerGroupMetaData testpgrp;
        std::cout << filename << std::endl;
        try{
            ReadPlayerGroupStateFromFile(testpgrp,filename);
            std::cout << pgrp.m_id << " " << testpgrp.m_id << std::endl;
            BOOST_CHECK(pgrp.m_id == testpgrp.m_id);
            if(remove(filename.c_str()) != 0){
                std::cout << "Unable to delete protobuf file" << std::endl;
            }
        }
        catch(std::exception& e){
            std::cout << e.what() << std::endl;
        }
    }
    
}


BOOST_AUTO_TEST_CASE( test_suite_TSGROUP )
{
    int tt = rand () % 10 + 1;
    int margin = rand () % 6;
    int nn = 2*tt+1 + margin;
    
    
    std::cout << "Creating a group with parameters tt = " << tt << " nn = " << nn << std::endl;

    playerCommsInfo localPlayer;
    localPlayer.m_userid = random_string(10);
    localPlayer.m_ip = "127.0.0.1";
    localPlayer.m_port = "8079";
    
    std::string grpid=generateUUID();
    GroupMetadata grp (grpid,localPlayer, tt+1, nn, tt);
    
    for(int i=0;i<nn-1; ++i){
        grp.addUserToGroup(random_string(10), "","127.0.0.1","808" + std::to_string(i));
    }
    
    
    
    for(int j=0;j<grp.n();++j){
        std::vector<std::string> polys;
        for(int i=0; i < grp.n(); ++i){
            ECPoint pt; 
            pt.SetRandom();
            polys.push_back(pt.ToHex());
        }
        grp.addCollatedHiddenPolys(std::to_string(j), polys);
    }
    
    
    
    for(int j=0;j<grp.n(); ++j){
        for(int i=0;i<grp.n(); ++i){
            if(i != j){
                ECPoint pt;
                pt.SetRandom();
                grp.addCollatedHiddenEvals(std::to_string(j), std::to_string(i), pt.ToHex());
            }
        }
    }
    
    //set the group replies so 
    for(int j=0;j<grp.n(); ++j){
        BigNumber v = GenerateRand(512);
        BigNumber sig = GenerateRand(256);
        ECPoint pt;
        pt.SetRandom();
        grp.addCollatedVW(std::to_string(j), v.ToHex(), pt.ToHex()); 
        grp.addCollatedPartialSignautre(std::to_string(j), sig.ToHex());
    }
    //std::cout << grp << std::endl;
    // write the grp to file
    std::string filename ("GroupStreamTest.txt");
    {
        std::fstream output(filename, std::ios::out | std::ios::trunc | std::ios::binary);
        WriteGroupMetaDataState(grp, output);
    }
    
    // read it back 
    //std::cout << "Reading it back in" << std::endl; 
    {
        std::fstream input(filename, std::ios::in | std::ios::binary);
        if(!input){
            std::cerr << "Failed to open file " << filename << std::endl;
        }else{
            GroupMetadata anotherGrp;
            ReadGroupMetaDataState(anotherGrp,input); 
            BOOST_CHECK(grp.groupid() == anotherGrp.groupid());
            if(remove(filename.c_str()) != 0){
                std::cerr << "Weirdness deleting the file " << filename << std::endl;
            }
        }
    }

}

////
BOOST_AUTO_TEST_SUITE_END( ) ;


