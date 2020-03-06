import orchestrator
import player
from random import *

mod = player.Player.modulo
msg = 'Hello, I am a message, generate Signature'

def printLog(*args, **kwargs):
    print(*args, **kwargs)

def test_ThresholdPlusThresholdPlusOne_LessThan_TotalPlayer():
    for num in range(10):
        # threshold = recombination number of private key
        # totalPlayers = total number in group
        totalPlayers = randint(101, 150)
        threshold = randint(3, 50)
        coordinator = orchestrator.Orchestrator()
        proposer = "proposer" + str(num) + "@sdk.co.uk"
        groupId, invitees = coordinator.createGroup(proposer, threshold, totalPlayers)

        # Verify Recombination Threshold calculation applies, which is 2*t + 1 < = total number of player
        assert len(groupId) == 36, "Test failed"
        assert len(invitees) == 0, "Test failed"

def test_ThresholdPlusThresholdPlusOne_Equal_TotalPlayer():
    for num in range(10):
        # threshold = recombination number of private key
        # totalPlayers = total number in group
        threshold = randint(3, 50)
        totalPlayers = threshold + threshold + 1
        coordinator = orchestrator.Orchestrator()
        proposer = "proposer" + str(num) + "@sdk.co.uk"
        groupId, invitees = coordinator.createGroup(proposer, threshold, totalPlayers)

        # Verify Recombination Threshold calculation applies, which is 2*t + 1 < = total number of player
        assert len(groupId) == 36, "Test failed"
        assert len(invitees) == 0, "Test failed"

def test_ThresholdPlusThresholdPlusOne_GreaterThan_TotalPlayer():
    for num in range(10):
        # threshold = recombination number of private key
        # totalPlayers = total number in group
        threshold = randint(101, 150)
        totalPlayers = randint(3, 50)
        coordinator = orchestrator.Orchestrator()
        proposer = "proposer" + str(num) + "@sdk.co.uk"
        try:
            groupId, invitees = coordinator.createGroup(proposer, threshold, totalPlayers)
            # Verify Recombination Threshold calculation applies, which is 2*t + 1 < = total number of player
            assert len(groupId) == 36, "Test failed"
            assert len(invitees) == 0, "Test failed"

        except Exception as errorMsg:

            assert "Parameters are not valid!" in str(errorMsg), "Test failed"

def test_TotalPlayer_Less_Or_Equal_Two():
    for num in range(10):
        # threshold = recombination number of private key
        # totalPlayers = total number in group
        threshold = randint(-100, 0)
        totalPlayers = threshold + threshold + 1
        coordinator = orchestrator.Orchestrator()
        proposer = "proposer" + str(num) + "@sdk.co.uk"
        try:
            groupId, invitees = coordinator.createGroup(proposer, threshold, totalPlayers)
            # Verify Recombination Threshold calculation applies, which is 2*t + 1 < = total number of player
            assert len(groupId) == 36, "Test failed"
            assert len(invitees) == 0, "Test failed"

        except Exception as errorMsg:

            assert "Parameters are not valid!" in str(errorMsg), "Test failed"

def test_OrchestratorGroupMetadata():
    for num in range(10):
        # threshold = recombination number of private key
        # totalPlayers = total number in group
        # degree = degree of polynomial (dervied from threshold - 1 )
        totalPlayers = randint(101, 150)
        threshold = randint(3, 50)
        degree = threshold - 1
        proposer = "proposer" + str(num) + "@sdk.co.uk"

        gpMetadata = orchestrator.GroupMetadata(num, proposer, threshold, totalPlayers, degree)

        # verifying the Orchestrator group metadata
        assert gpMetadata.proposer == proposer, "Test failed"
        assert gpMetadata.m == threshold, "Test failed"
        assert gpMetadata.n == totalPlayers, "Test failed"
        assert gpMetadata.t == degree, "Test failed"
        assert gpMetadata.groupIsSet is False, "Test failed"

