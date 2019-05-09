#################################################################
#  Date             25/04/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

set(PY_MODULE_BUILDER_IN "${CMAKE_CURRENT_SOURCE_DIR}/py_module_builder.py.in")

## From cmake list, return the python list (single quoted with comma separator)
function(sdkGetDoubleQuoted cmake_list output)
  ## Only Win32 need to protect by double quote
  if(WIN32)
     string(REPLACE ";" "\" \"" list_comma_separated "${cmake_list}")
     set(${output} "\"${list_comma_separated}\"" PARENT_SCOPE)
  else()
     set(${output} ${cmake_list} PARENT_SCOPE)
  endif()
endfunction()

## For all core module, get the list of link libraries
function(getListOfCoreLinkage output)
  set(_output "")
  foreach(_core_component ${SDK_CORE_COMPONENT_LIST})
    if(NOT TARGET ${_core_component})
      message(FATAL_ERROR "SDK error ${_core_component} is not indentified as target in cmake system")
    endif()
    list(APPEND _output "\$<TARGET_LINKER_FILE:${_core_component}>")
  endforeach()
  set(${output} "${_output}" PARENT_SCOPE)
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

  set(PYMODULE_NAME Py${_MODULE_NAME})
  set(CPP_INCLUDE_DIR_LIST ${SDK_GENERATED_HPP_DIR} ${CMAKE_SOURCE_DIR}/src/core ${CMAKE_CURRENT_SOURCE_DIR} ${OPENSSL_INCLUDE_DIR} ${Python_INCLUDE_DIRS})
  sdkGetDoubleQuoted("${CPP_INCLUDE_DIR_LIST}" PYMODULE_INCLUDE_DIR_LIST)

  get_property(_openssl_ssl TARGET OpenSSL::SSL PROPERTY LOCATION)
  get_property(_openssl_crypto TARGET OpenSSL::Crypto PROPERTY LOCATION)
  set(CPP_EXTERN_LIBRARIES_LIST ${_openssl_ssl} ${_openssl_crypto})
  getListOfCoreLinkage(CPP_CORE_DEPENDENCIES_LIST)
  set(CPP_DEPENDENCIES_LIST ${CPP_EXTERN_LIBRARIES_LIST} ${CPP_CORE_DEPENDENCIES_LIST})
  sdkGetDoubleQuoted("${CPP_DEPENDENCIES_LIST}" PYMODULE_DEPENDENCIES_LIST)

  set(CPP_LIST_SOURCE_FILES ${additional_cpp_file})
  sdkGetDoubleQuoted("${CPP_LIST_SOURCE_FILES}" PYMODULE_LIST_SOURCE_FILES)

  if(WIN32)
    set(CPP_LIST_NATIVE_LIBRARIES CRYPT32 WS2_32 GDI32 ADVAPI32 USER32)## Native libraries dependencies of OpenSSL on Windows
    sdkGetDoubleQuoted("${CPP_LIST_NATIVE_LIBRARIES}" PYMODULE_LIST_NATIVE_LIBRARIES)
  endif()

  set(PyBuilderScriptName Py${_MODULE_NAME}Builder.py)
  set(Py_TARGET_NAME Py${_MODULE_NAME})
  add_custom_target(${Py_TARGET_NAME} ALL SOURCES ${CPP_LIST_SOURCE_FILES})
  set_property(TARGET ${Py_TARGET_NAME} PROPERTY FOLDER "modules") ## Set Folder for MSVC
  add_dependencies(${Py_TARGET_NAME} ${SDK_CORE_COMPONENT_LIST})
  if(CMAKE_CONFIGURATION_TYPES)
    file(TO_NATIVE_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/Build${Py_TARGET_NAME} _Py_BUILD_DIR_DEBUG)
    file(TO_NATIVE_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/Build${Py_TARGET_NAME} _Py_BUILD_DIR_RELEASE)
    file(TO_NATIVE_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/${Py_TARGET_NAME} _Py_INSTALL_DIR_DEBUG)
    file(TO_NATIVE_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/${Py_TARGET_NAME} _Py_INSTALL_DIR_RELEASE)

    set(PYMODULE_BUILD_DIR "${_Py_BUILD_DIR_DEBUG}")
    set(PYMODULE_INSTALL_DIR "${_Py_INSTALL_DIR_DEBUG}")
    configure_file(${PY_MODULE_BUILDER_IN} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/${PyBuilderScriptName})
    file(TO_NATIVE_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/${PyBuilderScriptName} _Py_BUILDER_DEBUG)

    set(PYMODULE_BUILD_DIR "${_Py_BUILD_DIR_RELEASE}")
    set(PYMODULE_INSTALL_DIR "${_Py_INSTALL_DIR_RELEASE}")
    configure_file(${PY_MODULE_BUILDER_IN} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/${PyBuilderScriptName})
    file(TO_NATIVE_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/${PyBuilderScriptName} _Py_BUILDER_RELEASE)

    add_custom_command(TARGET ${Py_TARGET_NAME} POST_BUILD
                       COMMAND ${Python_EXECUTABLE} "$<IF:$<CONFIG:Debug>,${_Py_BUILDER_DEBUG},${_Py_BUILDER_RELEASE}>"
                       --include_dirs ${PYMODULE_INCLUDE_DIR_LIST}
                       --sys_libraries ${PYMODULE_LIST_NATIVE_LIBRARIES}
                       --dependencies ${PYMODULE_DEPENDENCIES_LIST}
                       --sources ${CPP_LIST_SOURCE_FILES}
    )
    install(DIRECTORY ${_Py_INSTALL_DIR_DEBUG} DESTINATION "lib" CONFIGURATIONS Debug)
    install(DIRECTORY ${_Py_INSTALL_DIR_RELEASE} DESTINATION "lib" CONFIGURATIONS Release)
  else()
    set(PYMODULE_BUILD_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Build${Py_TARGET_NAME}")
    set(PYMODULE_INSTALL_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${Py_TARGET_NAME}")
    configure_file(${PY_MODULE_BUILDER_IN} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PyBuilderScriptName})
    add_custom_command(TARGET ${Py_TARGET_NAME} POST_BUILD
                       COMMAND ${Python_EXECUTABLE} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PyBuilderScriptName}
                       --include_dirs ${PYMODULE_INCLUDE_DIR_LIST}
                       #--sys_libraries ${PYMODULE_LIST_NATIVE_LIBRARIES}## Linux build don't need any native library
                       --dependencies ${PYMODULE_DEPENDENCIES_LIST}
                       --sources ${CPP_LIST_SOURCE_FILES}
    )
    install(DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${Py_TARGET_NAME}" DESTINATION "lib")
  endif()
endmacro()
