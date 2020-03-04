import random

import orchestrator
import player
import Nakasendo
import ecdsa

G= ecdsa.SECP256k1.generator

def myPrintFunction(text):
    print(text)

def hashMessage(message):
    # hash the message
    Hm = Nakasendo.hash256(message)
    return Hm.value

#Verify hashMessage
def test_hashMessage():
    pl = player.Player(myPrintFunction)

    #checking for a normal string
    res = pl.hashMessage('This is a Test Message')
    assert res == 'd8632127fc9a382d750344cf07e3f66c05354014a47abd86318aef9fbad5a16c' , "Test Failed"

    #checking for special characters
    res = pl.hashMessage('!"£$%^&*()!!"£$%^&*()!')
    assert res == '7ac05fb0bbe35aba95466fe7b20770972a0a0b7806c4412738da15efa6b22819', "Test Failed"


#Verify defs -
# setShareInitiator, isShareInitiator, setPresignInitiator, isPresignInitiator, numberPresignsLeftToDo, checkGroup, deleteGroup
def test_Initiator():
    orc = orchestrator.Orchestrator()

    noOfPlayers  = 6
    m = 3 #recombination number
    n = 6 # total number
    degree = m - 1 # recombination number - 1

    playersList= []
    remplayersList = []

    # register players with Orchestrator
    for num in range(noOfPlayers):
        orc.register('player' + str(num), 'ref' + str(num))
        playersList.append('player'+str(num))

    #create group
    groupid, invites = orc.createGroup(playersList[0], m, n)

    #remaining list
    for p in playersList:
        if p != playersList[0]:
            remplayersList.append(p)

    # accept invite
    for user in remplayersList:
        orc.acceptInvite(user, groupid, True)

    #get the users who belong to the groupid
    participantList = orc.getParticipants(groupid)

    #add the player list
    players=[]
    for num in participantList:
        players.append(player.Player(myPrintFunction))

    #add player to the group
    for num,participant in enumerate(participantList):
        ord = 1
        myord = 1
        ordlist=[]
        for num1,participant1 in enumerate(participantList):
            if participant == participant1:
                myord = ord
                ord+=1
            else:
                ordlist.append((ord,' '))
                ord+=1
        players[num].addGroup(groupid,myord,ordlist,degree)

        # setShareInitiator
        players[num].setShareInitiator(groupid)

        # isShareInitiator
        assert players[num].isShareInitiator(groupid) is True, " Test Failed"

    for f in players:

        # isPresignInitiator
        assert f.isPresignInitiator(groupid) is False, " Test Failed "

        # setPresignInitiator
        f.setPresignInitiator(groupid, 10)

        # isPresignInitiator
        assert f.isPresignInitiator(groupid) is True, " Test Failed "

        # numberPresignsLeftToDo
        assert f.numberPresignsLeftToDo(groupid) == 9, " Test Failed"
        assert f.numberPresignsLeftToDo(groupid) == 8, " Test Failed"

        #pass zero and negative values
        #revisit once the fix is done
        f.setPresignInitiator(groupid, -2)
        assert f.numberPresignsLeftToDo(groupid) == 0, " Test Failed" # need to check if negative  values are allowed
        assert f.numberPresignsLeftToDo(groupid) == 0, " Test Failed" # need to check if negative  values are allowed

        #getOrdinal
        ordinal = f.getOrdinal(groupid)
        assert ordinal == f.groups[groupid].ordinal, " Test failed since getOrdinal fails to return correct ordinal "

        # getOrdinal
        ordinalList = f.getOrdinalList(groupid)
        assert ordinalList == f.groups[groupid].ordinalList, " Test failed since getOrdinal fails to return correct ordinal List"

    #delete Group
    for f in players:
        #check group exists
        assert f.checkGroup(groupid) is 1, "Test Failed"

    actualOrdinals = [1,2,3,4,5,6]
    for f in players:
        expectedOrdinals = f.groups[groupid].allOrdinals()
        assert set(expectedOrdinals)  == set(actualOrdinals)

    #delete above group
    for f in players:
        f.deleteGroup(groupid)
        assert f.checkGroup(groupid) is 0, "Test Failed"


#Verify createPolynomial definition
def test_CreatePolynomial():
    degree = random.randint(1,10)

    #call createPolynomial function to generate the polynomial
    pl = player.Player(myPrintFunction)

    # Hexadecimal Type
    polynomialHex = pl.createPolynomial(degree)
    assert polynomialHex.degree == degree , "Test Failed"
    assert polynomialHex.modulo == pl.modulo, "Test Failed"
    assert len(polynomialHex.coefficients) == degree + 1 , "Test Failed"


#Verify polynomialprecalculation definition
def test_VerifyPolynomialsforPlayer():
    orc = orchestrator.Orchestrator()
    noOfPlayers = 3
    m = 2  # recombination number
    n = 3  # total number
    degree = m - 1  # recombination number - 1

    playersList = []
    remplayersList = []

    # register players with Orchestrator
    for num in range(noOfPlayers):
        orc.register('player' + str(num), 'ref' + str(num))
        playersList.append('player' + str(num))

    # create group
    groupid, invites = orc.createGroup(playersList[0], m, n)

    #remaining list
    for p in playersList:
        if p != playersList[0]:
            remplayersList.append(p)

    # accept invite
    for user in remplayersList:
        orc.acceptInvite(user, groupid, True)

    # get the users who belong to the groupid
    participantList = orc.getParticipants(groupid)

    # add the player list
    players = []
    for num in participantList:
        players.append(player.Player(myPrintFunction))

    # add player to the group
    for num, participant in enumerate(participantList):
        ord = 1
        myord = 1
        ordlist = []

        for num1, participant1 in enumerate(participantList):
            if participant == participant1:
                myord = ord
                ord += 1
            else:
                ordlist.append((ord, ' '))
                ord += 1
        players[num].groups[groupid] = player.PlayerGroupMetadata(myPrintFunction, groupid, myord, ordlist, degree, player.Player.modulo)

        group = players[num].groups[groupid]
        group.privateKeyPolynomial = players[num].createPolynomial(degree)
        group.polynomialPreCalculation(group.privateKeyPolynomial,player.Player.modulo,group.ordinal)

    for f in players:
        assert f.groups[groupid].transientData.f_x is not None, "Test Failed"
        assert f.groups[groupid].transientData.hiddenEvals is not None, "Test Failed"
        assert f.groups[groupid].transientData.evals is not None, "Test Failed"

        if f.groups[groupid].ordinal == 1:
            fxOrd1 = f.groups[groupid].transientData.f_x
            hiddenEvalsOrd1 = f.groups[groupid].transientData.hiddenEvals
            evalsOrd1 = f.groups[groupid].transientData.evals
        elif f.groups[groupid].ordinal == 2:
            fxOrd2 = f.groups[groupid].transientData.f_x
            hiddenEvalsOrd2 = f.groups[groupid].transientData.hiddenEvals
            evalsOrd2 = f.groups[groupid].transientData.evals
        elif f.groups[groupid].ordinal == 3:
            fxOrd3 = f.groups[groupid].transientData.f_x
            hiddenEvalsOrd3 = f.groups[groupid].transientData.hiddenEvals
            evalsOrd3 = f.groups[groupid].transientData.evals

    # checking if the new polynomial is recreated everytime its called
    for f in players:
        #generating polynomial again
        group = f.groups[groupid]
        group.privateKeyPolynomial = f.createPolynomial(degree)
        group.polynomialPreCalculation(group.privateKeyPolynomial, player.Player.modulo, group.ordinal)

    for f in players:
        assert f.groups[groupid].transientData.f_x is not None, "Test Failed"
        assert f.groups[groupid].transientData.hiddenEvals is not None, "Test Failed"
        assert f.groups[groupid].transientData.evals is not None, "Test Failed"

        if f.groups[groupid].ordinal == 1:
            assert f.groups[groupid].transientData.f_x != fxOrd1, "Test Failed"
            assert f.groups[groupid].transientData.hiddenEvals != hiddenEvalsOrd1, "Test Failed"
            assert f.groups[groupid].transientData.evals != evalsOrd1, "Test Failed"
        elif f.groups[groupid].ordinal == 2:
            assert f.groups[groupid].transientData.f_x != fxOrd2, "Test Failed"
            assert f.groups[groupid].transientData.hiddenEvals != hiddenEvalsOrd2, "Test Failed"
            assert f.groups[groupid].transientData.evals != evalsOrd2, "Test Failed"
        elif f.groups[groupid].ordinal == 3:
            assert f.groups[groupid].transientData.f_x != fxOrd3, "Test Failed"
            assert f.groups[groupid].transientData.hiddenEvals != hiddenEvalsOrd3, "Test Failed"
            assert f.groups[groupid].transientData.evals != evalsOrd3, "Test Failed"


#Validate PlayerGroupMetaData definition
def test_PlayerGroupMetaData():

    orc = orchestrator.Orchestrator()
    noOfPlayers  = 6
    m = 3 #recombination number
    n = 6 # total number
    degree = m - 1 # recombination number - 1

    modulo = player.Player.modulo
    playersList= []
    remplayersList = []

    # register players with Orchestrator
    for num in range(noOfPlayers):
        orc.register('player' + str(num), 'ref' + str(num))
        playersList.append('player'+str(num))

    #create group
    groupid, invites = orc.createGroup(playersList[0], m, n)

    #remaining list
    for p in playersList:
        if p != playersList[0]:
            remplayersList.append(p)

    # accept invite
    for user in remplayersList:
        orc.acceptInvite(user, groupid, True)

    #get the users who belong to the groupid
    participantList = orc.getParticipants(groupid)

    #add the player list
    players=[]
    for num in participantList:
        players.append(player.Player(myPrintFunction))

    #add player to the group
    for num,participant in enumerate(participantList):
        ord = 1
        myord = 1
        ordlist=[]
        for num1,participant1 in enumerate(participantList):
            if participant == participant1:
                myord = ord
                ord+=1
            else:
                ordlist.append((ord,' '))
                ord+=1
        players[num].addGroup(groupid,myord,ordlist,degree)

        #assertions to check if the group has been added
        assert len(players[num].groups) == 1, "Test Failed"
        assert players[num].GroupIDs() == groupid, "Test Failed"
        assert players[num].checkGroup(groupid) == 1, "Test Failed"

        #assertions to verify the attributes of players in the group
        if num > 0:
            assert players[num].groups[groupid].ordinal != players[num-1].groups[groupid].ordinal, "Test Failed"
            assert players[num].groups[groupid].ordinalList != players[num-1].groups[groupid].ordinalList, "Test Failed"
            assert players[num].groups[groupid].ordinal not in players[num].groups[groupid].ordinalList, "Test Failed"
            assert players[num].groups[groupid].degree == players[num-1].groups[groupid].degree, "Test Failed"
            assert players[num].groups[groupid].ordinalList != players[num - 1].groups[groupid].ordinalList, "Test Failed"
            assert players[num].groups[groupid].privateKeyPolynomial is not None, "Test Failed"
            assert players[num].groups[groupid].privateKeyPolynomial.degree == 2, "Test Failed"
            assert players[num].groups[groupid].privateKeyPolynomial.modulo == modulo, "Test Failed"
            assert len(players[num].groups[groupid].privateKeyPolynomial.coefficients) == degree+1, "Test Failed"
            assert players[num].groups[groupid].privateKeyShare is None, "Test Failed"
            assert players[num].groups[groupid].publicKeyShare is None, "Test Failed"
            assert len(players[num].groups[groupid].ephemeralKeyList) == 0, "Test Failed"
            assert players[num].groups[groupid].littleK is None, "Test Failed"
            assert players[num].groups[groupid].alpha is None, "Test Failed"
            assert players[num].groups[groupid].presignInitiator is False, "Test Failed"
            assert players[num].groups[groupid].numberPresigns is 1, "Test Failed"

            #assertions on transientData
            assert len(players[num].groups[groupid].transientData.f_x) != 0, "Test Failed"
            assert len(players[num].groups[groupid].transientData.f_x) is not None, "Test Failed"
            assert len(players[num].groups[groupid].transientData.f_x) == 64, "Test Failed"
            assert str(players[num].groups[groupid].transientData.sumOfEvals) == '0', "Test Failed"
            assert len(players[num].groups[groupid].transientData.evals) == len(participantList)-1, "Test Failed"
            assert len(players[num].groups[groupid].transientData.hiddenEvals) == len(participantList), "Test Failed"
            assert len(players[num].groups[groupid].transientData.hiddenPolynomial) == degree + 1, "Test Failed"
            assert len(players[num].groups[groupid].transientData.allHiddenPolynomials) == 0, "Test Failed"
            assert len(players[num].groups[groupid].transientData.allVWshares) == 0, "Test Failed"

            # checking if the elemens in the list hiddenPoly ends with 02/03
            for poly in players[num].groups[groupid].transientData.hiddenPolynomial:
                poly = str(poly)
                assert poly.startswith('02') or poly.startswith(
                    '03'), " Test Failed , incorrect hiddenPolynomial format"
                assert len(poly) == 66, " Test Failed incorrect hiddenPolynomails value"

            # checking if the elemens in the list hiddenPoly ends with 02/03
            for key, eval in players[num].groups[groupid].transientData.hiddenEvals.items():
                eval = str(eval)
                assert eval.startswith('02') or eval.startswith(
                    '03'), " Test Failed , incorrect hiddenPolynomial format"
                assert len(eval) == 66, " Test Failed incorrect hiddenEval value "


