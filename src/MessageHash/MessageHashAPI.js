
function HashMsgSHA256API (argHash){
  const hashsha256func = Module.cwrap('HashMsgSHA256','string',['string']);
  var res = hashsha256func(argHash);
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
