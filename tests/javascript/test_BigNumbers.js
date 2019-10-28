var assert = require('assert');

//console.log('Test run in test_BigNumbers.js SDK_MODULE_PATH=' + SDK_MODULE_PATH);
//console.log('Test run in test_BigNumbers.js SDK_DEBUG_POSTFIX=' + SDK_DEBUG_POSTFIX);

module.paths.push(SDK_MODULE_PATH); //console.log(module.paths);
var JsModuleName = 'JsBigNumbers'+ SDK_DEBUG_POSTFIX +'.js'
const JSBigNumbers = require(JsModuleName);


describe('test_javascript_BigNumbers', function() {
  it('Generate random Hex', function() {
      const rand_hex_str  = JSBigNumbers.RandHex(128);
      console.log(rand_hex_str);
  });
  it('Generate random Dex', function() {
    const rand_dec_str  = JSBigNumbers.RandDec(128);
    console.log(rand_dec_str);
  });

});

// Remove the require cache
delete require.cache[require.resolve(JsModuleName)]