# Verify allEvalsReceived definition
def test_AllEvalsReceived():
    orc = orchestrator.Orchestrator()
    noOfPlayers = 3
    m = 2  # recombination number
    n = 3  # total number
    degree = m - 1  # recombination number - 1

    playersList = []
    remplayersList = []

    # register players with Orchestrator
    for num in range(noOfPlayers):
        orc.register('player' + str(num), 'ref' + str(num))
        playersList.append('player' + str(num))

    # create group
    groupid, invites = orc.createGroup(playersList[0], m, n)

    #remaining list
    for p in playersList:
        if p != playersList[0]:
            remplayersList.append(p)

    # accept invite
    for user in remplayersList:
        orc.acceptInvite(user, groupid, True)

    # get the users who belong to the groupid
    participantList = orc.getParticipants(groupid)

    # add the player list
    players = []
    for num in participantList:
        players.append(player.Player(myPrintFunction))

    # add player to the group
    for num, participant in enumerate(participantList):
        ord = 1
        myord = 1
        ordlist = []
        for num1, participant1 in enumerate(participantList):
            if participant == participant1:
                myord = ord
                ord += 1
            else:
                ordlist.append((ord, ' '))
                ord += 1
        players[num].addGroup(groupid, myord, ordlist, degree)

#call call allEvalsReceived def to get the evals from other players
# say for player1 f1(1) eval get the evals from player2 f2(1) eval and player3 f3(1) eval
    for f in players:
        num = 1
        for fx in players:
            if f != fx:
                num+=1
                fxValue = fx.getEvals(groupid,f.groups[groupid].ordinal)
                assert fxValue is not None, "Test Failed as the getEvals didnt return the correct number of evals"
                allEvalsReceived = f.allEvalsReceived(groupid, f.groups[groupid].ordinal, fx.groups[groupid].ordinal, fxValue)
                if f != fx and (num == len(participantList)):
                    assert allEvalsReceived is True, "Test Failed" # allEvalsReceived should return True once it is called by all the players in the ordinal list
                elif f != fx:
                    assert allEvalsReceived is False, "Test Failed" # allEvalsReceived should return False until it is called by all the players in the ordinal list


#Verify createSecret for caltype - privatekeyshare
def test_CreateSecretPrivateKeyShare():
    orc = orchestrator.Orchestrator()

    noOfPlayers  = 6
    m = 3 #recombination number
    n = 6 # total number
    degree = m - 1 # recombination number - 1

    playersList= []
    remplayersList = []
#
    # register players with Orchestrator
    for num in range(noOfPlayers):
        orc.register('player' + str(num), 'ref' + str(num))
        playersList.append('player'+str(num))

    #create group
    groupid, invites = orc.createGroup(playersList[0], m, n)

    #remaining list
    for p in playersList:
        if p != playersList[0]:
            remplayersList.append(p)

    # accept invite
    for user in remplayersList:
        orc.acceptInvite(user, groupid, True)

    #get the users who belong to the groupid
    participantList = orc.getParticipants(groupid)

    #add the player list
    players=[]
    for num in participantList:
        players.append(player.Player(myPrintFunction))

    #add player to the group
    for num,participant in enumerate(participantList):
        ord = 1
        myord = 1
        ordlist=[]
        for num1,participant1 in enumerate(participantList):
            if participant == participant1:
                myord = ord
                ord+=1
            else:
                ordlist.append((ord,' '))
                ord+=1
        players[num].addGroup(groupid,myord,ordlist,degree)

    # request data for PRIVATEKEYSHARE
    for num,pl in enumerate(players):
        groupId, ordinal, hiddenPoly, hiddenEvals = pl.requestData(groupid, 'PRIVATEKEYSHARE')

        assert groupId is not None, "Test Failed, requestData definition fails to return groupid for PRIVATESHAREKEY calculation"
        assert groupId == pl.groups[groupid].id, "Test Failed, requestData definition fails to return correct groupid for PRIVATESHAREKEY calculation"
        assert ordinal is not None, "Test Failed, requestData definition fails to return ordinal for PRIVATESHAREKEY calculation"
        assert ordinal == pl.groups[groupid].ordinal, " Test Failed, equestData definition fails to return correct ordinal for PRIVATESHAREKEY calculation"
        assert hiddenPoly is not None and len(hiddenPoly) == m, "Test Failed, requestData definition fails to return hiddenPoly for PRIVATESHAREKEY calculation"
        assert hiddenEvals is not None and len(hiddenEvals) == len(players), "Test Failed, requestData definition fails to return hiddenEvals for PRIVATESHAREKEY calculation"

        #checking if the elemens in the list hiddenPoly ends with 02/03
        for poly in hiddenPoly:
            poly = str(poly)
            assert poly.startswith('02') or poly.startswith('03'), " Test Failed , incorrect hiddenPolynomial format"
            assert len(poly) == 66, " Test Failed incorrect hiddenPolynomails value"

        #checking if the elemens in the list hiddenPoly ends with 02/03
        for key,eval in hiddenEvals.items():
            eval = str(eval)
            assert eval.startswith('02') or eval.startswith('03'), " Test Failed , incorrect hiddenPolynomial format"
            assert len(eval) == 66, " Test Failed incorrect hiddenEval value "

        #collate data of every player
        collatedData = orc.collateData(groupid, pl.groups[groupid].ordinal, hiddenPoly, hiddenEvals)

        if (num == len(participantList)-1):
            assert collatedData is True, "Test Failed" # collatedData should return True once it is called by all the participants
        else:
            assert collatedData is False, "Test Failed" # allEvalsReceived should return False until it is called by all the participants

    #get collated data
    collatedHiddenPolys, collatedHiddenEvals = orc.getCollatedData(groupid)

    #assertion on len of collatedHiddenEvals
    for key, values in collatedHiddenEvals.items():
        assert len(values) == len(participantList), "Test Failed"

    # assertion on len of collatedHiddenPolys
    for key, values in collatedHiddenPolys.items():
        assert len(values) == degree+1, "Test Failed"

    for f in players:
        for fx in players:
            if f != fx:
                fxValue = fx.getEvals(groupid,f.groups[groupid].ordinal)
                assert fxValue is not None, "Test Failed as the getEvals didnt return the correct number of evals"

                f.allEvalsReceived(groupid, f.groups[groupid].ordinal, fx.groups[groupid].ordinal, fxValue)
                sumOfEvals = f.groups[groupid].transientData.sumOfEvals
        groupId, result = f.createSecret(groupid, 'PRIVATEKEYSHARE', collatedHiddenPolys, collatedHiddenEvals)
        assert groupId is not None, "Test Failed, createSecret definition fails to return groupid for PRIVATESHAREKEY calculation"
        assert result is not None, "Test Failed , createSecret definition fails to return result for PRIVATESHAREKEY calculation"
        assert result == sumOfEvals, "Test Failed , createSecret definition fails to return result for PRIVATESHAREKEY calculation"

    assert len(f.groups[groupid].transientData.f_x) == 64, " Incorrect length of f_x value"
    assert len(f.groups[groupid].transientData.allHiddenPolynomials) is not 0, " AllHiddenPolynomials attribute value missing"
    assert len(f.groups[groupid].transientData.allHiddenPolynomials) == len(participantList), " AllHiddenPolynomials attribute value missing"
    assert f.groups[groupid].publicKeyShare is not None and len(str(f.groups[groupid].publicKeyShare)) == 66, "Test Failed"
    assert f.groups[groupid].privateKeyShare is not None and len(str(f.groups[groupid].privateKeyShare)) == 64, "Test Failed"
    assert f.groups[groupid].privateKeyShare == sumOfEvals, "Test Failed"
    assert f.groups[groupid].littleK is None, "Test Failed"
    assert f.groups[groupid].alpha is None, "Test Failed"


# Verify createSecret for caltype - littlek
def test_CreateSecretLittleK():
    orc = orchestrator.Orchestrator()

    noOfPlayers  = 6
    m = 3 #recombination number
    n = 6 # total number
    degree = m - 1 # recombination number - 1

    playersList= []
    remplayersList = []
