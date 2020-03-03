################################################################
#  Date             23/05/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################
message("SDK INFO : To add a functional test, create a file name test_MyTestName.py")
message("SDK INFO : pytest, are required")
message("SDK INFO : To run a single functional test :")
message("SDK INFO :    python test_MyTestName.py --data_dir=/absolute/path/to/data/dir --module_dir=/absolute/path/to/PyModules/dir1,/absolute/path/to/PyModules/dir2")
message("SDK INFO : To run with debug build on windows, remember to use python_d instead of python")

set(FTEST_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test_data")
## Get all test scripts
file(GLOB _FUNCTIONAL_TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test_*.py")

## Create each test case corresponding to a test file
foreach(_ftest_file ${_FUNCTIONAL_TEST_FILES})
  get_filename_component(_ftest_file_name "${_ftest_file}" NAME_WE)
  string(REGEX REPLACE "test_" "py_test_" _test_name ${_ftest_file_name})

  if(CMAKE_CONFIGURATION_TYPES)
  set(_tpost_fix d)
    ## Note at the moment, on Windows Py Module can be loaded only in release build. There are problem in debug build
    add_test(NAME ${_test_name} COMMAND  ${Python_EXECUTABLE} -m pytest -s ${_ftest_file} -o junit_suite_name=${_test_name} --rootdir=${CMAKE_CURRENT_SOURCE_DIR} --data_dir=${FTEST_DATA_DIR} --module_dir=${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE},${CMAKE_SOURCE_DIR}/src/modules/PyNakasendo,${CMAKE_BINARY_DIR}/generated/protobuf,${CMAKE_SOURCE_DIR}/src/applications/TS_protobuf/py --junitxml=${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/${_test_name}.xml CONFIGURATIONS Release)
    ## To be able to run python on 2 configurations platform type, user need to add to PATH the location where python_d is
    add_test(NAME ${_test_name}${_tpost_fix} COMMAND  python_d -m pytest -s ${_ftest_file} -o junit_suite_name=${_test_name}${_tpost_fix} --rootdir=${CMAKE_CURRENT_SOURCE_DIR} --data_dir=${FTEST_DATA_DIR} --module_dir=${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG},${CMAKE_SOURCE_DIR}/src/modules/PyNakasendo,${CMAKE_BINARY_DIR}/generated/protobuf,${CMAKE_SOURCE_DIR}/src/applications/TS_protobuf/py --junitxml=${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/${_test_name}${_tpost_fix}.xml CONFIGURATIONS Debug)
  else()
    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
      set(_tpost_fix d)
    endif()
    add_test(NAME ${_test_name}${_tpost_fix} COMMAND ${Python_EXECUTABLE} -m pytest -s ${_ftest_file} -o junit_suite_name=${_test_name}${_tpost_fix} --rootdir=${CMAKE_CURRENT_SOURCE_DIR} --data_dir=${FTEST_DATA_DIR} --module_dir=${CMAKE_RUNTIME_OUTPUT_DIRECTORY},${CMAKE_SOURCE_DIR}/src/modules/PyNakasendo,${CMAKE_BINARY_DIR}/generated/protobuf,${CMAKE_SOURCE_DIR}/src/applications/TS_protobuf/py --junitxml=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_test_name}${_tpost_fix}.xml)
  endif()
endforeach()
