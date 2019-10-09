import utils
import PyGroupState
import PyPolynomials
import PyGroupBuilder
import PyGroupSetupMessage
import PyGroupSecretSetupMessage
import PyGroupSecretIntermediary
import PyGroupSecretResponseMessage
import PyGroupSecretPlayerBuilder_Updated
from random import *


def test_ThresholdPlusThresholdPlusOne_LessThan_TotalPlayer():
    for x in range(20):
        totalPlayers = randint(101, 150)
        threshold = randint(3, 50)
        # Verify Recombination Threshold calculation applies, which is 2*t + 1 < = total number of player
        assert PyGroupSecretPlayerBuilder_Updated.GroupSecretPlayerBuilder.verifyThresholdValue(totalPlayers,
                                                                                                threshold) is True, "Test failed"


def test_ThresholdPlusThresholdPlusOne_Equal_TotalPlayer():
    for x in range(20):
        threshold = randint(3, 50)
        totalPlayers = threshold + threshold + 1
        # Verify Recombination Threshold calculation applies, which is 2*t + 1 < = total number of player
        assert PyGroupSecretPlayerBuilder_Updated.GroupSecretPlayerBuilder.verifyThresholdValue(totalPlayers,
                                                                                                threshold) is True, "Test failed"


def test_ThresholdPlusThresholdPlusOne_GreaterThan_TotalPlayer():
    for x in range(20):
        totalPlayers = randint(3, 50)
        threshold = randint(101, 150)
        # Verify Recombination Threshold calculation applies, which is 2*t + 1 < = total number of player
        assert PyGroupSecretPlayerBuilder_Updated.GroupSecretPlayerBuilder.verifyThresholdValue(totalPlayers,
                                                                                                threshold) is False, "Test failed"


def test_GroupSecretPlayerBuilder():
    for x in range(10):

        totalPlayers = randint(101, 150)
        threshold = randint(3, 50)
        # Verify Recombination Threshold value calculation applies, which is 2*t + 1 < = total number of player
        assert PyGroupSecretPlayerBuilder_Updated.GroupSecretPlayerBuilder.verifyThresholdValue(totalPlayers,
                                                                                                threshold) is True, "Test failed"

        # Get the list of players in the group
        groupBuildersList = []
        for ordinal, player_url in utils.getPlayers(totalPlayers):
            # Create Build for each player in the group and add it to the group list
            groupBuildersList.append(PyGroupBuilder.GroupBuilder(uri=player_url, ordinal=ordinal))

        # Verifying each player's group ID and ordinal number
        assert groupBuildersList[0] is not None, "Test failed"
        assert len(groupBuildersList[0].mGroupID) == 64, "Test failed"
        assert groupBuildersList[0].ordinal != groupBuildersList[-1].ordinal, "Test failed"
        assert groupBuildersList[0].mMyUri != groupBuildersList[-1].mMyUri, "Test failed"
        assert groupBuildersList[0].mGroupID != groupBuildersList[1].mGroupID, "Test failed"
        assert len(groupBuildersList) == totalPlayers, "Test failed"

        # The coordinator creates setup message with the number of threshold value and send it to other players
        setupMsg = groupBuildersList[0].getInitialSetupMessage(threshold_value=threshold)
        assert setupMsg is not None, "Test failed"
        # Serialize & deserialize the setup message
        setupMsgFromJson = PyGroupBuilder.GroupSetupMessage()
        setupMsgFromJson.from_json(setupMsg.to_json())
        assert setupMsgFromJson is not None, "Test failed"

        for x in range(1, len(groupBuildersList)):
            # Each player processes the setup message created & sent by the coordinatorf
            groupBuildersList[x].processInitialSetupMessage(setupMsgFromJson)
            # Verifying each player's group ID and Coordinator's Group ID are SAME
            assert groupBuildersList[0].mGroupID == groupBuildersList[x].mGroupID, "Test failed"

        for i in range(1, len(groupBuildersList)):
            # Each player creates Response Message to the setup message created by the coordinator
            playerSetupRes = groupBuildersList[i].getInitialSetupResponseMessage()
            assert playerSetupRes is not None, "Test failed"
            # Serialize & deserialize the Response Setup Message from each player
            SetupResFromJson = PyGroupSetupMessage.GroupSetupResponseMessage()
            SetupResFromJson.from_json(playerSetupRes.to_json())

            # The Coordinator processes each player's Response Setup Messages
            groupBuildersList[0].processInitialSetupResponseMessage(SetupResFromJson)

        # The coordinator setup & broadcast message to the group member
        bcMsg = groupBuildersList[0].getBroadcastSetupMessage()

        # Serialize & deserialize the Broadcast message
        bcMsgFromJson = PyGroupSetupMessage.GroupBroadcastMessage()
        bcMsgFromJson.from_json(bcMsg.to_json())

        for j in range(1, len(groupBuildersList)):
            # Each player processes the broadcast setup message created & sent by the coordinator
            groupBuildersList[j].processBroadMessage(bcMsgFromJson)
            assert groupBuildersList[i] is not None, "Test failed"


