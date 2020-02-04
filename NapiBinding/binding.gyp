{
    "targets": [{
        "target_name": "nakasendo",
        'cflags!': [ '-fno-exceptions' ],
        'cflags_cc!': [ '-fno-exceptions' ],
        'cflags_cc' : ['-std=c++14'],
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
            "<openssl>/include",
	    "<sdklibraries_root>/NapiBinding/cppsrc/Nakasendo_Bindings",
	    "<sdklibraries_root>/NapiBinding/cppsrc/Nakasendo_TS_Bindings",
            "<sdklibraries_root>/src/core",
	    "<installation_root>/generated/hpp",
	    "<sdklibraries_root>/src/applications/TS_protobuf/cpp",
	    "<installation_root>/generated/protobuf"
        ],  
        'libraries': ['-Wl,<openssl>/lib/libcrypto.a -L <installation_root>/x64/release -lBigNumbers -lPolynomial -lECPoint -lMessageHash -lSymEncDec -lAsymKey -lBSVAddress -lTSCore'],
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
