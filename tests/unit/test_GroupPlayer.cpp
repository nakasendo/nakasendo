#define BOOST_TEST_MODULE test_ECPoints
#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <Group.h>
#include <Player.h>
#include <UUID.h>
#include <single_include/nlohmann/json.hpp>

bool playerMapCompare(const Group::PlayerMap& lhs, const Group::PlayerMap& rhs)
{
    return lhs.size() == rhs.size() && lhs == rhs;
}

BOOST_AUTO_TEST_SUITE(test_suite_ThresholdSignatures)

/*test case : test the addition of ECPoints, and compare the result*/
BOOST_AUTO_TEST_CASE(testCreatePlayer)
{
    Player p1 {"p1", 1};
    Player p2 {"p2", 2};
    Player p3 {"p3", 3}; 
    BOOST_CHECK(p1.getURI() == "p1" && p1.getOrdinal() == 1);
    BOOST_CHECK(p2.getURI() == "p2" && p2.getOrdinal() == 2);
    BOOST_CHECK(p3.getURI() == "p3" && p3.getOrdinal() == 3);
}

BOOST_AUTO_TEST_CASE(testCreateGroup)
{
    Player p1 {"p1", 1};
    Player p2 {"p2", 2};
    Player p3 {"p3", 3}; 

    Group::PlayerMap playersMap 
    {
        {"player1@mycorp.com", p1},
        {"player2@otherplace.org", p2},
        {"player3@secret.org", p3}
    };

    /// A unique id of the group
    UUID gGr1Id {"group1"};

    Group gGroup1 {gGr1Id, playersMap};
    BOOST_CHECK(gGroup1.getPlayers().size() == playersMap.size());
}

BOOST_AUTO_TEST_CASE(testCreateGroupWithRandomUUID)
{
    Player p1 {"p1", 1};
    Player p2 {"p2", 2};
    Player p3 {"p3", 3}; 

    Group::PlayerMap playersMap 
    {
        {"player1@mycorp.com", p1},
        {"player2@otherplace.org", p2},
        {"player3@secret.org", p3}
    };

    /// A unique id of the group
    UUID gGr1Id ;

    Group gGroup1 {gGr1Id, playersMap};

    BOOST_CHECK(gGr1Id == gGroup1.getID());
    
}

BOOST_AUTO_TEST_CASE(testGroupGetPlayers)
{
    Player p1 {"p1", 1};
    Player p2 {"p2", 2};
    Player p3 {"p3", 3}; 

    Group::PlayerMap playersMap 
    {
        {"player1@mycorp.com", p1},
        {"player2@otherplace.org", p2},
        {"player3@secret.org", p3}
    };

    /// A unique id of the group
    UUID gGr1Id ;

    Group gGroup1 {gGr1Id, playersMap};

    BOOST_CHECK(playerMapCompare(playersMap, gGroup1.getPlayers()));
}

BOOST_AUTO_TEST_CASE(testGroupClearPlayersAndSize)
{
    Player p1 {"p1", 1};
    Player p2 {"p2", 2};
    Player p3 {"p3", 3}; 

    Group::PlayerMap playersMap 
    {
        {"player1@mycorp.com", p1},
        {"player2@otherplace.org", p2},
        {"player3@secret.org", p3}
    };

    /// A unique id of the group
    UUID gGr1Id ;

    Group gGroup1 {gGr1Id, playersMap};
    
    gGroup1.clearPlayers();
    BOOST_CHECK(gGroup1.size() == 0);
}