#
    # register players with Orchestrator
    for num in range(noOfPlayers):
        orc.register('player' + str(num), 'ref' + str(num))
        playersList.append('player'+str(num))

    #create group
    groupid, invites = orc.createGroup(playersList[0], m, n)

    #remaining list
    for p in playersList:
        if p != playersList[0]:
            remplayersList.append(p)

    # accept invite
    for user in remplayersList:
        orc.acceptInvite(user, groupid, True)

    #get the users who belong to the groupid
    participantList = orc.getParticipants(groupid)

    #add the player list
    players=[]
    for num in participantList:
        players.append(player.Player(myPrintFunction))

    #add player to the group
    for num,participant in enumerate(participantList):
        ord = 1
        myord = 1
        ordlist=[]
        for num1,participant1 in enumerate(participantList):
            if participant == participant1:
                myord = ord
                ord+=1
            else:
                ordlist.append((ord,' '))
                ord+=1
        players[num].addGroup(groupid,myord,ordlist,degree)

    # request data for PRIVATEKEYSHARE
    for num,pl in enumerate(players):
        groupId, ordinal, hiddenPoly, hiddenEvals = pl.requestData(groupid, 'LITTLEK')

        assert groupId is not None, "Test Failed, requestData definition fails to return groupid for LITTLEK calculation"
        assert groupId == pl.groups[groupid].id, "Test Failed, requestData definition fails to return correct groupid for LITTLEK calculation"
        assert ordinal is not None, "Test Failed, requestData definition fails to return ordinal for LITTLEK calculation"
        assert ordinal == pl.groups[groupid].ordinal, " Test Failed, equestData definition fails to return correct ordinal for LITTLEK calculation"
        assert hiddenPoly is not None and len(hiddenPoly) == m, "Test Failed, requestData definition fails to return hiddenPoly for LITTLEK calculation"
        assert hiddenEvals is not None and len(hiddenEvals) == len(players), "Test Failed, requestData definition fails to return hiddenEvals for LITTLEK calculation"

        #checking if the elemens in the list hiddenPoly ends with 02/03
        for poly in hiddenPoly:
            poly = str(poly)
            assert poly.startswith('02') or poly.startswith('03'), " Test Failed , incorrect hiddenPolynomial format"
            assert len(poly) == 66, " Test Failed incorrect hiddenPolynomails value"

        #checking if the elemens in the list hiddenPoly ends with 02/03
        for key,eval in hiddenEvals.items():
            eval = str(eval)
            assert eval.startswith('02') or eval.startswith('03'), " Test Failed , incorrect hiddenPolynomial format"
            assert len(eval) == 66, " Test Failed incorrect hiddenEval value "

        #collate data of every player
        collatedData = orc.collateData(groupid, pl.groups[groupid].ordinal, hiddenPoly, hiddenEvals)

        if (num == len(participantList)-1):
            assert collatedData is True, "Test Failed" # collatedData should return True once it is called by all the participants
        else:
            assert collatedData is False, "Test Failed" # allEvalsReceived should return False until it is called by all the participants

    #get collated data
    collatedHiddenPolys, collatedHiddenEvals = orc.getCollatedData(groupid)

    #assertion on len of collatedHiddenEvals
    for key, values in collatedHiddenEvals.items():
        assert len(values) == len(participantList), "Test Failed"

    # assertion on len of collatedHiddenPolys
    for key, values in collatedHiddenPolys.items():
        assert len(values) == degree+1, "Test Failed"

    for f in players:
        for fx in players:
            if f != fx:
                fxValue = fx.getEvals(groupid,f.groups[groupid].ordinal)
                assert fxValue is not None, "Test Failed as the getEvals didnt return the correct number of evals"

                f.allEvalsReceived(groupid, f.groups[groupid].ordinal, fx.groups[groupid].ordinal, fxValue)
                sumOfEvals = f.groups[groupid].transientData.sumOfEvals
        groupId, result = f.createSecret(groupid, 'LITTLEK', collatedHiddenPolys, collatedHiddenEvals)
        assert groupId is not None, "Test Failed, createSecret definition fails to return groupid for LITTLEK calculation"
        assert result is not None, "Test Failed , createSecret definition fails to return result for LITTLEK calculation"
        assert result == sumOfEvals, "Test Failed , createSecret definition fails to return result for LITTLEK calculation"

    assert len(f.groups[groupid].transientData.f_x) == 64, " Incorrect length of f_x value"
    assert len(f.groups[groupid].transientData.allHiddenPolynomials) is not 0, " AllHiddenPolynomials attribute value missing"
    assert len(f.groups[groupid].transientData.allHiddenPolynomials) == len(participantList), " AllHiddenPolynomials attribute value missing"
    assert f.groups[groupid].publicKeyShare is None, "Test Failed"
    assert f.groups[groupid].privateKeyShare is None, "Test Failed"
    assert f.groups[groupid].littleK is not None and len(str(f.groups[groupid].littleK)) == 64, "Test Failed"
    assert f.groups[groupid].littleK == sumOfEvals, "Test Failed"
    assert f.groups[groupid].alpha is None, "Test Failed"

    #tamper collateddata before calling createsecret
    collatedHiddenPolys = {1: ['037159B7B3F8F80593C5E3E48DBBCCBF6251F100BFEC5157E605C30909DBDDE823', '02556C938748EB9B16CC1320672B0DD9A3F4743CEA1F7AAA2275119BBFD80BCF4E', '02E254877BE36D146223A1B70C45956F2D1EA2A2D14E4CD99E23432CFDDE24D9E1'], 2: ['02A232640597182320E92F612F8C4DC3BA18AF84C178E554D247FCFB0256051780', '02B4516776DED6075D88A02EFB9D1299BFE49155A8B6309020F100FA7C30F3443A', '02C8D6049A7133DACAFE58C2569BF16F67C6DEC262CC5F0A7955BC02D66AD58217'], 3: ['03C78B0EC004BBFB20427B2B8FFAA8E463A717DDE0686C82DE2ECB1A3D8BACE394', '033D050193E9692F6961BA53CF6B5A39ABA9E7A2EBA5E45D69D278608AF3FB165A', '038153AEC2B32FFE212964BA859D4CF21BCAD9591799C115BA5FB7CAA537334714'], 4: ['0242C28610306912AB62AE042CCB312CAA502F127650585EBC0B730DBD3BA8DE8B', '02DE78A9725D3B1171B94D233573C6C6C74073A976462B5A0B06A582D24FCC08BD', '02D6AE43F38F633743A4BBFD5B4E949DB340E24771BDE87B5C8303AE8014635030'], 5: ['03B284BBCDA0964FDD6AE4DEF8064B507E98401B5291791D6C0D346AB294C7B4EF', '022AB71950B6E918401773FEA70E70A7D7C040A6E7F193375B216D1AC45A7FA3BF', '0206EE3E796A08B2F0507726F2D8044737E87EEDFAB0B113E4753E7F4D22E3C314'], 6: ['029635F56EA0D81D19B87DB9A0E9A7CA34000641DEB99BE584B816D10C13A53ECB', '03B10F83843BE9E7E7A587DD0B7152BDCDC59D44E1BD6BE3326510E1653C902F0C', '0226780E1243F988A08F573AD693ED6F2AC65046E0E498E076A54A0659409D47E5']}

    #negative testing to verify the verification of honesty fails when the data is tampered
    for f in players:
        for fx in players:
            if f != fx:
                fxValue = fx.getEvals(groupid,f.groups[groupid].ordinal)
                assert fxValue is not None, "Test Failed as the getEvals didnt return the correct number of evals"
                f.allEvalsReceived(groupid, f.groups[groupid].ordinal, fx.groups[groupid].ordinal, fxValue)
                sumOfEvalsNeg = f.groups[groupid].transientData.sumOfEvals
        try:
            groupId, resultNeg = f.createSecret(groupid, 'LITTLEK', collatedHiddenPolys, collatedHiddenEvals)
        except:
            resultNeg = None

        assert resultNeg is None, " Negative Test Failed , while testign tampered against verifation of honesty"


# Verify createSecret for caltype - alpha
def test_CreateSecretAlpha():
    orc = orchestrator.Orchestrator()

    noOfPlayers  = 6
    m = 3 #recombination number
    n = 6 # total number
    degree = m - 1 # recombination number - 1

    playersList= []
    remplayersList = []
#
    # register players with Orchestrator
    for num in range(noOfPlayers):
        orc.register('player' + str(num), 'ref' + str(num))
        playersList.append('player'+str(num))

    #create group
    groupid, invites = orc.createGroup(playersList[0], m, n)

    #remaining list
    for p in playersList:
        if p != playersList[0]:
            remplayersList.append(p)

    # accept invite
    for user in remplayersList:
        orc.acceptInvite(user, groupid, True)

    #get the users who belong to the groupid
    participantList = orc.getParticipants(groupid)

    #add the player list
    players=[]
    for num in participantList:
        players.append(player.Player(myPrintFunction))


    #add player to the group
    for num,participant in enumerate(participantList):
        ord = 1
        myord = 1
        ordlist=[]
        for num1,participant1 in enumerate(participantList):
            if participant == participant1:
                myord = ord
                ord+=1
            else:
                ordlist.append((ord,' '))
                ord+=1
        players[num].addGroup(groupid,myord,ordlist,degree)

    # request data for PRIVATEKEYSHARE
    for num,pl in enumerate(players):
        groupId, ordinal, hiddenPoly, hiddenEvals = pl.requestData(groupid, 'ALPHA')

        assert groupId is not None, "Test Failed, requestData definition fails to return groupid for ALPHA calculation"
        assert groupId == pl.groups[groupid].id, "Test Failed, requestData definition fails to return correct groupid for ALPHA calculation"
        assert ordinal is not None, "Test Failed, requestData definition fails to return ordinal for ALPHA calculation"
        assert ordinal == pl.groups[groupid].ordinal, " Test Failed, equestData definition fails to return correct ordinal for ALPHA calculation"
        assert hiddenPoly is not None and len(hiddenPoly) == m, "Test Failed, requestData definition fails to return hiddenPoly for ALPHA calculation"
        assert hiddenEvals is not None and len(hiddenEvals) == len(players), "Test Failed, requestData definition fails to return hiddenEvals for ALPHA calculation"

        #checking if the elemens in the list hiddenPoly ends with 02/03
        for poly in hiddenPoly:
            poly = str(poly)
            assert poly.startswith('02') or poly.startswith('03'), " Test Failed , incorrect hiddenPolynomial format"
            assert len(poly) == 66, " Test Failed incorrect hiddenPolynomails value"

        #checking if the elemens in the list hiddenPoly ends with 02/03
        for key,eval in hiddenEvals.items():
            eval = str(eval)
            assert eval.startswith('02') or eval.startswith('03'), " Test Failed , incorrect hiddenPolynomial format"
            assert len(eval) == 66, " Test Failed incorrect hiddenEval value "

        #collate data of every player
        collatedData = orc.collateData(groupid, pl.groups[groupid].ordinal, hiddenPoly, hiddenEvals)

        if (num == len(participantList)-1):
            assert collatedData is True, "Test Failed" # collatedData should return True once it is called by all the participants
        else:
            assert collatedData is False, "Test Failed" # allEvalsReceived should return False until it is called by all the participants

    #get collated data
    collatedHiddenPolys, collatedHiddenEvals = orc.getCollatedData(groupid)

    #assertion on len of collatedHiddenEvals
    for key, values in collatedHiddenEvals.items():
        assert len(values) == len(participantList), "Test Failed"

    # assertion on len of collatedHiddenPolys
    for key, values in collatedHiddenPolys.items():
        assert len(values) == degree+1, "Test Failed"

    for f in players:
        for fx in players:
            if f != fx:
                fxValue = fx.getEvals(groupid,f.groups[groupid].ordinal)
                assert fxValue is not None, "Test Failed as the getEvals didnt return the correct number of evals"

                f.allEvalsReceived(groupid, f.groups[groupid].ordinal, fx.groups[groupid].ordinal, fxValue)
                sumOfEvals = f.groups[groupid].transientData.sumOfEvals
        groupId, result = f.createSecret(groupid, 'ALPHA', collatedHiddenPolys, collatedHiddenEvals)
        assert groupId is not None, "Test Failed, createSecret definition fails to return groupid for ALPHA calculation"
        assert result is not None, "Test Failed , createSecret definition fails to return result for ALPHA calculation"
        assert result == sumOfEvals, "Test Failed , createSecret definition fails to return result for ALPHA calculation"

    assert len(f.groups[groupid].transientData.f_x) == 64, " Incorrect length of f_x value"
    assert len(f.groups[groupid].transientData.allHiddenPolynomials) is not 0, " AllHiddenPolynomials attribute value missing"
    assert len(f.groups[groupid].transientData.allHiddenPolynomials) == len(participantList), " AllHiddenPolynomials attribute value missing"
    assert f.groups[groupid].publicKeyShare is None, "Test Failed"
    assert f.groups[groupid].privateKeyShare is None, "Test Failed"
    assert f.groups[groupid].alpha is not None and len(str(f.groups[groupid].alpha)) == 64, "Test Failed"
    assert f.groups[groupid].alpha == sumOfEvals, "Test Failed"
    assert f.groups[groupid].littleK is None, "Test Failed"


# Verification of honesty test
def test_VerificationOfHonesty():
    orc = orchestrator.Orchestrator()

    noOfPlayers  = 6
    m = 3 #recombination number
    n = 6 # total number
    degree = m - 1 # recombination number - 1

    playersList= []
    remplayersList = []
#
    # register players with Orchestrator
    for num in range(noOfPlayers):
        orc.register('player' + str(num), 'ref' + str(num))
        playersList.append('player'+str(num))

    #create group
    groupid, invites = orc.createGroup(playersList[0], m, n)

    #remaining list
    for p in playersList:
        if p != playersList[0]:
            remplayersList.append(p)

    # accept invite
    for user in remplayersList:
        orc.acceptInvite(user, groupid, True)

    #get the users who belong to the groupid
    participantList = orc.getParticipants(groupid)

    #add the player list
    players=[]
    for num in participantList:
        players.append(player.Player(myPrintFunction))

    #add player to the group
    for num,participant in enumerate(participantList):
        ord = 1
        myord = 1
        ordlist=[]
        for num1,participant1 in enumerate(participantList):
            if participant == participant1:
                myord = ord
                ord+=1
            else:
                ordlist.append((ord,' '))
                ord+=1
        players[num].addGroup(groupid,myord,ordlist,degree)

    # request data for PRIVATEKEYSHARE
    for num,pl in enumerate(players):
        groupId, ordinal, hiddenPoly, hiddenEvals = pl.requestData(groupid, 'LITTLEK')

        #checking if the elemens in the list hiddenPoly ends with 02/03
        for poly in hiddenPoly:
            poly = str(poly)
            assert poly.startswith('02') or poly.startswith('03'), " Test Failed , incorrect hiddenPolynomial format"

        #checking if the elemens in the list hiddenPoly ends with 02/03
        for key,eval in hiddenEvals.items():
            eval = str(eval)
            assert eval.startswith('02') or eval.startswith('03'), " Test Failed , incorrect hiddenPolynomial format"

        #collate data of every player
        collatedData = orc.collateData(groupid, pl.groups[groupid].ordinal, hiddenPoly, hiddenEvals)

    #get collated data
    collatedHiddenPolys, collatedHiddenEvals = orc.getCollatedData(groupid)

    #tamper collateddata before calling createsecret
    collatedHiddenPolys = {1: ['037159B7B3F8F80593C5E3E48DBBCCBF6251F100BFEC5157E605C30909DBDDE823', '037159B7B3F8F80593C5E3E48DBBCCBF6251F100BFEC5157E605C30909DBDDE823', '02E254877BE36D146223A1B70C45956F2D1EA2A2D14E4CD99E23432CFDDE24D9E1'], 2: ['02A232640597182320E92F612F8C4DC3BA18AF84C178E554D247FCFB0256051780', '02B4516776DED6075D88A02EFB9D1299BFE49155A8B6309020F100FA7C30F3443A', '02C8D6049A7133DACAFE58C2569BF16F67C6DEC262CC5F0A7955BC02D66AD58217'], 3: ['03C78B0EC004BBFB20427B2B8FFAA8E463A717DDE0686C82DE2ECB1A3D8BACE394', '033D050193E9692F6961BA53CF6B5A39ABA9E7A2EBA5E45D69D278608AF3FB165A', '038153AEC2B32FFE212964BA859D4CF21BCAD9591799C115BA5FB7CAA537334714'], 4: ['0242C28610306912AB62AE042CCB312CAA502F127650585EBC0B730DBD3BA8DE8B', '02DE78A9725D3B1171B94D233573C6C6C74073A976462B5A0B06A582D24FCC08BD', '02D6AE43F38F633743A4BBFD5B4E949DB340E24771BDE87B5C8303AE8014635030'], 5: ['03B284BBCDA0964FDD6AE4DEF8064B507E98401B5291791D6C0D346AB294C7B4EF', '022AB71950B6E918401773FEA70E70A7D7C040A6E7F193375B216D1AC45A7FA3BF', '0206EE3E796A08B2F0507726F2D8044737E87EEDFAB0B113E4753E7F4D22E3C314'], 6: ['029635F56EA0D81D19B87DB9A0E9A7CA34000641DEB99BE584B816D10C13A53ECB', '03B10F83843BE9E7E7A587DD0B7152BDCDC59D44E1BD6BE3326510E1653C902F0C', '0226780E1243F988A08F573AD693ED6F2AC65046E0E498E076A54A0659409D47E5']}

    #negative testing to verify the verification of honesty fails when the data is tampered
    for f in players:
        for fx in players:
            if f != fx:
                fxValue = fx.getEvals(groupid,f.groups[groupid].ordinal)
                assert fxValue is not None, "Test Failed as the getEvals didnt return the correct number of evals"
                f.allEvalsReceived(groupid, f.groups[groupid].ordinal, fx.groups[groupid].ordinal, fxValue)
                sumOfEvalsNeg = f.groups[groupid].transientData.sumOfEvals
        try:
            groupId, resultNeg = f.createSecret(groupid, 'LITTLEK', collatedHiddenPolys, collatedHiddenEvals)
        except:
            resultNeg = None

        assert resultNeg is None, " Negative Test Failed , while testing tampered data against verification of honesty"