def test_CreateGroup():
    for num in range(10):
        # threshold = recombination number of private key
        # totalPlayers = total number in group
        totalPlayers = randint(101, 150)
        threshold = randint(3, 50)
        # Contains core logic for orchestrating (co-ordination) Threshold Signature
        coordinator = orchestrator.Orchestrator()
        proposer = "proposer" + str(num) + "@sdk.co.uk"

        # Orchestrator creates the group with n total player
        groupId, invitees = coordinator.createGroup(proposer, threshold, totalPlayers)

        # Verify Recombination Threshold calculation applies, which is 2*t + 1 < = total number of player
        assert len(groupId) == 36, "Test failed"
        assert len(invitees) == 0, "Test failed"

        totalInvitation = []
        for ordinal in range(totalPlayers):
            userId = "player" + str(ordinal) + "@sdk.co.uk"

            # Register each player in the group with Orchestrator
            coordinator.register(userId, "player" + str(ordinal))
            # Orchestrator receiving inivitation reply from each player
            totalInvitation.append(coordinator.acceptInvite(userId, groupId, True))

        # Returns a list of players participating in the group
        teamList = coordinator.getParticipants(groupId)
        # Verify the validation of the group
        assert len(teamList) == totalPlayers, "Test failed"
        assert coordinator.validGroup(groupId), "Test failed"
        assert len(totalInvitation) == totalPlayers, "Test failed"

def test_PlayerGroupMetadata():
    for num in range(10):
        # threshold = recombination number of private key
        # totalPlayers = total number in group
        totalPlayers = randint(101, 150)
        threshold = randint(3, 50)
        # Contains core logic for orchestrating (co-ordination) Threshold Signature
        coordinator = orchestrator.Orchestrator()
        proposer = "proposer" + str(num) + "@sdk.co.uk"

        # Orchestrator creates the group with n total player
        groupId, invitees = coordinator.createGroup(proposer, threshold, totalPlayers)

        # Verify Recombination Threshold calculation applies, which is 2*t + 1 < = total number of player
        assert len(groupId) == 36, "Test failed"
        assert len(invitees) == 0, "Test failed"

        totalInvitation = []
        for ordinal in range(totalPlayers):
            userId = "player" + str(ordinal) + "@sdk.co.uk"

            # Register player in the group with Orchestrator
            coordinator.register(userId, "player" + str(ordinal))
            totalInvitation.append(coordinator.acceptInvite(userId, groupId, True))

        # Returns a list of players participating in the group
        teamList = coordinator.getParticipants(groupId)
        # Verify the validation of the group
        assert len(teamList) == totalPlayers, "Test failed"
        assert coordinator.validGroup(groupId), "Test failed"
        assert len(totalInvitation) == totalPlayers, "Test failed"

        playerList = []
        for num, userName in enumerate(teamList):
            # Returns a list of users which is the participant List
            ordinal, listExcludingUser, degree = coordinator.getGroupIsSetParameters(userName, groupId)
            playerList.append(player.Player(printLog))

            # To create each player metadata
            gpMetadata = player.PlayerGroupMetadata(printLog, num, ordinal, listExcludingUser, degree, mod)

            # verifying metadata info of each player in the group
            assert gpMetadata.ordinal == ordinal, "Test failed"
            assert gpMetadata.degree == coordinator.getDegree(groupId), "Test failed"
            assert gpMetadata.presignInitiator is False, "Test failed"
        assert len(playerList) == totalPlayers, "Test failed"

def test_AddGroup():
    for num in range(10):
        # threshold = recombination number of private key
        # totalPlayers = total number in group
        totalPlayers = randint(11, 13)
        threshold = randint(3, 5)
        # Contains core logic for orchestrating (co-ordination) Threshold Signature
        coordinator = orchestrator.Orchestrator()
        proposer = "proposer" + str(num) + "@sdk.co.uk"

        # Orchestrator creates the group with n total player
        groupId, invitees = coordinator.createGroup(proposer, threshold, totalPlayers)

        # Verify Recombination Threshold calculation applies, which is 2*t + 1 < = total number of player
        assert len(groupId) == 36, "Test failed"
        assert len(invitees) == 0, "Test failed"

        totalInvitation = []
        for ordinal in range(totalPlayers):
            userId = "player" + str(ordinal) + "@sdk.co.uk"

            # Register player in the group with Orchestrator
            coordinator.register(userId, "player" + str(ordinal))
            totalInvitation.append(coordinator.acceptInvite(userId, groupId, True))

        # Returns a list of players participating in the group
        teamList = coordinator.getParticipants(groupId)
        # Verify the validation of the group
        assert len(teamList) == totalPlayers, "Test failed"
        assert coordinator.validGroup(groupId), "Test failed"
        assert len(totalInvitation) == totalPlayers, "Test failed"

        playerList = []
        for num in range(len(teamList)):
            playerList.append(player.Player(printLog))

        for num, userName in enumerate(playerList):
            ordinal = 1
            userOrdinal = 1
            ordlist = []
            for num1, userid in enumerate(playerList):
                if userName == userid:
                    userOrdinal = ordinal
                    ordinal += 1
                else:
                    ordlist.append((ordinal, ' '))
                    ordinal += 1
            # Add Group - create the PlayerGroupMetadata, create the Polynomial, and call pre-calculation
            playerList[num].addGroup(groupId, userOrdinal, ordlist, threshold - 1)
            fx = len(playerList[num].groups[groupId].transientData.f_x)

            # verifying metadata info of each player in the group
            assert playerList[num].GroupIDs() == groupId, "Test failed"
            assert playerList[num].groups[groupId].ordinal == userOrdinal, "Test failed"
            assert len(playerList[num].groups[groupId].transientData.evals) == len(teamList) - 1, "Test failed"
            assert fx == 62 or fx == 64, "Test failed"

