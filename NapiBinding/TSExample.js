const nakesendoBindings = require('./build/Release/nakasendo.node');

var {   
    BigNumber,
    AddBigNumber,
    SubBigNumber,
    MulBigNumber,
    DivBigNumber,
    InvBigNumber
} = require('./src/Nakasendo/BigNumber.js')

var{
    ECPoint,
    AddECPoint,
    EC_Compare,
    EC_Double,
    EC_Invert,
    EC_ScalerMultiply,
    EC_MultiplyScalerByGenerator
} = require('./src/Nakasendo/ECPoint.js')


var{
    polynomial
} = require('./src/Nakasendo/Polynomial.js');

var {
    LGInterpolator
} = require('./src/Nakasendo/LGInterpolator.js');

var {
    ECLGInterpolator
} = require('./src/Nakasendo/ECLGInterpolator.js');


var {
    AsymKey,
    verify,
    verifyPubKeyHex,
    verifyDER,
    createDERFormat,
    pubKeyAsHexPt,
    pubKeyHexPtstoPEM
} = require ('./src/Nakasendo/AsymKey.js');

var {
    BSVAddress
} = require ('./src/Nakasendo/BSVAddress.js');

var {
    player,
    PlayerList,
    EmphemeralKeyPair,
    GetEmphemeralKeyList,
    GetPlayerList,
    createPolyWithGrp,
    AddPlayerToGroup,
    GetGlobalPlayerList,
    RemovePlayerFromGlobalList,
    GetPlayerDetails,
    SinglePlayer
} = require ('./src/Nakasendo/TS.js');

var mod = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141"

// create a number of players
function getRandomInt(min, max) {
  min = Math.ceil(min);
  max = Math.floor(max);
  return Math.floor(Math.random() * (max - min)) + min; //The maximum is exclusive and the minimum is inclusive
}

function generator() {
	// word lists
	var firstPart = ["The Coldness of", "The Journey to", "Altered by", "A Parallel of", "The Return of", "Consider", "Iron in", "A State of", "The Destiny of", "Shades of", "Taken by", "A Stitch in Time Saves", "Saving", "Emperor of", "Whispers from", "The Book of", "The King of", "The Chronicles of", "The Broken", "Darker Than", "Song of", "Forever", "Love in the Time of", "The Moon is", "At least it's not", "The Abyss of"];
	var secondPart = ["the Coming Order", "the Relationship of Command", "Evil Androids", "Cybernetic Brain", "the Fate of Humanity", "Waltz", "Canto", "a Robot Dog", "Jupiter", "Humanity's Shame", "the Gunslinger", "the Black Tower", "Xenocide", "Blades", "Bel-Shazzar", "Titan", "the Desert", "Mirrors", "the Worm", "Zanzibar", "the End of the World", "Moondust", "Picnic", "Hyperion"];

	// generate name
	var name = firstPart[Math.floor(Math.random() * firstPart.length)] + " " + secondPart[Math.floor(Math.random() * secondPart.length)];

	//alert(name);
	return name;
}

function sleep(millis) {
    return new Promise(resolve => setTimeout(resolve, millis));
}

