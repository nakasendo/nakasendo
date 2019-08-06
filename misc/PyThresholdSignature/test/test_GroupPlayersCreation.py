import PyPlayer
import PyGroup
import PyGroupBuilder
import PyGroupSetupMessage
import PyGroupState

from random import *

randomPlayerRange = 100

def test_PlayerCreation():

    for x in range(randomPlayerRange):

        playerURI = "player"+str(randint(1, 100))+"@nchain.co.uk"
        playerDetails = PyPlayer.Player(playerURI)
        assert playerDetails.URI == playerURI, "Test failed"

def test_GroupCreation():

    noOfPlayer = 100
    playersMap = PyGroup.PlayerMap()

    for i in range (1, noOfPlayer + 1):

        playerURL = "player" + str(i) + "@nchain.com"
        player = PyPlayer.Player(playerURL, i, False, False)
        playerDetails = PyGroup.PlayerItem(playerURL, player)
        playersMap.add(playerDetails)

    groupPlayer = PyGroup.Group(mGroupID=None, playerMap = playersMap)
    assert len(groupPlayer.mGroupID) == 64, "Test failed"
    assert groupPlayer.size() == noOfPlayer, "Test failed"

def test_CreateGroupWitDuplicateUris():# Verify Group can NOT be created with duplicate player's URIs

    duplicateListOfURIs = []
    for x in range(randomPlayerRange):

        playerURI = "player" + str(randint(1, 100)) + "@nchain.co.uk"
        duplicateListOfURIs.append(playerURI)

    try:

        groupPlayer = PyGroupBuilder.GroupBuilder(playerUris=duplicateListOfURIs, myUri=duplicateListOfURIs[0])
        assert groupPlayer is None, "Test failed"

    except ValueError as e:

        errorMgs = e.args
        assert "Failed to create and insert player details" in str(errorMgs), "Test failed"

def test_createGroupWitPlayer():#Verify Group can NOT be created with a player

    playerURI = ["player1@iemail.com"]

    try:

        groupPlayer = PyGroupBuilder.GroupBuilder(playerUris=playerURI, myUri=playerURI[0])
        assert groupPlayer is None, "Test failed"

    except ValueError as e:

        errorMgs = e.args
        assert "A group should have more than one player" in str(errorMgs), "Test failed"

def test_createGroupWithPlayers():#Verify Group can be created with 2 or more players

    listOfPlayerURIs = []
    for x in range(randomPlayerRange):
        playerURI = "player" + str(randint(1, 100)) + "@nchain.co.uk"
        listOfPlayerURIs.append(playerURI)

    #convert list to set of URIs
    setOfPlayerURIs = set(listOfPlayerURIs)
    groupPlayer = PyGroupBuilder.GroupBuilder(playerUris=setOfPlayerURIs, myUri=listOfPlayerURIs[0])
    assert groupPlayer is not None, "Test failed"
    assert len(setOfPlayerURIs) == groupPlayer.getPlayers().getCount(), "Test failed"

def test_playerMultiGroup():#Verify player can be in multiple Group

    listOfPlayerURIs = []
    for x in range(randomPlayerRange):
        playerURI = "player" + str(randint(1, 100)) + "@nchain.co.uk"
        listOfPlayerURIs.append(playerURI)

    #convert list to set of URIs
    setOfPlayerURIs = set(listOfPlayerURIs)
    firstGroupOfPlayer = PyGroupBuilder.GroupBuilder(playerUris=setOfPlayerURIs, myUri=listOfPlayerURIs[0])
    secondGroupOfPlayer = PyGroupBuilder.GroupBuilder(playerUris=setOfPlayerURIs, myUri=listOfPlayerURIs[1])
    assert firstGroupOfPlayer is not None, "Test failed"
    assert secondGroupOfPlayer is not None, "Test failed"
    assert len(setOfPlayerURIs) == firstGroupOfPlayer.getPlayers().getCount(), "Test failed"
    assert len(setOfPlayerURIs) == secondGroupOfPlayer.getPlayers().getCount(), "Test failed"
    assert firstGroupOfPlayer.getID() != secondGroupOfPlayer.getID(), "Test failed"
    assert firstGroupOfPlayer.getProposerURI() != secondGroupOfPlayer.getProposerURI(), "Test failed"
    assert firstGroupOfPlayer.getState() == secondGroupOfPlayer.getState(), "Test failed"

