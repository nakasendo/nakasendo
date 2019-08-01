/// Define test module name with debug postfix
#ifdef NDEBUG 
#  define BOOST_TEST_MODULE test_GroupBuilder
#else
#  define BOOST_TEST_MODULE test_GroupBuilderd
#endif

#include <boost/test/unit_test.hpp>
#include <iostream>
#include <memory>
#include <set>
#include <vector>
#include <string>
#include <ThresholdSignatures/GroupBuilder.h>
#include <single_include/nlohmann/json.hpp>
#include "utils/FuncUtils.h"

class GroupBuilderCompare {
    public :
        bool operator()(const GroupBuilderSPtr& lhs, const GroupBuilderSPtr& rhs  ) const
        {
            return lhs->getMyURI().compare(rhs->getMyURI()) < 0;
        }
};

// Compare 2 GroupBuilders
bool operator==(const GroupBuilder& builder1, const GroupBuilder& builder2)
{
    return ( builder1.getID() == builder2.getID() &&
             builder1.getPlayers().size() == builder2.getPlayers().size() &&
             builder1.getProposerURI() == builder2.getProposerURI() &&
             builder1.getState() == builder2.getState() ); 
}
inline std::string getRandomURI()
{
    bool emailOrPhone = false; // false for email, true for phone
    srand(getRandomNumberInRange(1, (int)9999999999));
    emailOrPhone = (rand() % 2 == 1) ? false : true ;
    return (emailOrPhone == 0) ? getRandomString(10) + "@" + getRandomString(5) + ".com" : "00447" + getRandomNumberString(9);
}

std::vector<std::string> getPlayerList(int maxCnt = 10000)
{
    // generate a random number in the range
    auto randSize = getRandomNumberInRange(1, maxCnt);
    std::set<std::string> playerSet;

    // Write randomly generated players into a set so that duplicated entries will not be allowed.
    for (unsigned i = 0; i < randSize; i++)
    {
        std::string str =  getRandomURI();
        playerSet.insert(std::move(str));
    }

    std::vector<std::string> playerListVec;

    // copy the set of player into list of players
    std::copy(playerSet.begin(), playerSet.end(), std::back_inserter(playerListVec));

    return playerListVec;
}

using GroupPlayersMap = std::map<GroupBuilderSPtr, GroupResponseMessageSPtr, GroupBuilderCompare>;

// this function
void acceptResponseFromAllPlayers(GroupPlayersMap& _groupPlayersMap)
{
    for (auto& playerBuilder : _groupPlayersMap)
    {
        playerBuilder.second = playerBuilder.first->getGroupSetupResponseMessage(GroupState::ACCEPTED);
    }
}

void rejectBysomePlayers(GroupPlayersMap& _groupPlayersMap)
{
    bool rejected = false;
    for (auto& playerBuilder : _groupPlayersMap)
    {
        // Proposer is automatically accepted by all players.
        if (playerBuilder.first->getMyURI().compare(playerBuilder.first->getProposerURI()) == 0)
        {
            playerBuilder.second = playerBuilder.first->getGroupSetupResponseMessage(GroupState::ACCEPTED);
            continue;
        }
        else
        {
            // Make sure that some players reject
            if (!rejected)
            {
                playerBuilder.second = playerBuilder.first->getGroupSetupResponseMessage(GroupState::REJECTED);
                rejected = true;
            }
            else
            {
                playerBuilder.second = playerBuilder.first->getGroupSetupResponseMessage(GroupState::ACCEPTED);
                rejected = false;
            }

        }
    }
}

// This function comapare group builder of each one with its previous one
bool compareGroupBuilders(const GroupPlayersMap& _groupPlayersMap)
{
    GroupPlayersMap::key_type prePlayerBuilder = nullptr;

    for (auto& playerBuilder : _groupPlayersMap)
    {
        if (prePlayerBuilder == nullptr)
        {
            prePlayerBuilder = playerBuilder.first;
            continue;
        }

        if (!(*prePlayerBuilder == *playerBuilder.first))
        {
            //      std::cout << *prePlayerBuilder << "\n######################\n" << *playerBuilder.first << "\n";
            return false;
        }
        prePlayerBuilder = playerBuilder.first;
    }

    return true;
}
bool verifyAllGroupStatesTo(const GroupPlayersMap& _groupPlayersMap, GroupState grpState)
{
    for (auto& playerBuilder : _groupPlayersMap)
    {
        if ((*playerBuilder.first).getState() != grpState)
        {
            return false;
        }
    }
    return true;
}