def test_GetGroupSecretSetupMessage():
    for x in range(2):

        totalPlayers = randint(11, 15)
        threshold = randint(1, 5)
        # Verify Recombination Threshold value calculation applies, which is 2*t + 1 < = total number of player
        assert PyGroupSecretPlayerBuilder_Updated.GroupSecretPlayerBuilder.verifyThresholdValue(totalPlayers,
                                                                                                threshold) is True, "Test failed"

        # Get the list of players in the group
        groupBuildersList = []
        for ordinal, player_url in utils.getPlayers(totalPlayers):
            # Create Build for each player in the group and add it to the group list
            groupBuildersList.append(PyGroupBuilder.GroupBuilder(uri=player_url, ordinal=ordinal))

        # Verifying each player's group ID and ordinal number
        assert groupBuildersList[0].mGroupID != groupBuildersList[1].mGroupID, "Test failed"
        assert len(groupBuildersList) == totalPlayers, "Test failed"

        # The coordinator creates setup message with the number of threshold value and send it to other players
        setupMsg = groupBuildersList[0].getInitialSetupMessage(threshold_value=threshold)
        assert setupMsg is not None, "Test failed"
        # Serialize & deserialize the setup message
        setupMsgFromJson = PyGroupBuilder.GroupSetupMessage()
        setupMsgFromJson.from_json(setupMsg.to_json())
        assert setupMsgFromJson is not None, "Test failed"

        for x in range(1, len(groupBuildersList)):
            # Each player processes the setup message created & sent by the coordinatorf
            groupBuildersList[x].processInitialSetupMessage(setupMsgFromJson)
            # Verifying each player's group ID and Coordinator's Group ID are SAME
            assert groupBuildersList[0].mGroupID == groupBuildersList[x].mGroupID, "Test failed"

        for i in range(1, len(groupBuildersList)):
            # Each player creates Response Message to the setup message created by the coordinator
            playerSetupRes = groupBuildersList[i].getInitialSetupResponseMessage()
            assert playerSetupRes is not None, "Test failed"
            # Serialize & deserialize the Response Setup Message from each player
            SetupResFromJson = PyGroupSetupMessage.GroupSetupResponseMessage()
            SetupResFromJson.from_json(playerSetupRes.to_json())

            # The Coordinator processes each player's Response Setup Messages
            groupBuildersList[0].processInitialSetupResponseMessage(SetupResFromJson)

        # The coordinator setup & broadcast message to the group member
        bcMsg = groupBuildersList[0].getBroadcastSetupMessage()

        # Serialize & deserialize the Broadcast message
        bcMsgFromJson = PyGroupSetupMessage.GroupBroadcastMessage()
        bcMsgFromJson.from_json(bcMsg.to_json())

        for j in range(1, len(groupBuildersList)):
            # Each player processes the broadcast setup message created & sent by the coordinator
            groupBuildersList[j].processBroadMessage(bcMsgFromJson)
            assert groupBuildersList[i] is not None, "Test failed"

        # Get the list of player's URIs in the group
        playerUris = []
        for ordinal, player_url in utils.getPlayers(totalPlayers):
            # Create Build for each player in the group and add it to the group list
            playerUris.append(player_url)

        # Verifying each player's group ID and ordinal number
        assert len(playerUris) == totalPlayers, "Test failed"

        gpSecPlayerBuilder = []
        # Make Group secret builder for proposer
        coordinatorBuilder = PyGroupSecretPlayerBuilder_Updated.GroupSecretPlayerBuilder(myUri=playerUris[0],
                                                                                         playerUris=playerUris,
                                                                                         groupBuilder=groupBuildersList[
                                                                                             0])
        gpSecPlayerBuilder.append(coordinatorBuilder)

        for x in range(1, len(playerUris)):
            # Get group Secret Setup Message for other player
            setupMsgForPlayer = coordinatorBuilder.getGroupSecretSetupMessage(forUrls=[playerUris[x]])
            assert setupMsgForPlayer is not None, "Test failed"
            setupMsgDataForPlayer = PyGroupSecretSetupMessage.GroupSecretSetupMessage()
            setupMsgDataForPlayer.from_json(setupMsgForPlayer.to_json())
            playerBuilder = PyGroupSecretPlayerBuilder_Updated.GroupSecretPlayerBuilder(myUri=playerUris[x],
                                                                                        seretSetupMsg=setupMsgDataForPlayer,
                                                                                        groupBuilder=groupBuildersList[
                                                                                            x])
            gpSecPlayerBuilder.append(playerBuilder)

        # Verify that all players get group Secret Setup Message
        assert len(gpSecPlayerBuilder) == len(playerUris), "Test failed"
        for x in range(1, len(playerUris)):
            # List of acceptance Group Secret Setup Responses message from each player
            playerResponse = gpSecPlayerBuilder[x].getGroupSecretSetupResponseMessage(
                state=PyGroupState.GroupState.Type.ACCEPTED, forUrls=[])
            assert playerResponse is not None, "Test failed"

            # Serialize & deserialize the setup message
            playerResponseFromJson = PyGroupSecretResponseMessage.GroupSecretResponseMessage()
            playerResponseFromJson.from_json(playerResponse.to_json())
            assert playerResponseFromJson is not None, "Test failed"
            # The coordinator processes the Group Setup Responses messages from Other players
            coordinatorBuilder.processGroupSetupResponse(response=playerResponseFromJson)
            assert coordinatorBuilder is not None


