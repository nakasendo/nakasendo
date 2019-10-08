import PyGroupBuilder
import PyGroupSetupMessage

from random import *

noOfPlayers = randint(3, 1000)

def test_GroupBuilder():

    groupBuildersList = []
    for ordinal in range (noOfPlayers):

        #Create Build for each player in the group
        playerBuild = PyGroupBuilder.GroupBuilder(uri="player" + str(ordinal) + "@nchain.com", ordinal= ordinal)
        groupBuildersList.append(playerBuild)

    # Verifying each player's group ID and ordinal number
    assert groupBuildersList[0] is not None, "Test failed"
    assert len(groupBuildersList[0].mGroupID) == 64, "Test failed"
    assert groupBuildersList[0].ordinal != groupBuildersList[-1].ordinal, "Test failed"
    assert groupBuildersList[0].mMyUri != groupBuildersList[-1].mMyUri, "Test failed"
    assert groupBuildersList[0].mGroupID != groupBuildersList[1].mGroupID , "Test failed"
    assert len(groupBuildersList) == noOfPlayers, "Test failed"

def test_GetInitialSetupMessage():

    groupBuildersList = []
    for ordinal in range (noOfPlayers):

        if (ordinal == 0):

            #Create Build for the coordinator of the group
            coordinateBuilder = PyGroupBuilder.GroupBuilder(uri="player" + str(ordinal) + "@nchain.com", ordinal= ordinal)
            groupBuildersList.append(coordinateBuilder)

            #Verifying the build creation for the proposer is not empty & the length of Group ID is 64
            assert coordinateBuilder is not None, "Test failed"
            assert len(coordinateBuilder.mGroupID) == 64, "Test failed"

            #The coordinator creates setup message and send it to other players
            setupMsg = coordinateBuilder.getInitialSetupMessage()
            assert setupMsg is not None, "Test failed"
            #Serialize & deserialize the setup message
            setupMsgFromJson = PyGroupBuilder.GroupSetupMessage()
            setupMsgFromJson.from_json(setupMsg.to_json())
            assert setupMsgFromJson is not None, "Test failed"

        else:

            # Create Build for each player in the group
            playerBuild = PyGroupBuilder.GroupBuilder(uri="player" + str(ordinal) + "@nchain.com", ordinal= ordinal)
            groupBuildersList.append(playerBuild)
            # Verifying each player's group ID and ordinal number
            assert playerBuild is not None, "Test failed"
            assert len(playerBuild.mGroupID) == 64, "Test failed"
            assert coordinateBuilder.mGroupID != playerBuild.mGroupID, "Test failed"
            assert coordinateBuilder.playerInfo != playerBuild.playerInfo, "Test failed"

def test_ProcessInitialSetupMessage():

    playerBuildersList = []
    for ordinal in range (noOfPlayers):

        if (ordinal == 0):

            # Create Build for the coordinator of the group
            coordinateBuilder = PyGroupBuilder.GroupBuilder(uri="player" + str(ordinal) + "@nchain.com", ordinal= ordinal)
            # Verifying the build creation for the proposer is not empty & the length of Group ID is 64
            assert coordinateBuilder is not None, "Test failed"
            assert len(coordinateBuilder.mGroupID) == 64, "Test failed"

            #The coordinator initialized the creation of setup message
            setupMsg = coordinateBuilder.getInitialSetupMessage()
            assert setupMsg is not None, "Test failed"

        else:

            # Create Build for each player in the group
            playerBuild = PyGroupBuilder.GroupBuilder(uri="player" + str(ordinal) + "@nchain.com", ordinal= ordinal)
            playerBuildersList.append(playerBuild)
            # Verifying each player's group ID and ordinal number are NOT same
            assert playerBuild is not None, "Test failed"
            assert len(playerBuild.mGroupID) == 64, "Test failed"
            assert coordinateBuilder.mGroupID != playerBuild.mGroupID, "Test failed"
            assert coordinateBuilder.playerInfo != playerBuild.playerInfo, "Test failed"

    # Serialize & deserialize the setup message
    setupMsgFromJson = PyGroupBuilder.GroupSetupMessage()
    setupMsgFromJson.from_json(setupMsg.to_json())
    assert setupMsgFromJson is not None, "Test failed"

    procesSetupMsgBuild = []
    for x in range (len(playerBuildersList)):

        #Each player processes the setup message created and sent by the coordinator
        playerBuildersList[x].processInitialSetupMessage(setupMsgFromJson)
        procesSetupMsgBuild.append(playerBuildersList[x])

        # Verifying each player's group ID and Coordinator's Group ID are SAME
        assert coordinateBuilder.mGroupID == playerBuildersList[x].mGroupID, "Test failed"
    # Ensure each player in the group processes their setup message created by the coordinator
    assert len(procesSetupMsgBuild) == len(playerBuildersList)