function CreateSharedSecret(GlbGroup, Players, calctype,grpID){
    // depending on the calctype .. run the pre-poly calc function
    // set up the sharing
    // verify
    // share the evals & public data.


    // calctype == 'PRIVATEKEY
    if(calctype == "LITTLEK" || calctype == "ALPHA"){
        for(pi=0;pi<Players.length;++pi){
            Players[pi].GetGroup(grpID).SecretPreCalc();
        }
    }
    
    for(pi=0;pi<Players.length;++pi){
        // Hiddenpoly
        HiddenPoly = Players[pi].GetGroup(grpID).GetHiddenPolynomial();
        var pOrd = Players[pi].GetGroup(grpID).GetOrdinal();
        GlobalGroup.AddCollatedHiddenPolys(pOrd.toString(),HiddenPoly);
        // hiddenevals
        var hiddenvalsarray = Players[pi].GetGroup(grpID).GetHiddenEvals();
        //first entry is the size of evals vector per ordinal
        var index = 0;
        var playerEvalLen = hiddenvalsarray[index++];
        
        for(i=1;i<hiddenvalsarray.length;++i){
            var playerOrd = hiddenvalsarray[index++];
            if(playerOrd != pOrd){
                throw ("Weirdness going on with player sharing")
                
            }
            for(j=0;j<playerEvalLen;++j){
                var evalOrd = hiddenvalsarray[index++];
                var evalPoint = hiddenvalsarray[index++];
                GlobalGroup.AddCollatedHiddenEvals(pOrd.toString(),evalOrd,evalPoint); 
            }
            i += index;
        }
        
    }

    AllHiddenPolys = GlobalGroup.CollectedHiddenPolys();
    AllHiddenEvals = GlobalGroup.CollatedHiddenEvals();

    for(pi=0;pi<Players.length;++pi){
        var index = 0;
        var polyVecLen = AllHiddenPolys[index++];
        for(i=1;i<AllHiddenPolys.length;++i){
            var playerOrd = AllHiddenPolys[index++];
            polyarray = []
            for(j=0;j<polyVecLen;++j){
                polyarray.push(AllHiddenPolys[index++]);
            }
            Players[pi].GetGroup(grpID).addHiddenPolynomialToJVRSS(playerOrd,polyarray);
            i+=polyVecLen;
        }
        
        index=0;
        var playerEvalVecLen = AllHiddenEvals[index++];
        for(i=0;i<AllHiddenEvals.length;++i){
            var fromOrd = AllHiddenEvals[index++];
            playerEvalArray = []
            for(j=0;j<playerEvalVecLen;++j){
                playerEvalArray.push(AllHiddenEvals[index++]);
                playerEvalArray.push(AllHiddenEvals[index++]);
            }
            Players[pi].GetGroup(grpID).addHiddenEvalsToJVRSS(fromOrd,playerEvalArray);
            i += (playerEvalVecLen*2)+1;
        }
    }

    // player pi will give the correct eval to pj
    for(pi=0;pi<Players.length;++pi){
        piOrd = Players[pi].GetGroup(grpID).GetOrdinal();
        for(pj=0;pj<Players.length;++pj){
            if(pi != pj){
                pjOrd = Players[pj].GetGroup(grpID).GetOrdinal();
                evalForOrd = Players[pi].GetJVRSSForGroup(grpID).GetEvalForPlayerOrdinal(pjOrd.toString());
                Players[pj].GetGroup(grpID).addPublicEvalsToJVRSS(piOrd.toString(),evalForOrd);
            }
        }
    }

    for(pi=0;pi<Players.length;++pi){
        Players[pi].GetGroup(grpID).verifyCorrectness();
        Players[pi].GetGroup(grpID).verifyHonesty();
    }

    for(pi=0;pi<Players.length;++pi){
        var secretshare = Players[pi].GetGroup(grpID).createSecret();
        if(calctype == "PRIVATEKEYSHARE"){
            Players[pi].GetGroup(grpID).SetPrivateKeyShare(secretshare);
            GrpPubKey = Players[pi].GetGroup(grpID).createGroupPublicKey();
        }else if(calctype == "LITTLEK"){
            Players[pi].GetGroup(grpID).SetKValue(secretshare);
        }else if(calctype == "ALPHA"){
            Players[pi].GetGroup(grpID).SetAlpha(secretshare);
        }
    }
    // clear out the secret sharing containers
    for(pi=0;pi<Players.length;++pi){
        Players[pi].GetJVRSSForGroup(grpID).reset();
    }

    GlobalGroup.ClearSharedDataContainers();
    
}