def test_GetPolynomialFunctionsOfThePlayer():
    for x in range(2):

        totalPlayers = randint(5, 8)
        threshold = randint(1, 2)

        # Verify Recombination Threshold value calculation applies, which is 2*t + 1 < = total number of player
        assert PyGroupSecretPlayerBuilder_Updated.GroupSecretPlayerBuilder.verifyThresholdValue(totalPlayers,
                                                                                                threshold) is True, "Test failed"

        # Get the list of players in the group
        groupBuildersList = []
        for ordinal, player_url in utils.getPlayers(totalPlayers):
            # Create Build for each player in the group and add it to the group list
            groupBuildersList.append(PyGroupBuilder.GroupBuilder(uri=player_url, ordinal=ordinal))

        # Verifying each player's group ID and ordinal number
        assert groupBuildersList[0].mGroupID != groupBuildersList[1].mGroupID, "Test failed"
        assert len(groupBuildersList) == totalPlayers, "Test failed"

        # The coordinator creates setup message with the number of threshold value and send it to other players
        setupMsg = groupBuildersList[0].getInitialSetupMessage(threshold_value=threshold)
        assert setupMsg is not None, "Test failed"
        # Serialize & deserialize the setup message
        setupMsgFromJson = PyGroupBuilder.GroupSetupMessage()
        setupMsgFromJson.from_json(setupMsg.to_json())
        assert setupMsgFromJson is not None, "Test failed"

        for x in range(1, len(groupBuildersList)):
            # Each player processes the setup message created & sent by the coordinatorf
            groupBuildersList[x].processInitialSetupMessage(setupMsgFromJson)
            # Verifying each player's group ID and Coordinator's Group ID are SAME
            assert groupBuildersList[0].mGroupID == groupBuildersList[x].mGroupID, "Test failed"

        for i in range(1, len(groupBuildersList)):
            # Each player creates Response Message to the setup message created by the coordinator
            playerSetupRes = groupBuildersList[i].getInitialSetupResponseMessage()
            assert playerSetupRes is not None, "Test failed"
            # Serialize & deserialize the Response Setup Message from each player
            SetupResFromJson = PyGroupSetupMessage.GroupSetupResponseMessage()
            SetupResFromJson.from_json(playerSetupRes.to_json())

            # The Coordinator processes each player's Response Setup Messages
            groupBuildersList[0].processInitialSetupResponseMessage(SetupResFromJson)

        # The coordinator setup & broadcast message to the group member
        bcMsg = groupBuildersList[0].getBroadcastSetupMessage()

        # Serialize & deserialize the Broadcast message
        bcMsgFromJson = PyGroupSetupMessage.GroupBroadcastMessage()
        bcMsgFromJson.from_json(bcMsg.to_json())

        for j in range(1, len(groupBuildersList)):
            # Each player processes the broadcast setup message created & sent by the coordinator
            groupBuildersList[j].processBroadMessage(bcMsgFromJson)
            assert groupBuildersList[i] is not None, "Test failed"

        # Get the list of player's URIs in the group
        playerUris = []
        for ordinal, player_url in utils.getPlayers(totalPlayers):
            # Create Build for each player in the group and add it to the group list
            playerUris.append(player_url)

        # Verifying each player's group ID and ordinal number
        assert len(playerUris) == totalPlayers, "Test failed"

        gpSecPlayerBuilder = []
        # Make Group secret builder for proposer
        coordinatorBuilder = PyGroupSecretPlayerBuilder_Updated.GroupSecretPlayerBuilder(myUri=playerUris[0],
                                                                                         playerUris=playerUris,
                                                                                         groupBuilder=groupBuildersList[
                                                                                             0])
        gpSecPlayerBuilder.append(coordinatorBuilder)

        for x in range(1, len(playerUris)):
            # Get group Secret Setup Message for other player
            setupMsgForPlayer = coordinatorBuilder.getGroupSecretSetupMessage(forUrls=[playerUris[x]])
            assert setupMsgForPlayer is not None, "Test failed"
            setupMsgDataForPlayer = PyGroupSecretSetupMessage.GroupSecretSetupMessage()
            setupMsgDataForPlayer.from_json(setupMsgForPlayer.to_json())
            playerBuilder = PyGroupSecretPlayerBuilder_Updated.GroupSecretPlayerBuilder(myUri=playerUris[x],
                                                                                        seretSetupMsg=setupMsgDataForPlayer,
                                                                                        groupBuilder=groupBuildersList[
                                                                                            x])

            gpSecPlayerBuilder.append(playerBuilder)

        # Verify that all players get group Secret Setup Message
        assert len(gpSecPlayerBuilder) == len(playerUris), "Test failed"

        for x in range(1, len(playerUris)):
            # List of acceptance Group Secret Setup Responses message from each player
            playerResponse = gpSecPlayerBuilder[x].getGroupSecretSetupResponseMessage(
                state=PyGroupState.GroupState.Type.ACCEPTED, forUrls=[])
            assert playerResponse is not None, "Test failed"

            # Serialize & deserialize the setup message
            playerResponseFromJson = PyGroupSecretResponseMessage.GroupSecretResponseMessage()
            playerResponseFromJson.from_json(playerResponse.to_json())
            # The coordinator processes the Group Setup Responses messages from Other players
            coordinatorBuilder.processGroupSetupResponse(response=playerResponseFromJson)

        for x in range(1, len(playerUris)):
            # Get the polynomials for other players
            playerPolyFuncs = coordinatorBuilder.getPolyFunctionsOfThePlayer(playerUris[x])

            playerPolyFuncsFromcoordinator = PyPolynomials.GroupSecretPolynomialVals()
            playerPolyFuncsFromcoordinator.from_json(playerPolyFuncs.to_json())
            # The coordinator process messages from all other players
            gpSecPlayerBuilder[x].processPolyFunctionsOfThePlayer(playerPolyFuncsFromcoordinator)


