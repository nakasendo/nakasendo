
function BNRandom(argSize)
{
 // const BNRandFunc = Module.cwrap('BNRandomHex','string',['number']);
  //var rand = BNRandFunc(argSize);
  //return rand;

     var outputVec =Module['BNRandomHexEx'](parseInt(argSize)); 
    strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }
    return strRes;    
        
}

function BNRandomWithSeed(argSize, seedarg){
  //const BNRandWithSeedFunc = Module.cwrap('BNRandomHexWithSeed','string',['string','number']);
  //var res = BNRandWithSeedFunc(seedValue, argSize);
  //return res;
    var bufa = new ArrayBuffer(seedarg.length);
    var bufview8a = new Uint8Array(bufa); 
    for( var i=0,strLen=seedarg.length; i < strLen; i++){
        bufview8a[i] = seedarg.charCodeAt(i); 
    } 
    var inputVec = Module['returnEmptyVector'](); 
    var outputVec =Module['BNRandomHexWithSeedEx'](inputVec,parseInt(argSize)); 
    strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }
    delete bufa; 
    delete bufview8a;
    return strRes;    
}

function addFromHex (arga, argb)
{
  //const addFromHexFunc = Module.cwrap('addFromHex','string',['string','string']);
  //var res = addFromHexFunc(arga, argb);
  //return res; 
    var bufa = new ArrayBuffer(arga.length);
    var bufview8a = new Uint8Array(bufa); 
    for( var i=0,strLen=arga.length; i < strLen; i++){
        bufview8a[i] = arga.charCodeAt(i); 
    } 
    var bufb = new ArrayBuffer(argb.length);
    var bufview8b = new Uint8Array(bufb); 
    for( var i=0,strLen=argb.length; i < strLen; i++){
        bufview8b[i] = argb.charCodeAt(i); 
    } 

    var inputVecA = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8a.length; ++j){
        inputVecA.push_back(bufview8a[j]); 
    }
    var inputVecB = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8b.length; ++j){
        inputVecB.push_back(bufview8b[j]); 
    }
    var outputVec =Module['addFromHexEx'](inputVecA,inputVecB); 
    strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }

    delete bufa; 
    delete bufview8a;

    delete bufb; 
    delete bufview8b;
    delete outputVec;
    delete inputVecA;
    delete inputVecB;
    return strRes;
}



function leftShiftFromHex (arga, argb)
{
 // const leftShiftFromHexFunc = Module.cwrap('leftShiftFromHex','string',['string','string']);
  //var res = leftShiftFromHexFunc(arga, argb);
 // return res; 
    var bufa = new ArrayBuffer(arga.length);
    var bufview8a = new Uint8Array(bufa); 
    for( var i=0,strLen=arga.length; i < strLen; i++){
        bufview8a[i] = arga.charCodeAt(i); 
    } 

    var bufb = new ArrayBuffer(argb.length);
    var bufview8b = new Uint8Array(bufb); 
    for( var i=0,strLen=argb.length; i < strLen; i++){
        bufview8b[i] = argb.charCodeAt(i); 
    } 

    var inputVecA = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8a.length; ++j){
        inputVecA.push_back(bufview8a[j]); 
    }
    var inputVecB = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8b.length; ++j){
        inputVecB.push_back(bufview8b[j]); 
    }
    var outputVec =Module['leftShiftFromHexEx'](inputVecA,inputVecB); 
    strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }

    delete bufa; 
    delete bufview8a;

    delete bufb; 
    delete bufview8b;
    delete outputVec;
    delete inputVecA;
    delete inputVecB;

    return strRes;
}

function leftShiftFromDec (arga, argb)
{
    var bufa = new ArrayBuffer(arga.length);
    var bufview8a = new Uint8Array(bufa); 
    for( var i=0,strLen=arga.length; i < strLen; i++){
        bufview8a[i] = arga.charCodeAt(i); 
    } 

    var bufb = new ArrayBuffer(argb.length);
    var bufview8b = new Uint8Array(bufb); 
    for( var i=0,strLen=argb.length; i < strLen; i++){
        bufview8b[i] = argb.charCodeAt(i); 
    } 

    var inputVecA = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8a.length; ++j){
        inputVecA.push_back(bufview8a[j]); 
    }
    var inputVecB = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8b.length; ++j){
        inputVecB.push_back(bufview8b[j]); 
    }
    var outputVec =Module['leftShiftFromDecEx'](inputVecA,inputVecB); 
    strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }

    delete bufa; 
    delete bufview8a;

    delete bufb; 
    delete bufview8b;
    delete outputVec;
    delete inputVecA;
    delete inputVecB;

    return strRes;
}