def test_GetInitialSetupResponseMessage():

    playerBuildersList = []
    for ordinal in range (noOfPlayers):

        if (ordinal == 0):

            # Create Build for the coordinator of the group
            coordinateBuilder = PyGroupBuilder.GroupBuilder(uri="player" + str(ordinal) + "@nchain.com", ordinal= ordinal)
            # Verifying the build creation for the proposer is not empty & the length of Group ID is 64
            assert coordinateBuilder is not None, "Test failed"
            assert len(coordinateBuilder.mGroupID) == 64, "Test failed"

            #The coordinator initialized the creation of setup message
            setupMsg = coordinateBuilder.getInitialSetupMessage()
            assert setupMsg is not None, "Test failed"

        else:

            # Create Build for each player in the group with their URI & Ordinal number
            playerBuild = PyGroupBuilder.GroupBuilder(uri="player" + str(ordinal) + "@nchain.com", ordinal= ordinal)
            playerBuildersList.append(playerBuild)
            # Verifying each player's group ID and ordinal number are NOT same
            assert playerBuild is not None, "Test failed"
            assert len(playerBuild.mGroupID) == 64, "Test failed"
            assert coordinateBuilder.mGroupID != playerBuild.mGroupID, "Test failed"
            assert coordinateBuilder.playerInfo != playerBuild.playerInfo, "Test failed"

    # Serialize & deserialize the setup message
    setupMsgFromJson = PyGroupSetupMessage.GroupSetupMessage()
    setupMsgFromJson.from_json(setupMsg.to_json())
    assert setupMsgFromJson is not None, "Test failed"

    playerSetupRes = []
    for x in range (len(playerBuildersList)):

        #Each player processes the setup message created and sent by the coordinator
        playerBuildersList[x].processInitialSetupMessage(setupMsgFromJson)
        #Each player creates Response Message to the setup message created by the coordinator
        playerBuildersList[x].getInitialSetupResponseMessage()
        playerSetupRes.append(playerBuildersList[x])
        # Verifying each player's group ID and Coordinator's Group ID are SAME
        assert playerBuildersList[x] is not None, "Test failed"
        assert coordinateBuilder.mGroupID == playerBuildersList[x].mGroupID, "Test failed"

    # Ensure each player in the group processes their setup message created by the coordinator
    assert len(playerSetupRes) == len(playerBuildersList)

def test_ProcessInitialSetupResponseMessage():

    playerBuildersList = []
    for ordinal in range (noOfPlayers):

        if (ordinal == 0):

            # Create Build for the coordinator of the group
            coordinateBuilder = PyGroupBuilder.GroupBuilder(uri="player" + str(ordinal) + "@nchain.com", ordinal= ordinal)
            # Verifying the build creation for the proposer is not empty & the length of Group ID is 64
            assert coordinateBuilder is not None, "Test failed"
            assert len(coordinateBuilder.mGroupID) == 64, "Test failed"

            #The coordinator initializes the creation of setup message
            setupMsg = coordinateBuilder.getInitialSetupMessage()
            assert setupMsg is not None, "Test failed"

        else:

            # Create Build for each player in the group with their URI & Ordinal number
            playerBuild = PyGroupBuilder.GroupBuilder(uri="player" + str(ordinal) + "@nchain.com", ordinal= ordinal)
            playerBuildersList.append(playerBuild)
            # Verifying each player's group ID and ordinal number are NOT same
            assert playerBuild is not None, "Test failed"
            assert len(playerBuild.mGroupID) == 64, "Test failed"
            assert coordinateBuilder.mGroupID != playerBuild.mGroupID, "Test failed"
            assert coordinateBuilder.playerInfo != playerBuild.playerInfo, "Test failed"

    # Serialize & deserialize the setup message
    setupMsgFromJson = PyGroupSetupMessage.GroupSetupMessage()
    setupMsgFromJson.from_json(setupMsg.to_json())
    assert setupMsgFromJson is not None, "Test failed"

    playerSetupRes = []
    for x in range (len(playerBuildersList)):

        # Each player processes the setup message created & sent by the coordinator
        playerBuildersList[x].processInitialSetupMessage(setupMsgFromJson)
        # Each player creates Response Message to the setup message created by the coordinator
        playerSetupRes = playerBuildersList[x].getInitialSetupResponseMessage()
        # Verifying each player's group ID and Coordinator's Group ID are SAME
        assert playerSetupRes is not None, "Test failed"
        assert coordinateBuilder.mGroupID == playerBuildersList[x].mGroupID, "Test failed"

        # Serialize & deserialize the Response Setup Message from each player
        playerSetupResFromJson = PyGroupSetupMessage.GroupSetupResponseMessage()
        playerSetupResFromJson.from_json(playerSetupRes.to_json())

        # The Coordinator processes each player's Response Setup Messages
        coordinateBuilder.processInitialSetupResponseMessage(playerSetupResFromJson)
        assert coordinateBuilder is not None, "Test failed"