def test_GetPolyFunctionsEncryptedPoints():
    for x in range(2):

        totalPlayers = randint(5, 8)
        threshold = randint(1, 2)

        # Verify Recombination Threshold value calculation applies, which is 2*t + 1 < = total number of player
        assert PyGroupSecretPlayerBuilder_Updated.GroupSecretPlayerBuilder.verifyThresholdValue(totalPlayers,
                                                                                                threshold) is True, "Test failed"

        # Get the list of players in the group
        groupBuildersList = []
        for ordinal, player_url in utils.getPlayers(totalPlayers):
            # Create Build for each player in the group and add it to the group list
            groupBuildersList.append(PyGroupBuilder.GroupBuilder(uri=player_url, ordinal=ordinal))

        # Verifying each player's group ID and ordinal number
        assert groupBuildersList[0].mGroupID != groupBuildersList[1].mGroupID, "Test failed"
        assert len(groupBuildersList) == totalPlayers, "Test failed"

        # The coordinator creates setup message with the number of threshold value and send it to other players
        setupMsg = groupBuildersList[0].getInitialSetupMessage(threshold_value=threshold)
        assert setupMsg is not None, "Test failed"
        # Serialize & deserialize the setup message
        setupMsgFromJson = PyGroupBuilder.GroupSetupMessage()
        setupMsgFromJson.from_json(setupMsg.to_json())
        assert setupMsgFromJson is not None, "Test failed"

        for x in range(1, len(groupBuildersList)):
            # Each player processes the setup message created & sent by the coordinator
            groupBuildersList[x].processInitialSetupMessage(setupMsgFromJson)
            # Verifying each player's group ID and Coordinator's Group ID are SAME
            assert groupBuildersList[0].mGroupID == groupBuildersList[x].mGroupID, "Test failed"

        for i in range(1, len(groupBuildersList)):
            # Each player creates Response Message to the setup message created by the coordinator
            playerSetupRes = groupBuildersList[i].getInitialSetupResponseMessage()
            assert playerSetupRes is not None, "Test failed"
            # Serialize & deserialize the Response Setup Message from each player
            SetupResFromJson = PyGroupSetupMessage.GroupSetupResponseMessage()
            SetupResFromJson.from_json(playerSetupRes.to_json())

            # The Coordinator processes each player's Response Setup Messages
            groupBuildersList[0].processInitialSetupResponseMessage(SetupResFromJson)

        # The coordinator setup & broadcast message to the group member
        bcMsg = groupBuildersList[0].getBroadcastSetupMessage()

        # Serialize & deserialize the Broadcast message
        bcMsgFromJson = PyGroupSetupMessage.GroupBroadcastMessage()
        bcMsgFromJson.from_json(bcMsg.to_json())

        for j in range(1, len(groupBuildersList)):
            # Each player processes the broadcast setup message created & sent by the coordinator
            groupBuildersList[j].processBroadMessage(bcMsgFromJson)
            assert groupBuildersList[i] is not None, "Test failed"

        # Get the list of player's URIs in the group
        playerUris = []
        for ordinal, player_url in utils.getPlayers(totalPlayers):
            # Create Build for each player in the group and add it to the group list
            playerUris.append(player_url)

        # Verifying each player's group ID and ordinal number
        assert len(playerUris) == totalPlayers, "Test failed"

        gpSecPlayerBuilder = []
        # Coordinator makes the Group secret builder for itself
        coordinatorBuilder = PyGroupSecretPlayerBuilder_Updated.GroupSecretPlayerBuilder(myUri=playerUris[0],
                                                                                         playerUris=playerUris,
                                                                                         groupBuilder=groupBuildersList[
                                                                                             0])
        gpSecPlayerBuilder.append(coordinatorBuilder)

        for x in range(1, len(playerUris)):
            # Get group Secret Setup Message for other player
            setupMsgForPlayer = coordinatorBuilder.getGroupSecretSetupMessage(forUrls=[playerUris[x]])
            assert setupMsgForPlayer is not None, "Test failed"
            setupMsgDataForPlayer = PyGroupSecretSetupMessage.GroupSecretSetupMessage()
            setupMsgDataForPlayer.from_json(setupMsgForPlayer.to_json())
            playerBuilder = PyGroupSecretPlayerBuilder_Updated.GroupSecretPlayerBuilder(myUri=playerUris[x],
                                                                                        seretSetupMsg=setupMsgDataForPlayer,
                                                                                        groupBuilder=groupBuildersList[
                                                                                            x])

            gpSecPlayerBuilder.append(playerBuilder)

        # Verify that all players get group Secret Setup Message
        assert len(gpSecPlayerBuilder) == len(playerUris), "Test failed"

        for x in range(1, len(playerUris)):
            # List of acceptance Group Secret Setup Responses message from each player
            playerResponse = gpSecPlayerBuilder[x].getGroupSecretSetupResponseMessage(
                state=PyGroupState.GroupState.Type.ACCEPTED, forUrls=[])
            assert playerResponse is not None, "Test failed"

            # Serialize & deserialize the setup message
            playerResponseFromJson = PyGroupSecretResponseMessage.GroupSecretResponseMessage()
            playerResponseFromJson.from_json(playerResponse.to_json())
            # The coordinator processes the Group Setup Responses messages from Other players
            coordinatorBuilder.processGroupSetupResponse(response=playerResponseFromJson)

        for x in range(1, len(playerUris)):
            # Get the polynomials for other players
            playerPolyFuncs = coordinatorBuilder.getPolyFunctionsOfThePlayer(playerUris[x])

            playerPolyFuncsFromcoordinator = PyPolynomials.GroupSecretPolynomialVals()
            playerPolyFuncsFromcoordinator.from_json(playerPolyFuncs.to_json())

            # The coordinator process messages from all other players
            gpSecPlayerBuilder[x].processPolyFunctionsOfThePlayer(playerPolyFuncsFromcoordinator)

        for playerNumber in range(0, len(playerUris)):
            # Each player get his/her PublicSecretA0 message
            playera0Msg = gpSecPlayerBuilder[playerNumber].getPublicSecretA0Message()
            assert playera0Msg is not None, "Test failed"

            # Serialize & deserialize the Public Secret message of other players to coordinator
            playera0MsgFromJson = PyGroupSecretIntermediary.GroupSecretPublicMessage()
            playera0MsgFromJson.from_json(playera0Msg.to_json())
            assert playera0MsgFromJson is not None, "Test failed"

            # The coordinator process messages from all other players
            coordinatorBuilder.processPublicSecretA0Message(playera0MsgFromJson)

        # get the broadcast message from proposer
        coordinatora0BcastMsg = coordinatorBuilder.getPublicSecretA0Message()
        coordinatora0BcastMsgFromJson = PyGroupSecretIntermediary.GroupSecretPublicMessage()
        coordinatora0BcastMsgFromJson.from_json(coordinatora0BcastMsg.to_json())

        for x in range(1, len(playerUris)):
            # Other player processes sent message from coordinator
            gpSecPlayerBuilder[x].processPublicSecretA0Message(publicSecretA0=coordinatora0BcastMsgFromJson)

        # The Coordinator send the coefficients data to the other players
        coordinatorCoefficientMsg = coordinatorBuilder.getCoefficientMessages()
        coordinatorCoefficientMsgFromJson = PyGroupSecretIntermediary.EncryptedECPointsMessages()
        coordinatorCoefficientMsgFromJson.from_json(coordinatorCoefficientMsg.to_json())

        # All other players must process the incoming Coefficients from coordinator
        for x in range(1, len(playerUris)):
            # Send non-cooperating players Coefficients to the proposer
            playersCoefficientMsg = gpSecPlayerBuilder[x].getCoefficientMessages()
            playersCoefficientMsgFromJson = PyGroupSecretIntermediary.EncryptedECPointsMessages()
            playersCoefficientMsgFromJson.from_json(playersCoefficientMsg.to_json())
            coordinatorBuilder.processEncryptedCoefficients(playersCoefficientMsgFromJson)

        # coordinator broadcasts messages to other players
        coordinatorCoefficientMsg = coordinatorBuilder.getCoefficientMessages()
        coordinatorCoefficientMsgFromJson = PyGroupSecretIntermediary.EncryptedECPointsMessages()
        coordinatorCoefficientMsgFromJson.from_json(coordinatorCoefficientMsg.to_json())

        for x in range(1, len(playerUris)):
            # The coordinator process messages from all other players
            gpSecPlayerBuilder[x].processEncryptedCoefficients(coordinatorCoefficientMsgFromJson)

        # the coordinator send the Encrypted EC Points to the other players
        coordinatorEncryptedECPoints = coordinatorBuilder.getPolyFunctionsEncryptedPoints()
        coordinatorEncryptedECPointsFromJson = PyGroupSecretIntermediary.EncryptedFunctionECPointsMessages()
        coordinatorEncryptedECPointsFromJson.from_json(coordinatorEncryptedECPoints.to_json())
        assert coordinatorEncryptedECPointsFromJson is not None, "Test failed"

        # Other players must process the Encrypted EC Points from Coordinator
        for x in range(1, len(playerUris)):
            playersECPointsMessages = gpSecPlayerBuilder[x].getPolyFunctionsEncryptedPoints()
            playersECPointsMessagesFromJson = PyGroupSecretIntermediary.EncryptedFunctionECPointsMessages()
            playersECPointsMessagesFromJson.from_json(playersECPointsMessages.to_json())
            # Other players ECPoints  are processed by the proposer
            coordinatorBuilder.processPolyFunctionsEncryptedPoints(playersECPointsMessagesFromJson)

        # coordinator gets the polynomial value at the elliptical curve point
        coordinatorECPointsMsg = coordinatorBuilder.getPolyFunctionsEncryptedPoints()
        coordinatorEncryptedECPointsFromJson = PyGroupSecretIntermediary.EncryptedFunctionECPointsMessages()
        coordinatorEncryptedECPointsFromJson.from_json(coordinatorECPointsMsg.to_json())
        assert coordinatorEncryptedECPointsFromJson is not None, "Test failed"

        # Each player process the polynomial value at the elliptical curve point sent by the coordinator
        for x in range(1, len(playerUris)):
            gpSecPlayerBuilder[x].processPolyFunctionsEncryptedPoints(coordinatorEncryptedECPointsFromJson)
            assert gpSecPlayerBuilder[x] is not None, "Test failed"