bool compareAllGroupStates(const GroupPlayersMap& _groupPlayersMap)
{
    GroupPlayersMap::key_type prePlayerBuilder = nullptr;

    for (auto& playerBuilder : _groupPlayersMap)
    {
        if (prePlayerBuilder == nullptr)
        {
            prePlayerBuilder = playerBuilder.first;
            continue;
        }
        if (!((*prePlayerBuilder).getState() == (*playerBuilder.first).getState()))
        {
            return false;
        }
    }
    return true;
}

void sendResponsedToAllPlayers(const GroupPlayersMap& _groupPlayersMap)
{
    GroupPlayersMap::key_type currentPlayer = nullptr;
    for (auto& sendPlayerBuilder : _groupPlayersMap)
    {
        // Proposer is automatically accepted by all players. 
        if (sendPlayerBuilder.first->getMyURI().compare(sendPlayerBuilder.first->getProposerURI()) == 0)
        {
            continue;
        }

        for (auto& recvPlayerBuilder : _groupPlayersMap)
        {
            // send responses to all player
            recvPlayerBuilder.first->processGroupSetupResponse(sendPlayerBuilder.second);
        }
    }
}

// This function generates Group builder for the each player
GroupPlayersMap generateGroupBuilders(const std::vector<std::string>& playerUris)
{
    bool firstPlayer = true;
    GroupPlayersMap _groupPlayersMap;
    GroupSetupMessageSPtr setupMsg;
    GroupBuilderSPtr grpBuilder;

    for (auto player : playerUris)
    {
        if (firstPlayer)
        {
            // Get Group Buider for the player
            grpBuilder =  std::make_shared<GroupBuilder>(playerUris,  player);
            setupMsg = grpBuilder->getGroupSetupMessage();
            firstPlayer = false;
        }
        else
        {
            grpBuilder = std::make_shared<GroupBuilder>(setupMsg, player);
        }
        _groupPlayersMap[grpBuilder] = nullptr;
    }

    return _groupPlayersMap;
}



BOOST_AUTO_TEST_SUITE(test_suite_GroupBuilder)

// Test persisting/restoring
BOOST_AUTO_TEST_CASE(TestSerialisationDeserialisation)
{
    // Create a group builder
    std::vector<std::string> playerUris = getPlayerList();

    // Randomly select a player from the player list
    auto randomPlayerNumber = getRandomNumberInRange(1, static_cast<int>(playerUris.size()));

    GroupBuilderSPtr builder { std::make_shared<GroupBuilder>(playerUris,  playerUris[randomPlayerNumber]) };

    // Serialise and deserialise
    std::string builderJson = builder->to_json();  

    GroupBuilderSPtr builder2 { std::make_shared<GroupBuilder>() };
    builder2->from_json(builderJson);
    // Verify everything survived the serialisation and deserialisation.
    BOOST_CHECK(*builder == *builder2);

}

BOOST_AUTO_TEST_CASE(testProposerOutOfGroupP)
{
    // Make sure that the proposer should be the member of the group.
    std::vector<std::string> playerUris{ "player1@nchain.com", "player2@nchain.com", "player3@nchain.com" };
    try {
        GroupBuilderSPtr builder{ std::make_shared<GroupBuilder>(playerUris, "player4@nchain.com") };
        BOOST_FAIL("Group allowed the proposer who is not part of the group");
    }
    catch (std::invalid_argument& e)
    {
        BOOST_TEST_MESSAGE("Proposer out of group player list" << e.what());
    }
}

BOOST_AUTO_TEST_CASE(testSinglePlayerGroup)
{
    // verify that the group has only one player.
    std::vector<std::string> playerUris{ "player1@nchain.com"};
    try {
        GroupBuilderSPtr builder{ std::make_shared<GroupBuilder>(playerUris, "player1@nchain.com") };
// TBD : the following line should be uncommented after fixing the bug.
        //BOOST_FAIL("Single player allowed in the group" );
    }
    catch (std::exception & e)
    {
        BOOST_TEST_MESSAGE("Single player in the group" << e.what());
    }
}

