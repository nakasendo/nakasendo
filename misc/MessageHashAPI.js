
function HashMsgSHA256API (argHash){
  const hashsha256func = Module.cwrap('HashMsgSHA256Test','string',['string']);
  var argLen = lengthBytesUTF8(argHash); 
  var strOnHeap = _malloc(argLen+1);
  stringToUTF8(argHash, strOnHeap, argLen+1);
    console.log(strOnHeap);
 // var res = hashsha256func(strOnHeap); 
    var res = Module.ccall('HashMsgSHA256Test','string',['number'],[strOnHeap]);
    _free(strOnHeap);
  return res;
}
  
function HashMsg (argHash, argHashFunc){
  const hashfunc = Module.cwrap('HashMsg','string',['string']);
  var res = hashfunc(argHash, argHashFunc);
  return res;
}

function ListHashFunc(){
  const listhashfunc = Module.cwrap('ListHashFunc','string',[]);
  var res = listhashfunc();
  return res; 
}

function EncodeBase64(argToEncode){
    const encodebase64 = Module.cwrap('EncodeBase64','string',['string']);
    var argLen = lengthBytesUTF8(argToEncode); 
    console.log(argLen);
    var strOnHeap = _malloc(argLen+1);
    stringToUTF8(argToEncode, strOnHeap, argLen+1);
    console.log(strOnHeap);
    let res = Module.ccall('EncodeBase64','string',['number','number'],[strOnHeap,argLen]);

    var buf = new ArrayBuffer(argToEncode.length);
    var bufview8 = new Uint8Array(buf); 
    for( var i=0,strLen=argToEncode.length; i < strLen; i++){
        bufview8[i] = argToEncode.charCodeAt(i); 
    } 
    var inputVec = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8.length; ++j){
        inputVec.push_back(bufview8[j]); 
    }
    
    var outputVec =Module['EncodeBase64Ex'](inputVec); 
    
    str1 = ""
    for ( var len = 0; len < outputVec.size(); ++len){
        str1 = str1 + String.fromCharCode(outputVec.get(len) ); 
    }
    _free(strOnHeap);       
    
    return str1;
}

function PassStrViaMemory(argToPass){
    var str = argToPass;
    var strlen = lengthBytesUTF8(argToPass); 
    var strOnHeap = _malloc(strlen+1);
    stringToUTF8(argToPass, strOnHeap, strlen+1);
    Module.ccall('printValue','null',['number'],[strOnHeap]);
    _free(strOnHeap);
}

function DecodeBase64(argToDecode){
    //const decodebase64 = Module.cwrap('DecodeBase64','string',['string']);
    //var res = decodebase64(argToDecode);
    var argLen = lengthBytesUTF8(argToDecode);
    console.log (argLen); 
    var strOnHeap = _malloc(argLen+1); 
    stringToUTF8(argToDecode, strOnHeap, argLen+1);
    var res = Module.ccall('DecodeBase64','string',['number','number'],[strOnHeap,argLen]);
    console.log(lengthBytesUTF8(res)); 
    _free (strOnHeap); 
    var buf = new ArrayBuffer(argToDecode.length);
    var bufview8 = new Uint8Array(buf); 
    for( var i=0,strLen=argToDecode.length; i < strLen; i++){
        bufview8[i] = argToDecode.charCodeAt(i); 
    } 

     var inputVec = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8.length; ++j){
        inputVec.push_back(bufview8[j]); 
    }
    var outputVec =Module['DecodeBase64Ex'](inputVec); 
    
    str1 = ""
    for ( var len = 0; len < outputVec.size(); ++len){
        str1 = str1 + String.fromCharCode(outputVec.get(len) ); 
    }
    return str1;
}

function EncodeBase58(argToEncode){
    var buf = new ArrayBuffer(argToEncode.length);
    var bufview8 = new Uint8Array(buf); 
    for( var i=0,strLen=argToEncode.length; i < strLen; i++){
        bufview8[i] = argToEncode.charCodeAt(i); 
    } 
    var inputVec = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8.length; ++j){
        inputVec.push_back(bufview8[j]); 
    }
    
    var outputVec =Module['EncodeBase58Ex'](inputVec); 
    
    str1 = ""
    for ( var len = 0; len < outputVec.size(); ++len){
        str1 = str1 + String.fromCharCode(outputVec.get(len) ); 
    }
    
    return str1;
}

