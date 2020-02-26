## Threshold Signatures <a name="ThresholdSignatures"></a>

The Threshold Signature scheme (powered by Nakasendo<sup>TM</sup>) can provide developers with ways of managing groups of users who can cooperate to produce signatures without ever creating a single private key.

While there are several digital signature schemes available, the **Threshold Signature** scheme stands out because **the private key _never_ exists**. 

Here you will find a generic code example of how to set up a Threshold Signature scheme along with some sample Use Cases.

**How to use it**

*Step 1 - Registering a Group of users*

A single set of classes model registration of users and management of groups. When a TS client starts up for the first time it could send its details to a central point that uses the GlobalGroup classes to maintain a list of users. 

```c++
createRegisterRequest(SinglePlayer::Instance() -> publicInfo(), os);
```
---

*Step 2 - Creating a Threshold Signature group*

When a transaction needs made one of the registered Users/ Players will propose a Group with various parameters. The parameters under the Proposer's control are the total size of the Group and the **recombination threshold** to construct a private key. This parameter is referred to as m. The signature threshold is a function of this number. Specifically, it’s $2(m-1)+1$. Please refer to the more detailed documentation on ThresholdSignatures for further details. 

Once a Client proposes a Group, the Orchestrator class will send an invite to every registered user. The first $m$ users to respond positively are added to the Group. Once the Group is constructed completely, the **Orchestrator broadcasts** the Group details to all Players in the Group. 

```c++
createGroupRequest(SinglePlayer::Instance() -> publicInfo(), n, m, os);
```
---

*3. Secret Sharing*

This secret sharing process can now be initiated by one of the group members. It doesn’t have to be the proposer. The first round of secret sharing is to compute the share of the private key and group public key. The code required to do this is: 

```C++
createSecretSharingRequest (SinglePlayer::Instance() -> publicInfo(), grpid, std::string("PRIVATEKEYSHARE"), os);
```

Below is a code example showing how to set up the secret sharing. This is a contrived situation with limited practical use as all the players are in the same process. However, it is useful to visualise what is involved in secret sharing and verification for the data. This example is using the data structures in Nakasendo<sup>TM</sup> but from within a single process. In a real world application this data would have to be transmitted between player processes and orchestrator process. 

