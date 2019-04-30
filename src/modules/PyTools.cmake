#################################################################
#  Date             25/04/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

set(PY_MODULE_BUILDER_IN "${CMAKE_CURRENT_SOURCE_DIR}/py_module_builder.py.in")

## From cmake list, return the python list (single quoted with comma separator)
function(sdkGetPythonList cmake_list output)
  string(REPLACE ";" "','" list_comma_separated "${cmake_list}")
  set(${output} "'${list_comma_separated}'" PARENT_SCOPE)
endfunction()

macro(build_py_module modulename additional_cpp_file)

  set(_MODULE_NAME ${modulename})

  if(NOT Python_FOUND)
    message(STATUS "SDK WARNS : Python was not found on your system, module ${_MODULE_NAME} for python will not be built")
    return()
  endif()

  if(NOT OPENSSL_FOUND)
    message(STATUS "SDK WARNS : OpenSSL was not found on your system, module ${_MODULE_NAME} for python will not be built")
    return()
  endif()

  set(PY_MODULE_NAME Py${_MODULE_NAME})

  set(CPP_INCLUDE_DIR_LIST ${SDK_GENERATED_HPP_DIR} ${CMAKE_SOURCE_DIR}/src/core ${CMAKE_CURRENT_SOURCE_DIR} ${OPENSSL_INCLUDE_DIR} ${Python_INCLUDE_DIRS})
  sdkGetPythonList("${CPP_INCLUDE_DIR_LIST}" PY_LIST_INCLUDE_DIRS)

  get_property(_openssl_ssl TARGET OpenSSL::SSL PROPERTY LOCATION)
  get_property(_openssl_crypto TARGET OpenSSL::Crypto PROPERTY LOCATION)
  set(CPP_EXTERN_LIBRARIES_LIST ${_openssl_ssl} ${_openssl_crypto})
  sdkGetPythonList("${CPP_EXTERN_LIBRARIES_LIST}" PY_LIST_LIBRARIES)

  set(CPP_LIST_SOURCE_FILES ${${_MODULE_NAME}_CPP_FILES} ${additional_cpp_file})
  sdkGetPythonList("${CPP_LIST_SOURCE_FILES}" PY_LIST_SOURCE_FILES)

  if(WIN32)
    set(CPP_LIST_NATIVE_LIBRARIES CRYPT32 WS2_32 GDI32 ADVAPI32 USER32)## Native libraries dependencies of OpenSSL on Windows
    sdkGetPythonList("${CPP_LIST_NATIVE_LIBRARIES}" PY_LIST_NATIVE_LIBRARIES)
  endif()

  set(PyBuilderScriptName Py${_MODULE_NAME}Builder.py)
  set(Py_TARGET_NAME Py${_MODULE_NAME})
  add_custom_target(${Py_TARGET_NAME} ALL SOURCES ${CPP_LIST_SOURCE_FILES})
  set_property(TARGET ${Py_TARGET_NAME} PROPERTY FOLDER "modules") ## Set Folder for MSVC
  if(CMAKE_CONFIGURATION_TYPES)
   configure_file(${PY_MODULE_BUILDER_IN} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/${PyBuilderScriptName})
    configure_file(${PY_MODULE_BUILDER_IN} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/${PyBuilderScriptName})
    file(TO_NATIVE_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/${PyBuilderScriptName} _Py_BUILDER_DEBUG)
    file(TO_NATIVE_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/${PyBuilderScriptName} _Py_BUILDER_RELEASE)
    file(TO_NATIVE_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/Build${Py_TARGET_NAME} _Py_BUILD_DIR_DEBUG)
    file(TO_NATIVE_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/Build${Py_TARGET_NAME} _Py_BUILD_DIR_RELEASE)
    file(TO_NATIVE_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/${Py_TARGET_NAME} _Py_INSTALL_DIR_DEBUG)
    file(TO_NATIVE_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/${Py_TARGET_NAME} _Py_INSTALL_DIR_RELEASE)
    add_custom_command(TARGET ${Py_TARGET_NAME} POST_BUILD
                       COMMAND ${Python_EXECUTABLE} "$<IF:$<CONFIG:Debug>,${_Py_BUILDER_DEBUG},${_Py_BUILDER_RELEASE}>"
                       build --build-base="$<IF:$<CONFIG:Debug>,${_Py_BUILD_DIR_DEBUG},${_Py_BUILD_DIR_RELEASE}>"
                       install --prefix="$<IF:$<CONFIG:Debug>,${_Py_INSTALL_DIR_DEBUG},${_Py_INSTALL_DIR_RELEASE}>"
    )
    install(DIRECTORY ${_Py_INSTALL_DIR_DEBUG} DESTINATION "lib" CONFIGURATIONS Debug)
    install(DIRECTORY ${_Py_INSTALL_DIR_RELEASE} DESTINATION "lib" CONFIGURATIONS Release)
  else()
    configure_file(${PY_MODULE_BUILDER_IN} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PyBuilderScriptName})
    add_custom_command(TARGET ${Py_TARGET_NAME} POST_BUILD
                       COMMAND ${Python_EXECUTABLE} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PyBuilderScriptName} build
                       build --build-base="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Build${Py_TARGET_NAME}"
                       install --prefix="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${Py_TARGET_NAME}"
    )
    install(DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${Py_TARGET_NAME}" DESTINATION "lib")
  endif()
endmacro()