def test_createSecret():
    for num in range(1):
        # threshold = recombination number of private key
        # totalPlayers = total number in group
        totalPlayers = randint(9, 10)
        threshold = randint(3, 4)
        # Contains core logic for orchestrating (co-ordination) Threshold Signature
        coordinator = orchestrator.Orchestrator()
        proposer = "proposer" + str(num) + "@sdk.co.uk"

        # Orchestrator creates the group with n total player
        groupId, invitees = coordinator.createGroup(proposer, threshold, totalPlayers)

        # Verify Recombination Threshold calculation applies, which is 2*t + 1 < = total number of player
        assert len(groupId) == 36, "Test failed"
        assert len(invitees) == 0, "Test failed"

        totalInvitation = []
        for ordinal in range(totalPlayers):
            userId = "player" + str(ordinal) + "@sdk.co.uk"

            # Register player in the group with Orchestrator
            coordinator.register(userId, "player" + str(ordinal))
            totalInvitation.append(coordinator.acceptInvite(userId, groupId, True))

        # Returns a list of players participating in the group
        teamList = coordinator.getParticipants(groupId)

        # Verify the validation of the group
        assert len(teamList) == totalPlayers, "Test failed"
        assert coordinator.validGroup(groupId), "Test failed"
        assert len(totalInvitation) == totalPlayers, "Test failed"

        playerList = []
        for num in range(len(teamList)):
            playerList.append(player.Player(printLog))

        for num, userName in enumerate(playerList):
            ordinal = 1
            userOrdinal = 1
            ordlist = []
            for num1, userid in enumerate(playerList):
                if userName == userid:
                    userOrdinal = ordinal
                    ordinal += 1
                else:
                    ordlist.append((ordinal, ' '))
                    ordinal += 1
            # Add Group - create the PlayerGroupMetadata, create the Polynomial, and call pre-calculation
            playerList[num].addGroup(groupId, userOrdinal, ordlist, threshold - 1)
            fx = len(playerList[num].groups[groupId].transientData.f_x)

            # verifying metadata info of each player in the group
            assert playerList[num].GroupIDs() == groupId, "Test failed"
            assert playerList[num].groups[groupId].ordinal == userOrdinal, "Test failed"
            assert len(playerList[num].groups[groupId].transientData.evals) == len(teamList) - 1, "Test failed"
            assert fx == 62 or fx == 64, "Test failed"

        # Received all encrypted coefficient data, distribute of the players
        for num in range(len(teamList)):
            gpId = playerList[num].GroupIDs()
            ord = playerList[num].groups[groupId].ordinal
            hidPoly = playerList[num].groups[groupId].transientData.hiddenPolynomial
            hidEval = playerList[num].groups[groupId].transientData.hiddenEvals

            # Received all encrypted coefficient data, distribute
            allEncryptedRec = coordinator.collateData(gpId, ord, hidPoly, hidEval)

        # Verify all encrypted coefficient data, distribute of the players are received
        assert allEncryptedRec, "Test failed"

        # To create a secret - used to create a privateKeyShare, little-k, alpha
        for caltype in ['PRIVATEKEYSHARE', 'ALPHA', 'LITTLEK']:

            for num in range(len(teamList)):
                gpId = playerList[num].GroupIDs()
                tordinal = playerList[num].groups[groupId].ordinal
                frmOrdinal = playerList[num].groups[groupId].ordinalList
                fx = playerList[num].groups[groupId].transientData.f_x

                # 1st time through JVRSS don't need to generate new polynomial
                # 2nd and 3rd times through JVRSS need to create new polynomial
                groupid, ord, hidPoly, hidEval = playerList[num].requestData(gpId, caltype)

                # The Orchestrator collates data for all players
                allCollateData = coordinator.collateData(groupid, ord, hidPoly, hidEval)

                # The Orchestrator receives collated data
                collatedHidPolys, collatedHidEval = coordinator.getCollatedData(groupid)

                # add evals to sumOfEvals - this is the shared secret
                #   - used for privateKeyShare, little-k, alpha
                allEvalsReceivedOrch = coordinator.allEvalsReceived(gpId, tordinal)
                for frmOrd in enumerate(frmOrdinal):
                    # All Players have received all their evals
                    allEvalsReceivedPlay = playerList[num].allEvalsReceived(gpId, tordinal, frmOrd, fx)

                # To create a secret - used to create a privateKeyShare, little-k, alpha
                groupId, result = playerList[num].createSecret(gpId, caltype, collatedHidPolys, collatedHidEval)
                priKey = playerList[num].groups[groupId].privateKeyShare
                pubKey = playerList[num].groups[groupId].publicKeyShare

                # Verify the created Public keys
                assert groupId == gpId, "Test failed"
                assert len(str(result)) == 64 or len(str(result)) == 62, "Test failed"
                assert len(str(priKey)) == 64 or len(str(priKey)) == 62, "Test failed"
                assert len(str(pubKey)) == 66, "Test failed"

            # Verify all Players have received their evaluation values
            assert allEvalsReceivedPlay, "Test failed"
            # assert allEvalsReceivedOrch, "Test failed"

            # Verify all data for all players are collated by Orchestrator
            assert allCollateData, "Test failed"