```c++
void ShareSecret(GroupMetadata &glbgrp, std::vector<ExtendedPlayer> &players, const std::string &calctype, const int &t, const BigNumber &mod)
{
    if (calctype == "LITTLEK" || calctype == "ALPHA")
    {
        for (std::vector<ExtendedPlayer>::iterator iter = players.begin(); iter != players.end(); ++iter)
        {
            Polynomial poly(t, mod);
            playerGroupMetaData &pgrp = iter->PlayerGroup().find(glbgrp.groupid())->second;
            pgrp.polynomialPreCalculation(poly);
        }
    }

    // for each player, add the hidden polynomial & ordinal to the global
    // group add the ordinal + a list of the poly evals to the global group

    for (std::vector<ExtendedPlayer>::iterator iter = players.begin(); iter != players.end(); ++iter)
    {
        playerGroupMetaData &pgrp = iter->PlayerGroup().find(glbgrp.groupid())->second;
        std::vector<std::string> hpcoeffs;
        for (std::vector<ECPoint>::const_iterator hpIter = pgrp.m_transientData.m_hiddenPolynomial.begin(); hpIter != pgrp.m_transientData.m_hiddenPolynomial.end(); ++hpIter)
        {
            hpcoeffs.push_back(hpIter->ToHex());
        }

        glbgrp.addCollatedHiddenPolys(std::to_string(pgrp.m_ordinal), hpcoeffs);

        for (std::map<std::string, std::vector<std::pair<std::string, ECPoint>>>::iterator heIter = pgrp.m_transientData.m_hiddenEvals.begin(); heIter != pgrp.m_transientData.m_hiddenEvals.end(); ++heIter)
        {
            for (std::vector<std::pair<std::string, ECPoint>>::iterator innerHeIter = heIter->second.begin();
                innerHeIter != heIter->second.end(); ++innerHeIter)
            {
                glbgrp.addCollatedHiddenEvals(std::to_string(pgrp.m_ordinal), innerHeIter->first, innerHeIter->second.ToHex());
            }
        }
    }

    // share the evals between the players
    // please note these values DO NOT go to the global group

    for (int pi = 0; pi < players.size(); ++pi)
    {
        playerGroupMetaData &pipgrp = players[pi].PlayerGroup().find(glbgrp.groupid())->second;
        int piOrd = pipgrp.m_ordinal;

        for (int pj = 0; pj < players.size(); ++pj)
        {
            if (pi != pj)
            {
                playerGroupMetaData &pjgrp = players[pj].PlayerGroup().find(glbgrp.groupid())->second;
                int pjOrd = pjgrp.m_ordinal;
                std::map<std::string, BigNumber>::const_iterator iter = pipgrp.m_transientData.m_evals.find(std::to_string(pjOrd));
                if (iter != pipgrp.m_transientData.m_evals.end())

                    pjgrp.addPublicEvalsToJVRSS(std::to_string(piOrd), iter->second.ToHex());
            }
        }
    }

    // load the hidden poly information & hidden eval info from the global
    // group in a real application this is achieved via a messaging layer

    for (std::vector<ExtendedPlayer>::iterator iter = players.begin(); iter != players.end(); ++iter)
    {
        playerGroupMetaData &pgrp = iter->PlayerGroup().find(glbgrp.groupid())->second;
        for (std::map<std::string, std::vector<ECPoint>>::const_iterator hpIter = glbgrp.CollatedHiddenPolys().begin();
            hpIter != glbgrp.CollatedHiddenPolys().end(); ++hpIter)
        {
            std::vector<std::string> hpStrs;

            for (std::vector<ECPoint>::const_iterator ecPtiter = hpIter->second.begin(); ecPtiter != hpIter->second.end(); ++ecPtiter)
            {
                hpStrs.push_back(ecPtiter->ToHex());
            }

            pgrp.addHiddenPolynomialToJVRSS(hpIter->first, hpStrs);
        }

        for (std::map<std::string, std::vector<std::pair<std::string, ECPoint>>>::const_iterator iter = glbgrp.CollatedHiddenEvals().begin();
            iter != glbgrp.CollatedHiddenEvals().end(); ++iter)
        {
            std::vector<std::pair<std::string, std::string>> he;
            for (std::vector<std::pair<std::string, ECPoint>>::const_iterator innerIter = iter->second.begin(); innerIter != iter->second.end(); ++innerIter)
            {
                he.push_back(std::make_pair(innerIter->first, innerIter->second.ToHex()));
            }

            pgrp.addHiddenEvalsToJVRSS(iter->first, he);
        }
    }

    // call the verification routines

    for (std::vector<ExtendedPlayer>::iterator iter = players.begin(); iter != players.end(); ++iter)
    {
    playerGroupMetaData &pgrp = iter->PlayerGroup().find(glbgrp.groupid())->second;
        pgrp.verifyCorrectness();
        pgrp.verifyHonesty();
    }

    // compute the share of secret for each player

    for (std::vector<ExtendedPlayer>::iterator iter = players.begin(); iter != players.end(); ++iter)
    {
    playerGroupMetaData &pgrp = iter->PlayerGroup().find(glbgrp.groupid())->second;
        BigNumber secretShare = pgrp.createSecret();


        if (calctype == "PRIVATEKEYSHARE")
        {
            pgrp.m_privateKeyShare = secretShare;
            pgrp.m_GroupPublicKey = pgrp.createGroupPublicKey();
        }
        else if (calctype == "LITTLEK")
        {
            pgrp.m_littleK = secretShare;
        }
        else if (calctype == "ALPHA")
        {
            pgrp.m_alpha = secretShare;
        }
    }

    // clear the data structures for the players & in the global group

    for (std::vector<ExtendedPlayer>::iterator iter = players.begin(); iter != players.end(); ++iter)
    {
    playerGroupMetaData &pgrp = iter->PlayerGroup().find(glbgrp.groupid())->second;
        pgrp.m_transientData.reset();
    }

    glbgrp.clearSharedDataContainers();
    return;
}
```
---

*4. Ephemeral Key*

The Group public key is now available for use. The next step in the Signature generation process is to create a single use Ephemeral Key. This involves two more rounds of secret sharing as described above except with the calculation type set to LITTLEK and ALPHA. This can be run many times to generate a list of ephemeral keys per Player. 

