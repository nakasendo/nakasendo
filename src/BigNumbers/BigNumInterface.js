
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