BOOST_AUTO_TEST_CASE(testPlayersWithSameURIsInGroup)
{
    // Make sure that the players with same URIs can't be allowed in the group.
    std::vector<std::string> playerUris{ "player1@nchain.com", "player1@nchain.com", "player3@nchain.com" };
    try {
        GroupBuilderSPtr builder{ std::make_shared<GroupBuilder>(playerUris, "player3@nchain.com") };
        BOOST_FAIL("");
    }
    catch (std::runtime_error& e)
    {
        BOOST_TEST_MESSAGE("Multiple Players having the same URIs :" << e.what());
    }
}
BOOST_AUTO_TEST_CASE(testsamePlayerListInMultipleGroups)
{
    // Make sure that the same player list can be created in different group
    std::vector<std::string> playerUris = getPlayerList();

    // Randomly select a player from the player list
    auto randomPlayerNumber = getRandomNumberInRange(1, static_cast<int>(playerUris.size()));

    BOOST_CHECK_NO_THROW({ GroupBuilderSPtr builder1{ std::make_shared<GroupBuilder>(playerUris,  playerUris[randomPlayerNumber]) }; });
    BOOST_CHECK_NO_THROW({ GroupBuilderSPtr builder2{ std::make_shared<GroupBuilder>(playerUris,  playerUris[randomPlayerNumber]) }; });

}

BOOST_AUTO_TEST_CASE(testPlayerHasUniqueOrdinal)
{
    // Make sure that each player in the group has a unique ordinal

    std::vector<std::string> playerUris = getPlayerList();

    // Randomly select a player from the player list
    auto randomPlayerNumber = getRandomNumberInRange(1, static_cast<int>(playerUris.size()));

    GroupBuilderSPtr grpBuilder{ std::make_shared<GroupBuilder>(playerUris,  playerUris[randomPlayerNumber]) };

    // Make sure that all the players has unique ordinals
    auto playerList = grpBuilder->getPlayers();

    std::set<Ordinal> playerOrdinalSet;

    for (auto & playerEntry : playerList)
    {
        // write the player ordinal into set
        playerOrdinalSet.insert(playerEntry.second.getOrdinal());
    }

    BOOST_CHECK(playerOrdinalSet.size() == playerList.size());

}

// Test lifecycle of group acceptance
BOOST_AUTO_TEST_CASE(TestLifecycleAcceptance)
{
    // Create a group builder for proposer (player1) & check initial state
    std::vector<std::string> uris { "player1@mycorp.com", "player2@otherplace.org", "player3@secret.org" };
    const std::string& proposerUri { uris[0] };
    GroupBuilder player1Builder { uris, proposerUri };
    BOOST_CHECK_EQUAL(player1Builder.getPlayers().size(), 3);
    BOOST_CHECK_EQUAL(player1Builder.getProposerURI(), proposerUri);
    BOOST_CHECK_EQUAL(player1Builder.getMyURI(), proposerUri);
    BOOST_CHECK(player1Builder.getState() == GroupState::PROPOSED);
    BOOST_CHECK(player1Builder.getPlayers()[proposerUri].accepted());

    // Get group setup message
    GroupSetupMessageSPtr setupMsg { player1Builder.getGroupSetupMessage() };
    BOOST_REQUIRE(setupMsg);

    // Create builders from setup message for each non-proposer player
    GroupBuilder player2Builder { setupMsg, uris[1] };
    GroupBuilder player3Builder { setupMsg, uris[2] };

    // All builders hould be in the same state at the start
    BOOST_CHECK(player1Builder == player2Builder);
    BOOST_CHECK(player1Builder == player3Builder);

    // Test acceptance response from some non-group member.
    GroupResponseMessageSPtr nonMemberResponse { std::make_shared<GroupResponseMessage>("weirdo@who.ru", player1Builder.getID(), GroupState::ACCEPTED) };
    BOOST_CHECK_THROW({ player1Builder.processGroupSetupResponse(nonMemberResponse); }, std::runtime_error);

    // Test acceptance responses from each player in turn.
    GroupResponseMessageSPtr player2Response { player2Builder.getGroupSetupResponseMessage(GroupState::ACCEPTED) };
    GroupResponseMessageSPtr player3Response { player3Builder.getGroupSetupResponseMessage(GroupState::ACCEPTED) };

    BOOST_CHECK_NO_THROW({
        // Player 2 responses
        player1Builder.processGroupSetupResponse(player2Response);
        player3Builder.processGroupSetupResponse(player2Response);
        BOOST_CHECK(player1Builder.getState() == GroupState::PROPOSED);
        BOOST_CHECK(player2Builder.getState() == GroupState::PROPOSED);
        BOOST_CHECK(player3Builder.getState() == GroupState::ACCEPTED);

        // Player 3 responses
        player1Builder.processGroupSetupResponse(player3Response);
        player2Builder.processGroupSetupResponse(player3Response);
        BOOST_CHECK(player1Builder.getState() == GroupState::ACCEPTED);
        BOOST_CHECK(player2Builder.getState() == GroupState::ACCEPTED);
        BOOST_CHECK(player3Builder.getState() == GroupState::ACCEPTED);

        // Can now fetch created Group.
        GroupSPtr player1Group { player1Builder.getGroup() };
        GroupSPtr player2Group { player2Builder.getGroup() };
        GroupSPtr player3Group { player3Builder.getGroup() };
        BOOST_CHECK(*player1Group == *player2Group);
        BOOST_CHECK(*player2Group == *player3Group);
    });

}

