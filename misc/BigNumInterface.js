
function BNRandom(argSize)
{
 // const BNRandFunc = Module.cwrap('BNRandomHex','string',['number']);
  //var rand = BNRandFunc(argSize);
  //return rand;

    var outputVec =Module['BNRandomHexEx'](parseInt(argSize)); 
    var strRes = ""
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
    bufa=null; 
    bufview8a=null;
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
    var strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }

    bufa=null; 
    bufview8a=null;

    bufb=null; 
    bufview8b=null;
    outputVec=null;
    inputVecA=null;
    inputVecB=null;
    return strRes;
}


function subFromHex (arga, argb)
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
    var outputVec =Module['subFromHexEx'](inputVecA,inputVecB); 
    var strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }

    bufa=null; 
    bufview8a=null;

    bufb=null; 
    bufview8b=null;
    outputVec=null;
    inputVecA=null;
    inputVecB=null;
    return strRes;
}


function addFromDec (arga, argb)
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
    var outputVec =Module['addFromDecEx'](inputVecA,inputVecB); 
    var strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }

    bufa=null; 
    bufview8a=null;

    bufb=null; 
    bufview8b=null;
    outputVec=null;
    inputVecA=null;
    inputVecB=null;
    return strRes;
}

function subFromDec (arga, argb)
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
    var outputVec =Module['subFromDecEx'](inputVecA,inputVecB); 
    var strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }

    bufa=null; 
    bufview8a=null;

    bufb=null; 
    bufview8b=null;
    outputVec=null;
    inputVecA=null;
    inputVecB=null;
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
    var strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }

    bufa=null; 
    bufview8a=null;

    bufb=null; 
    bufview8b=null;
    outputVec=null;
    inputVecA=null;
    inputVecB=null;

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
    var strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }

    bufa=null; 
    bufview8a=null;

    bufb=null; 
    bufview8b=null;
    outputVec=null;
    inputVecA=null;
    inputVecB=null;

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
    var strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }

    bufa=null; 
    bufview8a=null;

    bufb=null; 
    bufview8b=null;
    outputVec=null;
    inputVecA=null;
    inputVecB=null;

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
    var strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }

    bufa=null; 
    bufview8a=null;
    bufb=null; 
    bufview8b=null;
    
    outputVec=null;
    inputVecA=null;
    inputVecB=null;

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
    var strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }

    bufa=null; 
    bufview8a=null;

    bufb=null; 
    bufview8b=null;
    outputVec=null;
    inputVecA=null;
    inputVecB=null;

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
    var strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }

    bufa=null; 
    bufview8a=null;

    bufb=null; 
    bufview8b=null;
    outputVec=null;
    inputVecA=null;
    inputVecB=null;

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
    var strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }

    bufa=null; 
    bufview8a=null;

    bufb=null; 
    bufview8b=null;
    outputVec=null;
    inputVecA=null;
    inputVecB=null;

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
    var strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }

    bufa=null; 
    bufview8a=null;

    bufb=null; 
    bufview8b=null;
    outputVec=null;
    inputVecA=null;
    inputVecB=null;

    return strRes;
}



function modHex(arga,argb){
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
    var strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }

    bufa=null; 
    bufview8a=null;
    bufb=null; 
    bufview8b=null;
    outputVec=null;
    inputVecA=null;
    inputVecB=null;

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
    var strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }

    bufa=null; 
    bufview8a=null;

    bufb=null; 
    bufview8b=null;
    outputVec=null;
    inputVecA=null;
    inputVecB=null;

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
    var strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }

    bufa=null; 
    bufview8a=null;
    bufb=null; 
    bufview8b=null;

    outputVec=null;
    inputVecA=null;
    inputVecB=null;
    inputVecC=null; 
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
    var strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }

    bufa=null; 
    bufview8a=null;
    bufb=null; 
    bufview8b=null;

    outputVec=null;
    inputVecA=null;
    inputVecB=null;
    inputVecC=null; 
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
    var strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }

    bufa=null; 
    bufview8a=null;
    bufb=null; 
    bufview8b=null;

    outputVec=null;
    inputVecA=null;
    inputVecB=null;
    inputVecC=null; 
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
    var strRes = ""
    for (var k=0;k<outputVec.size();++k){
        strRes = strRes + String.fromCharCode(outputVec.get(k) ); 
    }

    bufa=null; 
    bufview8a=null;
    bufb=null; 
    bufview8b=null;

    outputVec=null;
    inputVecA=null;
    inputVecB=null;
    inputVecC=null; 
    return strRes;
}


// Currently implemented in JavaScript by forwarding directly to low level function
// Consider implementing it by forwarding to C++ class
// Consider storing value in C++ to avoid copying around (also for low level functions)
export class BigNumber
 {
    constructor(theValue = "0") {
        this.value = theValue;
    }
    random(argSize) {
        return new BigNumber(BNRandom(argSize));
    }
    add(other) {
        return new BigNumber(addFromHex(this.value, other.value));
    }
    sub(other) {
        return new BigNumber(subFromHex(this.value, other.value));
    }
    multiply(other) {
        return new BigNumber(multiplyFromHex(this.value, other.value));
    }
    divide(other) {
        return new BigNumber(divideFromHex(this.value, other.value));
    }
    toString() {
        return this.value;
    }
}




