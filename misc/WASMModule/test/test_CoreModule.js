// npm install --global mocha mocha-junit-reporter
// Copy this javascript to the same directory of WASMCoreModule.js
// mocha /abs/path/to/test_CoreModule.js --reporter-options mochaFile=/test_CoreModule.xml
const Module = require('./WASMCoreModule.js');

var assert = require('assert');
describe('Example test suite', function() {
  describe('Example test case', function() {
    it('should return -1 when the value is not present', function() {
      assert.equal([1, 2, 3].indexOf(4), -1);
    });
  });
});

describe('Example test suite', function() {
  describe('Example test case', function() {
    it('test imported lerf', function() {
        const test_lerp = Module.lerp(1, 2, 0.5);
        assert.equal(test_lerp, 1.5);
    });
    it('test imported sayhello', function() {
        const test_sayhello = Module.say_hello("nChain");
        assert.equal(test_sayhello, "hello nChain");
    });
    it('test hash 256', function() {
        const test_hash1 = Module.hash_sha256_1("nChain Bitcoin SV");
        const test_hash2 = Module.hash_sha256_2("nChain Bitcoin SV");
        assert.equal(test_hash1, "AFAAB95D36DD2D1462225A274708CD98C2BA176627CAE94FA5934474BD3076DD");
        assert.equal(test_hash2, "AFAAB95D36DD2D1462225A274708CD98C2BA176627CAE94FA5934474BD3076DD");
    });
    it('test print log', function() {
        console.log('lerp result: ' + Module.lerp(1, 2, 0.5));
        console.log(Module.say_hello("nChain"));
        console.log("hash_sha256_1=" + Module.hash_sha256_1("nChain Bitcoin SV"));
        console.log("hash_sha256_2=" + Module.hash_sha256_2("nChain Bitcoin SV"));
    });
  });
});