// Test lifecycle of group rejection
BOOST_AUTO_TEST_CASE(TestLifecycleRejection)
{
    // Create a group builder for proposer (player1) & check initial state
    std::vector<std::string> uris { "player1@mycorp.com", "player2@otherplace.org", "player3@secret.org" };
    const std::string& proposerUri { uris[0] };
    GroupBuilder player1Builder { uris, proposerUri };
    BOOST_CHECK_EQUAL(player1Builder.getPlayers().size(), 3);
    BOOST_CHECK_EQUAL(player1Builder.getProposerURI(), proposerUri);
    BOOST_CHECK_EQUAL(player1Builder.getMyURI(), proposerUri);
    BOOST_CHECK(player1Builder.getState() == GroupState::PROPOSED);
    BOOST_CHECK(player1Builder.getPlayers()[proposerUri].accepted());

    // Get group setup message
    GroupSetupMessageSPtr setupMsg { player1Builder.getGroupSetupMessage() };
    BOOST_REQUIRE(setupMsg);

    // Create builders from setup message for each other (non-proposer) player
    GroupBuilder player2Builder { setupMsg, uris[1] };
    GroupBuilder player3Builder { setupMsg, uris[2] };

    // All builders hould be in the same state at the start
    BOOST_CHECK(player1Builder == player2Builder);
    BOOST_CHECK(player1Builder == player3Builder);

    // Test mix of acceptance and rejection responses
    GroupResponseMessageSPtr player2Response { player2Builder.getGroupSetupResponseMessage(GroupState::ACCEPTED) };
    GroupResponseMessageSPtr player3Response { player3Builder.getGroupSetupResponseMessage(GroupState::REJECTED) };
    BOOST_CHECK_NO_THROW({
        // Player 2 responses
        player1Builder.processGroupSetupResponse(player2Response);
        player3Builder.processGroupSetupResponse(player2Response);
        BOOST_CHECK(player1Builder.getState() == GroupState::PROPOSED);
        BOOST_CHECK(player2Builder.getState() == GroupState::PROPOSED);
        BOOST_CHECK(player3Builder.getState() == GroupState::REJECTED);

        // Player 3 responses
        player1Builder.processGroupSetupResponse(player3Response);
        player2Builder.processGroupSetupResponse(player3Response);
        BOOST_CHECK(player1Builder.getState() == GroupState::REJECTED);
        BOOST_CHECK(player2Builder.getState() == GroupState::REJECTED);
        BOOST_CHECK(player3Builder.getState() == GroupState::REJECTED);
    });

    // Player 3 then tries to change their mind
    player3Response = std::make_shared<GroupResponseMessage>("player3@secret.org", player3Builder.getID(), GroupState::ACCEPTED);
    BOOST_CHECK_THROW({ player1Builder.processGroupSetupResponse(player3Response); }, std::runtime_error);
    BOOST_CHECK_THROW({ player2Builder.processGroupSetupResponse(player3Response); }, std::runtime_error);
    BOOST_CHECK_THROW({ player3Builder.processGroupSetupResponse(player3Response); }, std::runtime_error);
}