console.log ('Starting TS Demo')
var tt = getRandomInt(2,8);
//tt=1;
var msg = 'I love deadlines. I love the whooshing noise they make as they go by.'
Hm = nakesendoBindings.SHA256(msg)
console.log(Hm);
margin = getRandomInt(4,5)

var nn = 2 * tt + 1 + margin


var startTime, endTime;

function start() {
  startTime = new Date();
};

function end() {
  endTime = new Date();
  var timeDiff = endTime - startTime; //in ms
  console.log(timeDiff + " ms ");
  // strip the ms
  timeDiff /= 1000;

  // get seconds 
  var seconds = Math.round(timeDiff);
  console.log(seconds + " seconds");
}


console.log ('the degree t %s, the Signing Threshold is %s and group size %s' , tt, (2*tt +1), nn)

Players=[];
for(i=0;i<nn;++i){
    var SP = new SinglePlayer();
    var p = new player();
    p.UserID = generator();
    p.addr = "127.0.0.1"
    p.port = "800" + i.toString();
    SP.PublicInfo = p;
    Players.push(SP);
}

console.log("Number of players created %s",Players.length);
// create a Global group (to mimic an orchestrator)
// pick a player from the PlayersList to be the proposer
var proposerIndex = getRandomInt(0,nn);
var grpID = nakesendoBindings.GenerateUUID();
GlobalGroup = new nakesendoBindings.GroupMetaDataWrap
                        (
                            nakesendoBindings.GenerateUUID(),
                            Players[proposerIndex].PublicInfo.UserID,
                            Players[proposerIndex].PublicInfo.uri,
                            Players[proposerIndex].PublicInfo.addr,
                            Players[proposerIndex].PublicInfo.port,
                            (tt + 1),nn,tt
                        );
GlobalGroup.SetGroupID(grpID);

// add the rest of the players (to simulate accepting invites to the group))
for(i=0;i<Players.length;++i){
    if(i != proposerIndex){
        GlobalGroup.AddUserToGroup(Players[i].PublicInfo.UserID,
                                   Players[i].PublicInfo.uri,
                                   Players[i].PublicInfo.addr,
                                   Players[i].PublicInfo.port);
    }
}

console.log(GlobalGroup.printPlayerGrp());
var playerlist = PlayerList(GlobalGroup.GetParticipantList());


for(j=0;j<Players.length; ++j){
    playerGroup = new nakesendoBindings.playerGroupMetaDataWrap(tt, mod);
    playerGroup.SetGrpID(GlobalGroup.GetGroupID());
    var ordinal=1;
    for(i=0;i<playerlist.length;++i){
        if(Players[j].PublicInfo.UserID == playerlist[i].UserID){
            playerGroup.SetOrdinal(ordinal++);
        }
        else{
            playerGroup.AddOrdinalAndPlayerToGrp(
                            ordinal++,
                            playerlist[i].UserID,
                            playerlist[i].uri,
                            playerlist[i].addr,
                            playerlist[i].port
                        );
        }
    }
    playerGroup.SecretPreCalcWithKeySharePolynomial();
    Players[j].addPlayerGroup(playerGroup);
}

CreateSharedSecret(GlobalGroup,Players,"PRIVATEKEYSHARE",grpID);

var EmphemeralKeyCount=1;

for(keyi=0;keyi<EmphemeralKeyCount;++keyi){
    // part of creating an emphermal key
    CreateSharedSecret(GlobalGroup,Players,"LITTLEK",grpID);
    CreateSharedSecret(GlobalGroup,Players,"ALPHA",grpID);

    // k & alpha should be set
    //for(j=0;j<Players.length; ++j){
    //    console.log(Players[j].GetGroup(grpID).printPlayerGrp());
    //}

    for(pi=0;pi<Players.length;++pi){

        piOrd = Players[pi].GetGroup(grpID).GetOrdinal();
        keyarr =  Players[pi].GetGroup(grpID).EphemeralKeyPlayerData();
        GlobalGroup.AddCollatedVW(piOrd.toString(),keyarr[0],keyarr[1]);
    }

    //console.log(GlobalGroup.printPlayerGrp());

    var VWArray = GlobalGroup.CollatedVW();
    for(pi=0;pi<Players.length;++pi){
        for(i=0;i<VWArray.length;++i){
            var playerord = VWArray[i++];
            var vval = VWArray[i++];
            var wval = VWArray[i];
            Players[pi].GetGroup(grpID).addPreSignDataToJVRSS(playerord,vval,wval);
        }
        Players[pi].GetGroup(grpID).CalculateEphemeralKey();
        //console.log(Players[pi].GetGroup(grpID).printPlayerGrp());
    }
    GlobalGroup.ClearPreSignatureContainers();
}