Once a key is used to sign a piece of data it is removed and deleted. There are three function calls involved in this process. 

```c++
for (int i = 0; i < keycount; ++i)
{
    std::cout << "LITTLE K CALCULATION" << std::endl;
    createSecretSharingRequest(p, grpid, std::string("LITTLEK"), os);
    std::cout << "ALPHA CALCULATION" << std::endl;
    createSecretSharingRequest(p, grpid, std::string("ALPHA"), os);
    createEphemeralKeyDataRequest(p.userID(), grpid, os);
}
```

For illustration purposes the code section below will calculate an Ephemeral Key Pair in a Group. 

```c++
ShareSecret(GetGroup(grpID), distributedplayers, "LITTLEK", tt, mod);
ShareSecret(GetGroup(grpID), distributedplayers, "ALPHA", tt, mod);

// push the VW calculation to the global group for each player

for (std::vector<ExtendedPlayer>::iterator iter = distributedplayers.begin(); iter != distributedplayers.end(); ++iter)
{
    playerGroupMetaData &pgrp = iter->PlayerGroup().find(grpID)->second;
    BigNumber kalpha = (pgrp.m_littleK * pgrp.m_alpha);
    ECPoint alphaG = MultiplyByGeneratorPt(pgrp.m_alpha);
    GetGroup(grpID).addCollatedVW(std::to_string(pgrp.m_ordinal), kalpha.ToHex(), alphaG.ToHex());
}

for (std::vector<ExtendedPlayer>::iterator iter = distributedplayers.begin(); iter != distributedplayers.end(); ++iter)
{
    playerGroupMetaData &pgrp = iter->PlayerGroup().find(grpID)->second;

    for (std::map<std::string, std::pair<BigNumber, ECPoint>>::const_iterator iter = GetGroup(grpID).CollatedVW().begin();
         iter != GetGroup(grpID).CollatedVW().end(); ++iter)
    {
        pgrp.addPreSignDataToJVRSS(iter->first, iter->second.first.ToHex(), iter->second.second.ToHex());
    }

    if (!pgrp.CalculateEphemeralKey())
    {
        std::cout << "Failed to calculate an emphemeral key" << std::endl;
    }
}

GetGroup(grpID).clearPreSignatureContainers();
```
---

*5. Collaborative Signature Generation*

The Group is now able to collaborate for signature generation. One of the Players will broadcast a message/ document/ content to be signed. The information broadcast by the Player requesting the signature is the content to be signed plus an index indicating which key to use. 

The number of Players this information is sent to is application specific, but it can be all of the Group or a Sub-Group which must be of a size greater than $2(m-1)$ where m is the private key generation threshold. 

The example code below shows how to pick a sub-group of players, make a request to them for a partial signature and then computes a full signature.

```c++
std::string msg("This is a message that we wish to sign as a group");

std::vector<int> playerSigIndex;

while (playerSigIndex.size() < (2 * tt))
{
    int index = GenerateRandomRange(0, distributedplayers.size() - 1);
    if (std::find(playerSigIndex.begin(), playerSigIndex.end(), index) == playerSigIndex.end())
    {
        playerSigIndex.push_back(index);
    }
}

for (int i = 0; i < playerSigIndex.size(); ++i)
{
        std::pair<std::string,BigNumber>sig=distributedplayers[playerSigIndex[i]].PlayerGroup().find(grpID>second.CalcPartialSignature(msg,0);
        GetGroup(grpID).addCollatedPartialSignautre(sig.first,sig.second.ToHex());
}

// pick one more player who will finish the signature

int playerCompSigIndex = 1;
while (std::find(playerSigIndex.begin(), playerSigIndex.end(), playerCompSigIndex) != playerSigIndex.end())
{
    playerCompSigIndex = GenerateRandomRange(0, distributedplayers.size() - 1);
}

std::cout << "Player at index " << playerCompSigIndex << " will complete the signature" << std::endl;
const std::vector<std::pair<std::string, BigNumber>> &partialSigs = GetGroup(grpID).CollatedPartialSignatures();
std::vector<std::pair<std::string, std::string>> partSig;

for (std::vector<std::pair<std::string, BigNumber>>::const_iterator iter = partialSigs.begin(); iter != partialSigs.end(); ++iter)
{
    partSig.push_back(std::make_pair(iter->first, iter->second.ToHex()));
}

std::pair<BigNumber, BigNumber> signature = distributedplayers[playerCompSigIndex].PlayerGroup().find(grpID)->second.CalculateGroupSignature(msg, 0, partSig);

GetGroup(grpID).clearSignatureContainers();
```
---

