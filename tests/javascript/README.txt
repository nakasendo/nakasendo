All javascript tests here are supposed to run with mocha. To report junit xml test result, use the option 
mocha /path/to/test.js --reporter mocha-junit-reporter --reporter-options mochaFile=/path/to/test/result.xml

At the moment, all wasm compiled modules in javascript need to be copied into the directory ./node_modules so that tests files can find them


In future, it need an automatic mechanism 
- Don't need to copy module js files
- Deal with debug postfix
Idea : maybe when running mocha test, it is possible for each test to load a common file test_config.js, where inside it, specify the way to preload, or modify the module path dynamically
and bind those loaded module to a global variable. From that, all the path and debug postfix issue are resolved.
Then each test just need to reassign the globally loaded module to a local variable, then use it.

Example test_config.js in release directory :
const global.JsBigNumbers = require('./JsBigNumbers.js');
Example test_config.js in debug directory :
const global.JsBigNumbers = require('./JsBigNumbersd.js');
Then inside the test_BigNumbers.js 
const Module = global.JsBigNumbers