function rightShiftFromHex (arga, argb)
{
    var bufa = new ArrayBuffer(arga.length);
    var bufview8a = new Uint8Array(bufa); 
    for( var i=0,strLen=arga.length; i < strLen; i++){
        bufview8a[i] = arga.charCodeAt(i); 
    } 

    var bufb = new ArrayBuffer(argb.length);
    var bufview8b = new Uint8Array(bufb); 
    for( var i=0,strLen=argb.length; i < strLen; i++){
        bufview8b[i] = argb.charCodeAt(i); 
    } 

    var inputVecA = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8a.length; ++j){
        inputVecA.push_back(bufview8a[j]); 
    }
    var inputVecB = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8b.length; ++j){
        inputVecB.push_back(bufview8b[j]); 
    }
    var outputVec =Module['rightShiftFromHexEx'](inputVecA,inputVecB); 
    strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }

    delete bufa; 
    delete bufview8a;

    delete bufb; 
    delete bufview8b;
    delete outputVec;
    delete inputVecA;
    delete inputVecB;

    return strRes;
}

function rightShiftFromDec (arga, argb)
{
    var bufa = new ArrayBuffer(arga.length);
    var bufview8a = new Uint8Array(bufa); 
    for( var i=0,strLen=arga.length; i < strLen; i++){
        bufview8a[i] = arga.charCodeAt(i); 
    } 

    var bufb = new ArrayBuffer(argb.length);
    var bufview8b = new Uint8Array(bufb); 
    for( var i=0,strLen=argb.length; i < strLen; i++){
        bufview8b[i] = argb.charCodeAt(i); 
    } 

    var inputVecA = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8a.length; ++j){
        inputVecA.push_back(bufview8a[j]); 
    }
    var inputVecB = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8b.length; ++j){
        inputVecB.push_back(bufview8b[j]); 
    }
    var outputVec =Module['rightShiftFromDecEx'](inputVecA,inputVecB); 
    strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }

    delete bufa; 
    delete bufview8a;

    delete bufb; 
    delete bufview8b;
    delete outputVec;
    delete inputVecA;
    delete inputVecB;

    return strRes;
}

function multiplyFromHex(arga, argb)
{
    var bufa = new ArrayBuffer(arga.length);
    var bufview8a = new Uint8Array(bufa); 
    for( var i=0,strLen=arga.length; i < strLen; i++){
        bufview8a[i] = arga.charCodeAt(i); 
    } 

    var bufb = new ArrayBuffer(argb.length);
    var bufview8b = new Uint8Array(bufb); 
    for( var i=0,strLen=argb.length; i < strLen; i++){
        bufview8b[i] = argb.charCodeAt(i); 
    } 

    var inputVecA = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8a.length; ++j){
        inputVecA.push_back(bufview8a[j]); 
    }
    var inputVecB = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8b.length; ++j){
        inputVecB.push_back(bufview8b[j]); 
    }
    var outputVec =Module['multiplyFromHexEx'](inputVecA,inputVecB); 
    strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }

    delete bufa; 
    delete bufview8a;

    delete bufb; 
    delete bufview8b;
    delete outputVec;
    delete inputVecA;
    delete inputVecB;

    return strRes;
}

function multiplyFromDec(arga, argb)
{
    var bufa = new ArrayBuffer(arga.length);
    var bufview8a = new Uint8Array(bufa); 
    for( var i=0,strLen=arga.length; i < strLen; i++){
        bufview8a[i] = arga.charCodeAt(i); 
    } 

    var bufb = new ArrayBuffer(argb.length);
    var bufview8b = new Uint8Array(bufb); 
    for( var i=0,strLen=argb.length; i < strLen; i++){
        bufview8b[i] = argb.charCodeAt(i); 
    } 

    var inputVecA = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8a.length; ++j){
        inputVecA.push_back(bufview8a[j]); 
    }
    var inputVecB = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8b.length; ++j){
        inputVecB.push_back(bufview8b[j]); 
    }
    var outputVec =Module['multiplyFromDecEx'](inputVecA,inputVecB); 
    strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }

    delete bufa; 
    delete bufview8a;

    delete bufb; 
    delete bufview8b;
    delete outputVec;
    delete inputVecA;
    delete inputVecB;

    return strRes;
}


function divideFromHex (arga, argb)
{
    var bufa = new ArrayBuffer(arga.length);
    var bufview8a = new Uint8Array(bufa); 
    for( var i=0,strLen=arga.length; i < strLen; i++){
        bufview8a[i] = arga.charCodeAt(i); 
    } 

    var bufb = new ArrayBuffer(argb.length);
    var bufview8b = new Uint8Array(bufb); 
    for( var i=0,strLen=argb.length; i < strLen; i++){
        bufview8b[i] = argb.charCodeAt(i); 
    } 

    var inputVecA = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8a.length; ++j){
        inputVecA.push_back(bufview8a[j]); 
    }
    var inputVecB = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8b.length; ++j){
        inputVecB.push_back(bufview8b[j]); 
    }
    var outputVec =Module['divideFromHexEx'](inputVecA,inputVecB); 
    strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }

    delete bufa; 
    delete bufview8a;

    delete bufb; 
    delete bufview8b;
    delete outputVec;
    delete inputVecA;
    delete inputVecB;

    return strRes;
}

