
function BNRandom(argSize)
{
  const BNRandFunc = Module.cwrap('BNRandomHex','string',['number']);
  var rand = BNRandFunc(argSize);
  return rand;
}

function BNRandomWithSeed(argSize, seedValue){
  const BNRandWithSeedFunc = Module.cwrap('BNRandomHexWithSeed','string',['string','number']);
  var res = BNRandWithSeedFunc(seedValue, argSize);
  return res;
}

function addFromHex (arga, argb)
{
  const addFromHexFunc = Module.cwrap('addFromHex','string',['string','string']);
  var res = addFromHexFunc(arga, argb);
  return res; 
}


function leftShiftFromHex (arga, argb)
{
  const leftShiftFromHexFunc = Module.cwrap('leftShiftFromHex','string',['string','string']);
  var res = leftShiftFromHexFunc(arga, argb);
  return res; 
}

function leftShiftFromDec (arga, argb)
{
  const leftShiftFromDecFunc = Module.cwrap('leftShiftFromDec','string',['string','string']);
  var res = leftShiftFromDecFunc(arga, argb);
  return res; 
}


function rightShiftFromHex (arga, argb)
{
  const rightShiftFromHexFunc = Module.cwrap('rightShiftFromHex','string',['string','string']);
  var res = rightShiftFromHexFunc(arga, argb);
  return res; 
}

function rightShiftFromDec (arga, argb)
{
  const rightShiftFromDecFunc = Module.cwrap('rightShiftFromDec','string',['string','string']);
  var res = rightShiftFromDecFunc(arga, argb);
  return res; 
}

function multiplyFromHex(arga, argb)
{
  const multiplyFromHexFunc = Module.cwrap('multiplyFromHex','string',['string','string']);
  var res = multiplyFromHexFunc(arga, argb);
  return res; 
}

function multiplyFromDec(arga, argb)
{
  const multiplyFromDecFunc = Module.cwrap('multiplyFromDec','string',['string','string']);
  var res = multiplyFromDecFunc(arga, argb);
  return res; 
}


function divideFromHex (arga, argb)
{
  const divideFromHexFunc = Module.cwrap('divideFromHex','string',['string','string']);
  var res = divideFromHexFunc(arga, argb);
  return res; 
}

function divideFromDec (arga, argb)
{
  const divideFromDecFunc = Module.cwrap('divideFromDec','string',['string','string']);
  var res = divideFromDecFunc(arga, argb);
  return res; 
}
