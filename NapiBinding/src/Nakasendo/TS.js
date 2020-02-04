const bindings = require('../../build/Release/nakasendo.node');
var{
    polynomial
} = require('./Polynomial.js');

class player{
    #m_userid;
    #m_uri;
    #m_addr;
    #m_port;
    
    constructor(){
        this.#m_userid=""
        this.#m_uri=""
        this.#m_addr=""
        this.#m_port=""
    }
    
    
    set UserID(val){this.#m_userid = val;}
    get UserID(){ return this.#m_userid;}
    
    set uri(val){this.#m_uri = val;}
    get uri(){return this.#m_uri;}
    
    set addr(val){this.#m_addr = val;}
    get addr(){return this.#m_addr;}
    
    set port(val){this.#m_port = val;}
    get port(){return this.#m_port;}
    
    toString(){
        return this.#m_userid + " " + this.#m_uri + " " + this.#m_addr + " " + this.#m_port; 
    }
}

// Helper functions to create player lists

function PlayerList(playerArr){
    // Can the type to checked.
    players=[];
    for(i=0;i<playerArr.length;++i){
        p = new player();
        p.UserID = playerArr[i++];
        p.uri = playerArr[i++];
        p.addr = playerArr[i++];
        p.port = playerArr[i];
        players.push(p);
    }
    return players;
}
// Each "player program" can have multiple groups.

class SinglePlayer {
    #m_PlayerGroups;
    #m_PublicPlayerInfo;

    constructor(){
        this.#m_PublicPlayerInfo = new player();
        this.#m_PlayerGroups = new Map();
    }
    
    set PublicInfo(p) { this.#m_PublicPlayerInfo = p;}
    get PublicInfo() { return this.#m_PublicPlayerInfo; }
    
    addPlayerGroup(grp){
        this.#m_PlayerGroups.set(grp.GetGrpID(), grp);
    }
    GroupIDS(){
        return Array.from(this.#m_PlayerGroups.keys());
    }
    
    GetGroup(grpid){
        return this.#m_PlayerGroups.get(grpid);
    }
    
    GetJVRSSForGroup(grpid){
        var jvrssInstance = new bindings.jvrssWrap();
        this.#m_PlayerGroups.get(grpid).GetJVRSS(jvrssInstance);
        return jvrssInstance;
    }

}

        
class EmphemeralKeyPair{
    #m_k;
    #m_r;
    
    constructor(){}
    
    set K(val){this.#m_k = val;}
    get R(){return this.#m_k;}
    
    set R(val){this.#m_r = val;}
    get R(){return this.#m_r;}    
    
    toString(){return this.#m_k + " " + this.#m_r;}
}

function GetEmphemeralKeyList(grp){

    
    var keyList = [];
    const KeyPairList = grp.GetEmpheralKeyList();
    
    for(i=0;i<KeyPairList.length;++i){
        keys = new EmphemeralKeyPair();
        keys.SetK(KeyPairList[i++]);
        keys.SetR(KeyPairList[i]);
        keyList.push(keys);
    }
    
    return keyList;
}

function GetPlayerList(grp){
    var players = new Map();
    const playerList = grp.GetPlayerAndOrdinalList();
    for(i=0;i<playerList.length; ++i){
        p = new player();
        ord = playerList[i++];
        p.UserID =  playerList[i++];
        p.uri =  playerList[i++];
        p.addr =  playerList[i++];
        p.port =  playerList[i];
        players.set(ord,p);
    }
    return players;
}


function createPolyWithGrp(grp,degree,mod){
    var myPoly = new polynomial(degree,mod);
    myPoly.poly = grp.createPolynomial(degree,mod);
    return myPoly
}

function AddPlayerToGroup(grp, ord, p){
    grp.AddOrdinalAndPlayerToGrp(ord, p.UserID, p.uri,p.addr,p.port);
}

function GetGlobalPlayerList(){
    var GlobalPlayerList = [];
    const PlayerListArray = bindings.GetGlobalPlayerList();
    for(i=0;i<PlayerListArray.length;++i){
        p = new player();
        p.UserID = PlayerListArray[i++];
        p.uri = PlayerListArray[i++];
        p.addr = PlayerListArray[i++];
        p.port = PlayerListArray[i];
        GlobalPlayerList.push(p);
    }
    return GlobalPlayerList;
}

function AddPlayerToGlobalList(p){
    bindings.AddPlayerToGlobalList(p.UserID,p.uri,p.addr, p.port);
}

function RemovePlayerFromGlobalList(p){
    bindings.RemovePlayerFromGlobalList(p.UserID,p.uri,p.addr, p.port);
}

function GetPlayerDetails(userid){
    const playerdetails = bindings.GetPlayerDetailsFromGlobalList(userid);
    i=0;
    
    if(playerdetails.length == 0)
        throw "No details found for player with userid " + userid;
     
    p = new player();   
    p.UserID = playerdetails[i++];
    if(i > playerdetails.length)
        throw "Invalid number of player details in GetPlayerDetails";
        
    p.uri = playerdetails[i++];
    if(i > playerdetails.length)
        throw "Invalid number of player details in GetPlayerDetails";
    p.addr = playerdetails[i++];
    if(i > playerdetails.length)
        throw "Invalid number of player details in GetPlayerDetails";
    p.port = playerdetails[i];
    
    return p;
}

module.exports = { 
    player,
    PlayerList,
    EmphemeralKeyPair,
    GetEmphemeralKeyList,
    GetPlayerList,
    createPolyWithGrp,
    AddPlayerToGroup,
    AddPlayerToGlobalList,
    GetGlobalPlayerList,
    RemovePlayerFromGlobalList,
    GetPlayerDetails,
    SinglePlayer
}

