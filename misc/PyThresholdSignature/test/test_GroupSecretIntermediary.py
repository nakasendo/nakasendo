import utils
import ecdsa
import FiniteGroup
import PyGroupState
import PyPolynomials
import PyGroupBuilder
import PyGroupSetupMessage
import PyGroupSecretSetupMessage
import PyGroupSecretIntermediary
import PyGroupSecretResponseMessage
import PyGroupSecretPlayerBuilder_Updated

from random import *

# To generate Elliptic Curve Digital Signature Algorithm (ECDSA)
Order = ecdsa.SECP256k1.order

def test_GetKeyShare():
    for x in range(1):

        # Randomly generate the total number of players
        totalPlayers = randint(5, 8)
        # Randomly generate the threshold value
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

        key_shares = []

        # Get the list of player's URIs in the group
        playerUris = []
        for ordinal, player_url in utils.getPlayers(totalPlayers):
            # Create Build for each player in the group and add it to the group list
            playerUris.append(player_url)

        # Verifying each player's group ID and ordinal number
        assert len(playerUris) == totalPlayers, "Test failed"

        # Joint Random Secret Sharing ( JRSS )
        gpSecPlayerBuilder = []
        # Coordinator makes the Group secret builder for itself
        coordinatorBuilder = PyGroupSecretPlayerBuilder_Updated.GroupSecretPlayerBuilder(myUri=playerUris[0],playerUris=playerUris,groupBuilder=groupBuildersList[0])
        gpSecPlayerBuilder.append(coordinatorBuilder)

        for x in range(1, len(playerUris)):
            # Get group Secret Setup Message for other player
            setupMsgForPlayer = coordinatorBuilder.getGroupSecretSetupMessage(forUrls=[playerUris[x]])
            assert setupMsgForPlayer is not None, "Test failed"
            setupMsgDataForPlayer = PyGroupSecretSetupMessage.GroupSecretSetupMessage()
            setupMsgDataForPlayer.from_json(setupMsgForPlayer.to_json())
            playerBuilder = PyGroupSecretPlayerBuilder_Updated.GroupSecretPlayerBuilder(myUri=playerUris[x],seretSetupMsg=setupMsgDataForPlayer,groupBuilder=groupBuildersList[x])
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

        # get the broadcast message from coordinator
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
            # Send non-cooperating players Coefficients to the coordinator
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
            # Other players ECPoints  are processed by the coordinator
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

        for playerNumber in range(0, len(playerUris)):
            key_shares.append(gpSecPlayerBuilder[playerNumber].getKeyShare())
            assert gpSecPlayerBuilder[playerNumber].getState() == PyGroupState.GroupState.Type.ACCEPTED, "Test failed"

