################################################################
#  Date             23/05/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################
message("SDK INFO : Functional tests will not run in Debug mode because actually PyModules can not be loaded in debug")
message("SDK INFO : To add a functional test, create a file name test_MyTestName.py")
message("SDK INFO : import TestArgsParser")
message("SDK INFO : To run a single functional test :")
message("SDK INFO :    python test_MyTestName.py --data_dir=/absolute/path/to/data/dir --module_dir=/absolute/path/to/PyModules/dir")

set(FTEST_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test_data")
## Get all test scripts
file(GLOB _FUNCTIOINAL_TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test_*.py")

## Create each test case corresponding to a test file
foreach(_ftest_file ${_FUNCTIOINAL_TEST_FILES})
  get_filename_component(_ftest_file_name "${_ftest_file}" NAME_WE)
  string(REGEX REPLACE "test_" "ftest_" _test_name ${_ftest_file_name})

  if(CMAKE_CONFIGURATION_TYPES)
    ## Note at the moment, on Windows Py Module can be loaded only in release build. There are problem in debug build
    add_test(NAME ${_test_name} COMMAND  ${Python_EXECUTABLE} -m pytest -s ${_ftest_file} --rootdir=${CMAKE_CURRENT_SOURCE_DIR} --data_dir=${FTEST_DATA_DIR} --module_dir=${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE} CONFIGURATIONS Release)
  else()
    add_test(NAME ${_test_name} COMMAND ${Python_EXECUTABLE} -m pytest -s ${_ftest_file} --rootdir=${CMAKE_CURRENT_SOURCE_DIR} --data_dir=${FTEST_DATA_DIR} --module_dir=${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
  endif()
endforeach()