BOOST_AUTO_TEST_CASE(testGroupGetPlayersSortedByOrdinal)
{
    Player p1 {"p1", 1};
    Player p2 {"p2", 2};
    Player p3 {"p3", 3}; 

    std::vector<Player> playerVec1 {p1, p2, p3};

    Group::PlayerMap playersMap 
    {
        {"player1@mycorp.com", p1},
        {"player2@otherplace.org", p2},
        {"player3@secret.org", p3}
    };


    /// A unique id of the group
    UUID gGr1Id ;

    Group gGroup1 {gGr1Id, playersMap};
    
    // get the Player vector
    auto playerVec2 = gGroup1.getPlayersSortedByOrdinal();

    // compare both playerVec1 and playerVec2
    BOOST_CHECK(std::equal(playerVec1.begin(), playerVec1.end(), playerVec2.begin(), [](Player& lhs, Player& rhs){ return (lhs.getURI() == rhs.getURI() && lhs.getOrdinal() == rhs.getOrdinal());}));
}

BOOST_AUTO_TEST_CASE(testGroupGetPlayerFromOrdinal)
{
    Player p1 {"p1", 1};
    Player p2 {"p2", 2};
    Player p3 {"p3", 3}; 

    Group::PlayerMap playersMap 
    {
        {"player1@mycorp.com", p1},
        {"player2@otherplace.org", p2},
        {"player3@secret.org", p3}
    };

    /// A unique id of the group
    UUID gGr1Id ;

    Group gGroup1 {gGr1Id, playersMap};
    
    Player tmpPlayer = gGroup1.getPlayerFromOrdinal(1);

    BOOST_CHECK(tmpPlayer.getURI() == p1.getURI() && tmpPlayer.getOrdinal() == p1.getOrdinal());
}

BOOST_AUTO_TEST_CASE(testGroupCompare)
{
    Player p1 {"p1", 1};
    Player p2 {"p2", 2};
    Player p3 {"p3", 3}; 

    Group::PlayerMap playersMap 
    {
        {"player1@mycorp.com", p1},
        {"player2@otherplace.org", p2},
        {"player3@secret.org", p3}
    };

    /// A unique id of the group
    UUID gGr1Id, gGr1Id1 ;

    Group gGroup1 {gGr1Id, playersMap};
    Group gGroup2 {gGr1Id, playersMap};
    Group gGroup3 {gGr1Id1, playersMap};

     BOOST_CHECK  (gGroup1 == gGroup2); 
     BOOST_CHECK  (!(gGroup1 == gGroup3)); 

}

BOOST_AUTO_TEST_CASE(testGroupEmplacePLayer)
{

    Player p1 {"p1", 1};
    Player p2 {"p2", 2};
    Player p3 {"p3", 3}; 
    Player p4 {"p4", 4}; 

    Group::PlayerMap playersMap 
    {
        {"player1@mycorp.com", p1},
        {"player2@otherplace.org", p2},
        {"player3@secret.org", p3}
    };

    /// A unique id of the group
    UUID gGr1Id, gGr1Id1 ;

    Group gGroup1 {gGr1Id, playersMap};
    gGroup1.emplace_player(std::make_pair("player4@secret.org", p4));

    Player tmpPlayer = gGroup1.getPlayerFromOrdinal(4);

    BOOST_CHECK(tmpPlayer.getURI() == p4.getURI() && tmpPlayer.getOrdinal() == p4.getOrdinal());
}

BOOST_AUTO_TEST_CASE(testGroupToJsonFromJson)
{

    Player p1 {"p1", 1};
    Player p2 {"p2", 2};
    Player p3 {"p3", 3}; 
    Player p4 {"p4", 4}; 

    Group::PlayerMap playersMap 
    {
        {"player1@mycorp.com", p1},
        {"player2@otherplace.org", p2},
        {"player3@secret.org", p3}
    };

    /// A unique id of the group
    UUID gGr1Id, gGr1Id1 ;

    Group gGroup1 {gGr1Id, playersMap};
    gGroup1.emplace_player(std::make_pair("player4@secret.org", p4));

    std::string gGroup1JSONString = Group::to_json(gGroup1);
    Group gGroup2  = Group::from_json(gGroup1JSONString);
    
    BOOST_CHECK  (gGroup1 == gGroup2); 
}

BOOST_AUTO_TEST_SUITE_END();