def test_IntermediaryMessages():
    for x in range(1):

        #Randomly generate the total number of players
        totalPlayers = randint(5, 8)
        #Randomly generate the threshold value
        threshold = randint(1, 2)

        # Verify Recombination Threshold value calculation applies, which is 2*t + 1 < = total number of player
        assert PyGroupSecretPlayerBuilder_Updated.GroupSecretPlayerBuilder.verifyThresholdValue(totalPlayers, threshold) is True, "Test failed"

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

        key_shares = []
        ephemeral_key_shares = []
        blinding_key_shares = []

        # Get the list of player's URIs in the group
        playerUris = []
        for ordinal, player_url in utils.getPlayers(totalPlayers):
            # Create Build for each player in the group and add it to the group list
            playerUris.append(player_url)

        # Verifying each player's group ID and ordinal number
        assert len(playerUris) == totalPlayers, "Test failed"

        # Joint Random Secret Sharing ( JRSS )
        # Make key share, ephemeral and blinding secrets
        for key in ['KEY_SHARE', 'EPHEMERAL', 'BLINDING']:

            gpSecPlayerBuilder = []
            # Coordinator makes the Group secret builder for itself
            coordinatorBuilder = PyGroupSecretPlayerBuilder_Updated.GroupSecretPlayerBuilder(myUri=playerUris[0],playerUris=playerUris,groupBuilder=groupBuildersList[0])
            gpSecPlayerBuilder.append(coordinatorBuilder)

            for x in range(1, len(playerUris)):
                # Get group Secret Setup Message for other player
                setupMsgForPlayer = coordinatorBuilder.getGroupSecretSetupMessage(forUrls=[playerUris[x]])
                assert setupMsgForPlayer is not None, "Test failed"
                setupMsgDataForPlayer = PyGroupSecretSetupMessage.GroupSecretSetupMessage()
                setupMsgDataForPlayer.from_json(setupMsgForPlayer.to_json())
                playerBuilder = PyGroupSecretPlayerBuilder_Updated.GroupSecretPlayerBuilder(myUri=playerUris[x],seretSetupMsg=setupMsgDataForPlayer,groupBuilder=groupBuildersList[x])
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

            # get the broadcast message from coordinator
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
                # Send non-cooperating players Coefficients to the coordinator
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
                # Other players ECPoints  are processed by the coordinator
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

            for playerNumber in range(0, len(playerUris)):
                if (key == 'EPHEMERAL'):
                    ephemeral_key_shares.append(gpSecPlayerBuilder[playerNumber].getKeyShare())
                elif (key == 'BLINDING'):
                    blinding_key_shares.append(gpSecPlayerBuilder[playerNumber].getKeyShare())
                elif (key == 'KEY_SHARE'):
                    key_shares.append(gpSecPlayerBuilder[playerNumber].getKeyShare())

                assert gpSecPlayerBuilder[playerNumber].getState() == PyGroupState.GroupState.Type.ACCEPTED

            if (key == 'KEY_SHARE' or key == 'EPHEMERAL'):
                for playerNumber in range(0, len(playerUris)):
                    # Let each player get his/her PublicSecretA0
                    playera0Msg = gpSecPlayerBuilder[playerNumber].getPublicSecretA0Message()
                    assert playera0Msg is not None, "Test failed"

                    playera0MsgFromJson = PyGroupSecretIntermediary.GroupSecretPublicMessage()
                    playera0MsgFromJson.from_json(playera0Msg.to_json())
                    assert playera0MsgFromJson is not None, "Test failed"

                    # the coordinator processes all messages recieved from other players
                    coordinatorBuilder.processPublicSecretA0Message(playera0MsgFromJson)
                    assert coordinatorEncryptedECPointsFromJson is not None, "Test failed"

                # Coordinator broadcasts message to other players
                coordinatora0BcastMsg = coordinatorBuilder.getPublicSecretA0Message()
                coordinatora0BcastMsgFromJson = PyGroupSecretIntermediary.GroupSecretPublicMessage()
                coordinatora0BcastMsgFromJson.from_json(coordinatora0BcastMsg.to_json())

                for playerNumber in range(1, len(playerUris)):
                    # Other players process the broadcasted message from coordinator
                    gpSecPlayerBuilder[playerNumber].processPublicSecretA0Message(
                        publicSecretA0=coordinatora0BcastMsgFromJson)

                if (key == 'KEY_SHARE'):
                    key_share_pubKey = coordinatorBuilder.calculatePublicKeyUsingSecretA0()
                if (key == 'EPHEMERAL'):
                    ephemeral_key_share = coordinatorBuilder.calculatePublicKeyUsingSecretA0().x()
                    ephemeral_key_share = FiniteGroup.normalize_mod(ephemeral_key_share,Order) if Order else ephemeral_key_share

            # SETUP VERIFICATION DATA
            # Let the coordinator send their verification data(Coefficients) to the non-coordinating players
            coordinatorCoefficientMessages = coordinatorBuilder.getCoefficientMessages()
            coordinatorCoefficientMessagesFromJson = PyGroupSecretIntermediary.EncryptedECPointsMessages()
            coordinatorCoefficientMessagesFromJson.from_json(coordinatorCoefficientMessages.to_json())

            # All non-cooperating players must process the incoming Coefficients from coordinator
            for playerNumber in range(1, len(playerUris)):
                # Send non-cooperating players Coefficients to the coordinator
                playersCoefficientMessages = gpSecPlayerBuilder[playerNumber].getCoefficientMessages()
                playersCoefficientMessagesFromJson = PyGroupSecretIntermediary.EncryptedECPointsMessages()
                playersCoefficientMessagesFromJson.from_json(playersCoefficientMessages.to_json())
                coordinatorBuilder.processEncryptedCoefficients(playersCoefficientMessagesFromJson)

            # Broadcast messages to non-cooperating players
            coordinatorCoefficientMessages = coordinatorBuilder.getCoefficientMessages()
            coordinatorCoefficientMessagesFromJson = PyGroupSecretIntermediary.EncryptedECPointsMessages()
            coordinatorCoefficientMessagesFromJson.from_json(coordinatorCoefficientMessages.to_json())
            for playerNumber in range(1, len(playerUris)):
                gpSecPlayerBuilder[playerNumber].processEncryptedCoefficients(coordinatorCoefficientMessagesFromJson)

            # Let the coordinator send their verification data(Encrypted EC Points) to the non-coordinating players
            coordinatorEncryptedECPoints = coordinatorBuilder.getPolyFunctionsEncryptedPoints()
            coordinatorEncryptedECPointsFromJson = PyGroupSecretIntermediary.EncryptedFunctionECPointsMessages()
            coordinatorEncryptedECPointsFromJson.from_json(coordinatorEncryptedECPoints.to_json())

            # All non-cooperating players must process the incoming Functiona ECPoints from coordinator
            for playerNumber in range(1, len(playerUris)):
                # Send non-cooperating players ECPoints to the coordinator
                playersECPointsMessages = gpSecPlayerBuilder[playerNumber].getPolyFunctionsEncryptedPoints()
                playersECPointsMessagesFromJson = PyGroupSecretIntermediary.EncryptedFunctionECPointsMessages()
                playersECPointsMessagesFromJson.from_json(playersECPointsMessages.to_json())
                coordinatorBuilder.processPolyFunctionsEncryptedPoints(playersECPointsMessagesFromJson)

            # Broadcast messages to other players
            coordinatorECPointsMessages = coordinatorBuilder.getPolyFunctionsEncryptedPoints()
            coordinatorEncryptedECPointsFromJson = PyGroupSecretIntermediary.EncryptedFunctionECPointsMessages()
            coordinatorEncryptedECPointsFromJson.from_json(coordinatorECPointsMessages.to_json())
            for playerNumber in range(1, len(playerUris)):
                gpSecPlayerBuilder[playerNumber].processPolyFunctionsEncryptedPoints(
                    coordinatorEncryptedECPointsFromJson)

            # Verify the honesty
            for playerNumber in range(0, len(playerUris)):
                gpSecPlayerBuilder[playerNumber].verifyHonesty()

            # Verify the Correctness
            for playerNumber in range(0, len(playerUris)):
                gpSecPlayerBuilder[playerNumber].verifyCorrectness()

        ### Verification of Data
        # Generate Ephemeral Key and Pre-Signature Data
        # coordinator generates and setup Intermediary shares Mesage
        setupIntermediaryMsg = coordinatorBuilder.getSetupIntermediaryShareMessage(ephemeral_key=ephemeral_key_shares[0], blinding_key=blinding_key_shares[0], modulo=Order)
        assert setupIntermediaryMsg is not None, "Test failed"

        setupIntermediaryMsgFromJson = PyGroupSecretIntermediary.GroupSetupIntermediaryMessage()
        setupIntermediaryMsgFromJson.from_json(setupIntermediaryMsg.to_json())
        assert setupIntermediaryMsgFromJson is not None, "Test failed"

        for playerNumber in range(1, len(playerUris)):
            # Each player processes the intermediary setup message from the coordinator
            gpSecPlayerBuilder[playerNumber].processSetupIntermediaryMessages(intermediarymessage=setupIntermediaryMsgFromJson)

            playergrpSetupResponseIntermediaryMessage = gpSecPlayerBuilder[playerNumber].getIntermediaryMessageResponse(ephemeral_key=ephemeral_key_shares[playerNumber], blinding_key=blinding_key_shares[playerNumber],modulo=Order)
            playergrpSetupResponseIntermediaryMessageFromJson = PyGroupSecretIntermediary.GroupSetupResponseIntermediaryMessage()
            playergrpSetupResponseIntermediaryMessageFromJson.from_json(playergrpSetupResponseIntermediaryMessage.to_json())

            # The coordinator processes each player's intermediary setup response message
            coordinatorBuilder.processSetupIntermediaryMessages(intermediarymessage=playergrpSetupResponseIntermediaryMessageFromJson)

        coordinatorBroadcastIntermediaryMsg = coordinatorBuilder.getBroadcastIntermediaryMessage()
        coordinatorBroadcastIntermediaryMsgFromJson = PyGroupSecretIntermediary.GroupBroadcastIntermediaryMessage()
        coordinatorBroadcastIntermediaryMsgFromJson.from_json(coordinatorBroadcastIntermediaryMsg.to_json())

        for playerNumber in range(1, len(playerUris)):
            gpSecPlayerBuilder[playerNumber].processSetupIntermediaryMessages(intermediarymessage=coordinatorBroadcastIntermediaryMsgFromJson)

        # Dump Intermediary shares
        coordinatorBuilder.dumpSetupIntermediaryMessages()

        # Calculate Intermediate share : Curve point
        # Coordinator generates Intermediary shares curve point for the other players
        setupCurvePointIntermediaryMsg = coordinatorBuilder.getSetupIntermediaryShareCurvePointMessage(
            blinding_key=blinding_key_shares[0])

        setupCurvePointIntermediaryMsgFromJson = PyGroupSecretIntermediary.GroupSetupIntermediaryMessage()
        setupCurvePointIntermediaryMsgFromJson.from_json(setupCurvePointIntermediaryMsg.to_json())

        for playerNumber in range(1, len(playerUris)):
            # Each player processes the setup Intermediary curve Point message
            gpSecPlayerBuilder[playerNumber].processSetupIntermediaryCurvePointMessages(
                intermediarymessage=setupCurvePointIntermediaryMsgFromJson)

            playergrpSetupResponseCurvePointIntermediaryMessage = gpSecPlayerBuilder[
                playerNumber].getIntermediaryCurvePointMessageResponse(blinding_key=blinding_key_shares[playerNumber])
            playergrpSetupResponseCurvePointIntermediaryMessageFromJson = PyGroupSecretIntermediary.GroupSetupResponseIntermediaryMessage()
            playergrpSetupResponseCurvePointIntermediaryMessageFromJson.from_json(
                playergrpSetupResponseCurvePointIntermediaryMessage.to_json())
            coordinatorBuilder.processSetupIntermediaryCurvePointMessages(
                intermediarymessage=playergrpSetupResponseCurvePointIntermediaryMessageFromJson)

        coordinatorBroadcastCurvePointIntermediaryMsg = coordinatorBuilder.getBroadcastIntermediaryCurvePointMessage()
        coordinatorBroadcastCurvePointIntermediaryMsgFromJson = PyGroupSecretIntermediary.GroupBroadcastIntermediaryMessage()
        coordinatorBroadcastCurvePointIntermediaryMsgFromJson.from_json(
            coordinatorBroadcastCurvePointIntermediaryMsg.to_json())

        for playerNumber in range(1, len(playerUris)):
            # Each player processes the setup Intermediary curve Point message
            gpSecPlayerBuilder[playerNumber].processSetupIntermediaryCurvePointMessages(
                intermediarymessage=coordinatorBroadcastCurvePointIntermediaryMsgFromJson)

        # Dump Intermediary shares curve points
        coordinatorBuilder.dumpSetupIntermediaryCurvePointMessages()
        assert coordinatorBuilder is not None, "Test failed"

