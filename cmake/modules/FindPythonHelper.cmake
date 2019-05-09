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

#### Recommanded to install python3
#### We need to have both python interpreter and dev lib
#### 

#### Help to find Python. It will prefer python3
macro(HelpFindPython)########################################################################################
  # find python interpreter and python dev libs
  find_package(Python3 COMPONENTS Interpreter Development)
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