*6. Verification of the Signature*

The signing process is now complete. The signature can be verified with the Group public key by anyone using standard ECDSA methods or by using the Nakasendo<sup>TM</sup> verify functions. 

The group public key must be converted to a PEM format. The example code below shows how to do this.

```c++
std::pair<std::string, std::string> pubkeypoints = distributedplayers[playerCompSigIndex].PlayerGroup().find(grpID)->second.m_GroupPublicKey.GetAffineCoords_GFp();

std::pair<std::string, std::string> sigAsStr(signature.first.ToHex(), signature.second.ToHex());

std::string pubkeypem = pubkey_coordinates2pem(pubkeypoints.first, pubkeypoints.second);

if (verify(msg, pubkeypem, sigAsStr))
    std::cout << "Message verified" << std::endl;
```
---

*7. Create the Private Key*

A some point perhaps the group would like to come together to create the private key from the key shares. Should they wish to do this, our scheme allows for it. However, please note that this requires Players to share private data and generate a valuable private key. 

**Nakasendo<sup>TM</sup> does not store this value** but it does pass it back to the application. 

```c++
// Should you wish to create the private key from the group.
std::vector<int> playerKeyGenIndex;

while (playerKeyGenIndex.size() < tt)
{
    int index = GenerateRandomRange(0, distributedplayers.size() - 1);

    if (std::find(playerKeyGenIndex.begin(), playerKeyGenIndex.end(), index) == playerKeyGenIndex.end())
    {
        std::cout << index << std::endl;
        playerKeyGenIndex.push_back(index);
    }
}

int playerIndexCompleteKeyGen = 1;
while (std::find(playerKeyGenIndex.begin(), playerKeyGenIndex.end(), playerIndexCompleteKeyGen) != playerKeyGenIndex.end())
{

    playerIndexCompleteKeyGen = GenerateRandomRange(0, distributedplayers.size() - 1);
}

std::cout << "This player will generate a private key with private data from the other players in the group " << playerIndexCompleteKeyGen << std::endl;
for (int i = 0; i < playerKeyGenIndex.size(); ++i)
{
    std::string pord = std::to_string(distributedplayers[playerKeyGenIndex[i]].PlayerGroup().find(grpID)->second.m_ordinal);

    BigNumber pKeyShare = distributedplayers[playerKeyGenIndex[i]].PlayerGroup().find(grpID)->second.privateKeyShare();

    distributedplayers[playerIndexCompleteKeyGen].PlayerGroup().find(grpID)->second.addPrivateKeyInfo(pord, pKeyShare.ToHex());
}

BigNumber privatekey = distributedplayers[playerIndexCompleteKeyGen].PlayerGroup().find(grpID)->second.CalculateGroupPrivateKey();

if (distributedplayers[playerIndexCompleteKeyGen].PlayerGroup().find(grpID)->second.ValidateGroupPrivateKey(privatekey))
    std::cout << "Private key calculated and verified " << privatekey.ToHex() << std::endl;

```
---

**Definition**

Threshold Signatures are backed by pure maths. To truly understand Threshold Signatures, please download a wonderfully crafted nChain Limited whitepaper via the [Nakasendo<sup>TM</sup> homepage](https://nakasendoproject.org/) website.

**Use Cases**

There is a myriad of applications for Threshold Signatures. For some inspiration, you could consider some of the following:

*_Digital Payment Security_*

- No one person can have full control of a private key. A digital asset custodian could satisfy their financial controls in this instance.

*_Insurance Company Pay-outs_*

- _m_ of _n_ directors are required to sign before large customer pay-outs can be made. This could prevent paying accidentally or unfairly.

*_Personal Wealth Management_*

- An individual could protect their wealth by including them self, their lawyer, sibling, friend and accountant into a scheme. 3 of 5 shares could be required before funds can be spend.

*_Voting records_*

 - Where a Board of Directors wish to vote on making a specific acquisition, such an acquisition could only be triggered once the true majority is reached.

 There are many more applications of Threshold Signatures....