def test_setSigner():
    for num in range(1):
        # threshold = recombination number of private key
        # totalPlayers = total number in group
        totalPlayers = 5
        threshold = 3
        # Contains core logic for orchestrating (co-ordination) Threshold Signature
        coordinator = orchestrator.Orchestrator()
        proposer = "proposer" + str(num) + "@sdk.co.uk"

        # Orchestrator creates the group with n total player
        groupId, invitees = coordinator.createGroup(proposer, threshold, totalPlayers)

        # Verify Recombination Threshold calculation applies, which is 2*t + 1 < = total number of player
        assert len(groupId) == 36, "Test failed"
        assert len(invitees) == 0, "Test failed"

        totalInvitation = []
        for ordinal in range(totalPlayers):
            userId = "player" + str(ordinal) + "@sdk.co.uk"

            # Register player in the group with Orchestrator
            coordinator.register(userId, "player" + str(ordinal))
            totalInvitation.append(coordinator.acceptInvite(userId, groupId, True))

        # Returns a list of players participating in the group
        teamList = coordinator.getParticipants(groupId)

        # Verify the validation of the group
        assert len(teamList) == totalPlayers, "Test failed"
        assert coordinator.validGroup(groupId), "Test failed"
        assert len(totalInvitation) == totalPlayers, "Test failed"

        playerList = []
        for num in range(len(teamList)):
            playerList.append(player.Player(printLog))

        for num, userName in enumerate(playerList):
            ordinal = 1
            userOrdinal = 1
            ordlist = []
            for num1, userid in enumerate(playerList):
                if userName == userid:
                    userOrdinal = ordinal
                    ordinal += 1
                else:
                    ordlist.append((ordinal, ' '))
                    ordinal += 1
            # Add Group - create the PlayerGroupMetadata, create the Polynomial, and call pre-calculation
            playerList[num].addGroup(groupId, userOrdinal, ordlist, threshold - 1)
            fx = len(playerList[num].groups[groupId].transientData.f_x)

            # verifying metadata info of each player in the group
            assert playerList[num].GroupIDs() == groupId, "Test failed"
            assert playerList[num].groups[groupId].ordinal == userOrdinal, "Test failed"
            assert len(playerList[num].groups[groupId].transientData.evals) == len(teamList) - 1, "Test failed"
            assert fx == 64, "Test failed"

        # Received all encrypted coefficient data, distribute of the players
        for num in range(len(teamList)):
            gpId = playerList[num].GroupIDs()
            ord = playerList[num].groups[groupId].ordinal
            hidPoly = playerList[num].groups[groupId].transientData.hiddenPolynomial
            hidEval = playerList[num].groups[groupId].transientData.hiddenEvals

            # Received all encrypted coefficient data, distribute
            allEncryptedRec = coordinator.collateData(gpId, ord, hidPoly, hidEval)

        # Verify all encrypted coefficient data, distribute of the players are received
        assert allEncryptedRec, "Test failed"

        # To create a secret - used to create a privateKeyShare, little-k, alpha
        for caltype in ['PRIVATEKEYSHARE', 'ALPHA', 'LITTLEK']:

            for num in range(len(teamList)):
                gpId = playerList[num].GroupIDs()
                tordinal = playerList[num].groups[groupId].ordinal
                frmOrdinal = playerList[num].groups[groupId].ordinalList
                fx = playerList[num].groups[groupId].transientData.f_x
                # 1st time through JVRSS don't need to generate new polynomial
                # 2nd and 3rd times through JVRSS need to create new polynomial
                groupid, ord, hidPoly, hidEval = playerList[num].requestData(gpId, caltype)

                # The Orchestrator collates data for all players
                allCollateData = coordinator.collateData(groupid, ord, hidPoly, hidEval)

                # The Orchestrator receives collated data
                collatedHidPolys, collatedHidEval = coordinator.getCollatedData(groupid)

                # add evals to sumOfEvals - this is the shared secret
                #   - used for privateKeyShare, little-k, alpha
                allEvalsReceivedOrch = coordinator.allEvalsReceived(gpId, tordinal)
                for frmOrd in enumerate(frmOrdinal):
                    # All Players have received all their evals
                    allEvalsReceivedPlay = playerList[num].allEvalsReceived(gpId, tordinal, frmOrd, fx)

                # To create a secret - used to create a privateKeyShare, little-k, alpha
                groupId, result = playerList[num].createSecret(gpId, caltype, collatedHidPolys, collatedHidEval)
                priKey = playerList[num].groups[groupId].privateKeyShare
                pubKey = playerList[num].groups[groupId].publicKeyShare

                # Verify the created Public keys
                assert groupId == gpId, "Test failed"
                assert len(str(result)) == 64 or len(str(result)) == 62, "Test failed"
                assert len(str(priKey)) == 64 or len(str(result)) == 62, "Test failed"
                assert len(str(pubKey)) == 66, "Test failed"

            # Verify all Players have received their evaluation values
            assert allEvalsReceivedPlay, "Test failed"
            # assert allEvalsReceivedOrch, "Test failed"

            # Verify all data for all players are colllated by Orchestrator
            assert allCollateData, "Test failed"

        # Calculate V and W share for each player
        for num in range(len(teamList)):
            grpOrdinal, shareVW = playerList[num].getVWshares(groupId)

            # collate V and W data
            allVWdataCollated = coordinator.collateVWData(groupId, grpOrdinal, shareVW)

        # get collated V and W data
        collatedVW = coordinator.getCollatedVWData(groupId)
        for num in range(len(teamList)):
            # sets the collated VW Data for all players in group
            playerList[num].setSharedVWData(groupId, collatedVW)
            # Verify ephemeral key for each player
            ephemeralKeysCompleted = coordinator.allEphemeralKeysCompleted(teamList[num], groupId)

        # Verify all V and W data for all players are collated
        assert allVWdataCollated, "Test failed"
        # Verify all ephemeral keys for all players are completed
        assert ephemeralKeysCompleted, "Test failed"

        for num in range(len(teamList)):
            playerList[num].setShareInitiator(playerList[num].GroupIDs())
            playerList[num].setPresignInitiator(playerList[num].GroupIDs(), num)
            playerShareInitiator = playerList[num].isShareInitiator(playerList[num].GroupIDs())
            playerPresignInitiator = playerList[num].isPresignInitiator(playerList[num].GroupIDs())
            # verify each player's share and pre-sign initiator in the group
            assert playerShareInitiator, "Test failed"
            assert playerPresignInitiator, "Test failed"
            playerList[num].setSigningInitiator(playerList[num].GroupIDs())
        #print(playerList[num])

        # collection of all player's signatures
        for num in range(len(teamList)):
            hMSG = playerList[num].hashMessage(msg)
            index = 0

            # Request all player's signature
            grpId, grpOrd, sValue, playersMsg = playerList[num].requestSignatureData(playerList[num].GroupIDs(),index, hMSG)

            # collate signature data
            allSignCollated = coordinator.signature(grpId, grpOrd, sValue)

        # get collated signature data
        collatedSign = coordinator.getSignatureData(grpId)

        # Verify all signature data from all players are collated
        assert allSignCollated, "Test failed"

        # for num in range(len(teamList)):
        signer, sign0 = playerList[0].sign(playerList[0].GroupIDs(), hMSG, collatedSign)
        assert len(str(signer)) == 64, "Test failed"
