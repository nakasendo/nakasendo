{
    "targets": [{
        "target_name": "nakasendo",
        'cflags!': [ '-fno-exceptions' ],
        'cflags_cc!': [ '-fno-exceptions' ],
        "sources": [
            "cppsrc/main.cpp",
	        "cppsrc/Nakasendo_Bindings/ecpointBindings.cpp",
	        "cppsrc/Nakasendo_Bindings/polynomialBindings.cpp",
	        "cppsrc/Nakasendo_Bindings/bignumBindings.cpp",
	        "cppsrc/Nakasendo_Bindings/messageHashBindings.cpp",
	        "cppsrc/Nakasendo_Bindings/SymEncDecBindings.cpp",
	        "cppsrc/Nakasendo_Bindings/AsymKeyBindings.cpp",
	        "cppsrc/Nakasendo_Bindings/BSVAddressBindings.cpp",
		"cppsrc/Nakasendo_TS_Bindings/playerWrapper.cpp",
		"cppsrc/Nakasendo_TS_Bindings/GroupMetaDataWrapper.cpp",
		"cppsrc/Nakasendo_TS_Bindings/OrchestratorWrapper.cpp"
        ],
        'include_dirs': [
            "<!@(node -p \"require('node-addon-api').include\")",
            "/usr/local/include",
	    "/Users/j.murphy/nChain/SDK-CLEAN/sdklibraries-john/NapiBinding/cppsrc/Nakasendo_Bindings",
	    "/Users/j.murphy/nChain/SDK-CLEAN/sdklibraries-john/NapiBinding/cppsrc/Nakasendo_TS_Bindings",
            "/Users/j.murphy/nChain/SDK-CLEAN/sdklibraries-john/src/core",
	    "/Users/j.murphy/nChain/SDK-CLEAN/build/generated/hpp",
	    "/Users/j.murphy/nChain/SDK-CLEAN/sdklibraries-john/src/applications/TS_protobuf/cpp",
	    "/Users/j.murphy/nChain/SDK-CLEAN/build/generated/protobuf"
        ],  
        'libraries': ['-Wl,/usr/local/lib/libcrypto.a -L /Users/j.murphy/nChain/SDK-CLEAN/build/x64/release -lBigNumbers -lPolynomial -lECPoint -lMessageHash -lSymEncDec -lAsymKey -lBSVAddress -lTSCore'],
        'dependencies': [
            "<!(node -p \"require('node-addon-api').gyp\")"
        ],
        'conditions': [
            ['OS=="mac"', {
                'xcode_settings': {
                'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
                }
            }]
        ],
        'defines': [ 'NAPI_CPP_EXCEPTIONS' ]
    }]
}
