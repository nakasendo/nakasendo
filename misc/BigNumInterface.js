
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

function leftShift (arga, argb)
{
  const leftShiftFunc = Module.cwrap('leftShift','string',['string','string']);
  var res = leftShiftFunc(arga, argb);
  return res; 
}

function rightShift (arga, argb)
{
  const rightShiftFunc = Module.cwrap('rightShift','string',['string','string']);
  var res = rightShiftFunc(arga, argb);
  return res; 
}

function multiply(arga, argb)
{
  const multiplyFunc = Module.cwrap('multiply','string',['string','string']);
  var res = multiplyFunc(arga, argb);
  return res; 
}

function divide (arga, argb)
{
  const divideFunc = Module.cwrap('divide','string',['string','string']);
  var res = divideFunc(arga, argb);
  return res; 
}
