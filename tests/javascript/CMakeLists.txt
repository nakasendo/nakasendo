################################################################
#  Date             21/10/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

## Get all test scripts
file(GLOB _JS_TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test_*.js")

## Create each test case corresponding to a test file
foreach(_jstest_file ${_JS_TEST_FILES})
  get_filename_component(_jstest_file_name "${_jstest_file}" NAME_WE)
  string(REGEX REPLACE "test_" "js_test_" _test_name ${_jstest_file_name})

  if(CMAKE_CONFIGURATION_TYPES)
    set(_tpost_fix d)
    ## Note at the moment, on Windows Py Module can be loaded only in release build. There are problem in debug build
    add_test(NAME ${_test_name} COMMAND ${mocha_EXECUTABLE} "${_jstest_file}" -r "${SDK_JS_MODULE_LOADER_RELEASE}" --reporter mocha-junit-reporter --reporter-options mochaFile=${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/${_test_name}.xml CONFIGURATIONS Release)
    add_test(NAME ${_test_name}${_tpost_fix} COMMAND ${mocha_EXECUTABLE} "${_jstest_file}" -r "${SDK_JS_MODULE_LOADER_DEBUG}" --reporter mocha-junit-reporter --reporter-options mochaFile=${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/${_test_name}${_tpost_fix}.xml CONFIGURATIONS Debug)
  else()
    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
      set(_tpost_fix d)
    endif()
    add_test(NAME ${_test_name}${_tpost_fix} COMMAND ${mocha_EXECUTABLE} "${_jstest_file}" -r "${SDK_JS_MODULE_LOADER}" --reporter mocha-junit-reporter --reporter-options mochaFile=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_test_name}${_tpost_fix}.xml)
  endif()
endforeach()