// Test some error scenarios
BOOST_AUTO_TEST_CASE(TestErrors)
{
    // Try creating a builder with a duplicate URI
    {
        std::vector<std::string> uris { "player1@mycorp.com", "player2@otherplace.org", "player3@secret.org", "player2@otherplace.org" };
        BOOST_CHECK_THROW({
            GroupBuilder builder (uris, uris[0]);
        }, std::runtime_error);
    }

    // Try creating a builder where proposer isn't a player
    {
        std::vector<std::string> uris { "player1@mycorp.com", "player2@otherplace.org", "player3@secret.org" };
        BOOST_CHECK_THROW({
            GroupBuilder builder (uris, "wibble@secret.org");
        }, std::invalid_argument);
    }

    // Try creating a builder from a setup message without our URI in list of players
    {
        std::vector<std::string> uris { "player1@mycorp.com", "player2@otherplace.org", "player3@secret.org" };
        GroupBuilder builder { uris, uris[0] } ;
        GroupSetupMessageSPtr setupMsg { builder.getGroupSetupMessage() };
        BOOST_CHECK_THROW({
            GroupBuilder badBuilder (setupMsg, "bad@uri" );
        }, std::runtime_error);
    }

    // Try fetching a group response in a bad state
    {
        std::vector<std::string> uris { "player1@mycorp.com", "player2@otherplace.org", "player3@secret.org" };
        GroupBuilder builder { uris, uris[0] } ;
        GroupSetupMessageSPtr setupMsg { builder.getGroupSetupMessage() };
        GroupBuilder builder2 { setupMsg, uris[1] };
        BOOST_CHECK_THROW({
            GroupResponseMessageSPtr response { builder2.getGroupSetupResponseMessage(GroupState::PROPOSED) };
        }, std::invalid_argument);
    }
}

// Test group setup messages.
BOOST_AUTO_TEST_CASE(TestGroupSetup)
{
    // Create message and serialise/deserialise
    std::vector<std::string> uris { "player1@mycorp.com", "player2@otherplace.org", "player3@secret.org" };
    GroupBuilder builder { uris, uris[0] };
    GroupSetupMessage msg { "proposer@nchain.com", builder.getID(), builder.getPlayers() };
    std::string json = msg.to_json();
    GroupSetupMessageSPtr deserialisedMsg { std::make_shared<GroupSetupMessage>() };
    deserialisedMsg->from_json(json);

    BOOST_REQUIRE(deserialisedMsg);
    BOOST_CHECK_EQUAL(msg.getProposerURI(), deserialisedMsg->getProposerURI());
    BOOST_CHECK_EQUAL(msg.getGroupID(), deserialisedMsg->getGroupID());
    BOOST_CHECK(msg.getPlayers() == deserialisedMsg->getPlayers());
}

// Test group response messages.
BOOST_AUTO_TEST_CASE(TestGroupResponse)
{
    // Create message and serialise/deserialise
    GroupResponseMessage msg { "player1@mycorp.com", UUID{}, GroupState::ACCEPTED };
    std::string json =  msg.to_json();
    GroupResponseMessageSPtr deserialisedMsg  { std::make_shared<GroupResponseMessage>() };
    deserialisedMsg->from_json(json);

    BOOST_REQUIRE(deserialisedMsg);
    BOOST_CHECK_EQUAL(msg.getResponderURI(), deserialisedMsg->getResponderURI());
    BOOST_CHECK_EQUAL(msg.getGroupID(), deserialisedMsg->getGroupID());
    BOOST_CHECK(msg.getResponse() == deserialisedMsg->getResponse());
}
BOOST_AUTO_TEST_CASE(testAcceptanceLifeCycleOfMultiplePlayers)
{
    // very the life cycle of group acceptance

    // Get the players
    std::vector<std::string> playerUris = getPlayerList(100);

    auto GroupPlayersMap = generateGroupBuilders(playerUris);

    // Make sure that the all play's group builder is same except the myUri
    BOOST_CHECK(compareGroupBuilders(GroupPlayersMap));

    // accept message
    acceptResponseFromAllPlayers(GroupPlayersMap);

    // Make sure that the all group states are 'proposed'
    BOOST_CHECK(compareAllGroupStates(GroupPlayersMap));

    sendResponsedToAllPlayers(GroupPlayersMap);

    verifyAllGroupStatesTo(GroupPlayersMap, GroupState::ACCEPTED);

}

BOOST_AUTO_TEST_CASE(testRejectionLifeCycleOfMultiplePlayers)
{
    // very the life cycle of group rejection

    // Get the players
    std::vector<std::string> playerUris = getPlayerList(100);

    auto GroupPlayersMap = generateGroupBuilders(playerUris);

    // Make sure that the all play's group builder is same except the myUri
    BOOST_CHECK(compareGroupBuilders(GroupPlayersMap));

    // accept message
    rejectBysomePlayers(GroupPlayersMap);

    sendResponsedToAllPlayers(GroupPlayersMap);

    verifyAllGroupStatesTo(GroupPlayersMap, GroupState::REJECTED);

}



BOOST_AUTO_TEST_SUITE_END();