function divideFromDec (arga, argb)
{
    var bufa = new ArrayBuffer(arga.length);
    var bufview8a = new Uint8Array(bufa); 
    for( var i=0,strLen=arga.length; i < strLen; i++){
        bufview8a[i] = arga.charCodeAt(i); 
    } 

    var bufb = new ArrayBuffer(argb.length);
    var bufview8b = new Uint8Array(bufb); 
    for( var i=0,strLen=argb.length; i < strLen; i++){
        bufview8b[i] = argb.charCodeAt(i); 
    } 

    var inputVecA = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8a.length; ++j){
        inputVecA.push_back(bufview8a[j]); 
    }
    var inputVecB = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8b.length; ++j){
        inputVecB.push_back(bufview8b[j]); 
    }
    var outputVec =Module['divideFromDecEx'](inputVecA,inputVecB); 
    strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }

    delete bufa; 
    delete bufview8a;

    delete bufb; 
    delete bufview8b;
    delete outputVec;
    delete inputVecA;
    delete inputVecB;

    return strRes;
}

function modHex(argb,argb){
    var bufa = new ArrayBuffer(arga.length);
    var bufview8a = new Uint8Array(bufa); 
    for( var i=0,strLen=arga.length; i < strLen; i++){
        bufview8a[i] = arga.charCodeAt(i); 
    } 

    var bufb = new ArrayBuffer(argb.length);
    var bufview8b = new Uint8Array(bufb); 
    for( var i=0,strLen=argb.length; i < strLen; i++){
        bufview8b[i] = argb.charCodeAt(i); 
    } 

    var inputVecA = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8a.length; ++j){
        inputVecA.push_back(bufview8a[j]); 
    }
    var inputVecB = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8b.length; ++j){
        inputVecB.push_back(bufview8b[j]); 
    }
    var outputVec =Module['Mod_HexEx'](inputVecA,inputVecB); 
    strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }

    delete bufa; 
    delete bufview8a;

    delete bufb; 
    delete bufview8b;
    delete outputVec;
    delete inputVecA;
    delete inputVecB;

    return strRes;
}

function InvModHex(arga, argb){
    var bufa = new ArrayBuffer(arga.length);
    var bufview8a = new Uint8Array(bufa); 
    for( var i=0,strLen=arga.length; i < strLen; i++){
        bufview8a[i] = arga.charCodeAt(i); 
    } 

    var bufb = new ArrayBuffer(argb.length);
    var bufview8b = new Uint8Array(bufb); 
    for( var i=0,strLen=argb.length; i < strLen; i++){
        bufview8b[i] = argb.charCodeAt(i); 
    } 

    var inputVecA = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8a.length; ++j){
        inputVecA.push_back(bufview8a[j]); 
    }
    var inputVecB = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8b.length; ++j){
        inputVecB.push_back(bufview8b[j]); 
    }
    var outputVec =Module['Inv_mod_HexEx'](inputVecA,inputVecB); 
    strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }

    delete bufa; 
    delete bufview8a;

    delete bufb; 
    delete bufview8b;
    delete outputVec;
    delete inputVecA;
    delete inputVecB;

    return strRes;
}

function AddModHex(arga, argb,argc){
    var bufa = new ArrayBuffer(arga.length);
    var bufview8a = new Uint8Array(bufa); 
    for( var i=0,strLen=arga.length; i < strLen; i++){
        bufview8a[i] = arga.charCodeAt(i); 
    } 

    var bufb = new ArrayBuffer(argb.length);
    var bufview8b = new Uint8Array(bufb); 
    for( var i=0,strLen=argb.length; i < strLen; i++){
        bufview8b[i] = argb.charCodeAt(i); 
    } 

    var bufc = new ArrayBuffer(argc.length);
    var bufview8c = new Uint8Array(bufc); 
    for( var i=0,strLen=argc.length; i < strLen; i++){
        bufview8c[i] = argc.charCodeAt(i); 
    } 

    var inputVecA = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8a.length; ++j){
        inputVecA.push_back(bufview8a[j]); 
    }
    var inputVecB = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8b.length; ++j){
        inputVecB.push_back(bufview8b[j]); 
    }

    var inputVecC = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8c.length; ++j){
        inputVecC.push_back(bufview8c[j]); 
    }
    var outputVec =Module['Add_mod_HexEx'](inputVecA,inputVecB,inputVecC); 
    strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }

    delete bufa; 
    delete bufview8a;
    delete bufb; 
    delete bufview8b;
    delete bufb; 
    delete bufview8b;

    delete outputVec;
    delete inputVecA;
    delete inputVecB;
    delete inputVecC; 
    return strRes;
}