//push the partial signature upoto the GlobalGroup

// pick a random number of players upto 2t+1 to create a signature.
subGroupSigningIndex = []
while(subGroupSigningIndex.length < (2*tt)){
    index = getRandomInt(0,(Players.length)-1);
    if(!subGroupSigningIndex.includes(index))
        subGroupSigningIndex.push(index)
    
}

console.log("These players will create a signature ", subGroupSigningIndex);
            
            
start()

//The Glonbal Group will send 2t+1 shares to one of the players & call CalculateGroupSignature
for(i=0;i<subGroupSigningIndex.length;++i){
    var sigarray = Players[subGroupSigningIndex[i]].GetGroup(grpID).CalcPartialSignature(msg,0);
    GlobalGroup.AddCollatedPartialSignature(sigarray[0],sigarray[1]);
}
var partialSigs = GlobalGroup.CollatedPartialSignatures();

// pick one more player index that's not in subGroupSigningIndex
playerSigIndex = 0;
while(subGroupSigningIndex.includes(playerSigIndex)){
    playerSigIndex = getRandomInt(0,(Players.length)-1);
}
console.log("This player is going to create the final signature ", playerSigIndex);
var GroupSignature = Players[playerSigIndex].GetGroup(grpID).CalculateGroupSignature(msg,0,partialSigs);
end()
console.log(GroupSignature);



// verify the signature
pi=0
console.log(Players[pi].GetGroup(grpID).GetGroupPublicKey())

var verified = verifyPubKeyHex(msg,Players[pi].GetGroup(grpID).GetGroupPublicKey(),GroupSignature[0], GroupSignature[1]);
if(verified)
    console.log('Message verified'); 


// add private key shares to recreate the group private key
subGroupKeyGenIndex = [];
while(subGroupKeyGenIndex.length < tt){
    index = getRandomInt(0,(Players.length)-1);
    if(!subGroupKeyGenIndex.includes(index))
        subGroupKeyGenIndex.push(index)
    
}

playerKeyRequestorIndex = 0;
while(subGroupKeyGenIndex.includes(playerKeyRequestorIndex)){
    playerKeyRequestorIndex = getRandomInt(0,(Players.length)-1);
}

console.log("This group of players will contribute to a private key generation", subGroupKeyGenIndex, "\nThis player will create the jey and validate it ", playerKeyRequestorIndex);

for(i=0;i<subGroupKeyGenIndex.length;++i){
    keyshare = Players[subGroupKeyGenIndex[i]].GetGroup(grpID).GetPrivateKeyShare();
    playerord = Players[subGroupKeyGenIndex[i]].GetGroup(grpID).GetOrdinal();
    Players[playerKeyRequestorIndex].GetGroup(grpID).addPrivateKeyInfo(playerord.toString(),keyshare);
        
}

var GroupPrivateKey = Players[playerKeyRequestorIndex].GetGroup(grpID).CalculateGroupPrivateKey();
if(Players[playerKeyRequestorIndex].GetGroup(grpID).ValidateGroupPrivateKey(GroupPrivateKey)){
    console.log("Group Private Key verified ", GroupPrivateKey);
}

GlobalGroup.ClearSignatureContainers();
//console.log(GlobalGroup.printPlayerGrp());