#Verify calculateShareOfVW
def test_calculateShareOfVW():
    orc = orchestrator.Orchestrator()
    noOfPlayers = 6
    m = 3  # recombination number
    n = 6  # total number
    degree = m - 1  # recombination number - 1

    playersList = []
    remplayersList = []

    # register players with Orchestrator
    for num in range(noOfPlayers):
        orc.register('player' + str(num), 'ref' + str(num))
        playersList.append('player'+str(num))

    #create group
    groupid, invites = orc.createGroup(playersList[0], m, n)

    #remaining list
    for p in playersList:
        if p != playersList[0]:
            remplayersList.append(p)

    # accept invite
    for user in remplayersList:
        orc.acceptInvite(user, groupid, True)

    # get the users who belong to the groupid
    participantList = orc.getParticipants(groupid)

    # add the player list
    players = []
    for num in participantList:
        players.append(player.Player(myPrintFunction))

    # add player to the group
    for num, participant in enumerate(participantList):
        ord = 1
        myord = 1
        ordlist = []
        for num1, participant1 in enumerate(participantList):
            if participant == participant1:
                myord = ord
                ord += 1
            else:
                ordlist.append((ord, ' '))
                ord += 1
        players[num].addGroup(groupid, myord, ordlist, degree)

    # request data for ALPHA
    for pl in players:
        groupId, ordinal, hiddenPoly, hiddenEvals = pl.requestData(groupid, 'ALPHA')

        # collate data
        orc.collateData(groupid, pl.groups[groupid].ordinal, hiddenPoly, hiddenEvals)

    collatedHiddenPolys, collatedHiddenEvals = orc.getCollatedData(groupid)

    # createseceret with calType = 'ALPHA'
    for f in players:
        for fx in players:
            if f != fx:
                fxValue = fx.getEvals(groupid,f.groups[groupid].ordinal)
                f.allEvalsReceived(groupid, f.groups[groupid].ordinal, fx.groups[groupid].ordinal, fxValue)
                sumOfEvals = f.groups[groupid].transientData.sumOfEvals
        groupId, result = f.createSecret(groupid, 'ALPHA', collatedHiddenPolys, collatedHiddenEvals)

    # request data for LITTLEK
    for pl in players:
        groupId, ordinal, hiddenPoly, hiddenEvals = pl.requestData(groupid, 'LITTLEK')

        # collate data
        orc.collateData(groupid, pl.groups[groupid].ordinal, hiddenPoly, hiddenEvals)
    collatedHiddenPolys, collatedHiddenEvals = orc.getCollatedData(groupid)

    # createseceret with calType = 'LITTLEK'
    for f in players:
        for fx in players:
            if f != fx:
                fxValue = fx.getEvals(groupid,f.groups[groupid].ordinal)
                f.allEvalsReceived(groupid, f.groups[groupid].ordinal, fx.groups[groupid].ordinal, fxValue)
                sumOfEvals = f.groups[groupid].transientData.sumOfEvals
        groupId, result = f.createSecret(groupid, 'LITTLEK', collatedHiddenPolys, collatedHiddenEvals)

    #cal calculateShareOfVW
    for num,f in enumerate(players):
        res = f.getVWshares(groupid)
        playerOrdinal = res[0]
        playerVWshares = res[1]

        assert playerOrdinal == f.groups[groupid].ordinal, "Test Failed"
        assert len(playerVWshares) is 2, "Test Failed"

        for n,value in enumerate(playerVWshares):
            if n == 0:
                assert len(value) == 64, " Test Failed"
            elif n == 1:
                assert len(value) == 66, " Test Failed"

        #collaeVW Data
        orc.collateVWData(groupid, f.groups[groupid].ordinal, playerVWshares)

    collatedVWData = orc.getCollatedVWData(groupid)
    assert len(collatedVWData) == len(participantList), "Test Failed"

    for num,f in enumerate(players):
        f.setSharedVWData(groupid,collatedVWData)

        if(num == 0):
            rPoints = f.groups[groupid].ephemeralKeyList[0][1] #accesing the second element in the ephemeralkeylist which should remain same among all other players
            littleK = f.groups[groupid].ephemeralKeyList[0][0] # accessing the second element in the ephemeralKeyList which should de different for all the players
        else:
            assert f.groups[groupid].ephemeralKeyList[0][1] == rPoints #assert if the second element of ephemeralkeylist oare same thought all players
            assert f.groups[groupid].ephemeralKeyList[0][1] != littleK

        assert len(f.groups[groupid].transientData.allVWshares) == len(participantList), "Test Failed"
        assert len(f.groups[groupid].ephemeralKeyList[0]) == 2, "Test Failed"
        assert len(str(f.groups[groupid].ephemeralKeyList[0][0])) == 64
        assert len(str(f.groups[groupid].ephemeralKeyList[0][1])) == 64


