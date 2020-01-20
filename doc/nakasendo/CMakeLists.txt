################################################################
#  Date             12/12/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/plantuml.jar DESTINATION ${SDK_GENERATED_TOOLS_DIR})
if(WIN32)
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/plantuml.cmd DESTINATION ${SDK_GENERATED_TOOLS_DIR})
else()
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/plantuml DESTINATION ${SDK_GENERATED_TOOLS_DIR} FILE_PERMISSIONS OWNER_EXECUTE)
endif()
set(SDK_NAKASENDO_MKDOCS_DIR ${CMAKE_CURRENT_SOURCE_DIR})


set(_TARGET_NAME nakasendo_doc)
add_custom_target(${_TARGET_NAME} ALL)

if(WIN32)## Assuming that in Windows, it use MSVC, so it is CMAKE_CONFIGURATION_TYPES ON
  set(_ENV_PATH $ENV{PATH})
  string(REPLACE ";" "\$<SEMICOLON>" ENV_PATH "${_ENV_PATH}")
  string(REPLACE "/" "\\\\" SDK_GENERATED_TOOLS_DIR_WINDOWS "${SDK_GENERATED_TOOLS_DIR}")
  string(REPLACE "/" "\\\\" SDK_GENERATED_DIR_WINDOWS "${SDK_GENERATED_DIR}/${_TARGET_NAME}")
  add_custom_command(TARGET ${_TARGET_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E env PATH="${ENV_PATH}$<SEMICOLON>${SDK_GENERATED_TOOLS_DIR_WINDOWS}" mkdocs build -c -d ${SDK_GENERATED_DIR_WINDOWS}
                    WORKING_DIRECTORY ${SDK_NAKASENDO_MKDOCS_DIR}
  )

  set_property(TARGET ${_TARGET_NAME} PROPERTY FOLDER "documentation")
else()
  add_custom_command(TARGET ${_TARGET_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E env LD_LIBRARY_PATH=${SDK_GENERATED_TOOLS_DIR} mkdocs build -c -d ${SDK_GENERATED_DIR}/${_TARGET_NAME}
                    WORKING_DIRECTORY ${SDK_NAKASENDO_MKDOCS_DIR}
  )
endif()

install(DIRECTORY "${SDK_GENERATED_DIR}/${_TARGET_NAME}/" DESTINATION "doc/${_TARGET_NAME}" COMPONENT Documentation)
