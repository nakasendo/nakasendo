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
	        "cppsrc/Nakasendo_Bindings/BSVAddressBindings.cpp"
	        
        ],
        'include_dirs': [
            "<!@(node -p \"require('node-addon-api').include\")",
            "/usr/local/include",
            "PATH-TO-NAKASENDO-INCLUDE/include"
        ],  
        'libraries': ['-Wl,/usr/local/lib/libcrypto.a -L "PATH-TO-NAKASENDO-RELEASE/lib" -lBigNumbers -lPolynomial -lECPoint -lMessageHash -lSymEncDec -lAsymKey -lBSVAddress'],
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
