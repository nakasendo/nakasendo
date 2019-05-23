##

if(UNIX)
  ## secp256k1 use a different set of compilation flags
  sdkAddCompilerFlags(-pedantic -Wshadow -Wno-unused-function -Wno-nonnull -Wno-nonnull-compare -fPIC -fvisibility=default)
endif()

## Configure build constants ##############################################
# We don't actually need GMP
set(USE_NUM_NONE 1)
set(USE_FIELD_INV_BUILTIN 1)
set(USE_SCALAR_INV_BUILTIN 1)

# We make sure __int128 is defined
include(CheckTypeSize)
check_type_size(__int128 SIZEOF___INT128)
if(SIZEOF___INT128 EQUAL 16)
  set(HAVE___INT128 1)
else()
  # If we do not support __int128, we should be falling back
  # on 32bits implementations for field and scalar.
endif()

# Detect if we are on a 32 or 64 bits plateform and chose
# scalar and filed implementation accordingly
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  # 64 bits implementationr require either __int128 or asm support.
  # TODO: support asm.
  #if(NOT SIZEOF___INT128 EQUAL 16)
  #  message(SEND_ERROR "Compiler does not support __int128")
  #endif()

  set(USE_SCALAR_4X64 1)
  set(USE_FIELD_5X52 1)
else()
  set(USE_SCALAR_8X32 1)
  set(USE_FIELD_10X26 1)
endif()

# Recovery module
option(SECP256K1_ENABLE_MODULE_RECOVERY "Build libsecp256k1's recovery module" ON)
if(SECP256K1_ENABLE_MODULE_RECOVERY)
  set(ENABLE_MODULE_RECOVERY 1)

  #TODO: recovery benchmark
endif()

# ECDH module
option(SECP256K1_ENABLE_MODULE_ECDH "Build libsecp256k1's ECDH module" OFF)
if(SECP256K1_ENABLE_MODULE_ECDH)
  set(ENABLE_MODULE_ECDH 1)

  #TODO: ECDH benchmark
endif()

# Key puzzle  module
option(SECP256K1_ENABLE_MODULE_KEYPUZZLE "Build nchain's libsecp256k1's KeyPuzzle module" ON)
if(SECP256K1_ENABLE_MODULE_KEYPUZZLE)
  set(ENABLE_MODULE_KEYPUZZLE 1)

  #TODO: ECDH benchmark
endif()

# Generate the config
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/libsecp256k1-config.h.cmake.in" "${SDK_GENERATED_HPP_DIR}/libsecp256k1-config.h" ESCAPE_QUOTES)
#########################################################################################
#########################################################################################
set(_SECP256K1_HEADER_FILES
    ## Please keep this list in alphabet order
    "${CMAKE_CURRENT_SOURCE_DIR}/include/secp256k1.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/secp256k1_ecdh.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/secp256k1_keypuzzle.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/secp256k1_recovery.h"
)

#### Setup build for secp256k1
add_library(secp256k1 ${_SECP256K1_HEADER_FILES} src/secp256k1.cpp)

target_compile_definitions(secp256k1 PRIVATE HAVE_CONFIG_H)
if(NOT MSVC)## Add SECP256K1_BUILD macro to non windows plateform
  target_compile_definitions(secp256k1 PRIVATE SECP256K1_BUILD)
endif()
target_include_directories(secp256k1 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(secp256k1 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
sdkLinkTargetToBoost(secp256k1 Boost::boost)

set_property(TARGET secp256k1 PROPERTY FOLDER "depends")

#### Setup build testing for secp256k1
macro(_add_secp256k1_test _targetName _testFile)
  add_executable(${_targetName} ${_testFile})
  target_link_libraries(${_targetName} secp256k1)
  #sdkLinkTargetToBoost(${_targetName} Boost::boost)

  target_compile_definitions(${_targetName} PRIVATE HAVE_CONFIG_H VERIFY)

  if(MSVC)## https://stackoverflow.com/questions/6832666/lnk2019-when-including-asio-headers-solution-generated-with-cmake
    target_compile_options(${_targetName} PRIVATE "/EHsc")
  else()
    target_compile_definitions(${_targetName}  PRIVATE SECP256K1_BUILD)
  endif()

  set_property(TARGET ${_targetName} PROPERTY FOLDER "depends")
  #add_test(NAME ${_targetName} COMMAND $<TARGET_FILE:${_targetName}>) ## Do not add test for external libraries. They are responsible to test
endmacro()

_add_secp256k1_test(tests_secp256k1 src/tests.cpp)
_add_secp256k1_test(tests_secp256k1_exhaustive src/tests_exhaustive.cpp)

## Install HEADERS
foreach(_HEADER ${_SECP256K1_HEADER_FILES})
  install(FILES "${_HEADER}" DESTINATION "include/secp256k1/include" COMPONENT core)
endforeach()

# TODO: Installation
# TODO: emult static precomputation
# TODO: ecdh module
# TODO: RECOVERY module

if(UNIX)
  ## restore the old compiler flags
  sdkRecoverCompilerFlags()
endif()