# #test signature data
def test_SignatureData():
    orc = orchestrator.Orchestrator()
    noOfPlayers = 3
    m = 2  # recombination number
    n = 3  # total number
    degree = m - 1  # recombination number - 1

    playersList = []
    remplayersList = []

    # register players with Orchestrator
    for num in range(noOfPlayers):
        orc.register('player' + str(num), 'ref' + str(num))
        playersList.append('player' + str(num))

    # create group
    groupid, invites = orc.createGroup(playersList[0], m, n)

    #remaining list
    for p in playersList:
        if p != playersList[0]:
            remplayersList.append(p)

    # accept invite
    for user in remplayersList:
        orc.acceptInvite(user, groupid, True)

    # get the users who belong to the groupid
    participantList = orc.getParticipants(groupid)

    # add the player list
    players = []
    for num in participantList:
        players.append(player.Player(myPrintFunction))

    # add player to the group
    for num, participant in enumerate(participantList):
        ord = 1
        myord = 1
        ordlist = []
        for num1, participant1 in enumerate(participantList):
            if participant == participant1:
                myord = ord
                ord += 1
            else:
                ordlist.append((ord, ' '))
                ord += 1
        players[num].addGroup(groupid, myord, ordlist, degree)

    # request data for PRIVATEKEYSHARE
    for num,pl in enumerate(players):
        groupId, ordinal, hiddenPoly, hiddenEvals = pl.requestData(groupid, 'PRIVATEKEYSHARE')

        #collate data of every player
        collatedData = orc.collateData(groupid, pl.groups[groupid].ordinal, hiddenPoly, hiddenEvals)

    #get collated data
    collatedHiddenPolys, collatedHiddenEvals = orc.getCollatedData(groupid)

    #assertion on len of collatedHiddenEvals
    for key, values in collatedHiddenEvals.items():
        assert len(values) == len(participantList), "Test Failed"

    # assertion on len of collatedHiddenPolys
    for key, values in collatedHiddenPolys.items():
        assert len(values) == degree+1, "Test Failed"

    for f in players:
        for fx in players:
            if f != fx:
                fxValue = fx.getEvals(groupid,f.groups[groupid].ordinal)
                f.allEvalsReceived(groupid, f.groups[groupid].ordinal, fx.groups[groupid].ordinal, fxValue)
        groupId, result = f.createSecret(groupid, 'PRIVATEKEYSHARE', collatedHiddenPolys, collatedHiddenEvals)

    #request data for ALPHA
    for num, pl in enumerate(players):
        groupId, ordinal, hiddenPoly, hiddenEvals = pl.requestData(groupid, 'ALPHA')

        # collate data
        collatedData = orc.collateData(groupid, pl.groups[groupid].ordinal, hiddenPoly, hiddenEvals)

        if (num == len(participantList)-1):
            assert collatedData is True, "Test Failed" # collatedData should return True once it is called by all the participants
        else:
            assert collatedData is False, "Test Failed" # allEvalsReceived should return False until it is called by all the participants

    collatedHiddenPolys, collatedHiddenEvals = orc.getCollatedData(groupid)

    #assertion on len of collatedHiddenEvals
    for key, values in collatedHiddenEvals.items():
        assert len(values) == len(participantList), "Test Failed"

    # assertion on len of collatedHiddenPolys
    for key, values in collatedHiddenPolys.items():
        assert len(values) == degree+1, "Test Failed"

    # createseceret with calType = 'ALPHA'
    for f in players:
        for fx in players:
            if f != fx:
                fxValue = fx.getEvals(groupid,f.groups[groupid].ordinal)
                f.allEvalsReceived(groupid, f.groups[groupid].ordinal, fx.groups[groupid].ordinal, fxValue)
        groupId, result = f.createSecret(groupid, 'ALPHA', collatedHiddenPolys, collatedHiddenEvals)

    # request data for LITTLEK
    for num, pl in enumerate(players):
        groupId, ordinal, hiddenPoly, hiddenEvals = pl.requestData(groupid, 'LITTLEK')

        # collate data
        collatedData = orc.collateData(groupid, pl.groups[groupid].ordinal, hiddenPoly, hiddenEvals)

        if (num == len(participantList)-1):
            assert collatedData is True, "Test Failed" # collatedData should return True once it is called by all the participants
        else:
            assert collatedData is False, "Test Failed" # allEvalsReceived should return False until it is called by all the participants

    collatedHiddenPolys, collatedHiddenEvals = orc.getCollatedData(groupid)

    # assertion on len of collatedHiddenEvals
    for key, values in collatedHiddenEvals.items():
        assert len(values) == len(participantList), "Test Failed"

    # assertion on len of collatedHiddenPolys
    for key, values in collatedHiddenPolys.items():
        assert len(values) == degree+1, "Test Failed"

    # # createseceret with calType = 'LITTLEK'
    for f in players:
        for fx in players:
            if f != fx:
                fxValue = fx.getEvals(groupid, f.groups[groupid].ordinal)
                f.allEvalsReceived(groupid, f.groups[groupid].ordinal, fx.groups[groupid].ordinal, fxValue)
        groupId, result = f.createSecret(groupid, 'LITTLEK', collatedHiddenPolys, collatedHiddenEvals)

    # cal calculateShareOfVW
    for num, f in enumerate(players):
        res = f.getVWshares(groupid)

        playerOrdinal = res[0]
        playerVWshares = res[1] # get VWshares for every player

        assert playerOrdinal != 0, " Test Failed, getVWshares fails to return ordinal player"
        assert len(playerVWshares) == 2, "Test Failed"

        orc.collateVWData(groupid, f.groups[groupid].ordinal, playerVWshares)

    collatedVWData = orc.getCollatedVWData(groupid)
    assert len(collatedVWData) == len(participantList)

    # setVWSharedData
    for f in players:
        f.setSharedVWData(groupid, collatedVWData)

    messageData = 'SDK Testing Message'
    messageDataNegativeTest = 'SDK Testing Message.......'

    # set sigining Initiaor to True before requesting Signture Data
    # set only to one player since we are trying to generate signature from only one player
    players[0].setSigningInitiator(groupid)

    # requestSignatureData
    # negative test by setting index = -2 the test should fail due to index out of range error
    for f in players:

        # requestSignatureData
        index = -2
        sValueExpected = None

        try:
            groupId, ordinal, sValueActual, message = f.requestSignatureData(groupid, index, f.hashMessage(messageData))
        except:
            # its verified the index error is displayed when index = -2
            sValueActual = None

        assert sValueActual == sValueExpected, " Test Failed"
        assert len(f.groups[groupid].ephemeralKeyList[0]) == 2, " Test failed"

    # requestSignatureData
    # negative test by setting index = 1 the test should fail
    for f in players:

        # requestSignatureData
        index = 1
        sValueExpected = None
        if index != 0:
            sValueExpected = ''

        groupId, ordinal, sValueActual, message = f.requestSignatureData(groupid, index, f.hashMessage(messageData))

        #verified that it displays msgs Choose a key in the range 0 ..0
        assert sValueActual == sValueExpected, " Test Failed"
        assert len(f.groups[groupid].ephemeralKeyList[0]) == 2, " Test failed"

    #requestSignatureData
    # positive test by setting index = 0 the test should pass.
    for f in players:

        # requestSignatureData
        index = 0 #use this index since we need to pop out first element from ephemeral list # happy path test
        littleK = f.groups[groupid].littleK
        list = f.groups[groupid].ephemeralKeyList
        r_bn = list[0][1]
        pks = f.groups[groupid].privateKeyShare

        # message needs to be in BigNum format
        Hm = Nakasendo.BigNum(hashMessage(messageData), player.Player.modulo)

        sValueExpected = littleK * (Hm + (pks * r_bn))

        groupId, ordinal, sValueActual, message = f.requestSignatureData(groupid,index,f.hashMessage(messageData))

        assert groupId == groupid, " Test Failed, requestSignatureData def fails to return groupid"
        assert ordinal == f.groups[groupid].ordinal, " Test Failed, requestSignatureData def fails to return ordinal"
        assert sValueExpected is not None, " Test Failed, requestSignatureData def fails to return signature value"
        assert str(sValueActual) == str(sValueExpected), " Test Failed, requestSignatureData def fails to return correct signature value"
        assert message == f.hashMessage(messageData), " Test Failed, requestSignatureData def fails to return groupid"

        assert len(f.groups[groupid].ephemeralKeyList) == 0, " Test failed"

        # collate signature data
        orc.signature(groupid,f.groups[groupid].ordinal,sValueActual)

    # requestSignatureData
    # negative test by passing empty ephemeral key list
    for f in players:

        # requestSignatureData
        index = 0  # use this index since we need to pop out first element from ephemeral list # happy path test
        sValueExpected = ''

        assert len(f.groups[groupid].ephemeralKeyList) == 0, " Test failed"

        groupId, ordinal, sValueActual, message = f.requestSignatureData(groupid, index, f.hashMessage(messageData))

        assert groupId == groupid, " Test Failed, requestSignatureData def fails to return groupid"
        assert ordinal == f.groups[
            groupid].ordinal, " Test Failed, requestSignatureData def fails to return ordinal"
        assert sValueExpected is not None, " Test Failed, requestSignatureData def fails to return signature value"
        assert str(sValueActual) == str(
            sValueExpected), " Test Failed, requestSignatureData should return zero due to empty ephemeralkeylist"
        assert message == f.hashMessage(
            messageData), " Test Failed, requestSignatureData def fails to return groupid"

    #get the collated signature for all the players
    collatedSignatures = orc.getSignatureData(groupid)
    assert len(collatedSignatures) == len(participantList), "Test Failed" # length of collated signature should be equal to the recombination number (m)

    mysignatureData = players[0].sign(groupid, messageData, collatedSignatures)

    assert len(mysignatureData) == 2, " Test Failed as sign definition fails to return correct signatureData"

    sigRStr = str(mysignatureData[0])
    sigSStr = str(mysignatureData[1])

    publicKeyStr = str(f.getPublicKeyShare(groupid))
    assert len(publicKeyStr) == 66, "Test Failed"

    #Verify the calculated signature verified with calculated PEM & call to verify
    # create an ECPoint
    pt = Nakasendo.ECPoint()
    pt.value = str(players[0].groups[groupid].publicKeyShare)
    coords = pt.GetAffineCoOrdinates()
    pempubkey = Nakasendo.pubKeyHexPtasPem(coords[0], coords[1])

    #Positive Test pass the same messageData which is used to reguestSignatureData
    valPem = Nakasendo.verify(messageData, pempubkey, sigRStr, sigSStr)
    assert valPem == 1, " Test Failed to verify with calculated PEM "

    #Negatve Test pass messageData different from the one used to reguestSignatureData
    valPem = Nakasendo.verify(messageDataNegativeTest, pempubkey, sigRStr, sigSStr)
    assert valPem == 0, " Test Failed to verify with calculated PEM "

    # positive Test verify the signature with the same messageData which was used to generatesignature
    val = Nakasendo.verifyPubKeyHex(messageData, publicKeyStr,sigRStr, sigSStr)
    assert val == 1 , " Test failed verification of signature failed"

    # negative Test verify the signature with different messageDataNegativeTest  and see if the verify fails
    val = Nakasendo.verifyPubKeyHex(messageDataNegativeTest, publicKeyStr, sigRStr, sigSStr)
    assert val == 0 , " Test failed verification of signature failed"

    #Verify the calculated threshold signature via ECDSA library
    #positive test use the same messageData used in getSignatureData
    hmMessage = Nakasendo.hash256(message=messageData,
                                  modulo="FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141")
    # create an ecdsa public key from the program parameter
    ecdsaPubKeyPoint = ecdsa.ellipticcurve.Point(ecdsa.SECP256k1.generator.curve(), int(coords[0], 16),
                                                 int(coords[1], 16))
    pubkeytestB = ecdsa.ecdsa.Public_key(G, ecdsaPubKeyPoint)

    thresHoldSigLocalCal = ecdsa.ecdsa.Signature(int(sigRStr, 16), int(sigSStr, 16))

    assert (pubkeytestB.verifies(int(hmMessage.value, 16), thresHoldSigLocalCal)), " Test Failed to verify threshold signature via ECDSA library"

    # Negative test use the messageData different from the one used in getSignatureData
    hmMessage = Nakasendo.hash256(message=messageDataNegativeTest,
                                  modulo="FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141")
    # create an ecdsa public key from the program parameter
    ecdsaPubKeyPoint = ecdsa.ellipticcurve.Point(ecdsa.SECP256k1.generator.curve(), int(coords[0], 16),
                                                 int(coords[1], 16))
    pubkeytestB = ecdsa.ecdsa.Public_key(G, ecdsaPubKeyPoint)

    thresHoldSigLocalCal = ecdsa.ecdsa.Signature(int(sigRStr, 16), int(sigSStr, 16))

    assert (pubkeytestB.verifies(int(hmMessage.value, 16),
                                 thresHoldSigLocalCal)) == 0 , " Test Failed to verify threshold signature via ECDSA library"


# # #test signature data
def test_SignatureDataNegativeScenario():
    orc = orchestrator.Orchestrator()
    noOfPlayers = 3
    m = 2  # recombination number
    n = 3  # total number
    degree = m - 1  # recombination number - 1

    playersList = []
    remplayersList = []

    # register players with Orchestrator
    for num in range(noOfPlayers):
        orc.register('player' + str(num), 'ref' + str(num))
        playersList.append('player' + str(num))

    # create group
    groupid, invites = orc.createGroup(playersList[0], m, n)

    #remaining list
    for p in playersList:
        if p != playersList[0]:
            remplayersList.append(p)

    # accept invite
    for user in remplayersList:
        orc.acceptInvite(user, groupid, True)

    # get the users who belong to the groupid
    participantList = orc.getParticipants(groupid)

    # add the player list
    players = []
    for num in participantList:
        players.append(player.Player(myPrintFunction))

    # add player to the group
    for num, participant in enumerate(participantList):
        ord = 1
        myord = 1
        ordlist = []
        for num1, participant1 in enumerate(participantList):
            if participant == participant1:
                myord = ord
                ord += 1
            else:
                ordlist.append((ord, ' '))
                ord += 1
        players[num].addGroup(groupid, myord, ordlist, degree)

    # request data for PRIVATEKEYSHARE
    for num,pl in enumerate(players):
        groupId, ordinal, hiddenPoly, hiddenEvals = pl.requestData(groupid, 'PRIVATEKEYSHARE')

        #collate data of every player
        collatedData = orc.collateData(groupid, pl.groups[groupid].ordinal, hiddenPoly, hiddenEvals)

    #get collated data
    collatedHiddenPolys, collatedHiddenEvals = orc.getCollatedData(groupid)

    #assertion on len of collatedHiddenEvals
    for key, values in collatedHiddenEvals.items():
        assert len(values) == len(participantList), "Test Failed"

    # assertion on len of collatedHiddenPolys
    for key, values in collatedHiddenPolys.items():
        assert len(values) == degree+1, "Test Failed"

    for f in players:
        for fx in players:
            if f != fx:
                fxValue = fx.getEvals(groupid,f.groups[groupid].ordinal)
                f.allEvalsReceived(groupid, f.groups[groupid].ordinal, fx.groups[groupid].ordinal, fxValue)
        groupId, result = f.createSecret(groupid, 'PRIVATEKEYSHARE', collatedHiddenPolys, collatedHiddenEvals)

    #request data for ALPHA
    for num, pl in enumerate(players):
        groupId, ordinal, hiddenPoly, hiddenEvals = pl.requestData(groupid, 'ALPHA')

        # collate data
        collatedData = orc.collateData(groupid, pl.groups[groupid].ordinal, hiddenPoly, hiddenEvals)

        if (num == len(participantList)-1):
            assert collatedData is True, "Test Failed" # collatedData should return True once it is called by all the participants
        else:
            assert collatedData is False, "Test Failed" # allEvalsReceived should return False until it is called by all the participants

    collatedHiddenPolys, collatedHiddenEvals = orc.getCollatedData(groupid)

    #assertion on len of collatedHiddenEvals
    for key, values in collatedHiddenEvals.items():
        assert len(values) == len(participantList), "Test Failed"

    # assertion on len of collatedHiddenPolys
    for key, values in collatedHiddenPolys.items():
        assert len(values) == degree+1, "Test Failed"

    # createseceret with calType = 'ALPHA'
    for f in players:
        for fx in players:
            if f != fx:
                fxValue = fx.getEvals(groupid,f.groups[groupid].ordinal)
                assert fxValue is not None, "Test Failed as the getEvals didnt return the correct number of evals"
                f.allEvalsReceived(groupid, f.groups[groupid].ordinal, fx.groups[groupid].ordinal, fxValue)
        groupId, result = f.createSecret(groupid, 'ALPHA', collatedHiddenPolys, collatedHiddenEvals)

    # request data for LITTLEK
    for num, pl in enumerate(players):
        groupId, ordinal, hiddenPoly, hiddenEvals = pl.requestData(groupid, 'LITTLEK')

        # collate data
        collatedData = orc.collateData(groupid, pl.groups[groupid].ordinal, hiddenPoly, hiddenEvals)

        if (num == len(participantList)-1):
            assert collatedData is True, "Test Failed" # collatedData should return True once it is called by all the participants
        else:
            assert collatedData is False, "Test Failed" # allEvalsReceived should return False until it is called by all the participants

    collatedHiddenPolys, collatedHiddenEvals = orc.getCollatedData(groupid)

    # assertion on len of collatedHiddenEvals
    for key, values in collatedHiddenEvals.items():
        assert len(values) == len(participantList), "Test Failed"

    # assertion on len of collatedHiddenPolys
    for key, values in collatedHiddenPolys.items():
        assert len(values) == degree+1, "Test Failed"

    # # createseceret with calType = 'LITTLEK'
    for f in players:
        for fx in players:
            if f != fx:
                fxValue = fx.getEvals(groupid, f.groups[groupid].ordinal)
                f.allEvalsReceived(groupid, f.groups[groupid].ordinal, fx.groups[groupid].ordinal, fxValue)
        groupId, result = f.createSecret(groupid, 'LITTLEK', collatedHiddenPolys, collatedHiddenEvals)

    # cal calculateShareOfVW
    for num, f in enumerate(players):
        res = f.getVWshares(groupid)

        playerOrdinal = res[0]
        playerVWshares = res[1] # get VWshares for every player

        assert playerOrdinal != 0, " Test Failed, getVWshares fails to return ordinal player"
        assert len(playerVWshares) == 2, "Test Failed"

        orc.collateVWData(groupid, f.groups[groupid].ordinal, playerVWshares)

    collatedVWData = orc.getCollatedVWData(groupid)
    assert len(collatedVWData) == len(participantList)

    # setVWSharedData
    for f in players:
        f.setSharedVWData(groupid, collatedVWData)

    messageData = 'SDK Testing Message'

    # set sigining Initiaor to True before requesting Signture Data
    # set only to one player since we are trying to generate signature from only one player
    players[0].setSigningInitiator(groupid)

    #requestSignatureData
    for f in players:

        # requestSignatureData
        index = 0

        #tampering the ephemeralKeyList before requestignSignatureData
        f.groups[groupid].ephemeralKeyList[0][1] = f.groups[groupid].ephemeralKeyList[0][0]

        groupId, ordinal, sValueActual, message = f.requestSignatureData(groupid,index,f.hashMessage(messageData))

        assert len(f.groups[groupid].ephemeralKeyList) == 0, " Test failed"

        # collate signature data
        orc.signature(groupid,f.groups[groupid].ordinal,sValueActual)

    #get the collated signature for all the players
    collatedSignatures = orc.getSignatureData(groupid)
    assert len(collatedSignatures) == len(participantList), "Test Failed" # length of collated signature should be equal to the recombination number (m)


    mysignatureData = players[0].sign(groupid, messageData, collatedSignatures)

    assert len(mysignatureData) == 2, " Test Failed as sign definition fails to return correct signatureData"

    sigRStr = str(mysignatureData[0])
    sigSStr = str(mysignatureData[1])

    publicKeyStr = str(f.getPublicKeyShare(groupid))
    assert len(publicKeyStr) == 66, "Test Failed"

    #Verify the calculated signature verified with calculated PEM & call to verify
    # create an ECPoint
    pt = Nakasendo.ECPoint()
    pt.value = str(f.groups[groupid].publicKeyShare)
    coords = pt.GetAffineCoOrdinates()
    pempubkey = Nakasendo.pubKeyHexPtasPem(coords[0], coords[1])

    #Positive Test pass the same messageData which is used to reguestSignatureData
    valPem = Nakasendo.verify(messageData, pempubkey, sigRStr, sigSStr)
    assert valPem == 0, " Negative Test failed for signature verification "

    # positive Test verify the signature with the same messageData which was used to generatesignature
    val = Nakasendo.verifyPubKeyHex(messageData, publicKeyStr,sigRStr, sigSStr)
    assert val == 0, " Negative Test failed for signature verification "

    #Verify the calculated threshold signature via ECDSA library
    #positive test use the same messageData used in getSignatureData
    hmMessage = Nakasendo.hash256(message=messageData,
                                  modulo="FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141")
    # create an ecdsa public key from the program parameter
    ecdsaPubKeyPoint = ecdsa.ellipticcurve.Point(ecdsa.SECP256k1.generator.curve(), int(coords[0], 16),
                                                 int(coords[1], 16))
    pubkeytestB = ecdsa.ecdsa.Public_key(G, ecdsaPubKeyPoint)

    thresHoldSigLocalCal = ecdsa.ecdsa.Signature(int(sigRStr, 16), int(sigSStr, 16))

    assert (pubkeytestB.verifies(int(hmMessage.value, 16), thresHoldSigLocalCal)) == False, " Negative Test Failed to verify threshold signature via ECDSA library"