def test_Get_ProcessBroadcastSetupMessage():

    playerBuildersList = []
    for ordinal in range (noOfPlayers):

        if (ordinal == 0):

            # Create Build for the coordinator of the group
            coordinateBuilder = PyGroupBuilder.GroupBuilder(uri="player" + str(ordinal) + "@nchain.com", ordinal= ordinal)
            # Verifying the build creation for the proposer is not empty & the length of Group ID is 64
            assert coordinateBuilder is not None, "Test failed"
            assert len(coordinateBuilder.mGroupID) == 64, "Test failed"

            # The coordinator creates setup message and send it to other players
            setupMsg = coordinateBuilder.getInitialSetupMessage()
            assert setupMsg is not None, "Test failed"

        else:

            # Create Build for each player in the group with their URI & Ordinal number
            playerBuild = PyGroupBuilder.GroupBuilder(uri="player" + str(ordinal) + "@nchain.com", ordinal= ordinal)
            playerBuildersList.append(playerBuild)
            # Verifying each player's group ID and ordinal number are NOT same
            assert playerBuild is not None, "Test failed"
            assert len(playerBuild.mGroupID) == 64, "Test failed"
            assert coordinateBuilder.mGroupID != playerBuild.mGroupID, "Test failed"
            assert coordinateBuilder.playerInfo != playerBuild.playerInfo, "Test failed"

    # Serialize & deserialize the setup message
    setupMsgFromJson = PyGroupSetupMessage.GroupSetupMessage()
    setupMsgFromJson.from_json(setupMsg.to_json())
    assert setupMsgFromJson is not None, "Test failed"

    for x in range (len(playerBuildersList)):

        # Each player processes the setup message created & sent by the coordinator
        playerBuildersList[x].processInitialSetupMessage(setupMsgFromJson)
        # Each player creates Response Message to the setup message created by the coordinator
        playerSetupRes = playerBuildersList[x].getInitialSetupResponseMessage()
        # Verifying each player's group ID and Coordinator's Group ID are SAME
        assert playerSetupRes is not None, "Test failed"
        assert coordinateBuilder.mGroupID == playerBuildersList[x].mGroupID, "Test failed"

        # Serialize & deserialize the Response Setup Message from each player
        playerSetupResFromJson = PyGroupSetupMessage.GroupSetupResponseMessage()
        playerSetupResFromJson.from_json(playerSetupRes.to_json())

        # The Coordinator processes each player's Response Setup Messages
        coordinateBuilder.processInitialSetupResponseMessage(playerSetupResFromJson)

    #The coordinator setup & broadcast message to the group member
    bcMsg = coordinateBuilder.getBroadcastSetupMessage()

    # Serialize & deserialize the Broadcast message
    bcMsgFromJson = PyGroupSetupMessage.GroupBroadcastMessage()
    bcMsgFromJson.from_json(bcMsg.to_json())
    assert bcMsgFromJson is not None, "Test failed"
    assert coordinateBuilder.mGroupID == playerBuildersList[x].mGroupID, "Test failed"

    for i in range (len(playerBuildersList)):

        # Each player processes the broadcast setup message created & sent by the coordinator
        playerBuildersList[i].processBroadMessage(bcMsgFromJson)
        assert playerBuildersList[i] is not None, "Test failed"