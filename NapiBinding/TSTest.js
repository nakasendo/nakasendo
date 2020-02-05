//index.js

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
    EmphemeralKeyPair,
    GetEmphemeralKeyList,
    GetPlayerList,
    createPolyWithGrp,
    AddPlayerToGroup,
    GetGlobalPlayerList,
    RemovePlayerFromGlobalList,
    GetPlayerDetails
} = require ('./src/Nakasendo/TS.js');


var mod = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141"
var myPoly = new polynomial(4,mod);
console.log(myPoly.toString());

var xval = new BigNumber(false, mod);
var evalx = myPoly.evaluate(xval.value);

console.log(evalx);

//var myPoly = testAddon.randomPolynomial(3,mod,false);
//console.log (myPoly);
//var xval = testAddon.generate_random(512, false);
//var xEval = testAddon.evalulatePolynomial(myPoly.poly, xval, mod, false);

//console.log(xEval);



var myArray = []

for (i=1; i<10; ++i){

    
    var myval = myPoly.evaluate(i.toString());
    
    myArray.push([i.toString(),myval]);

}


//console.log(myArray);

var myInterpolator = new LGInterpolator(myArray, mod);
var newVal = '1';
console.log(myInterpolator.evaluate(newVal));




const jvrssInstance = new nakesendoBindings.jvrssWrap();
console.log("testing function all on jvrssInstance");
if(jvrssInstance.reset()){
    console.log("Reset returned successfully");
}

console.log(jvrssInstance.printJVRSS());

console.log("copy construting")
jvrssInstance.setFX(xval.toString());
const jvrssCopy = new nakesendoBindings.jvrssWrap(jvrssInstance);
console.log(jvrssCopy.printJVRSS());

console.log("creating a playermetadatagroup");
playerGrp = new nakesendoBindings.playerGroupMetaDataWrap();
console.log(playerGrp.printPlayerGrp());


var polydegree=10;
var modN = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141"
console.log("create a playermetadatagroup with parameters")
console.log(typeof polydegree);
console.log(typeof modN);
pG = new nakesendoBindings.playerGroupMetaDataWrap(polydegree, modN);
console.log(pG.printPlayerGrp());

var randonNr = new BigNumber(false, mod);
var grpid = nakesendoBindings.SHA256(randonNr.toString())


pG.SetGrpID(grpid);


var empKeyList = GetEmphemeralKeyList(pG);
console.log(empKeyList);

var p = new player();
p.UserID  = "muprhy";
p.addr =  "127.0.0.1"
p.port = "8080";

AddPlayerToGroup(pG,1,p);

var p1 = new player();
p1.UserID = "josie";
p1.addr = "127.0.0.1";
p1.port = "8081"
AddPlayerToGroup(pG,2,p1);


var p3 = new player();
p3.UserID = "chi";
p3.addr = "127.0.0.1"
p3.port = "8082";
AddPlayerToGroup(pG,3,p3);


var poly = createPolyWithGrp(pG,2,modN);
console.log(poly.toString());

// set ordinal
pG.SetOrdinal(4);
// call preCalculationPolyNomial 
pG.PolynomialPreCalculation(poly.poly);

console.log(pG.printPlayerGrp());

console.log("GroupMetadata test");
var GrpMetaData = new nakesendoBindings.GroupMetaDataWrap("E5672",p.UserID,"",p.addr,p.port,2,3,1);
GrpMetaData.AddUserToGroup(p1.UserID, "",p1.addr,p1.port);
GrpMetaData.AddUserToGroup(p3.UserID, "",p3.addr,p3.port);

console.log(GrpMetaData.printPlayerGrp());


nakesendoBindings.AddPlayerToGlobalList(p1.UserID,"",p1.addr, p1.port);
nakesendoBindings.AddPlayerToGlobalList(p.UserID,"",p.addr,p.port);
nakesendoBindings.AddPlayerToGlobalList(p3.UserID,"",p3.addr,p3.port);

console.log(nakesendoBindings.PrintGlobalPlayerList());

console.log(GetGlobalPlayerList().toString());

RemovePlayerFromGlobalList(p1);
console.log(GetGlobalPlayerList().toString());

var playerinfo = GetPlayerDetails(p3.UserID);
console.log(playerinfo.toString());

console.log(nakesendoBindings.GenerateUUID());



var localPlayerInfo = new nakesendoBindings.playerWrap();



module.exports = nakesendoBindings;