#Verify same signarture r and s values are generated when the polynomials are fixed
def test_FixedPolynomial():
    orc = orchestrator.Orchestrator()
    noOfPlayers = 3
    m = 2  # recombination number
    n = 3  # total number
    degree = m - 1  # recombination number - 1
    mod = player.Player.modulo

    # private_poly
    private_poly = { \
        1: ['6202FC74B84176AE6BAF0B1453AF99C3D59C33F88D4E656C7F84207200D8D4C4',
            '2A186451C4062FF810E4B4E1DD2F600B8DADFC40E3A510125B25DEB413F3B351'], \
        2: ['7DC9FEE24641D94EDC4B65E5E77975BE17ED9AACED7AFE2E3283E31538349A92',
            'D69EBE7C2BA98D0773BA7384EAEAF93951EEA22BD798E5BDD5897E3692BAA4F4'], \
        3: ['3C2DFA6A646DB6D6FFD71B1158240786981EE4B57A7E1AA6616456A8F060A1C3',
            '355BF8CE7BD16666F232C4AC2DD3DFDDD07ED06CD4E885B7A4BD0D7B5EB2773C'] \
        }

    # ALPHA
    alpha_poly = { \
        1: ['FEF65C573F47FDEDAE80218B61E42865F0A7674E7C5E0FBE8E7DCBA3BDA1FD64',
            '9EE5C7BCB2E249A34076B80E13CFB8E5F6C5C5FBA0C7F40D5D93AB9A55DCB90B'], \
        2: ['B88475BC2C3D1F3C7F5401AF2BDA88B520A50ADA1966EEC82E7FB67AA4EF8030',
            '0E6AC1D7B57D7443DCA8D53F1881FA3F34B99557C0435343F8D1528659A40816'], \
        3: ['806C77274B4C157CC17D78E0B60A8736868CA692DE82BEB49088020E2D7BAE27',
            '78BBCFF675203C2237C24FADCF8C3D60C6FFAEFD3B7EC0A5D87A3FE7FE45DD86']
    }

    # LITTLEK
    littlek_poly = { \
        1: ['48B3E0488D62D44386BADE3E7F1CBD65DC9F59307FF109C1323E867CBF31C9BB',
            '12209E01DD13E2CC9DC4ECBD7CB9FDEAD9392C6F314993C8482D0C1100A3DA98'], \
        2: ['0BFCA7C6537B95C95198E91DBB01A571CDB4B7DD6AEF963979E41368D676F39B',
            'B4AB93C74533C61C8B4C1E13BCF30864B9758853EA7D474A9033EA114F29024C'], \
        3: ['9F52C8AE1372C49530E7B46393F201663A3DE729EDA4FCDDF8754ED29F102D9C',
            '8CB522C222C136EF85A40BB6443074D03F57599FB24F77DDB4E6BF5FE192C925'] \
        }

    # pk = privateKeyShare.  i.e. this is the transient data produced in
    # calculating the privateKeyShare.  The JVRSS data structure is wiped and reused
    # for subsequent calculations of ALPHA and LITTLEK
    pk_f_x = { \
        1: '8C1B60C67C47A6A67C93BFF630DEF9CF634A303970F3757EDAA9FF2614CC8815', \
        2: '2B077BDA9D94F35DC3C04CEFBD4F6833466D25373E1B89325DF22268BD3D61F8', \
        2: 'DC41E4D5D7E1EA0BD66F6915E19FA720099B55FBF937ABCD4F9B7F1B0C780777' \
        }

    # f_x for the alpha calculation
    alpha_f_x = { \
        1: '9DDC2413F22A4790EEF6D99975B3E14D2CBE50636DDD63902C3F18B14348752E', \
        2: 'D559F96B973807C438A5AC2D5CDE7D338A18358999ED955020225B875837905C', \
        2: 'EA9FE70AAAACC9E368C467EA24AF3F5A20DCD6A3E1B6606A5A24633958170578' \
        }

    # outer-dict is the player ordinal, i.e. player 1 : { dict }
    # inner-dict is ordinal used in the evaluation (f_x), i.e. ordinal : hide(f_x)
    pk_hidden_evals = { \
        1: { \
            1: '0330FF8B9F2FE6BA6CAE844247DC331A9C3EFD39E535F348BE25B0E699138CF500', \
            2: '03D1641B342781E739DD3CFDF4D8965EC220D5937E0CEB69F6DAFE08E2B7CF449A', \
            3: '020D777CAAB25A0CDA8EA1F04F98CBA72531D67FDEA229C44600B8038095D8343F' \
            }, \
        2: { \
            2: '0319E9B39C8FDA5253D9468F7BD4C384E3FD36C2AE0CE86008C5FFDCF975365645', \
            1: '02B0849C8FED8AC74F984C6B82ADA083972CE968BA606BB830491042ED6FE2B542', \
            3: '0295D4D90B05B348BF78FF6E48A6E0C126FE33D405837FACB05AB6B0B28DAB3CB2' \
            }, \
        3: { \
            3: '02FFB22E4CE375671C17286812B11ADCD21C99D8D270A755AB36901703A5228009', \
            1: '03E2AF390D8BCA17538C17F424AFB1B6A7217288F23FC8DE4DC03650B9AF326808', \
            2: '025EC8213DEC342C17C0855CBD90D8972EEDF60760E0C60990342D2543E9623E86' \
            } \
        }

    alpha_hidden_evals = { \
        1: { \
            1: '03B5475C7B9B68D691BC8A0D19458B73536B9BD2330D136684F86BAFB8610AE696', \
            2: '0347D1B9C6A7F79B579D51FC45DDADB49CE1E5F58C013305D39C4FD88C839D4A62', \
            3: '02252D8E62CE57F4DF4ABF768B0AF46D2813E2DC777F09ABDBE1BFAA2777BFC0FD' \
            }, \
        2: { \
            2: '02FE44EAA626250A178B088A515C847AE48AF3B88B9D7750ED82716733AE0CC608', \
            1: '03DEE8EE68D092DB37E3EA7EEDB8D72B34FA974E239E15646BAE7AECEF2A3C74C1', \
            3: '033EA4E68A841DCDD098D276188F1FBF9BA6ECA1371C12079A0ABD6C106F5CBD96' \
            }, \
        3: { \
            3: '02BA3953A8A661EC9ECA3A5FFB4C6BD817D70E0ADC1404DBD85ECF883F4D764645', \
            1: '03D3266F66BE2C89C57B3E1F8CBF1419C8BEBE07B008D5F94A0E772FF9F9C63960', \
            2: '02CB6F81916DCB1A1B2490C462F4CD488915B454BFCEC0131DCC50497075089F43' \
            } \
        }

    pk_hidden_poly = { \
        1: ['02B186C23A7C410E94E3FCF869621F00FE0CE4E9243E1B56B772B651B3E677D099',
            '02A187F015D166B32C2D896E3B7B6CEBC65D07E7E25FF8E3B3D6BF6716F4E62F07'], \
        2: ['0271BA0247025D55A4783E292FDEF68DEE9DAC9B0934C38FB68320FCBD312E32B0',
            '038B84A926F1568797DB6CD05DF90EEF67E65EC8B3B4269F588792E7013D04EDDE'], \
        3: ['03C54C1593485AD5F4EE77D6115D6CCC85754BA2CA60260E2991A19C5CADBCC94A',
            '03C074DB725B75F2E5FEF24B351A8A3D0F72FFD3DD3026A93655E2F0CCA20E358A'] \
        }

    alpha_hidden_poly = { \
        1: ['03AF5F7C45924749366A4911E59B8AAB6FEDF9819353DC9FFE045535814E31E41D',
            '02CB19C881FBE1EEB171392B999E25BC8EE5FA3EEE1AA2734793045B4188563477'], \
        2: ['032DA156AAF9024B12FC96C55194AFCED148A8FFA89F63D009B24BA5C0EF84451D',
            '038AE8052671BD5E1F397ABC32934FC2C527C65DF82E010140667ED1C3FF94D640'], \
        3: ['0221C38C2BA9FA3574A9110E808BC7D42D12A0484E7DF1E76DD2F4E878E87384EF',
            '025912DB5D6689019DA60B97185E2E5364BE267919D4CBD3001D233CA992D721DA'] \
        }

    privateKeyShare = {
        1: '520E115DCE722A3ABEA3791E893B502DC066686726DCB9516934077C8E625E18', \
        2: '88212CFA39F34DA1357566317F298951B5D2FA5A07BA949D7ECE1355C38CEC58', \
        3: 'BE344896A5747107AC4753447517C275AB3F8C4CE8986FE994681F2EF8B77A98'
    }

    littleK = {
        1: '4784A548395A0E7AB7F092474BEDDF60413A4CCD480AAF51B23AE120C5AC0E79', \
        2: '9B05F9D37E62EE5366A5A8CEC9CB5A8158917E4966D862067FB0381626D57341', \
        3: 'EE874E5EC36BCE2C155ABF5647A8D5A26FE8AFC585A614BB4D258F0B87FED809'
    }

    alpha = {
        1: '5DF3A2C594512CB0443379163FA728DB5A4B8C5802F7E47F3CEDA68ECD31069F', \
        2: '83FFFC5071D126B9991556113B851962921BB9C1F0394C3AABFA860AAAC16405', \
        3: 'AA0C55DB4F5120C2EDF7330C376309E9C9EBE72BDD7AB3F61B0765868851C16B'
    }

    # This is the same for each Player
    publicKeyShare = '03F5CC07AF5F8AE651E8FAFA0D644D0FB389D3C3B2128D4C1C228493F1B5506B8E'

    # allHiddenPolynomials is the same as pk_hidden_poly above.  Worth testing this is the same, for completeness ?

    # This is the same data for each Player
    allVWshares = { \
        1: ('B5C39DB5FA5F0A36681166CC4E83184E9F19848C63A16C5E52296E857012EB82',
            '0319363904633B0104C8A181757E8DAA1C55C1D1A9A3446831852BCB62B1D2C9D2'), \
        2: ('E6DDF7D64FAD212F27623FE176AB9B181CBFB90CE0CA4868530855C2FBB72CAA',
            '029F085615A82D5E6DF975B15DBF8251E59CC557FF59F6F840AB36FD19663257CB'), \
        3: ('D13A852BA17ADB1E0477D97ED41F3D34CA7C276A8C65DF742B9DB9EDD70E4ACA',
            '0203199ECF40DC86F895AC37590567E999065F2854215A6EFAB969E8E50F1022C6') \
        }

    sigRStrExpected = 'DF6ECD56B34C4204D42301528F990FAE8B62875E4222D054900C4EF2B04EC99D'
    sigSStrExpected = '54B28A880E516FF39DEEC671CAC6A74BC7498766E2819A99C4866C3AC6CF7178'

    playersList = []
    remplayersList = []

    # register players with Orchestrator
    for num in range(noOfPlayers):
        orc.register('player' + str(num), 'ref' + str(num))
        playersList.append('player' + str(num))

    # create group
    groupid, invites = orc.createGroup(playersList[0], m, n)

    # remaining list
    for p in playersList:
        if p != playersList[0]:
            remplayersList.append(p)

    # accept invite
    for user in remplayersList:
        orc.acceptInvite(user, groupid, True)

    # get the users who belong to the groupid
    participantList = orc.getParticipants(groupid)

    # add the player list
    players = []
    for num in participantList:
        players.append(player.Player(myPrintFunction))

    # add player to the group
    for num, participant in enumerate(participantList):
        ord = 1
        myord = 1
        ordlist = []
        for num1, participant1 in enumerate(participantList):
            if participant == participant1:
                myord = ord
                ord += 1
            else:
                ordlist.append((ord, ' '))
                ord += 1
        players[num].addGroup(groupid, myord, ordlist, degree)

        # JAS: Added this hack to fix the polynomial to the test data above,
        # #    then recalculate f_x, hide polynomial and hide evals using the fixed polynomial
        players[num].groups[groupid].privateKeyPolynomial = Nakasendo.Polynomial.initFromListModuloHex( \
            private_poly[myord], mod)

        players[num].groups[groupid].polynomialPreCalculation(players[num].groups[groupid].privateKeyPolynomial, mod,
                                                              myord)
        # JAS: end hack

    # request data for PRIVATEKEYSHARE
    for num, pl in enumerate(players):
        groupId, ordinal, hiddenPoly, hiddenEvals = pl.requestData(groupid, 'PRIVATEKEYSHARE')

        # collate data of every player
        collatedData = orc.collateData(groupid, pl.groups[groupid].ordinal, hiddenPoly, hiddenEvals)

    # get collated data
    collatedHiddenPolys, collatedHiddenEvals = orc.getCollatedData(groupid)

    # assertion on len of collatedHiddenEvals
    for key, values in collatedHiddenEvals.items():
        assert len(values) == len(participantList), "Test Failed"

    # assertion on len of collatedHiddenPolys
    for key, values in collatedHiddenPolys.items():
        assert len(values) == degree + 1, "Test Failed"

    for f in players:
        for fx in players:
            if f != fx:
                fxValue = fx.getEvals(groupid, f.groups[groupid].ordinal)
                f.allEvalsReceived(groupid, f.groups[groupid].ordinal, fx.groups[groupid].ordinal, fxValue)
        groupId, result = f.createSecret(groupid, 'PRIVATEKEYSHARE', collatedHiddenPolys, collatedHiddenEvals)

    # request data for ALPHA
    for num, pl in enumerate(players):
        groupId, ordinal, hiddenPoly, hiddenEvals = pl.requestData(groupid, 'ALPHA')

        # HACK it for ALPHA
        group = pl.groups[groupid]

        ordinal = group.ordinal
        poly = Nakasendo.Polynomial.initFromListModuloHex(alpha_poly[ordinal], mod)

        group.polynomialPreCalculation(poly, mod, ordinal)

        hiddenPoly = group.transientData.hiddenPolynomial
        hiddenEvals = group.transientData.hiddenEvals

        # collate data
        collatedData = orc.collateData(groupid, pl.groups[groupid].ordinal, hiddenPoly, hiddenEvals)

    collatedHiddenPolys, collatedHiddenEvals = orc.getCollatedData(groupid)

    # createseceret with calType = 'ALPHA'
    for f in players:
        for fx in players:
            if f != fx:
                fxValue = fx.getEvals(groupid, f.groups[groupid].ordinal)
                f.allEvalsReceived(groupid, f.groups[groupid].ordinal, fx.groups[groupid].ordinal, fxValue)
        groupId, result = f.createSecret(groupid, 'ALPHA', collatedHiddenPolys, collatedHiddenEvals)

    # request data for LITTLEK
    for num, pl in enumerate(players):
        groupId, ordinal, hiddenPoly, hiddenEvals = pl.requestData(groupid, 'LITTLEK')

        # HACK it for LITTLEK
        group = pl.groups[groupid]

        ordinal = group.ordinal
        poly = Nakasendo.Polynomial.initFromListModuloHex(littlek_poly[ordinal], mod)

        group.polynomialPreCalculation(poly, mod, ordinal)

        hiddenPoly = group.transientData.hiddenPolynomial
        hiddenEvals = group.transientData.hiddenEvals

        # collate data
        collatedData = orc.collateData(groupid, pl.groups[groupid].ordinal, hiddenPoly, hiddenEvals)

    collatedHiddenPolys, collatedHiddenEvals = orc.getCollatedData(groupid)

    # # createseceret with calType = 'LITTLEK'
    for f in players:
        for fx in players:
            if f != fx:
                fxValue = fx.getEvals(groupid, f.groups[groupid].ordinal)
                f.allEvalsReceived(groupid, f.groups[groupid].ordinal, fx.groups[groupid].ordinal, fxValue)
        groupId, result = f.createSecret(groupid, 'LITTLEK', collatedHiddenPolys, collatedHiddenEvals)

    # cal calculateShareOfVW
    for num, f in enumerate(players):
        res = f.getVWshares(groupid)

        playerVWshares = res[1]  # get VWshares for every player

        orc.collateVWData(groupid, f.groups[groupid].ordinal, playerVWshares)

    collatedVWData = orc.getCollatedVWData(groupid)
    assert len(collatedVWData) == len(participantList)

    # setVWSharedData
    for f in players:
        f.setSharedVWData(groupid, collatedVWData)

    # set sigining Initiaor to True before requesting Signture Data
    players[0].setSigningInitiator(groupid)

    messageData = 'SDK Testing Message'

    # requestSignatureData
    for f in players:

        # requestSignatureData
        # ephemeral key index, can be in the range of 0....<number ephemeral keys -1>
        index = 0
        groupId, ordinal, value, message = f.requestSignatureData(groupid, index,f.hashMessage(messageData))

       # collate signature data
        orc.signature(groupid, f.groups[groupid].ordinal, value)

    # get the collated signature for all the players
    collatedSignatures = orc.getSignatureData(groupid)

    # for f in players:
    mysignatureData = players[0].sign(groupid, messageData, collatedSignatures)
    sigRStrActual = mysignatureData[0]
    sigSStrActual = mysignatureData[1]

    # #compare the actual and expected values for r and s values
    assert str(sigRStrActual) == str(sigRStrExpected), " Test failed to generated same r value while testing fixed polynomials"
    assert str(sigSStrActual) == str(sigSStrExpected), " Test failed to generated same r value while testing fixed polynomials"