def test_uniquePlayerUriInGroup():#Verify player's ordinal is unique within the Group

    listOfPlayerURIs = []
    for x in range(randomPlayerRange):
        playerURI = "player" + str(randint(1, 100)) + "@nchain.co.uk"
        listOfPlayerURIs.append(playerURI)

    #convert list to set of URIs
    setOfPlayerURIs = set(listOfPlayerURIs)
    groupPlayer = PyGroupBuilder.GroupBuilder(playerUris=setOfPlayerURIs, myUri=listOfPlayerURIs[0])
    assert groupPlayer is not None, "Test failed"

    #Empty set of ordinal number created
    playerOrdinal = set()

    for playerItem in groupPlayer.getPlayers().getItems():
        playerOrdinal.add(playerItem.getPlayer().getOrdinal())

    assert len(setOfPlayerURIs) == len(playerOrdinal), "Test failed"

def test_createGroupWitNonMemberProposer():#Verify Proposer must be a member of the proposed group

    notMemberProposerURI = "player1@sdk.com"
    listOfPlayerURIs = []
    for x in range(randomPlayerRange):
        playerURI = "player" + str(randint(1, 100)) + "@nchain.co.uk"
        listOfPlayerURIs.append(playerURI)

    #convert list to set of URIs
    setOfPlayerURIs = set(listOfPlayerURIs)
    try:

        groupPlayer = PyGroupBuilder.GroupBuilder(playerUris=setOfPlayerURIs, myUri=notMemberProposerURI)
        assert groupPlayer is not None, "Test failed"
        assert len(setOfPlayerURIs) == groupPlayer.getPlayers().getCount(), "Test failed"

    except ValueError as e:

        errorMgs = e.args
        assert "The group proposer must be a member of the proposed group" in str(errorMgs), "Test failed"

def test_memberAcceptance(): #Verify members of the proposed group can accept proposal

    #Create a group builder & check initial state
    listOfPlayerURIs = ["player1@sdk.com", "player2@sdk.com", "player3@sdk.com"]
    proposerUri = listOfPlayerURIs[0]

    try:
        player1Builder = PyGroupBuilder.GroupBuilder(playerUris=listOfPlayerURIs, myUri=proposerUri)
        assert player1Builder is not None
        assert player1Builder.getPlayers().getCount() == 3
        assert proposerUri == player1Builder.getProposerURI()
        assert proposerUri == player1Builder.getMyURI()
        assert PyGroupState.GroupState.Type.PROPOSED == player1Builder.getState()
        assert player1Builder.getPlayers().findPlayer(proposerUri).accepted() is True

        #Get group setup message
        setMsg = player1Builder.getGroupSetupMessage()
        assert setMsg is not None

        #Create builders from setup message for each player
        player2Builder = PyGroupBuilder.GroupBuilder(setupMsg=setMsg, myUri=listOfPlayerURIs[1])
        player3Builder = PyGroupBuilder.GroupBuilder(setupMsg=setMsg, myUri=listOfPlayerURIs[2])
        assert player2Builder is not None
        assert player3Builder is not None

        #Test acceptance responses from each player in turn
        player2Response = player2Builder.getGroupSetupResponseMessage(PyGroupState.GroupState.Type.ACCEPTED)
        player3Response = player3Builder.getGroupSetupResponseMessage(PyGroupState.GroupState.Type.ACCEPTED)

        #Player2 responses
        player1Builder.processGroupSetupResponse(player2Response)
        player3Builder.processGroupSetupResponse(player2Response)
        assert PyGroupState.GroupState.Type.ACCEPTED == player1Builder.getState()
        assert PyGroupState.GroupState.Type.PROPOSED == player2Builder.getState()
        assert PyGroupState.GroupState.Type.ACCEPTED == player3Builder.getState()

        #Player3 responses
        player1Builder.processGroupSetupResponse(player3Response)
        player2Builder.processGroupSetupResponse(player3Response)
        assert PyGroupState.GroupState.Type.ACCEPTED == player1Builder.getState()
        assert PyGroupState.GroupState.Type.ACCEPTED == player2Builder.getState()
        assert PyGroupState.GroupState.Type.ACCEPTED == player3Builder.getState()

        #fetch created Group
        player1Group = player1Builder.getGroup()
        player2Group = player2Builder.getGroup()
        player3Group = player3Builder.getGroup()
        assert player1Group is not None
        assert player2Group is not None
        assert player3Group is not None

    except ValueError as e:

        errorMgs = e.args
        print(errorMgs)

