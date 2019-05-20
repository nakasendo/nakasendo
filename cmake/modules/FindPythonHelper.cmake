#################################################################
#  Date             12/04/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################
## Include guard
if(FindPythonHelper_Include)
  return()
endif()
set(FindPythonHelper_Include TRUE)

## Recommanded to install python3
## We need to have both python interpreter and dev lib. It is preferred to install both Python3
## To build Python C extension, use
##   Python_add_library(MyPy_MODULE_NAME SOURCE_FILE1.cpp SOURCE_FILE2.cpp)
##   target_link_libraries(MyPy_MODULE_NAME PUBLIC ${my_other_dependencies})    ## Note the PRIVATE keyword here is important
##   set_target_properties(MyPy_MODULE_NAME PROPERTIES FOLDER DEBUG_POSTFIX "") ## Its better for Python module to not have debug postfix. It might creat problem when importing it
##
## Make sure all the shared libraries that the MyPy_MODULE_NAME depend on are foundable in PATH
## For linux system, make sure an empty file __init__.py is present in the same directory containing PyModule
## Write a python script
##  #!/usr/bin/env python3
##  import sys
##  sys.path.append('/absolute/path/to/directory/containing/MyPy_MODULE_NAME')
##  import MyPy_MODULE_NAME
##  # Start to use module functionalities here

#### Help to find Python. It will prefer python3
macro(HelpFindPython)########################################################################################
  # find python interpreter and python dev libs
  find_package(Python COMPONENTS Interpreter Development)
endmacro()

function(sdkPrintPythonInfo)
  sdkPrintProperties(Python::Interpreter)
  sdkPrintProperties(Python::Python)
  message(" --")
  message(" ---------- Python_FOUND [${Python_FOUND}]")
  message(" ---------- Python_EXECUTABLE [${Python_EXECUTABLE}]")
  message(" ---------- Python_Development_FOUND [${Python_Development_FOUND}]")
  message(" ---------- Python_INCLUDE_DIRS [${Python_INCLUDE_DIRS}]")
  message(" ---------- Python_LIBRARIES [${Python_LIBRARIES}]")
  message(" ---------- Python_VERSION_MAJOR [${Python_VERSION_MAJOR}]")
  message(" ---------- Python_VERSION_MINOR [${Python_VERSION_MINOR}]")
  message(" ---------- Python_VERSION_PATCH [${Python_VERSION_PATCH}]")
  message(" ---------- Python_VERSION       [${Python_VERSION}]")
endfunction()