function DecodeBase58(argToDecode){

    var buf = new ArrayBuffer(argToDecode.length);
    var bufview8 = new Uint8Array(buf); 
    for( var i=0,strLen=argToDecode.length; i < strLen; i++){
        bufview8[i] = argToDecode.charCodeAt(i); 
    } 

     var inputVec = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8.length; ++j){
        inputVec.push_back(bufview8[j]); 
    }
    
    var outputVec =Module['DecodeBase58Ex'](inputVec); 
    
    str1 = ""
    for ( var len = 0; len < outputVec.size(); ++len){
        str1 = str1 + String.fromCharCode(outputVec.get(len) ); 
    }
    return str1;
}

function EncodeBase58Checked(argToEncode){
 var buf = new ArrayBuffer(argToEncode.length);
    var bufview8 = new Uint8Array(buf); 
    for( var i=0,strLen=argToEncode.length; i < strLen; i++){
        bufview8[i] = argToEncode.charCodeAt(i); 
    } 
    var inputVec = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8.length; ++j){
        inputVec.push_back(bufview8[j]); 
    }
    
    var outputVec =Module['EncodeBase58CheckEx'](inputVec); 
    
    str1 = ""
    for ( var len = 0; len < outputVec.size(); ++len){
        str1 = str1 + String.fromCharCode(outputVec.get(len) ); 
    }
    
    return str1;
}

function DecodeBase58Checked(argToDecode){

    var buf = new ArrayBuffer(argToDecode.length);
    var bufview8 = new Uint8Array(buf); 
    for( var i=0,strLen=argToDecode.length; i < strLen; i++){
        bufview8[i] = argToDecode.charCodeAt(i); 
    } 

     var inputVec = Module['returnEmptyVector'](); 
    for (var j = 0; j<bufview8.length; ++j){
        inputVec.push_back(bufview8[j]); 
    }
    
    var outputVec =Module['DecodeBase58CheckEx'](inputVec); 
    
    str1 = ""
    for ( var len = 0; len < outputVec.size(); ++len){
        str1 = str1 + String.fromCharCode(outputVec.get(len) ); 
    }
    return str1;
}
function StrConCat(a,b) {
  // init inputs
  //var a = "hello "
 //var b = "world"

  //convert to ascii array
  a_ascii = [];
  for (var i = 0; i < a.length; i ++)
    a_ascii.push(a[i].charCodeAt(0));
  b_ascii = [];
  for (var i = 0; i < b.length; i ++)
    b_ascii.push(b[i].charCodeAt(0));

  //allocate Float32Array and fill with ascii data
  const a_typedArray = new Float32Array(a_ascii.length)
  for (let i=0; i<a_ascii.length; i++) {
    a_typedArray[i] = a_ascii[i]
  }
  const b_typedArray = new Float32Array(b_ascii.length)
  for (let i=0; i<b_ascii.length; i++) {
    b_typedArray[i] = b_ascii[i]
  }

  /* --Make WASM call to concatenate the strings-- */

  //allocate memory on wasm heap
  a_buffer = Module._malloc(a_typedArray.length * a_typedArray.BYTES_PER_ELEMENT)
  Module.HEAPF32.set(a_typedArray, a_buffer >> 2)

  b_buffer = Module._malloc(b_typedArray.length * b_typedArray.BYTES_PER_ELEMENT)
  Module.HEAPF32.set(b_typedArray, b_buffer >> 2)

  //call wasm func
  //var t0 = performance.now();
  var result = Module._concat(a_buffer, b_buffer, a.length, b.length)
  //var t1 = performance.now();

  /* ----- */

  // fill the result to a js array
  const arrayData = []
//  for (let pointer=0; pointer<(a.length + b.length); pointer++) {
    for (let pointer=0; pointer<11; pointer++) {
    arrayData.push(Module.HEAPF32[result/(Float32Array.BYTES_PER_ELEMENT+pointer)])
  }
  //convert ascii array to string and display the result
  var res_string = String.fromCharCode.apply(null, arrayData)
  console.log(`Input: a: "${a}", b: "${b}"`)
  console.log(`Result on concat: "${res_string}"`)
  //console.log("Call to concat using wasm took " + (t1 - t0) + " milliseconds.")
}

