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
    AddPlayerToGroup
} = require ('./src/Nakasendo/TS.js');


var mod = 'FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141'
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


console.log(myArray);

var myInterpolator = new LGInterpolator(myArray, mod);
var newVal = '1';
console.log(myInterpolator.evaluate(newVal));
/*

newXval = '1';
var lginterp = testAddon.LGInterpolatorFull(myArray, newXval, mod, false);
console.log(lginterp);


newXval = '2';
var lginterp = testAddon.LGInterpolatorFull(myArray, newXval, mod, false);
console.log(lginterp);

var basisPt = 3;

var lginterpBasis = testAddon.LGInterpolatorSingle(myArray, newXval, mod,false,basisPt);
console.log(lginterpBasis);
*/

console.log ("Big Number stuff");
var modN = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141";

var bnA = new BigNumber(false, modN);

var bnB = new BigNumber(false, modN);

var addres = AddBigNumber(bnA, bnB);

var subres = SubBigNumber(bnA,bnB);
var mulres = MulBigNumber(bnA,bnB);
var divres = DivBigNumber(bnA,bnB);


console.log("%s + %s = %s", bnA.value, bnB.value, addres.value);

console.log("%s - %s = %s", bnA.value, bnB.value, subres.value);
console.log("%s * %s = %s", bnA.value, bnB.value, mulres.value);
console.log("%s / %s = %s", bnA.value, bnB.value, divres.value);

console.log("%s inverted is %s",bnA.value, InvBigNumber(bnA).value);



console.log("ECPoint stuff");

var ecptA = new ECPoint();
var ecptB = new ECPoint();

console.log(ecptA.toString());
console.log(ecptB.toString());

var sumPts = AddECPoint(ecptA, ecptB);
console.log(sumPts.toString());

console.log("%s is the Group Order", sumPts.GetGroupOrder());
console.log("%s is the Group Degree", sumPts.GetGroupDegree());

var onCurve = sumPts.CheckonCurve();
if(onCurve)
    console.log("%s is on the curve", sumPts.toString());
    
console.log(sumPts.GetAffineCoordinates());

var ecptC = new ECPoint();
ecptC.SetValue = ecptA.value;
var equal = EC_Compare(ecptA,ecptC);
if(equal)
    console.log("%s and %s are equal", ecptA.value, ecptC.value);
    
var ecptD = new ECPoint();
equal = EC_Compare(ecptA, ecptD);
if(!equal)
    console.log("%s and %s are not equal", ecptA.value, ecptD.value);
    
    
var ecptE = EC_Double(ecptD);
console.log("%s is double %s", ecptE.value, ecptD.value);

var ecptF = EC_Invert(ecptE);
console.log("%s is %s inverted", ecptF.value, ecptE.value);


var ecptG = EC_ScalerMultiply(ecptF,addres.value);
console.log("%s is the result of a scaler multiply of %s by %s", ecptG.value, ecptF.value, addres.toString());

var ecptH = EC_MultiplyScalerByGenerator(addres.value, ecptG.nid, ecptG.isdec, false);
console.log("%s is %s multplied by the generator(non-compressed form)", ecptH.value, addres.value);


myECArray = []
for (i=1; i<10; ++i){

    
    var ecpt = new ECPoint();
    var coords = ecpt.GetAffineCoordinates();
    console.log(coords);
    var ptx = coords['x'];
    var pty = coords['y'];
    
    myECArray.push([i.toString(),ptx, pty]);

}

console.log(myECArray);
var ecInterp = new ECLGInterpolator (myECArray,mod);
var nexVal = '1';
var interp = ecInterp.evaluate(nexVal);

console.log(interp);

var ecptInterp = new ECPoint();

ecptInterp.SetValue = interp;
console.log(ecptInterp.GetAffineCoordinates());

var valToHash = 'The brown fox jumped over the lazy dog. And then some and then some more';
console.log ('Hashing value %s', valToHash);
console.log('SHA256 %s', nakesendoBindings.SHA256(valToHash));
console.log('RIPEMM160 %s',nakesendoBindings.RIPEMD160(valToHash));
console.log('SHA256(SHA256)) %s',nakesendoBindings.SHA256(nakesendoBindings.SHA256(valToHash)));
var testHashFunc = 'RIPEMD160'
console.log('User defined hash func call %s', nakesendoBindings.HASH(valToHash, testHashFunc));
var valEncoded = nakesendoBindings.EncodeBase64(valToHash);
console.log ('Encode base-64 %s', valEncoded);
var valDecoded = nakesendoBindings.DecodeBase64(valEncoded);
console.log('Decoded base-64 %s', valDecoded);

var valEnc58 = nakesendoBindings.EncodeBase58(valToHash);
console.log('Encoded base-58 %s ' , valEnc58);
console.log('Decoded base-58 %s ', nakesendoBindings.DecodeBase58(valEnc58));

var valEnc58C = nakesendoBindings.EncodeBase58Checked(valToHash);
console.log('Encoded base-58 Checked %s', valEnc58C);
console.log('Decoded base-58 Checked %s', nakesendoBindings.DecodeBase58Checked(valEnc58C));


var myRootKey = 'joebloggs@some-email.com'
var myIV = nakesendoBindings.GenerateNounce();
var myKey = nakesendoBindings.GenerateKey(myRootKey, myIV);

console.log('my key and iv %s %s', myKey, myIV); 

var encryptedData = nakesendoBindings.encodeAES(valToHash, myKey, myIV);
console.log(encryptedData);
console.log(nakesendoBindings.decodeAES(encryptedData,myKey, myIV));


console.log('Asym Key stuff');
var myKey = new AsymKey();
console.log(myKey.toString());

var sharesOfKey = myKey.SplitKey(3,6);
console.log(sharesOfKey);
var recoveredKey = new AsymKey();
recoveredKey.RecoverKey(sharesOfKey);
console.log(recoveredKey.toString());



var msg = 'THIS IS A BIG OLE MESSAGE';
var sig = recoveredKey.sign(msg); 
console.log(sig);

var verified = verify(msg, recoveredKey.pubkey, sig[0], sig[1]);
if(verified)
    console.log('Message verified'); 

console.log('Der formatted signature');
var dersig = createDERFormat(sig[0].toString(),sig[1].toString(),false);
console.log(dersig);

console.log('Verify DER format');
var dersigVerify = verifyDER(msg,recoveredKey.pubkey,dersig);
console.log(dersigVerify);

//diffe-hellman key exchange
var AliceKey = new AsymKey();
var BobKey = new AsymKey();

console.log('diffe-hellmen shared secret generation with Alice & Bob');
console.log(AliceKey.CalculateSharedSecret(BobKey.pubkey));
console.log(BobKey.CalculateSharedSecret(AliceKey.pubkey));

console.log("BSV ADDRESS STUFF");

//derive a public key from a msg (WP-42 example)
var derivedPubK = recoveredKey.derivePublicKey(msg);
console.log(derivedPubK);
var derivedPubKasHex = pubKeyAsHexPt(derivedPubK,true);

console.log(derivedPubKasHex);
var bsvaddr = new BSVAddress(derivedPubKasHex, 0);
if(bsvaddr.valid)
    console.log(bsvaddr.address);

var ecpt = new ECPoint();
var coords = ecpt.GetAffineCoordinates();
console.log(coords);
var ptx = coords['x'];
var pty = coords['y'];
var pemPt = pubKeyHexPtstoPEM(ptx,pty);
console.log(pemPt);

module.exports = nakesendoBindings;