def test_nonMemberAcceptance(): #Verify non members of the proposed group can NOT accept proposal

    # Create a group builder & check initial state
    listOfPlayerURIs = ["player1@sdk.com", "player2@sdk.com", "player3@sdk.com"]
    proposerUri = listOfPlayerURIs[0]

    try:
        player1Builder = PyGroupBuilder.GroupBuilder(playerUris=listOfPlayerURIs, myUri=proposerUri)
        assert player1Builder is not None
        assert player1Builder.getPlayers().getCount() == 3
        assert proposerUri == player1Builder.getProposerURI()
        assert proposerUri == player1Builder.getMyURI()
        assert PyGroupState.GroupState.Type.PROPOSED == player1Builder.getState()
        assert player1Builder.getPlayers().findPlayer(proposerUri).accepted() is True

        # Get group setup message
        setMsg = player1Builder.getGroupSetupMessage()
        assert setMsg is not None

        # Create builders from setup message for each player
        player2Builder = PyGroupBuilder.GroupBuilder(setupMsg=setMsg, myUri=listOfPlayerURIs[1])
        player3Builder = PyGroupBuilder.GroupBuilder(setupMsg=setMsg, myUri=listOfPlayerURIs[2])
        assert player2Builder is not None
        assert player3Builder is not None

        #Test acceptance response from some non-group member
        nonMemberResponse = PyGroupBuilder.GroupResponseMessage(uri="player4@sdk.com", groupID=player1Builder.getID(),GrpState= PyGroupState.GroupState.Type.ACCEPTED)
        player1Builder.processGroupSetupResponse(response= nonMemberResponse)

    except RuntimeError as e:

        errorMgs = e.args
        assert "Group response message is not from a proposed group member" in str(errorMgs), "Test failed"

def test_memberReject(): #Verify members of the proposed group can reject proposal

    #Create a group builder & check initial state
    listOfPlayerURIs = ["player1@sdk.com", "player2@sdk.com", "player3@sdk.com"]
    proposerUri = listOfPlayerURIs[0]

    try:
        player1Builder = PyGroupBuilder.GroupBuilder(playerUris=listOfPlayerURIs, myUri=proposerUri)
        assert player1Builder is not None, "Test failed"
        assert player1Builder.getPlayers().getCount() == 3, "Test failed"
        assert proposerUri == player1Builder.getProposerURI(), "Test failed"
        assert proposerUri == player1Builder.getMyURI(), "Test failed"
        assert PyGroupState.GroupState.Type.PROPOSED == player1Builder.getState(), "Test failed"
        assert player1Builder.getPlayers().findPlayer(proposerUri).accepted() is True, "Test failed"

        #Get group setup message
        setMsg = player1Builder.getGroupSetupMessage()
        assert setMsg is not None, "Test failed"

        #Create builders from setup message for each player
        player2Builder = PyGroupBuilder.GroupBuilder(setupMsg=setMsg, myUri=listOfPlayerURIs[1])
        player3Builder = PyGroupBuilder.GroupBuilder(setupMsg=setMsg, myUri=listOfPlayerURIs[2])
        assert player2Builder is not None, "Test failed"
        assert player3Builder is not None, "Test failed"

        #Test acceptance responses from each player in turn
        player2Response = player2Builder.getGroupSetupResponseMessage(PyGroupState.GroupState.Type.ACCEPTED)
        player3Response = player3Builder.getGroupSetupResponseMessage(PyGroupState.GroupState.Type.REJECTED)

        #Player2 responses
        player1Builder.processGroupSetupResponse(player2Response)
        player3Builder.processGroupSetupResponse(player2Response)
        assert PyGroupState.GroupState.Type.PROPOSED == player1Builder.getState(), "Test failed"
        assert PyGroupState.GroupState.Type.PROPOSED == player2Builder.getState(), "Test failed"
        assert PyGroupState.GroupState.Type.REJECTED == player3Builder.getState(), "Test failed"

        #Player3 responses
        player1Builder.processGroupSetupResponse(player3Response)
        player2Builder.processGroupSetupResponse(player3Response)
        assert PyGroupState.GroupState.Type.REJECTED == player1Builder.getState(), "Test failed"
        assert PyGroupState.GroupState.Type.REJECTED == player2Builder.getState(), "Test failed"
        assert PyGroupState.GroupState.Type.REJECTED == player3Builder.getState(), "Test failed"

        #fetch created Group
        player1Group = player1Builder.getGroup()
        player2Group = player2Builder.getGroup()
        player3Group = player3Builder.getGroup()
        assert player1Group is not None, "Test failed"
        assert player2Group is not None, "Test failed"
        assert player3Group is not None, "Test failed"

    except ValueError as e:

        errorMgs = e.args
        assert "create group in state Rejected" in str(errorMgs), "Test failed"

    except RuntimeError as e:

        errorMgs = e.args
        assert "Can't create group in state Rejected" in str(errorMgs), "Test failed"