#Verify same signarture r and s values are generated when the polynomials are fixed
def test_FixedPolynomialNegativeScenario():
    orc = orchestrator.Orchestrator()
    noOfPlayers = 3
    m = 2  # recombination number
    n = 3  # total number
    degree = m - 1  # recombination number - 1
    mod = player.Player.modulo

    # private_poly
    private_poly = { \
        1: ['6202FC74B84176AE6BAF0B1453AF99C3D59C33F88D4E656C7F84207200D8D4C4',
            '2A186451C4062FF810E4B4E1DD2F600B8DADFC40E3A510125B25DEB413F3B351'], \
        2: ['7DC9FEE24641D94EDC4B65E5E77975BE17ED9AACED7AFE2E3283E31538349A92',
            'D69EBE7C2BA98D0773BA7384EAEAF93951EEA22BD798E5BDD5897E3692BAA4F4'], \
        3: ['3C2DFA6A646DB6D6FFD71B1158240786981EE4B57A7E1AA6616456A8F060A1C3',
            '355BF8CE7BD16666F232C4AC2DD3DFDDD07ED06CD4E885B7A4BD0D7B5EB2773C'] \
        }

    # ALPHA
    alpha_poly = { \
        1: ['FEF65C573F47FDEDAE80218B61E42865F0A7674E7C5E0FBE8E7DCBA3BDA1FD64',
            '9EE5C7BCB2E249A34076B80E13CFB8E5F6C5C5FBA0C7F40D5D93AB9A55DCB90B'], \
        2: ['B88475BC2C3D1F3C7F5401AF2BDA88B520A50ADA1966EEC82E7FB67AA4EF8030',
            '0E6AC1D7B57D7443DCA8D53F1881FA3F34B99557C0435343F8D1528659A40816'], \
        3: ['806C77274B4C157CC17D78E0B60A8736868CA692DE82BEB49088020E2D7BAE27',
            '78BBCFF675203C2237C24FADCF8C3D60C6FFAEFD3B7EC0A5D87A3FE7FE45DD86']
    }

    # LITTLEK
    littlek_poly = { \
        1: ['48B3E0488D62D44386BADE3E7F1CBD65DC9F59307FF109C1323E867CBF31C9BB',
            '12209E01DD13E2CC9DC4ECBD7CB9FDEAD9392C6F314993C8482D0C1100A3DA98'], \
        2: ['0BFCA7C6537B95C95198E91DBB01A571CDB4B7DD6AEF963979E41368D676F39B',
            'B4AB93C74533C61C8B4C1E13BCF30864B9758853EA7D474A9033EA114F29024C'], \
        3: ['9F52C8AE1372C49530E7B46393F201663A3DE729EDA4FCDDF8754ED29F102D9C',
            '8CB522C222C136EF85A40BB6443074D03F57599FB24F77DDB4E6BF5FE192C925'] \
        }

    # pk = privateKeyShare.  i.e. this is the transient data produced in
    # calculating the privateKeyShare.  The JVRSS data structure is wiped and reused
    # for subsequent calculations of ALPHA and LITTLEK
    pk_f_x = { \
        1: '8C1B60C67C47A6A67C93BFF630DEF9CF634A303970F3757EDAA9FF2614CC8815', \
        2: '2B077BDA9D94F35DC3C04CEFBD4F6833466D25373E1B89325DF22268BD3D61F8', \
        2: 'DC41E4D5D7E1EA0BD66F6915E19FA720099B55FBF937ABCD4F9B7F1B0C780777' \
        }

    # f_x for the alpha calculation
    alpha_f_x = { \
        1: '9DDC2413F22A4790EEF6D99975B3E14D2CBE50636DDD63902C3F18B14348752E', \
        2: 'D559F96B973807C438A5AC2D5CDE7D338A18358999ED955020225B875837905C', \
        2: 'EA9FE70AAAACC9E368C467EA24AF3F5A20DCD6A3E1B6606A5A24633958170578' \
        }

    # outer-dict is the player ordinal, i.e. player 1 : { dict }
    # inner-dict is ordinal used in the evaluation (f_x), i.e. ordinal : hide(f_x)
    pk_hidden_evals = { \
        1: { \
            1: '0330FF8B9F2FE6BA6CAE844247DC331A9C3EFD39E535F348BE25B0E699138CF500', \
            2: '03D1641B342781E739DD3CFDF4D8965EC220D5937E0CEB69F6DAFE08E2B7CF449A', \
            3: '020D777CAAB25A0CDA8EA1F04F98CBA72531D67FDEA229C44600B8038095D8343F' \
            }, \
        2: { \
            2: '0319E9B39C8FDA5253D9468F7BD4C384E3FD36C2AE0CE86008C5FFDCF975365645', \
            1: '02B0849C8FED8AC74F984C6B82ADA083972CE968BA606BB830491042ED6FE2B542', \
            3: '0295D4D90B05B348BF78FF6E48A6E0C126FE33D405837FACB05AB6B0B28DAB3CB2' \
            }, \
        3: { \
            3: '02FFB22E4CE375671C17286812B11ADCD21C99D8D270A755AB36901703A5228009', \
            1: '03E2AF390D8BCA17538C17F424AFB1B6A7217288F23FC8DE4DC03650B9AF326808', \
            2: '025EC8213DEC342C17C0855CBD90D8972EEDF60760E0C60990342D2543E9623E86' \
            } \
        }

    alpha_hidden_evals = { \
        1: { \
            1: '03B5475C7B9B68D691BC8A0D19458B73536B9BD2330D136684F86BAFB8610AE696', \
            2: '0347D1B9C6A7F79B579D51FC45DDADB49CE1E5F58C013305D39C4FD88C839D4A62', \
            3: '02252D8E62CE57F4DF4ABF768B0AF46D2813E2DC777F09ABDBE1BFAA2777BFC0FD' \
            }, \
        2: { \
            2: '02FE44EAA626250A178B088A515C847AE48AF3B88B9D7750ED82716733AE0CC608', \
            1: '03DEE8EE68D092DB37E3EA7EEDB8D72B34FA974E239E15646BAE7AECEF2A3C74C1', \
            3: '033EA4E68A841DCDD098D276188F1FBF9BA6ECA1371C12079A0ABD6C106F5CBD96' \
            }, \
        3: { \
            3: '02BA3953A8A661EC9ECA3A5FFB4C6BD817D70E0ADC1404DBD85ECF883F4D764645', \
            1: '03D3266F66BE2C89C57B3E1F8CBF1419C8BEBE07B008D5F94A0E772FF9F9C63960', \
            2: '02CB6F81916DCB1A1B2490C462F4CD488915B454BFCEC0131DCC50497075089F43' \
            } \
        }

    pk_hidden_poly = { \
        1: ['02B186C23A7C410E94E3FCF869621F00FE0CE4E9243E1B56B772B651B3E677D099',
            '02A187F015D166B32C2D896E3B7B6CEBC65D07E7E25FF8E3B3D6BF6716F4E62F07'], \
        2: ['0271BA0247025D55A4783E292FDEF68DEE9DAC9B0934C38FB68320FCBD312E32B0',
            '038B84A926F1568797DB6CD05DF90EEF67E65EC8B3B4269F588792E7013D04EDDE'], \
        3: ['03C54C1593485AD5F4EE77D6115D6CCC85754BA2CA60260E2991A19C5CADBCC94A',
            '03C074DB725B75F2E5FEF24B351A8A3D0F72FFD3DD3026A93655E2F0CCA20E358A'] \
        }

    alpha_hidden_poly = { \
        1: ['03AF5F7C45924749366A4911E59B8AAB6FEDF9819353DC9FFE045535814E31E41D',
            '02CB19C881FBE1EEB171392B999E25BC8EE5FA3EEE1AA2734793045B4188563477'], \
        2: ['032DA156AAF9024B12FC96C55194AFCED148A8FFA89F63D009B24BA5C0EF84451D',
            '038AE8052671BD5E1F397ABC32934FC2C527C65DF82E010140667ED1C3FF94D640'], \
        3: ['0221C38C2BA9FA3574A9110E808BC7D42D12A0484E7DF1E76DD2F4E878E87384EF',
            '025912DB5D6689019DA60B97185E2E5364BE267919D4CBD3001D233CA992D721DA'] \
        }

    privateKeyShare = {
        1: '520E115DCE722A3ABEA3791E893B502DC066686726DCB9516934077C8E625E18', \
        2: '88212CFA39F34DA1357566317F298951B5D2FA5A07BA949D7ECE1355C38CEC58', \
        3: 'BE344896A5747107AC4753447517C275AB3F8C4CE8986FE994681F2EF8B77A98'
    }

    littleK = {
        1: '4784A548395A0E7AB7F092474BEDDF60413A4CCD480AAF51B23AE120C5AC0E79', \
        2: '9B05F9D37E62EE5366A5A8CEC9CB5A8158917E4966D862067FB0381626D57341', \
        3: 'EE874E5EC36BCE2C155ABF5647A8D5A26FE8AFC585A614BB4D258F0B87FED809'
    }

    alpha = {
        1: '5DF3A2C594512CB0443379163FA728DB5A4B8C5802F7E47F3CEDA68ECD31069F', \
        2: '83FFFC5071D126B9991556113B851962921BB9C1F0394C3AABFA860AAAC16405', \
        3: 'AA0C55DB4F5120C2EDF7330C376309E9C9EBE72BDD7AB3F61B0765868851C16B'
    }

    # This is the same for each Player
    publicKeyShare = '03F5CC07AF5F8AE651E8FAFA0D644D0FB389D3C3B2128D4C1C228493F1B5506B8E'

    # allHiddenPolynomials is the same as pk_hidden_poly above.  Worth testing this is the same, for completeness ?

    # This is the same data for each Player
    allVWshares = { \
        1: ('B5C39DB5FA5F0A36681166CC4E83184E9F19848C63A16C5E52296E857012EB82',
            '0319363904633B0104C8A181757E8DAA1C55C1D1A9A3446831852BCB62B1D2C9D2'), \
        2: ('E6DDF7D64FAD212F27623FE176AB9B181CBFB90CE0CA4868530855C2FBB72CAA',
            '029F085615A82D5E6DF975B15DBF8251E59CC557FF59F6F840AB36FD19663257CB'), \
        3: ('D13A852BA17ADB1E0477D97ED41F3D34CA7C276A8C65DF742B9DB9EDD70E4ACA',
            '0203199ECF40DC86F895AC37590567E999065F2854215A6EFAB969E8E50F1022C6') \
        }

    sigRStrExpected = 'DF6ECD56B34C4204D42301528F990FAE8B62875E4222D054900C4EF2B04EC99D'
    sigSStrExpected = '54B28A880E516FF39DEEC671CAC6A74BC7498766E2819A99C4866C3AC6CF7178'

    playersList = []
    remplayersList = []

    # register players with Orchestrator
    for num in range(noOfPlayers):
        orc.register('player' + str(num), 'ref' + str(num))
        playersList.append('player' + str(num))

    # create group
    groupid, invites = orc.createGroup(playersList[0], m, n)

    # remaining list
    for p in playersList:
        if p != playersList[0]:
            remplayersList.append(p)

    # accept invite
    for user in remplayersList:
        orc.acceptInvite(user, groupid, True)

    # get the users who belong to the groupid
    participantList = orc.getParticipants(groupid)

    # add the player list
    players = []
    for num in participantList:
        players.append(player.Player(myPrintFunction))

    # add player to the group
    for num, participant in enumerate(participantList):
        ord = 1
        myord = 1
        ordlist = []
        for num1, participant1 in enumerate(participantList):
            if participant == participant1:
                myord = ord
                ord += 1
            else:
                ordlist.append((ord, ' '))
                ord += 1
        players[num].addGroup(groupid, myord, ordlist, degree)

        # JAS: Added this hack to fix the polynomial to the test data above,
        # #    then recalculate f_x, hide polynomial and hide evals using the fixed polynomial
        players[num].groups[groupid].privateKeyPolynomial = Nakasendo.Polynomial.initFromListModuloHex( \
            private_poly[myord], mod)

        players[num].groups[groupid].polynomialPreCalculation(players[num].groups[groupid].privateKeyPolynomial, mod,
                                                              myord)
        # JAS: end hack

    # request data for PRIVATEKEYSHARE
    for num, pl in enumerate(players):
        groupId, ordinal, hiddenPoly, hiddenEvals = pl.requestData(groupid, 'PRIVATEKEYSHARE')

        # collate data of every player
        collatedData = orc.collateData(groupid, pl.groups[groupid].ordinal, hiddenPoly, hiddenEvals)

    # get collated data
    collatedHiddenPolys, collatedHiddenEvals = orc.getCollatedData(groupid)

    for f in players:
        for fx in players:
            if f != fx:
                fxValue = fx.getEvals(groupid, f.groups[groupid].ordinal)
                f.allEvalsReceived(groupid, f.groups[groupid].ordinal, fx.groups[groupid].ordinal, fxValue)
        groupId, result = f.createSecret(groupid, 'PRIVATEKEYSHARE', collatedHiddenPolys, collatedHiddenEvals)

    # request data for ALPHA
    for num, pl in enumerate(players):
        groupId, ordinal, hiddenPoly, hiddenEvals = pl.requestData(groupid, 'ALPHA')

        # HACK it for ALPHA
        group = pl.groups[groupid]

        ordinal = group.ordinal
        poly = Nakasendo.Polynomial.initFromListModuloHex(alpha_poly[ordinal], mod)

        group.polynomialPreCalculation(poly, mod, ordinal)

        hiddenPoly = group.transientData.hiddenPolynomial
        hiddenEvals = group.transientData.hiddenEvals

        # collate data
        collatedData = orc.collateData(groupid, pl.groups[groupid].ordinal, hiddenPoly, hiddenEvals)

    collatedHiddenPolys, collatedHiddenEvals = orc.getCollatedData(groupid)

    # createseceret with calType = 'ALPHA'
    for f in players:
        for fx in players:
            if f != fx:
                fxValue = fx.getEvals(groupid, f.groups[groupid].ordinal)
                f.allEvalsReceived(groupid, f.groups[groupid].ordinal, fx.groups[groupid].ordinal, fxValue)
        groupId, result = f.createSecret(groupid, 'ALPHA', collatedHiddenPolys, collatedHiddenEvals)

    # request data for LITTLEK
    for num, pl in enumerate(players):
        groupId, ordinal, hiddenPoly, hiddenEvals = pl.requestData(groupid, 'LITTLEK')

        # HACK it for LITTLEK
        group = pl.groups[groupid]

        ordinal = group.ordinal
        poly = Nakasendo.Polynomial.initFromListModuloHex(littlek_poly[ordinal], mod)

        group.polynomialPreCalculation(poly, mod, ordinal)

        hiddenPoly = group.transientData.hiddenPolynomial
        hiddenEvals = group.transientData.hiddenEvals

        # collate data
        collatedData = orc.collateData(groupid, pl.groups[groupid].ordinal, hiddenPoly, hiddenEvals)

    collatedHiddenPolys, collatedHiddenEvals = orc.getCollatedData(groupid)

    # # createseceret with calType = 'LITTLEK'
    for f in players:
        for fx in players:
            if f != fx:
                fxValue = fx.getEvals(groupid, f.groups[groupid].ordinal)
                f.allEvalsReceived(groupid, f.groups[groupid].ordinal, fx.groups[groupid].ordinal, fxValue)
        groupId, result = f.createSecret(groupid, 'LITTLEK', collatedHiddenPolys, collatedHiddenEvals)

    # cal calculateShareOfVW
    for num, f in enumerate(players):
        res = f.getVWshares(groupid)

        playerOrdinal = res[0]
        playerVWshares = res[1]  # get VWshares for every player

        orc.collateVWData(groupid, f.groups[groupid].ordinal, playerVWshares)

    collatedVWData = orc.getCollatedVWData(groupid)
    assert len(collatedVWData) == len(participantList)

    # setVWSharedData
    for f in players:
        f.setSharedVWData(groupid, collatedVWData)

    # set sigining Initiaor to True before requesting Signture Data
    players[0].setSigningInitiator(groupid)

    messageData = 'SDK Testing Message'

    # requestSignatureData
    for f in players:

        # requestSignatureData
        # ephemeral key index, can be in the range of 0....<number ephemeral keys -1>
        index = 0

        # tampering the ephemeralKeyList before requestignSignatureData
        f.groups[groupid].ephemeralKeyList[0][1] = f.groups[groupid].ephemeralKeyList[0][0]

        groupId, ordinal, value, message = f.requestSignatureData(groupid, index,f.hashMessage(messageData))

       # collate signature data
        orc.signature(groupid, f.groups[groupid].ordinal, value)

    # get the collated signature for all the players
    collatedSignatures = orc.getSignatureData(groupid)

    # for f in players:
    mysignatureData = players[0].sign(groupid, messageData, collatedSignatures)
    sigRStrActual = mysignatureData[0]
    sigSStrActual = mysignatureData[1]

    # #compare the actual and expected values for r and s values
    assert str(sigRStrActual) != str(sigRStrExpected), " Negative Test failed while testing fixed polynomials"
    assert str(sigSStrActual) != str(sigSStrExpected), " Negative Test failed value while testing fixed polynomials"