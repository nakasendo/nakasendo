################################################################
#  Date             04/09/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

file(GLOB _THRESHOLD_SIGNATURE_TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test/test_*.py")

## Create each test case corresponding to a test file
foreach(_ftest_file ${_THRESHOLD_SIGNATURE_TEST_FILES})
  get_filename_component(_ftest_file_name "${_ftest_file}" NAME_WE)
  string(REGEX REPLACE "test_" "testTS_" _test_name ${_ftest_file_name})

  if(CMAKE_CONFIGURATION_TYPES)
  set(_tpost_fix d)
    ## Note at the moment, on Windows Py Module can be loaded only in release build. There are problem in debug build
    add_test(NAME ${_test_name} COMMAND  ${Python_EXECUTABLE} -m pytest -s ${_ftest_file} -o junit_suite_name=${_test_name} --junitxml=${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/${_test_name}.xml CONFIGURATIONS Release)
    ## To be able to run python on 2 configurations platform type, user need to add to PATH the location where python_d is
    add_test(NAME ${_test_name}${_tpost_fix} COMMAND  python_d -m pytest -s ${_ftest_file} -o junit_suite_name=${_test_name}${_tpost_fix} --junitxml=${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/${_test_name}${_tpost_fix}.xml CONFIGURATIONS Debug)
  else()
    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
      set(_tpost_fix d)
    endif()
    add_test(NAME ${_test_name}${_tpost_fix} COMMAND ${Python_EXECUTABLE} -m pytest -s ${_ftest_file} -o junit_suite_name=${_test_name}${_tpost_fix} --junitxml=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_test_name}${_tpost_fix}.xml)
  endif()
endforeach()