function SubModHex(arga,argb,argc){
    var bufa = new ArrayBuffer(arga.length);
    var bufview8a = new Uint8Array(bufa); 
    for( var i=0,strLen=arga.length; i < strLen; i++){
        bufview8a[i] = arga.charCodeAt(i); 
    } 

    var bufb = new ArrayBuffer(argb.length);
    var bufview8b = new Uint8Array(bufb); 
    for( var i=0,strLen=argb.length; i < strLen; i++){
        bufview8b[i] = argb.charCodeAt(i); 
    } 

    var bufc = new ArrayBuffer(argc.length);
    var bufview8c = new Uint8Array(bufc); 
    for( var i=0,strLen=argc.length; i < strLen; i++){
        bufview8c[i] = argc.charCodeAt(i); 
    } 

    var inputVecA = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8a.length; ++j){
        inputVecA.push_back(bufview8a[j]); 
    }
    var inputVecB = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8b.length; ++j){
        inputVecB.push_back(bufview8b[j]); 
    }

    var inputVecC = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8c.length; ++j){
        inputVecC.push_back(bufview8c[j]); 
    }
    var outputVec =Module['Sub_mod_HexEx'](inputVecA,inputVecB,inputVecC); 
    strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }

    delete bufa; 
    delete bufview8a;
    delete bufb; 
    delete bufview8b;
    delete bufb; 
    delete bufview8b;

    delete outputVec;
    delete inputVecA;
    delete inputVecB;
    delete inputVecC; 
    return strRes;
}

function MulModHex(arga, argb,argc){
    var bufa = new ArrayBuffer(arga.length);
    var bufview8a = new Uint8Array(bufa); 
    for( var i=0,strLen=arga.length; i < strLen; i++){
        bufview8a[i] = arga.charCodeAt(i); 
    } 

    var bufb = new ArrayBuffer(argb.length);
    var bufview8b = new Uint8Array(bufb); 
    for( var i=0,strLen=argb.length; i < strLen; i++){
        bufview8b[i] = argb.charCodeAt(i); 
    } 

    var bufc = new ArrayBuffer(argc.length);
    var bufview8c = new Uint8Array(bufc); 
    for( var i=0,strLen=argc.length; i < strLen; i++){
        bufview8c[i] = argc.charCodeAt(i); 
    } 

    var inputVecA = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8a.length; ++j){
        inputVecA.push_back(bufview8a[j]); 
    }
    var inputVecB = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8b.length; ++j){
        inputVecB.push_back(bufview8b[j]); 
    }

    var inputVecC = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8c.length; ++j){
        inputVecC.push_back(bufview8c[j]); 
    }
    var outputVec =Module['Mul_mod_HexEx'](inputVecA,inputVecB,inputVecC); 
    strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }

    delete bufa; 
    delete bufview8a;
    delete bufb; 
    delete bufview8b;
    delete bufb; 
    delete bufview8b;

    delete outputVec;
    delete inputVecA;
    delete inputVecB;
    delete inputVecC; 
    return strRes;
}

function DivModHex(arga, argb,argc){
   var bufa = new ArrayBuffer(arga.length);
    var bufview8a = new Uint8Array(bufa); 
    for( var i=0,strLen=arga.length; i < strLen; i++){
        bufview8a[i] = arga.charCodeAt(i); 
    } 

    var bufb = new ArrayBuffer(argb.length);
    var bufview8b = new Uint8Array(bufb); 
    for( var i=0,strLen=argb.length; i < strLen; i++){
        bufview8b[i] = argb.charCodeAt(i); 
    } 

    var bufc = new ArrayBuffer(argc.length);
    var bufview8c = new Uint8Array(bufc); 
    for( var i=0,strLen=argc.length; i < strLen; i++){
        bufview8c[i] = argc.charCodeAt(i); 
    } 

    var inputVecA = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8a.length; ++j){
        inputVecA.push_back(bufview8a[j]); 
    }
    var inputVecB = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8b.length; ++j){
        inputVecB.push_back(bufview8b[j]); 
    }

    var inputVecC = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8c.length; ++j){
        inputVecC.push_back(bufview8c[j]); 
    }
    var outputVec =Module['Div_mod_HexEx'](inputVecA,inputVecB,inputVecC); 
    strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }

    delete bufa; 
    delete bufview8a;
    delete bufb; 
    delete bufview8b;
    delete bufb; 
    delete bufview8b;

    delete outputVec;
    delete inputVecA;
    delete inputVecB;
    delete inputVecC; 
    return strRes;
}



