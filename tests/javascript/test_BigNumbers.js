// Find a way to do const Module = global.JsBigNumbers
const Module = require('JsBigNumbers.js');

var assert = require('assert');
/*
describe('Example test suite', function() {
  describe('Example test case', function() {
    it('should return -1 when the value is not present', function() {
      assert.equal([1, 2, 3].indexOf(4), -1);
    });
  });
});
*/

describe('Random generator', function() {
  it('Generate random Hex', function() {
      const rand_hex_str  = Module.RandHex(128);
      console.log(rand_hex_str);
  });
  it('Generate random Dex', function() {
    const rand_dec_str  = Module.RandDec(128);
    console.log(rand_dec_str);
  });

});