def test_SignatureGeneration():
    for x in range(1):

        totalPlayers = randint(5, 8)
        threshold = randint(1, 2)

        # Verify Recombination Threshold value calculation applies, which is 2*t + 1 < = total number of player
        assert PyGroupSecretPlayerBuilder_Updated.GroupSecretPlayerBuilder.verifyThresholdValue(totalPlayers, threshold) is True, "Test failed"

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

        key_shares = []
        ephemeral_key_shares = []
        blinding_key_shares = []

        # Get the list of player's URIs in the group
        playerUris = []
        for ordinal, player_url in utils.getPlayers(totalPlayers):
            # Create Build for each player in the group and add it to the group list
            playerUris.append(player_url)

        # Verifying each player's group ID and ordinal number
        assert len(playerUris) == totalPlayers, "Test failed"

        # Joint Random Secret Sharing ( JRSS )
        # Make key share, ephemeral and blinding secrets
        for key in ['KEY_SHARE', 'EPHEMERAL', 'BLINDING']:

            gpSecPlayerBuilder = []
            # Coordinator makes the Group secret builder for itself
            coordinatorBuilder = PyGroupSecretPlayerBuilder_Updated.GroupSecretPlayerBuilder(myUri=playerUris[0],playerUris=playerUris,groupBuilder=groupBuildersList[0])
            gpSecPlayerBuilder.append(coordinatorBuilder)

            for x in range(1, len(playerUris)):
                # Get group Secret Setup Message for other player
                setupMsgForPlayer = coordinatorBuilder.getGroupSecretSetupMessage(forUrls=[playerUris[x]])
                assert setupMsgForPlayer is not None, "Test failed"
                setupMsgDataForPlayer = PyGroupSecretSetupMessage.GroupSecretSetupMessage()
                setupMsgDataForPlayer.from_json(setupMsgForPlayer.to_json())
                playerBuilder = PyGroupSecretPlayerBuilder_Updated.GroupSecretPlayerBuilder(myUri=playerUris[x],seretSetupMsg=setupMsgDataForPlayer,groupBuilder=groupBuildersList[x])
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

            for x in range(0, len(playerUris)):
                # Each player get his/her PublicSecretA0 message
                playera0Msg = gpSecPlayerBuilder[x].getPublicSecretA0Message()
                assert playera0Msg is not None, "Test failed"

                # Serialize & deserialize the Public Secret message of other players to coordinator
                playera0MsgFromJson = PyGroupSecretIntermediary.GroupSecretPublicMessage()
                playera0MsgFromJson.from_json(playera0Msg.to_json())
                assert playera0MsgFromJson is not None, "Test failed"

                # The coordinator process messages from all other players
                coordinatorBuilder.processPublicSecretA0Message(playera0MsgFromJson)

            # get the broadcast message from coordinator
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
                # Send non-cooperating players Coefficients to the coordinator
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
                # Other players ECPoints  are processed by the coordinator
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

            for x in range(0, len(playerUris)):
                if (key == 'EPHEMERAL'):
                    ephemeral_key_shares.append(gpSecPlayerBuilder[x].getKeyShare())
                elif (key == 'BLINDING'):
                    blinding_key_shares.append(gpSecPlayerBuilder[x].getKeyShare())
                elif (key == 'KEY_SHARE'):
                    key_shares.append(gpSecPlayerBuilder[x].getKeyShare())

                assert gpSecPlayerBuilder[x].getState() == PyGroupState.GroupState.Type.ACCEPTED

            if (key == 'KEY_SHARE' or key == 'EPHEMERAL'):
                for x in range(0, len(playerUris)):
                    # Let each player get his/her PublicSecretA0
                    playera0Msg = gpSecPlayerBuilder[x].getPublicSecretA0Message()
                    assert playera0Msg is not None, "Test failed"

                    playera0MsgFromJson = PyGroupSecretIntermediary.GroupSecretPublicMessage()
                    playera0MsgFromJson.from_json(playera0Msg.to_json())
                    assert playera0MsgFromJson is not None, "Test failed"

                    # the coordinator processes all messages recieved from other players
                    coordinatorBuilder.processPublicSecretA0Message(playera0MsgFromJson)
                    assert coordinatorEncryptedECPointsFromJson is not None, "Test failed"

                # Coordinator broadcasts message to other players
                coordinatora0BcastMsg = coordinatorBuilder.getPublicSecretA0Message()
                coordinatora0BcastMsgFromJson = PyGroupSecretIntermediary.GroupSecretPublicMessage()
                coordinatora0BcastMsgFromJson.from_json(coordinatora0BcastMsg.to_json())

                for x in range(1, len(playerUris)):
                    # Other players process the broadcasted message from coordinator
                    gpSecPlayerBuilder[x].processPublicSecretA0Message(
                        publicSecretA0=coordinatora0BcastMsgFromJson)

                if (key == 'KEY_SHARE'):
                    key_share_pubKey = coordinatorBuilder.calculatePublicKeyUsingSecretA0()
                if (key == 'EPHEMERAL'):
                    ephemeral_key_share = coordinatorBuilder.calculatePublicKeyUsingSecretA0().x()
                    ephemeral_key_share = FiniteGroup.normalize_mod(ephemeral_key_share,Order) if Order else ephemeral_key_share

            # SETUP VERIFICATION DATA
            # Let the coordinator send their verification data(Coefficients) to the non-coordinating players
            coordinatorCoefficientMessages = coordinatorBuilder.getCoefficientMessages()
            coordinatorCoefficientMessagesFromJson = PyGroupSecretIntermediary.EncryptedECPointsMessages()
            coordinatorCoefficientMessagesFromJson.from_json(coordinatorCoefficientMessages.to_json())

            # All non-cooperating players must process the incoming Coefficients from coordinator
            for x in range(1, len(playerUris)):
                # Send non-cooperating players Coefficients to the coordinator
                playersCoefficientMessages = gpSecPlayerBuilder[x].getCoefficientMessages()
                playersCoefficientMessagesFromJson = PyGroupSecretIntermediary.EncryptedECPointsMessages()
                playersCoefficientMessagesFromJson.from_json(playersCoefficientMessages.to_json())
                coordinatorBuilder.processEncryptedCoefficients(playersCoefficientMessagesFromJson)

            # Broadcast messages to non-cooperating players
            coordinatorCoefficientMessages = coordinatorBuilder.getCoefficientMessages()
            coordinatorCoefficientMessagesFromJson = PyGroupSecretIntermediary.EncryptedECPointsMessages()
            coordinatorCoefficientMessagesFromJson.from_json(coordinatorCoefficientMessages.to_json())
            for x in range(1, len(playerUris)):
                gpSecPlayerBuilder[x].processEncryptedCoefficients(coordinatorCoefficientMessagesFromJson)

            # Let the coordinator send their verification data(Encrypted EC Points) to the non-coordinating players
            coordinatorEncryptedECPoints = coordinatorBuilder.getPolyFunctionsEncryptedPoints()
            coordinatorEncryptedECPointsFromJson = PyGroupSecretIntermediary.EncryptedFunctionECPointsMessages()
            coordinatorEncryptedECPointsFromJson.from_json(coordinatorEncryptedECPoints.to_json())

            # All non-cooperating players must process the incoming Functiona ECPoints from coordinator
            for x in range(1, len(playerUris)):
                # Send non-cooperating players ECPoints to the coordinator
                playersECPointsMessages = gpSecPlayerBuilder[x].getPolyFunctionsEncryptedPoints()
                playersECPointsMessagesFromJson = PyGroupSecretIntermediary.EncryptedFunctionECPointsMessages()
                playersECPointsMessagesFromJson.from_json(playersECPointsMessages.to_json())
                coordinatorBuilder.processPolyFunctionsEncryptedPoints(playersECPointsMessagesFromJson)

            # Broadcast messages to other players
            coordinatorECPointsMessages = coordinatorBuilder.getPolyFunctionsEncryptedPoints()
            coordinatorEncryptedECPointsFromJson = PyGroupSecretIntermediary.EncryptedFunctionECPointsMessages()
            coordinatorEncryptedECPointsFromJson.from_json(coordinatorECPointsMessages.to_json())
            for x in range(1, len(playerUris)):
                gpSecPlayerBuilder[x].processPolyFunctionsEncryptedPoints(
                    coordinatorEncryptedECPointsFromJson)

            for x in range(0, len(playerUris)):
                # Verify the honesty
                gpSecPlayerBuilder[x].verifyHonesty()

                # Verify the Correctness
                gpSecPlayerBuilder[x].verifyCorrectness()

        ### Verification of Data
        # Generate Ephemeral Key and Pre-Signature Data
        # coordinator generates and setup Intermediary shares Mesage
        setupIntermediaryMsg = coordinatorBuilder.getSetupIntermediaryShareMessage(ephemeral_key=ephemeral_key_shares[0], blinding_key=blinding_key_shares[0], modulo=Order)
        assert setupIntermediaryMsg is not None, "Test failed"

        setupIntermediaryMsgFromJson = PyGroupSecretIntermediary.GroupSetupIntermediaryMessage()
        setupIntermediaryMsgFromJson.from_json(setupIntermediaryMsg.to_json())
        assert setupIntermediaryMsgFromJson is not None, "Test failed"

        for x in range(1, len(playerUris)):
            # Each player processes the intermediary setup message from the coordinator
            gpSecPlayerBuilder[x].processSetupIntermediaryMessages(intermediarymessage=setupIntermediaryMsgFromJson)

            playergrpSetupResponseIntermediaryMessage = gpSecPlayerBuilder[x].getIntermediaryMessageResponse(ephemeral_key=ephemeral_key_shares[x], blinding_key=blinding_key_shares[x],modulo=Order)
            playergrpSetupResponseIntermediaryMessageFromJson = PyGroupSecretIntermediary.GroupSetupResponseIntermediaryMessage()
            playergrpSetupResponseIntermediaryMessageFromJson.from_json(playergrpSetupResponseIntermediaryMessage.to_json())

            # The coordinator processes each player's intermediary setup response message
            coordinatorBuilder.processSetupIntermediaryMessages(intermediarymessage=playergrpSetupResponseIntermediaryMessageFromJson)

        coordinatorBroadcastIntermediaryMsg = coordinatorBuilder.getBroadcastIntermediaryMessage()
        coordinatorBroadcastIntermediaryMsgFromJson = PyGroupSecretIntermediary.GroupBroadcastIntermediaryMessage()
        coordinatorBroadcastIntermediaryMsgFromJson.from_json(coordinatorBroadcastIntermediaryMsg.to_json())

        for x in range(1, len(playerUris)):
            gpSecPlayerBuilder[x].processSetupIntermediaryMessages(intermediarymessage=coordinatorBroadcastIntermediaryMsgFromJson)

        # Dump Intermediary shares
        coordinatorBuilder.dumpSetupIntermediaryMessages()

        # Calculate Intermediate share : Curve point
        # Coordinator generates Intermediary shares curve point for the other players
        setupCurvePointIntermediaryMsg = coordinatorBuilder.getSetupIntermediaryShareCurvePointMessage(blinding_key=blinding_key_shares[0])

        setupCurvePointIntermediaryMsgFromJson = PyGroupSecretIntermediary.GroupSetupIntermediaryMessage()
        setupCurvePointIntermediaryMsgFromJson.from_json(setupCurvePointIntermediaryMsg.to_json())

        for x in range(1, len(playerUris)):
            # Each player processes the setup Intermediary curve Point message
            gpSecPlayerBuilder[x].processSetupIntermediaryCurvePointMessages(intermediarymessage=setupCurvePointIntermediaryMsgFromJson)

            playergrpSetupResponseCurvePointIntermediaryMessage = gpSecPlayerBuilder[x].getIntermediaryCurvePointMessageResponse(blinding_key=blinding_key_shares[x])
            playergrpSetupResponseCurvePointIntermediaryMessageFromJson = PyGroupSecretIntermediary.GroupSetupResponseIntermediaryMessage()
            playergrpSetupResponseCurvePointIntermediaryMessageFromJson.from_json(playergrpSetupResponseCurvePointIntermediaryMessage.to_json())
            coordinatorBuilder.processSetupIntermediaryCurvePointMessages(intermediarymessage=playergrpSetupResponseCurvePointIntermediaryMessageFromJson)

        coordinatorBroadcastCurvePointIntermediaryMsg = coordinatorBuilder.getBroadcastIntermediaryCurvePointMessage()
        coordinatorBroadcastCurvePointIntermediaryMsgFromJson = PyGroupSecretIntermediary.GroupBroadcastIntermediaryMessage()
        coordinatorBroadcastCurvePointIntermediaryMsgFromJson.from_json(coordinatorBroadcastCurvePointIntermediaryMsg.to_json())

        for x in range(1, len(playerUris)):
            # Each player processes the setup Intermediary curve Point message
            gpSecPlayerBuilder[x].processSetupIntermediaryCurvePointMessages(
                intermediarymessage=coordinatorBroadcastCurvePointIntermediaryMsgFromJson)

        # Dump Intermediary shares curve points
        coordinatorBuilder.dumpSetupIntermediaryCurvePointMessages()
        assert coordinatorBuilder is not None, "Test failed"

        # Threshold Signature Generation
        msg = 'Hello, I am a message, generate Signature'

        players_signature_share_list = []
        # Generate share of Signature by each player
        for x in range(0, len(playerUris)):

            # Generate Intermediary Share Interpolation for each player
            gpSecPlayerBuilder[x].generateIntermediaryShareInterpolation()
            players_signature_share_list.append(gpSecPlayerBuilder[x].getShareOfSignatureUpdated(msg=msg,key_share=key_shares[x],blinding_key=blinding_key_shares[x],ephemeral_share=ephemeral_key_share,modulo=Order))

        #Ensure all player Share of Signature
        assert len(players_signature_share_list) == len(playerUris), "Test failed"

        # Coordinator set the current share of signature
        signature = coordinatorBuilder.setSignatureShare(signature_share=players_signature_share_list[0])

        for x in range(1, len(playerUris)):
            # Each player get the current share of the signature from the coordinator
            SignatureShareMsg = gpSecPlayerBuilder[x].getSignatureShareMessage(signature_share=players_signature_share_list[x])
            assert SignatureShareMsg is not None, "Test failed"
            SignatureShareMsgFromJson = PyGroupSecretIntermediary.GroupSignatureShareMessage()
            SignatureShareMsgFromJson.from_json(SignatureShareMsg.to_json())
            assert SignatureShareMsgFromJson is not None, "Test failed"

            #The coordinator process the signature share response message from each player in the group
            coordinatorBuilder.processSignatureShareMessage(signature_share_msg=SignatureShareMsgFromJson)
            assert coordinatorBuilder is not None, "Test failed"