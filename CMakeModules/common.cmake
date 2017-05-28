#
#  CMake custom modules
#  Copyright (C) 2011-2015  Cedric OCHS
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

SET(COMMON_MODULE_FOUND TRUE)
SET(ALL_TARGETS CACHE INTERNAL "All targets" FORCE)
SET(VERSION_UPDATED OFF)

# You need to setup this before PROJECT
SET(CMAKE_CONFIGURATION_TYPES Debug Release CACHE TYPE INTERNAL FORCE)

# Force Release configuration for compiler checks
SET(CMAKE_TRY_COMPILE_CONFIGURATION "Release")

INCLUDE(QtSupport)
INCLUDE(AndroidSupport)
INCLUDE(MacSupport)
INCLUDE(WinSupport)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
ENDIF()

MESSAGE(STATUS "Using configuration ${CMAKE_BUILD_TYPE}")

###
# Helper macro that generates .pc and installs it.
# Argument: name - the name of the .pc package, e.g. "mylib.pc"
###
MACRO(GEN_PKGCONFIG name)
  IF(NOT WIN32 AND WITH_INSTALL_LIBRARIES)
    CONFIGURE_FILE(${name}.in "${CMAKE_CURRENT_BINARY_DIR}/${name}")
    INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/${name}" DESTINATION ${LIB_PREFIX}/pkgconfig)
  ENDIF()
ENDMACRO()

###
# Helper macro that generates config.h from config.h.in or config.h.cmake
###
MACRO(GEN_CONFIG_H)
  GEN_TARGET_CONFIG_H(${TARGET})
ENDMACRO()

MACRO(GEN_TARGET_CONFIG_H name)
  IF(LAST_CONFIG_H_IN)
    GEN_CONFIG_H_CUSTOM(${name} ${LAST_CONFIG_H_IN} config.h)
  ELSEIF(EXISTS ${CMAKE_SOURCE_DIR}/config.h.cmake)
    GEN_CONFIG_H_CUSTOM(${name} ${CMAKE_SOURCE_DIR}/config.h.cmake config.h)
  ELSEIF(EXISTS ${CMAKE_SOURCE_DIR}/config.h.in)
    GEN_CONFIG_H_CUSTOM(${name} ${CMAKE_SOURCE_DIR}/config.h.in config.h)
  ELSEIF(EXISTS ${CMAKE_MODULES_COMMON_DIR}/config.h.in)
    GEN_CONFIG_H_CUSTOM(${name} ${CMAKE_MODULES_COMMON_DIR}/config.h.in config.h)
  ENDIF()
ENDMACRO()

MACRO(GEN_CONFIG_H_CUSTOM name src dst)
  # convert relative to absolute paths
  IF(WIN32)
    IF(NOT TARGET_ICON)
      IF(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${name}.ico")
        SET(TARGET_ICON "${CMAKE_CURRENT_SOURCE_DIR}/${name}.ico")
      ELSEIF(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/res/${name}.ico")
        SET(TARGET_ICON "${CMAKE_CURRENT_SOURCE_DIR}/res/${name}.ico")
      ELSEIF(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/icons/${name}.ico")
        SET(TARGET_ICON "${CMAKE_CURRENT_SOURCE_DIR}/icons/${name}.ico")
      ELSEIF(EXISTS "${CMAKE_SOURCE_DIR}/icons/${name}.ico")
        SET(TARGET_ICON "${CMAKE_SOURCE_DIR}/icons/${name}.ico")
      ELSEIF(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/res/icon.ico")
        SET(TARGET_ICON "${CMAKE_CURRENT_SOURCE_DIR}/res/icon.ico")
      ELSEIF(EXISTS "${CMAKE_SOURCE_DIR}/res/icon.ico")
        SET(TARGET_ICON "${CMAKE_SOURCE_DIR}/res/icon.ico")
      ENDIF()
    ELSE()
      IF(EXISTS "${TARGET_ICON}")
      ELSEIF(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_ICON}")
        SET(TARGET_ICON "${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_ICON}")
      ELSEIF(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/res/${TARGET_ICON}")
        SET(TARGET_ICON "${CMAKE_CURRENT_SOURCE_DIR}/res/${TARGET_ICON}")
      ELSEIF(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/icons/${TARGET_ICON}")
        SET(TARGET_ICON "${CMAKE_CURRENT_SOURCE_DIR}/icons/${TARGET_ICON}")
      ELSEIF(EXISTS "${CMAKE_SOURCE_DIR}/icons/${TARGET_ICON}")
        SET(TARGET_ICON "${CMAKE_SOURCE_DIR}/icons/${TARGET_ICON}")
      ELSE()
        SET(TARGET_ICON)
      ENDIF()
    ENDIF()
  ENDIF()

  STRING(TOUPPER ${name} UPTARGET)
  CONFIGURE_FILE(${src} ${CMAKE_BINARY_DIR}/${name}.dir/${dst})
  ADD_DEFINITIONS(-DHAVE_CONFIG_H)
  SET(HAVE_CONFIG_H ON)
  SET(LAST_CONFIG_H_IN ${src})
ENDMACRO()

MACRO(GEN_INIT_D name)
  IF(NOT WIN32)
    CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/${name}.in" "${CMAKE_CURRENT_SOURCE_DIR}/${name}")
  ENDIF()
ENDMACRO()

###
# Helper macro that generates revision.h from revision.h.in
###
MACRO(GEN_REVISION_H)
  IF(EXISTS ${CMAKE_SOURCE_DIR}/revision.h.in)
    # Search GetRevision.cmake in each directory from CMAKE_MODULE_PATH
    FOREACH(ITEM ${CMAKE_MODULE_PATH})
      IF(EXISTS "${ITEM}/GetRevision.cmake")
        SET(GET_REVISION_DIR ${ITEM})
        MESSAGE(STATUS "Found GetRevision module in ${ITEM}")
        BREAK()
      ENDIF()
    ENDFOREACH()

    IF(EXISTS "${CMAKE_SOURCE_DIR}/.svn/")
      FIND_PACKAGE(Subversion)

      IF(NOT SUBVERSION_FOUND)
        SET(GET_REVISION_DIR OFF)
      ENDIF()
    ENDIF()

    IF(EXISTS "${CMAKE_SOURCE_DIR}/.hg/")
      FIND_PACKAGE(Mercurial)

      IF(NOT MERCURIAL_FOUND)
        SET(GET_REVISION_DIR OFF)
      ENDIF()
    ENDIF()

    IF(GET_REVISION_DIR)
      INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR})
      ADD_DEFINITIONS(-DHAVE_REVISION_H)
      SET(HAVE_REVISION_H ON)

      INCLUDE(GetRevision)

      NOW(BUILD_DATE)
      CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/revision.h.in ${CMAKE_BINARY_DIR}/revision.h)

      # revision.h is a generated file
      SET_SOURCE_FILES_PROPERTIES(${CMAKE_BINARY_DIR}/revision.h
        PROPERTIES GENERATED TRUE
        HEADER_FILE_ONLY TRUE)
    ENDIF()
  ENDIF()
ENDMACRO()

###
# Helper macro that generates version.h from version.h.in
###
MACRO(GEN_VERSION_H)
  SET(_FILE ${CMAKE_CURRENT_SOURCE_DIR}/version.h.in)

  IF(NOT EXISTS ${_FILE})
    SET(_FILE ${CMAKE_MODULES_COMMON_DIR}/version.h.in)
  ENDIF()

  IF(NOT EXISTS ${_FILE})
    SET(_FILE)
  ENDIF()

  IF(WITH_INSTALL_LIBRARIES AND _FILE)
    STRING(TOUPPER ${TARGET} UPTARGET)

    SET(_DST ${CMAKE_CURRENT_BINARY_DIR}/include/${TARGET}_version.h)
    CONFIGURE_FILE(${_FILE} ${_DST})
    INSTALL(FILES ${_DST} DESTINATION include/${TARGET} COMPONENT headers)
  ENDIF()
ENDMACRO()

FUNCTION(JOIN VALUES GLUE OUTPUT)
  STRING(REGEX REPLACE "([^\\]|^);" "\\1${GLUE}" _TMP_STR "${VALUES}")
  STRING(REGEX REPLACE "[\\](.)" "\\1" _TMP_STR "${_TMP_STR}") #fixes escaping
  SET(${OUTPUT} "${_TMP_STR}" PARENT_SCOPE)
ENDFUNCTION()

MACRO(PARSE_VERSION_OTHER FILENAME)
  IF(EXISTS ${FILENAME})
    SET(_FILTER_ARRAY ${ARGN})
    JOIN("${_FILTER_ARRAY}" "|" _FILTER_REGEX)
    FILE(STRINGS ${FILENAME} _FILE REGEX "(${_FILTER_REGEX})[: \t=\(\)\"]+([0-9.]+)")

    IF(_FILE)
      FOREACH(_LINE ${_FILE})
        FOREACH(_VAR ${_FILTER_ARRAY})
          IF("${${_VAR}}" STREQUAL "")
            STRING(REGEX REPLACE "^.*${_VAR}[: \t=\(\)\"]+([0-9.]+).*$" "\\1" ${_VAR} "${_LINE}")
            IF(${_VAR} STREQUAL "${_LINE}")
              SET(${_VAR})
            ENDIF()
            IF(NOT ${_VAR} AND NOT STREQUAL "0")
              SET(${_VAR} 0)
            ENDIF()
          ENDIF()
        ENDFOREACH()
      ENDFOREACH()
    ENDIF()
  ENDIF()
ENDMACRO()

MACRO(PARSE_VERSION FILENAME VAR_MAJOR VAR_MINOR VAR_PATCH)
  PARSE_VERSION_OTHER(${FILENAME} ${VAR_MAJOR} ${VAR_MINOR} ${VAR_PATCH})
  SET(VERSION_MAJOR ${${VAR_MAJOR}})
  SET(VERSION_MINOR ${${VAR_MINOR}})
  SET(VERSION_PATCH ${${VAR_PATCH}})
ENDMACRO()

MACRO(PARSE_VERSION_STRING VERSION_STRING)
  SET(_VARIABLES ${ARGN})

  SET(_PART_FILTER "([0-9]+)")
  SET(_FILTER ${_PART_FILTER})

  SET(_COUNT 0)
  FOREACH(_ARG ${_VARIABLES})
    MATH(EXPR _COUNT "${_COUNT}+1")
    STRING(REGEX REPLACE "${_FILTER}.*" "\\${_COUNT}" ${_ARG} "${VERSION_STRING}")
    IF(${_ARG} STREQUAL "${VERSION_STRING}")
      SET(${_ARG} 0)
    ENDIF()
    SET(_FILTER "${_FILTER}\\.${_PART_FILTER}")
  ENDFOREACH()
ENDMACRO()

MACRO(CONVERT_VERSION_NUMBER _VERSION_NUMBER _BASE)
  SET(${_VERSION_NUMBER} 0)
  FOREACH(_ARG ${ARGN})
    MATH(EXPR ${_VERSION_NUMBER} "${${_VERSION_NUMBER}} * ${_BASE} + ${_ARG}")
  ENDFOREACH()
ENDMACRO()

MACRO(CONVERT_NUMBER_VERSION _VERSION_NUMBER _BASE _OUT)
  SET(${_OUT})
  SET(_NUMBER ${_VERSION_NUMBER})
  WHILE(_NUMBER GREATER 0)
    MATH(EXPR _TEMP "${_NUMBER} % ${_BASE}")
    LIST(APPEND ${_OUT} ${_TEMP})
    MATH(EXPR _NUMBER "${_NUMBER} / ${_BASE}")
  ENDWHILE()
ENDMACRO()

MACRO(SIGN_FILE target)
  IF(WIN32)
    SIGN_FILE_WINDOWS(${target})
  ENDIF()
  IF(APPLE)
    SIGN_FILE_MAC(${target})
  ENDIF()
ENDMACRO()

MACRO(CREATE_SOURCE_GROUPS DIR FILES)
  SET(_NAMES)
  SET(_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
  SET(_GENERATED_DIR ${CMAKE_BINARY_DIR})

  FOREACH(_FILE ${FILES})
    # Get only directory from filename
    GET_FILENAME_COMPONENT(_DIR ${_FILE} PATH)

    SET(_NAME)
    SET(_GROUP)

    IF(_DIR)
      # Transform "+" to valid characters in a regular expression
      STRING(REPLACE "+" "\\+" _GENERATED_DIR ${_GENERATED_DIR})
      IF(_DIR MATCHES "${_GENERATED_DIR}")
        SET(_NAME "GENERATED")
        SET(_GROUP "generated")
      ELSE()
        # If directory is not absolute, fix it
        IF(NOT IS_ABSOLUTE _DIR)
          GET_FILENAME_COMPONENT(_DIR ${_DIR} ABSOLUTE)
        ENDIF()

        # Get relative path from CMake current directory
        FILE(RELATIVE_PATH _DIR ${_ROOT_DIR} ${_DIR})

        IF(_DIR)
          # Remove all parents directories
          STRING(REPLACE "../" "" _DIR ${_DIR})

          IF(_DIR)
            # Replace / by _ to use as variable name part
            STRING(REPLACE "/" "_" _NAME ${_DIR})
            STRING(REPLACE "/" "\\" _GROUP ${_DIR})
          ENDIF()
        ENDIF()
      ENDIF()
    ENDIF()

    IF(NOT _NAME)
      SET(_NAME "ROOT")
    ENDIF()

    IF(_GROUP)
      IF(NOT _GROUP STREQUAL "${DIR}" AND NOT _GROUP MATCHES "^(${DIR}\\\\|generated)")
        SET(_GROUP "${DIR}\\${_GROUP}")
      ENDIF()
    ELSE()
      SET(_GROUP "${DIR}")
    ENDIF()

    LIST(APPEND _NAMES ${_NAME})
    LIST(APPEND ${_NAME}_FILES ${_FILE})
    SET(${_NAME}_GROUP ${_GROUP})
  ENDFOREACH()

  IF(_NAMES)
    LIST(REMOVE_DUPLICATES _NAMES)

    FOREACH(_NAME ${_NAMES})
      SOURCE_GROUP("${${_NAME}_GROUP}" FILES ${${_NAME}_FILES})
    ENDFOREACH()
  ENDIF()
ENDMACRO()

MACRO(SET_TARGET_EXECUTABLE _TYPE name)
  IF(NOT BUILD_FLAGS_SETUP)
    SETUP_BUILD_FLAGS()
  ENDIF()

  SET(GUI OFF)
  SET(CONSOLE OFF)
  SET(SERVICE OFF)
  SET(CGI OFF)
  SET(FCGI OFF)

  SET(${_TYPE} ON)

  SET(_SOURCES)
  SET(_HEADERS)
  SET(_RESOURCES)
  SET(MISC_FILES)
  SET(_RCS)
  SET(_IDLS)
  SET(_GENERATED_IDLS)
  SET(_NO_GROUPS OFF)

  INIT_QT()
  INIT_MAC()

  FOREACH(ARG ${ARGN})
    IF(ARG MATCHES "\\.(cpp|mm|m|c|cxx|cc)$")
      LIST(APPEND _SOURCES ${ARG})
    ELSEIF(ARG MATCHES "\\.(h|pch|hpp|hh|hxx)$")
      LIST(APPEND _HEADERS ${ARG})
    ELSEIF(ARG STREQUAL "NO_GROUPS")
      SET(_NO_GROUPS ON)
    ELSE()
      SET(_INCLUDE ON)
      IF(ARG MATCHES ${MAC_FILES_FILTER})
        FILTER_MAC_FILES(${ARG})
      ELSEIF(ARG MATCHES "\\.idl$")
        LIST(APPEND _IDLS ${ARG})
      ELSEIF(ARG MATCHES ${QT_FILES_FILTER})
        FILTER_QT_FILES(${ARG})
      ELSEIF(ARG MATCHES "\\.rc$")
        LIST(APPEND _RCS ${ARG})
      ELSEIF(ARG MATCHES "\\.ico$")
        LIST(APPEND _ICNSS ${ARG})
      ELSE()
        # Miscellaneous file
        LIST(APPEND MISC_FILES ${ARG})
      ENDIF()

      IF(_INCLUDE)
        LIST(APPEND _RESOURCES ${ARG})
      ENDIF()
    ENDIF()
  ENDFOREACH()

  IF(WIN32)
    IF(_IDLS AND NMAKE)
      MACRO_ADD_INTERFACES(_GENERATED_IDLS ${_IDLS})
    ENDIF()
    IF(NOT WINDOWS_RESOURCES_DIR)
      FOREACH(ITEM ${CMAKE_MODULE_PATH})
        IF(EXISTS "${ITEM}/windows/resources.rc")
          SET(WINDOWS_RESOURCES_DIR "${ITEM}/windows")
          BREAK()
        ENDIF()
      ENDFOREACH()
    ENDIF()
    SET(_RC_ORG ${WINDOWS_RESOURCES_DIR}/resources.rc)
    IF(NOT _RCS AND HAVE_CONFIG_H AND WINDOWS_RESOURCES_DIR AND EXISTS ${_RC_ORG})
      SET(_RC ${CMAKE_CURRENT_BINARY_DIR}/resources.rc)
      CONFIGURE_FILE(${_RC_ORG} ${_RC})
      LIST(APPEND _RESOURCES ${_RC})
      LIST(APPEND _RCS ${_RC})
    ENDIF()
  ENDIF()

  # Specific Qt macros
  COMPILE_QT_TRANSLATIONS(${_SOURCES} ${_HEADERS})
  COMPILE_QT_RESOURCES()
  COMPILE_QT_UIS()
  COMPILE_QT_HEADERS(${name} ${_HEADERS})

  SET_QT_SOURCES()

  IF(NOT _NO_GROUPS)
    CREATE_SOURCE_GROUPS(src "${_SOURCES};${_HEADERS}")
  ENDIF()

  IF(_RCS OR QT_QRCS OR _ICNSS)
    SOURCE_GROUP("res" FILES ${_RCS} ${QT_QRCS} ${_ICNSS})
  ENDIF()

  IF(GUI)
    SET(_SOURCES WIN32 MACOSX_BUNDLE ${_SOURCES})
  ENDIF()

  ADD_EXECUTABLE(${name} ${_SOURCES} ${_HEADERS} ${QT_SOURCES} ${_RESOURCES} ${_FRAMEWORKS} ${_GENERATED_IDLS})

  IF(FCGI)
    SET_TARGET_EXTENSION(${name} fcgi)
  ELSEIF(CGI)
    SET_TARGET_EXTENSION(${name} cgi)
  ENDIF()

  SET_TARGET_FLAGS(${name})
  SET_SOURCES_FLAGS("${_SOURCES}")

  IF(APPLE AND GUI)
    INIT_BUNDLE(${name})
    INSTALL_MAC_RESOURCES(${name})
    COMPILE_MAC_XIBS(${name})

    IF(NOT XCODE)
      # for iOS
      FIX_IOS_BUNDLE(${name})
      CREATE_IOS_PACKAGE_TARGET(${name})
      CREATE_IOS_RUN_TARGET(${name})

      # for OS X
      CREATE_MAC_PACKAGE_TARGET(${name})
    ENDIF()
  ELSE()
    INSTALL_QT_LIBRARIES()

    IF(WITH_INSTALL_RUNTIMES)
      INCLUDE(InstallRequiredSystemLibraries)
    ENDIF()
  ENDIF()

  INSTALL_QT_TRANSLATIONS(${name})
  INSTALL_QT_MISC(${name})

  IF(MSVC AND GUI)
    GET_TARGET_PROPERTY(_LINK_FLAGS ${name} LINK_FLAGS)
    IF(NOT _LINK_FLAGS)
      SET(_LINK_FLAGS "")
    ENDIF()
    SET_TARGET_PROPERTIES(${name} PROPERTIES LINK_FLAGS "/MANIFESTDEPENDENCY:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' publicKeyToken='6595b64144ccf1df' language='*' processorArchitecture='*'\" ${_LINK_FLAGS}")
  ENDIF()

  IF(SERVICE)
    INSTALL(TARGETS ${name} RUNTIME DESTINATION ${SBIN_PREFIX})
  ELSEIF(CGI OR FCGI)
    INSTALL(TARGETS ${name} RUNTIME DESTINATION ${CGI_PREFIX})
  ELSE()
    INSTALL(TARGETS ${name} RUNTIME DESTINATION ${BIN_PREFIX} BUNDLE DESTINATION ${BIN_PREFIX})
  ENDIF()

  SIGN_FILE(${name})
ENDMACRO()

MACRO(SET_TARGET_CONSOLE_EXECUTABLE name)
  SET_TARGET_EXECUTABLE(CONSOLE ${name} ${ARGN})
ENDMACRO()

MACRO(SET_TARGET_GUI_EXECUTABLE name)
  SET_TARGET_EXECUTABLE(GUI ${name} ${ARGN})
ENDMACRO()

MACRO(SET_TARGET_SERVICE name)
  SET_TARGET_EXECUTABLE(SERVICE ${name} ${ARGN})
ENDMACRO()

MACRO(SET_TARGET_CGI name)
  SET_TARGET_EXECUTABLE(GCI ${name} ${ARGN})
ENDMACRO()

MACRO(SET_TARGET_FCGI name)
  SET_TARGET_EXECUTABLE(FCGI ${name} ${ARGN})
ENDMACRO()

MACRO(SET_TARGET_LIB name)
  IF(NOT BUILD_FLAGS_SETUP)
    SETUP_BUILD_FLAGS()
  ENDIF()

  # By default we're using project default
  SET(IS_STATIC ${WITH_STATIC})
  SET(IS_SHARED ${WITH_SHARED})
  SET(IS_PRIVATE OFF)
  SET(_NO_GROUPS OFF)
  SET(_NO_INSTALL OFF)

  SET(_SOURCES_STATIC)
  SET(_SOURCES_SHARED)
  SET(_HEADERS)
  SET(_RESOURCES)
  SET(MISC_FILES)
  SET(_RCS)
  SET(_DEFS)
  SET(_LIBRARIES_TO_MERGE)
  SET(_LIBRARIES_TO_LINK)

  INIT_QT()
  INIT_MAC()

  # If user specify STATIC or SHARED, override project default
  FOREACH(ARG ${ARGN})
    IF(ARG STREQUAL "STATIC")
      SET(IS_STATIC ON)
      SET(IS_SHARED OFF)
    ELSEIF(ARG STREQUAL "SHARED")
      SET(IS_SHARED ON)
      SET(IS_STATIC OFF)
    ELSEIF(ARG STREQUAL "PRIVATE")
      SET(IS_PRIVATE ON)
    ELSEIF(ARG STREQUAL "NO_GROUPS")
      SET(_NO_GROUPS ON)
    ELSEIF(ARG STREQUAL "NO_INSTALL")
      SET(_NO_INSTALL ON)
    ELSEIF(TARGET "${ARG}")
      IF(WIN32)
        GET_FINAL_FILENAME_FULLPATH_WITH_EXTENSION(${ARG} _OUTPUT_PATH "lib")

        SET(_LIBRARIES_TO_MERGE "${_LIBRARIES_TO_MERGE} ${_OUTPUT_PATH}")
      ENDIF()

      LIST(APPEND _LIBRARIES_TO_LINK ${ARG})
    ELSEIF(ARG MATCHES "\\.rc$")
      LIST(APPEND _SOURCES_SHARED ${ARG})
      LIST(APPEND _RCS ${ARG})
    ELSEIF(ARG MATCHES "\\.def$")
      LIST(APPEND _SOURCES_SHARED ${ARG})
      LIST(APPEND _DEFS ${ARG})
    ELSEIF(ARG MATCHES "\\.(cpp|mm|m|c|cxx|cc|obj|asm)$")
      LIST(APPEND _SOURCES_SHARED ${ARG})
      LIST(APPEND _SOURCES_STATIC ${ARG})
    ELSEIF(ARG MATCHES "\\.(h|pch|hpp|hh|hxx)$")
      # Private includes such as PCH
      IF(NOT ARG MATCHES "include/")
        LIST(APPEND _SOURCES_SHARED ${ARG})
        LIST(APPEND _SOURCES_STATIC ${ARG})
      ELSE()
        LIST(APPEND _HEADERS ${ARG})
      ENDIF()
    ELSE()
      SET(_INCLUDE ON)
      IF(ARG MATCHES ${QT_FILES_FILTER})
        FILTER_QT_FILES(${ARG})
      ELSEIF(ARG MATCHES ${MAC_FILES_FILTER})
        FILTER_MAC_FILES(${ARG})
      ENDIF()
      IF(_INCLUDE)
        LIST(APPEND _RESOURCES ${ARG})
      ENDIF()
    ENDIF()
  ENDFOREACH()

  IF(WIN32)
    IF(NOT WINDOWS_RESOURCES_DIR)
      FOREACH(ITEM ${CMAKE_MODULE_PATH})
        IF(EXISTS "${ITEM}/windows/resources.rc")
          SET(WINDOWS_RESOURCES_DIR "${ITEM}/windows")
          BREAK()
        ENDIF()
      ENDFOREACH()
    ENDIF()
    SET(_RC ${WINDOWS_RESOURCES_DIR}/resources.rc)
    IF(NOT _RCS AND HAVE_CONFIG_H AND WINDOWS_RESOURCES_DIR AND EXISTS ${_RC})
      LIST(APPEND _SOURCES_SHARED ${_RC})
      LIST(APPEND _RCS ${_RC})
    ENDIF()
  ENDIF()

  IF(IS_PRIVATE)
    SET(IS_STATIC ON)
    SET(IS_SHARED OFF)
  ENDIF()

  SET(STATIC_LIB OFF)

  # Specific Qt macros
  COMPILE_QT_TRANSLATIONS(${_SOURCES_STATIC} ${_HEADERS})
  COMPILE_QT_RESOURCES()
  COMPILE_QT_UIS()
  COMPILE_QT_HEADERS(${name} ${_HEADERS})

  IF(IS_SHARED AND IS_STATIC)
    COMPILE_QT_HEADERS(${name}_static ${_HEADERS})
  ENDIF()

  SET_QT_SOURCES()

  IF(NOT _NO_GROUPS)
    CREATE_SOURCE_GROUPS(include "${_HEADERS}")
    CREATE_SOURCE_GROUPS(src "${_SOURCES_SHARED}")
  ENDIF()

  IF(_RCS)
    SOURCE_GROUP("res" FILES ${_RCS})
  ENDIF()

  SET_SOURCES_FLAGS("${_SOURCES_STATIC}")

  IF(NAMESPACE)
    STRING(REGEX REPLACE "^lib" "" new_name ${name})
    SET(new_name "${NAMESPACE}_${new_name}")
    # TODO: check if name != new_name and prepend "lib" prefix before namespace
  ELSE()
    SET(new_name ${name})
  ENDIF()

  SET(_OUTPUT_NAME_DEBUG ${new_name})
  SET(_OUTPUT_NAME_RELEASE ${new_name})

  IF(DEFINED ${name}_OUTPUT_NAME_DEBUG)
    SET(_OUTPUT_NAME_DEBUG ${${name}_OUTPUT_NAME_DEBUG})
  ENDIF()

  IF(DEFINED ${name}_OUTPUT_NAME_RELEASE)
    SET(_OUTPUT_NAME_RELEASE ${${name}_OUTPUT_NAME_RELEASE})
  ENDIF()

  SET(_STATIC_LIB_TARGET)
  SET(_SHARED_LIB_TARGET)

  # If library mode is not specified, prepend it
  IF(IS_SHARED)
    ADD_LIBRARY(${name} SHARED ${_SOURCES_SHARED} ${_HEADERS} ${QT_SOURCES} ${_RESOURCES})
    SET(_SHARED_LIB_TARGET ${name})
    IF(IS_STATIC)
      SET(_STATIC_LIB_TARGET ${name}_static)
      SET(STATIC_LIB ON)
    ENDIF()
  ELSEIF(IS_STATIC)
    SET(_STATIC_LIB_TARGET ${name})
  ENDIF()

  IF(_STATIC_LIB_TARGET)
    ADD_LIBRARY(${_STATIC_LIB_TARGET} STATIC ${_SOURCES_STATIC} ${_HEADERS} ${QT_SOURCES} ${_RESOURCES})
  ENDIF()

  IF(_LIBRARIES_TO_MERGE AND IS_STATIC)
    SET_PROPERTY(TARGET ${_STATIC_LIB_TARGET} APPEND PROPERTY STATIC_LIBRARY_FLAGS "${_LIBRARIES_TO_MERGE}")
  ENDIF()

  IF(_LIBRARIES_TO_LINK AND IS_SHARED)
     TARGET_LINK_LIBRARIES(${_SHARED_LIB_TARGET} ${_LIBRARIES_TO_LINK})
  ENDIF()

  SET_TARGET_PROPERTIES(${name} PROPERTIES
    OUTPUT_NAME_DEBUG ${_OUTPUT_NAME_DEBUG}
    OUTPUT_NAME_RELEASE ${_OUTPUT_NAME_RELEASE})

  IF(STATIC_LIB)
    IF(NOT WIN32)
      SET_TARGET_PROPERTIES(${_STATIC_LIB_TARGET} PROPERTIES
        OUTPUT_NAME_DEBUG ${_OUTPUT_NAME_DEBUG}
        OUTPUT_NAME_RELEASE ${_OUTPUT_NAME_RELEASE})
    ENDIF()
  ENDIF()

  IF(IS_SHARED)
    SIGN_FILE(${name})
  ENDIF()

  IF(IS_STATIC OR IS_SHARED)
    # take into account prefix for PDB set in SET_TARGET_FLAGS
    IF(MSVC AND WITH_PREFIX_LIB)
      SET_TARGET_PROPERTIES(${name} PROPERTIES PREFIX "lib" IMPORT_PREFIX "lib")
      IF(STATIC_LIB)
        SET_TARGET_PROPERTIES(${_STATIC_LIB_TARGET} PROPERTIES PREFIX "lib")
      ENDIF()
    ENDIF()

    SET_TARGET_FLAGS(${name})
  ENDIF()

  IF(STATIC_LIB)
    SET_TARGET_FLAGS(${_STATIC_LIB_TARGET})
  ENDIF()

  INSTALL_QT_TRANSLATIONS(${name})
  INSTALL_QT_MISC(${name})

  IF(NOT _NO_INSTALL)
    IF(IS_STATIC OR IS_SHARED)
      IF(WIN32)
        # DLLs are in bin directory under Windows
        SET(LIBRARY_DEST ${BIN_PREFIX})
      ELSE()
        SET(LIBRARY_DEST ${LIB_PREFIX})
      ENDIF()

      IF(NOT IS_PRIVATE)
        # copy both DLL and LIB files
        IF(WITH_INSTALL_LIBRARIES)
          INSTALL(TARGETS ${name} RUNTIME DESTINATION ${BIN_PREFIX} LIBRARY DESTINATION ${LIBRARY_DEST} ARCHIVE DESTINATION ${LIB_PREFIX})
          IF(STATIC_LIB)
            INSTALL(TARGETS ${_STATIC_LIB_TARGET} RUNTIME DESTINATION ${BIN_PREFIX} LIBRARY DESTINATION ${LIBRARY_DEST} ARCHIVE DESTINATION ${LIB_PREFIX})
          ENDIF()
        ELSEIF(IS_SHARED)
          IF(WITH_INSTALL_RUNTIMES)
            INCLUDE(InstallRequiredSystemLibraries)
          ENDIF()

          INSTALL(TARGETS ${name} RUNTIME DESTINATION ${BIN_PREFIX} LIBRARY DESTINATION ${LIBRARY_DEST})
          IF(STATIC_LIB)
            INSTALL(TARGETS ${_STATIC_LIB_TARGET} RUNTIME DESTINATION ${BIN_PREFIX} LIBRARY DESTINATION ${LIBRARY_DEST})
          ENDIF()
        ENDIF()

        INSTALL_LIBRARY_PDB(${name})
      ELSE()
        IF(WITH_INSTALL_RUNTIMES)
          INCLUDE(InstallRequiredSystemLibraries)
        ENDIF()

        IF(IS_SHARED)
          # copy only DLL because we don't need development files
          INSTALL(TARGETS ${name} RUNTIME DESTINATION ${BIN_PREFIX} LIBRARY DESTINATION ${LIBRARY_DEST})
        ENDIF()
      ENDIF()
    ELSE()
      MESSAGE(FATAL_ERROR "You can't disable both static and shared libraries")
    ENDIF()
  ENDIF()
ENDMACRO()

###
#
###
MACRO(SET_TARGET_PLUGIN name)
  IF(NOT BUILD_FLAGS_SETUP)
    SETUP_BUILD_FLAGS()
  ENDIF()

  SET(_NO_GROUPS OFF)
  SET(_NO_INSTALL OFF)

  SET(_SOURCES)
  SET(_HEADERS)
  SET(_RESOURCES)
  SET(MISC_FILES)
  SET(_RCS)
  SET(_DEFS)

  INIT_QT()
  INIT_MAC()

  FOREACH(ARG ${ARGN})
    IF(ARG STREQUAL "NO_GROUPS")
      SET(_NO_GROUPS ON)
    ELSEIF(ARG STREQUAL "NO_INSTALL")
      SET(_NO_INSTALL ON)
    ELSEIF(ARG MATCHES "\\.rc$")
      IF(NOT WITH_STATIC_PLUGINS)
        LIST(APPEND _SOURCES ${ARG})
        LIST(APPEND _RCS ${ARG})
      ENDIF()
    ELSEIF(ARG MATCHES "\\.def$")
      IF(NOT WITH_STATIC_PLUGINS)
        LIST(APPEND _SOURCES ${ARG})
        LIST(APPEND _DEFS ${ARG})
      ENDIF()
    ELSEIF(ARG MATCHES "\\.(cpp|mm|m|c|cxx|cc|obj|asm)$")
      LIST(APPEND _SOURCES ${ARG})
    ELSEIF(ARG MATCHES "\\.(h|pch|hpp|hh|hxx)$")
      # Private includes such as PCH
      IF(NOT ARG MATCHES "include/")
        LIST(APPEND _SOURCES ${ARG})
      ELSE()
        LIST(APPEND _HEADERS ${ARG})
      ENDIF()
    ELSE()
      SET(_INCLUDE ON)
      IF(ARG MATCHES ${QT_FILES_FILTER})
        FILTER_QT_FILES(${ARG})
      ELSE()
        # Miscellaneous file
        LIST(APPEND MISC_FILES ${ARG})
      ENDIF()
      IF(_INCLUDE)
        LIST(APPEND _RESOURCES ${ARG})
      ENDIF()
    ENDIF()
  ENDFOREACH()

  IF(WIN32)
    IF(NOT WINDOWS_RESOURCES_DIR)
      FOREACH(ITEM ${CMAKE_MODULE_PATH})
        IF(EXISTS "${ITEM}/windows/resources.rc")
          SET(WINDOWS_RESOURCES_DIR "${ITEM}/windows")
          BREAK()
        ENDIF()
      ENDFOREACH()
    ENDIF()
    SET(_RC ${WINDOWS_RESOURCES_DIR}/resources.rc)
    IF(NOT _RCS AND HAVE_CONFIG_H AND WINDOWS_RESOURCES_DIR AND EXISTS ${_RC})
      LIST(APPEND _SOURCES ${_RC})
      LIST(APPEND _RCS ${_RC})
    ENDIF()
  ENDIF()

  # Specific Qt macros
  COMPILE_QT_TRANSLATIONS(${_SOURCES} ${_HEADERS})
  COMPILE_QT_RESOURCES()
  COMPILE_QT_UIS()
  COMPILE_QT_HEADERS(${name} ${_HEADERS})

  SET_QT_SOURCES()

  IF(NOT _NO_GROUPS)
    CREATE_SOURCE_GROUPS(include "${_HEADERS}")
    CREATE_SOURCE_GROUPS(src "${_SOURCES}")
  ENDIF()

  IF(_RCS)
    SOURCE_GROUP("res" FILES ${_RCS})
  ENDIF()

  SET_SOURCES_FLAGS(${_SOURCES})

  SET(_OUTPUT_NAME_DEBUG ${name})
  SET(_OUTPUT_NAME_RELEASE ${name})

  IF(DEFINED ${name}_OUTPUT_NAME_DEBUG)
    SET(_OUTPUT_NAME_DEBUG ${${name}_OUTPUT_NAME_DEBUG})
  ENDIF()

  IF(DEFINED ${name}_OUTPUT_NAME_RELEASE)
    SET(_OUTPUT_NAME_RELEASE ${${name}_OUTPUT_NAME_RELEASE})
  ENDIF()

  IF(WITH_STATIC_PLUGINS)
    ADD_LIBRARY(${name} STATIC ${_SOURCES} ${_HEADERS} ${QT_SOURCES} ${_RESOURCES})
  ELSE()
    ADD_LIBRARY(${name} MODULE ${_SOURCES} ${_HEADERS} ${QT_SOURCES} ${_RESOURCES})
    SIGN_FILE(${name})
  ENDIF()

  INSTALL_QT_TRANSLATIONS(${name})

  SET_TARGET_PROPERTIES(${name} PROPERTIES
    OUTPUT_NAME_DEBUG ${_OUTPUT_NAME_DEBUG}
    OUTPUT_NAME_RELEASE ${_OUTPUT_NAME_RELEASE})

  SET_TARGET_FLAGS(${name})

  IF(MSVC AND WITH_PREFIX_LIB)
    SET_TARGET_PROPERTIES(${name} PROPERTIES PREFIX "lib")
  ENDIF()

  IF(NOT _NO_INSTALL)
    IF(PLUGIN_PREFIX)
      IF(WITH_INSTALL_LIBRARIES AND WITH_STATIC_PLUGINS)
        INSTALL(TARGETS ${name} LIBRARY DESTINATION ${PLUGIN_PREFIX} ARCHIVE DESTINATION ${LIB_PREFIX})
      ELSE()
        IF(NOT WITH_STATIC_PLUGINS)
          INSTALL(TARGETS ${name} LIBRARY DESTINATION ${PLUGIN_PREFIX} ARCHIVE DESTINATION ${LIB_PREFIX})
        ENDIF()
      ENDIF()

      INSTALL_PLUGIN_PDB(${name})
    ENDIF()
  ENDIF()
ENDMACRO()

MACRO(SET_TARGET_LABEL name label)
  SET_TARGET_PROPERTIES(${name} PROPERTIES PROJECT_LABEL ${label})

  IF(TARGET "${name}_static")
    SET_TARGET_PROPERTIES(${name}_static PROPERTIES PROJECT_LABEL "${label} Static ")
  ENDIF()
ENDMACRO()

MACRO(SET_TARGET_EXECUTABLE_NAME name executable)
  IF(APPLE)
    # Under Mac OS X, executables should use project label
    GET_TARGET_PROPERTY(type ${name} TYPE)

    IF(${type} STREQUAL "EXECUTABLE")
      SET_TARGET_PROPERTIES(${name} PROPERTIES OUTPUT_NAME ${executable})
    ENDIF()
  ENDIF()
ENDMACRO()

MACRO(SET_TARGET_EXTENSION name extension)
  SET_TARGET_PROPERTIES(${name} PROPERTIES SUFFIX .${extension})
ENDMACRO()

MACRO(SET_TARGET_FLAGS name)
  LINK_QT_LIBRARIES(${name})

  # include directory where config.h have been generated
  IF(HAVE_CONFIG_H)
    SET(_DIR ${CMAKE_BINARY_DIR}/${name}.dir)
    IF(NOT EXISTS ${_DIR}/config.h)
      GEN_TARGET_CONFIG_H(${name})
    ENDIF()
    IF(CMAKE_VERSION VERSION_GREATER "2.8.10")
      TARGET_INCLUDE_DIRECTORIES(${name} PRIVATE ${_DIR})
    ELSE()
      INCLUDE_DIRECTORIES(${_DIR})
    ENDIF()
  ENDIF()

  IF(NAMESPACE)
    STRING(REGEX REPLACE "^lib" "" new_name ${name})
    SET(filename "${NAMESPACE}_${new_name}")
    # TODO: check if name != new_name and prepend "lib" prefix before namespace
  ENDIF()

  IF(TARGET revision)
    # explicitly say that the executable depends on revision.h
    ADD_DEPENDENCIES(${name} revision)
  ENDIF()

  GET_TARGET_PROPERTY(type ${name} TYPE)

  IF(filename)
    SET_TARGET_PROPERTIES(${name} PROPERTIES OUTPUT_NAME ${filename})
  ENDIF()

  IF("${type}" STREQUAL "SHARED_LIBRARY" AND NOT ANDROID)
    # Set versions only if target is a shared library
    IF(DEFINED VERSION)
      SET_TARGET_PROPERTIES(${name} PROPERTIES VERSION ${VERSION})
    ENDIF()
    IF(DEFINED VERSION_MAJOR)
      SET_TARGET_PROPERTIES(${name} PROPERTIES SOVERSION ${VERSION_MAJOR})
    ENDIF()
    IF(LIB_ABSOLUTE_PREFIX)
      SET_TARGET_PROPERTIES(${name} PROPERTIES INSTALL_NAME_DIR ${LIB_ABSOLUTE_PREFIX})
    ELSE()
      SET_TARGET_PROPERTIES(${name} PROPERTIES INSTALL_NAME_DIR ${CMAKE_INSTALL_PREFIX}/${LIB_PREFIX})
    ENDIF()
  ENDIF()

  TARGET_LINK_LIBRARIES(${name} ${CMAKE_THREAD_LIBS_INIT})

  IF(NOT "${type}" STREQUAL "STATIC_LIBRARY" AND ANDROID)
    TARGET_LINK_LIBRARIES(${name} ${STL_LIBRARY})
  ENDIF()

  IF(WITH_STLPORT)
    TARGET_LINK_LIBRARIES(${name} ${STLPORT_LIBRARIES})
  ENDIF()

  SET_TARGET_FLAGS_MAC(${name})

  IF(WIN32)
    SET(_DEBUG_POSTFIX "d")
    SET(_RELEASE_POSTFIX "")
  ELSE()
    SET(_DEBUG_POSTFIX "")
    SET(_RELEASE_POSTFIX "")
  ENDIF()

  IF(DEFINED ${name}_DEBUG_POSTFIX)
    SET(_DEBUG_POSTFIX ${${name}_DEBUG_POSTFIX})
  ENDIF()

  IF(DEFINED ${name}_RELEASE_POSTFIX)
    SET(_RELEASE_POSTFIX ${${name}_RELEASE_POSTFIX})
  ENDIF()

  SET(ALL_TARGETS ${ALL_TARGETS} ${name} CACHE INTERNAL "All targets")

  SET_TARGET_PROPERTIES(${name} PROPERTIES DEBUG_POSTFIX "${_DEBUG_POSTFIX}" RELEASE_POSTFIX "${_RELEASE_POSTFIX}")

  SET_TARGET_FLAGS_MSVC(${name})
ENDMACRO()

# Only call this macro for executables
MACRO(INSTALL_RESOURCES _TARGET _DIR)
  # Installing all UNIX resources in /usr/share
  IF(UNIX AND NOT APPLE)
    IF(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/res/desktop.in")
      SET(DESKTOP_FILE "${CMAKE_CURRENT_BINARY_DIR}/share/applications/${_TARGET}.desktop")
      CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/res/desktop.in" ${DESKTOP_FILE})
      INSTALL(FILES ${DESKTOP_FILE} DESTINATION share/applications)
    ENDIF()

    INSTALL(FILES res/icon.xpm DESTINATION share/pixmaps RENAME ${_TARGET}.png OPTIONAL)
    INSTALL(FILES res/icon16x16.png DESTINATION share/icons/hicolor/16x16/apps RENAME ${_TARGET}.png OPTIONAL)
    INSTALL(FILES res/icon22x22.png DESTINATION share/icons/hicolor/22x22/apps RENAME ${_TARGET}.png OPTIONAL)
    INSTALL(FILES res/icon24x24.png DESTINATION share/icons/hicolor/24x24/apps RENAME ${_TARGET}.png OPTIONAL)
    INSTALL(FILES res/icon32x32.png DESTINATION share/icons/hicolor/32x32/apps RENAME ${_TARGET}.png OPTIONAL)
    INSTALL(FILES res/icon48x48.png DESTINATION share/icons/hicolor/48x48/apps RENAME ${_TARGET}.png OPTIONAL)
    INSTALL(FILES res/icon128x128.png DESTINATION share/icons/hicolor/128x128/apps RENAME ${_TARGET}.png OPTIONAL)
    INSTALL(FILES res/icon.svg DESTINATION share/icons/hicolor/scalable/apps RENAME ${_TARGET}.svg OPTIONAL)
  ENDIF()

  # Source Packages
  SET(PACKAGE "${_TARGET}-${VERSION}")

  IF(WIN32)
    IF(TARGET_X64)
      SET(PACKAGE "${PACKAGE}-win64")
    ELSE()
      SET(PACKAGE "${PACKAGE}-win32")
    ENDIF()
  ELSEIF(APPLE)
    SET(PACKAGE "${PACKAGE}-osx")
  ELSE()
    SET(PACKAGE "${PACKAGE}-unix")
  ENDIF()

  SET(CPACK_PACKAGE_FILE_NAME "${PACKAGE}")
  SET(CPACK_SOURCE_PACKAGE_FILE_NAME "${PACKAGE}-src")

  # packaging information
  SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${DESCRIPTION})
  SET(CPACK_PACKAGE_VENDOR ${AUTHOR})
  IF(EXISTS ${CMAKE_SOURCE_DIR}/readme.txt)
    SET(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_SOURCE_DIR}/readme.txt)
  ENDIF()
  IF(EXISTS ${CMAKE_SOURCE_DIR}/COPYING)
    SET(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/COPYING)
  ENDIF()
  SET(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
  SET(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
  SET(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
  SET(CPACK_PACKAGE_INSTALL_DIRECTORY ${PRODUCT})
  SET(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_BINARY_DIR};${PRODUCT};ALL;/")
  SET(CPACK_PACKAGE_EXECUTABLES ${TARGET} ${PRODUCT})
  SET(CPACK_SOURCE_IGNORE_FILES "\\\\.hg" "^${CMAKE_SOURCE_DIR}/debian/")

  # Install documents
  IF(NOT APPLE)
    INSTALL(FILES
        ${CPACK_PACKAGE_DESCRIPTION_FILE}
        ${CPACK_RESOURCE_FILE_LICENSE}
      DESTINATION
        ${SHARE_PREFIX}
      OPTIONAL)
  ENDIF()

  IF(NOT "${_DIR}" STREQUAL "")
    IF(NOT IS_ABSOLUTE ${_DIR})
      # transform relative path to absolute one
      SET(_ABS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${_DIR})
    ELSE()
      SET(_ABS_DIR ${_DIR})
    ENDIF()

    IF(EXISTS "${_ABS_DIR}")
      INSTALL_RESOURCES_MAC(${_TARGET} ${_ABS_DIR})

      # Common code for Unix and Windows
      IF(NOT APPLE)
        IF(WIN32 OR UNIX)
          INSTALL(DIRECTORY ${_ABS_DIR}/ DESTINATION ${SHARE_PREFIX})
        ENDIF()
      ENDIF()
    ENDIF()
  ENDIF()

  # Use NSIS under Windows
  IF(WIN32)
    CREATE_NSIS_PACKAGE(${_TARGET})

    SET(CPACK_SOURCE_GENERATOR "ZIP")
  ELSEIF(APPLE)
    SET(CPACK_GENERATOR "DragNDrop")
    SET(CPACK_SOURCE_GENERATOR "TGZ")
  ELSE()
    SET(CPACK_GENERATOR "TGZ")
    SET(CPACK_SOURCE_GENERATOR "TGZ")
  ENDIF()

  INCLUDE(CPack)
ENDMACRO()

# Only call this macro for executables
MACRO(INSTALL_RESOURCES_DIR _TARGET _DIR)
  IF(NOT "${_DIR}" STREQUAL "" AND EXISTS ${_DIR})
    IF(NOT IS_ABSOLUTE ${_DIR})
      # transform relative path to absolute one
      SET(_ABS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${_DIR})
    ELSE()
      SET(_ABS_DIR ${_DIR})
    ENDIF()

    INSTALL_RESOURCES_DIR_MAC(${_TARGET} ${_ABS_DIR})

    # Common code for Unix and Windows
    IF(NOT APPLE)
      IF(WIN32 OR UNIX)
        INSTALL(DIRECTORY ${_ABS_DIR}/ DESTINATION ${SHARE_PREFIX})
      ENDIF()
    ENDIF()
  ENDIF()
ENDMACRO()

# Set special flags to sources depending on specific language based on their extension
FUNCTION(SET_SOURCES_FLAGS)
  SET(_C)
  SET(_CPP)
  SET(_OBJC)

  SET(_SOURCES ${ARGN})

  IF(_SOURCES)
    FOREACH(_SRC ${_SOURCES})
      IF(_SRC MATCHES "\\.c$")
        LIST(APPEND _C ${_SRC})
      ELSEIF(_SRC MATCHES "\\.(cpp|cxx|cc)$")
        LIST(APPEND _CPP ${_SRC})
      ELSEIF(_SRC MATCHES "\\.(mm|m)$")
        LIST(APPEND _OBJC ${_SRC})
      ENDIF()
    ENDFOREACH()

    IF(_OBJC)
      SET_SOURCE_FILES_PROPERTIES(${_OBJC} PROPERTIES COMPILE_FLAGS ${OBJC_FLAGS})
    ENDIF()
  ENDIF()
ENDFUNCTION()

###
# Checks build vs. source location. Prevents In-Source builds.
###
MACRO(CHECK_OUT_OF_SOURCE)
  IF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    MESSAGE(FATAL_ERROR "

CMake generation for this project is not allowed within the source directory!
Remove the CMakeCache.txt file and try again from another folder, e.g.:

   rm CMakeCache.txt
   mkdir cmake
   cd cmake
   cmake ..
    ")
  ENDIF()

ENDMACRO()

# Set option default value
MACRO(SET_OPTION_DEFAULT NAME VALUE)
  SET(${NAME}_DEFAULT ${VALUE})
ENDMACRO()

MACRO(ADD_OPTION NAME DESCRIPTION)
  IF(${NAME}_DEFAULT)
    SET(${NAME}_DEFAULT ON)
  ELSE()
    SET(${NAME}_DEFAULT OFF)
  ENDIF()

  OPTION(${NAME} ${DESCRIPTION} ${${NAME}_DEFAULT})
ENDMACRO()

MACRO(UPDATE_VERSION)
  IF(NOT VERSION_UPDATED)
    IF(VERSION_PATCH STREQUAL "REVISION")
      INCLUDE(GetRevision)

      IF(DEFINED REVISION)
        SET(VERSION_PATCH "${REVISION}")
        SET(VERSION_PATCH_REVISION ON)
      ELSE()
        SET(VERSION_PATCH 0)
      ENDIF()
    ENDIF()

    SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
    SET(VERSION_RC "${VERSION_MAJOR},${VERSION_MINOR},${VERSION_PATCH},0")

    IF(VERSION_PATCH_REVISION)
      CONVERT_VERSION_NUMBER(VERSION_NUM 100 ${VERSION_MAJOR} ${VERSION_MINOR} 0)
    ELSE()
      CONVERT_VERSION_NUMBER(VERSION_NUM 100 ${VERSION_MAJOR} ${VERSION_MINOR} ${VERSION_PATCH})
    ENDIF()

    SET(VERSION_UPDATED ON)
  ENDIF()
ENDMACRO()

MACRO(INIT_DEFAULT_OPTIONS)
  SET(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS ON)

  IF(APPLE AND NOT IOS)
    SET(OSX ON)
  ENDIF()

  # Undefined options are set to OFF
  SET_OPTION_DEFAULT(WITH_RTTI ON)
  SET_OPTION_DEFAULT(WITH_EXCEPTIONS ON)
  SET_OPTION_DEFAULT(WITH_PCH ON)
  SET_OPTION_DEFAULT(WITH_INSTALL_LIBRARIES ON)

  IF(WIN32)
    SET_OPTION_DEFAULT(WITH_STATIC ON)
    SET_OPTION_DEFAULT(WITH_STATIC_EXTERNAL ON)
  ELSE()
    IF(IOS OR ANDROID OR OSX)
      SET_OPTION_DEFAULT(WITH_STATIC ON)
      SET_OPTION_DEFAULT(WITH_STATIC_EXTERNAL ON)
    ELSE()
      SET_OPTION_DEFAULT(WITH_SHARED ON)
      SET_OPTION_DEFAULT(WITH_STATIC_EXTERNAL OFF)
    ENDIF()
    SET_OPTION_DEFAULT(WITH_UNIX_STRUCTURE ON)
  ENDIF()

  # Check if CMake is launched from a Debian packaging script
  SET(DEB_HOST_GNU_CPU $ENV{DEB_HOST_GNU_CPU})

  # Don't strip if generating a .deb
  IF(DEB_HOST_GNU_CPU)
    SET_OPTION_DEFAULT(WITH_SYMBOLS ON)
  ENDIF()

  # Hidden visibility is required for C++ on iOS and Android
  IF(IOS OR ANDROID)
    SET_OPTION_DEFAULT(WITH_VISIBILITY_HIDDEN ON)
  ENDIF()

  IF(MSVC14)
    SET_OPTION_DEFAULT(WITH_INSTALL_RUNTIMES OFF)
  ELSE()
    SET_OPTION_DEFAULT(WITH_INSTALL_RUNTIMES ON)
  ENDIF()

  UPDATE_VERSION()

  # Remove spaces in product name
  STRING(REGEX REPLACE " " "" PRODUCT_FIXED "${PRODUCT}")

  # Search version.h.in in each directory from CMAKE_MODULE_PATH
  FOREACH(_ITEM ${CMAKE_MODULE_PATH} ${CMAKE_MODULES_DIR})
    IF(EXISTS "${_ITEM}/common/version.h.in")
      SET(CMAKE_MODULES_COMMON_DIR ${_ITEM}/common)
      BREAK()
    ENDIF()
  ENDFOREACH()

  # Tells SETUP_DEFAULT_OPTIONS to not initialize options again
  SET(DEFAULT_OPTIONS_INIT ON)
ENDMACRO()

MACRO(SETUP_DEFAULT_OPTIONS)
  # Initialize default options if not already done
  IF(NOT DEFAULT_OPTIONS_INIT)
    INIT_DEFAULT_OPTIONS()
  ENDIF()

  ADD_OPTION(WITH_WARNINGS            "Show all compilation warnings")
  ADD_OPTION(WITH_LOGGING             "Enable logs")
  ADD_OPTION(WITH_COVERAGE            "With Code Coverage Support")
  ADD_OPTION(WITH_PCH                 "Use Precompiled Headers to speed up compilation")
  ADD_OPTION(WITH_PCH_DEBUG           "Debug Precompiled Headers")
  ADD_OPTION(WITH_STATIC              "Compile static libraries")
  ADD_OPTION(WITH_SHARED              "Compile dynamic libraries")
  ADD_OPTION(WITH_STATIC_PLUGINS      "Compile plugins as static or dynamic")
  ADD_OPTION(WITH_STATIC_EXTERNAL     "Use only static external libraries")
  ADD_OPTION(WITH_STATIC_RUNTIMES     "Use only static C++ runtimes")
  ADD_OPTION(WITH_UNIX_STRUCTURE      "Use UNIX structure (bin, include, lib)")
  ADD_OPTION(WITH_INSTALL_LIBRARIES   "Install development files (includes and static libraries)")
  ADD_OPTION(WITH_INSTALL_RUNTIMES    "Install runtimes")

  ADD_OPTION(WITH_STLPORT             "Use STLport instead of standard STL")
  ADD_OPTION(WITH_RTTI                "Enable RTTI support")
  ADD_OPTION(WITH_EXCEPTIONS          "Enable exceptions support")
  ADD_OPTION(WITH_TESTS               "Compile tests projects")
  ADD_OPTION(WITH_SYMBOLS             "Keep debug symbols in binaries")
  ADD_OPTION(WITH_CPP11               "Use C++ 2011")

  ADD_OPTION(WITH_UPDATE_TRANSLATIONS "Update Qt translations")

  # Specific Windows options
  IF(MSVC)
    ADD_OPTION(WITH_PCH_MAX_SIZE      "Specify precompiled header memory allocation limit")
    ADD_OPTION(WITH_SIGN_FILE         "Sign executables and libraries")
    ADD_OPTION(WITH_PREFIX_LIB        "Force lib prefix for libraries")
  ELSE()
    ADD_OPTION(WITH_VISIBILITY_HIDDEN "Hide all symbols by default")
  ENDIF()

  SET(DEFAULT_OPTIONS_SETUP ON)
ENDMACRO()

MACRO(ADD_PLATFORM_FLAGS _FLAGS)
  SET(PLATFORM_CFLAGS "${PLATFORM_CFLAGS} ${_FLAGS}")
  SET(PLATFORM_CXXFLAGS "${PLATFORM_CXXFLAGS} ${_FLAGS}")
ENDMACRO()

MACRO(ADD_PLATFORM_LINKFLAGS _FLAGS)
  SET(PLATFORM_LINKFLAGS "${PLATFORM_LINKFLAGS} ${_FLAGS}")
ENDMACRO()

MACRO(INIT_BUILD_FLAGS)
  IF(NOT DEFAULT_OPTIONS_SETUP)
    SETUP_DEFAULT_OPTIONS()
  ENDIF()

  # Redirect output files
  SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

  # DLL should be in the same directory as EXE under Windows
  IF(WIN32)
    SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  ELSE()
    SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  ENDIF()

  IF(CMAKE_CXX_LIBRARY_ARCHITECTURE)
    SET(HOST_CPU ${CMAKE_CXX_LIBRARY_ARCHITECTURE})
  ELSE()
    SET(HOST_CPU ${CMAKE_HOST_SYSTEM_PROCESSOR})
  ENDIF()

  IF(HOST_CPU MATCHES "(amd|AMD|x86_)64")
    SET(HOST_CPU "x86_64")
  ELSEIF(HOST_CPU MATCHES "i.86")
    SET(HOST_CPU "x86")
  ENDIF()

  # Determine target CPU

  # If not specified, use the same CPU as host
  IF(NOT TARGET_CPU)
    SET(TARGET_CPU ${HOST_CPU})
  ENDIF()

  IF(TARGET_CPU MATCHES "(amd|AMD|x86_)64")
    SET(TARGET_CPU "x86_64")
  ELSEIF(TARGET_CPU MATCHES "i.86")
    SET(TARGET_CPU "x86")
  ENDIF()

  IF(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    SET(CLANG ON)
    MESSAGE(STATUS "Using Clang ${CMAKE_CXX_COMPILER_VERSION} compiler")
  ENDIF()

  IF(CMAKE_GENERATOR MATCHES "Xcode")
    SET(XCODE ON)
    MESSAGE(STATUS "Generating Xcode project")
  ENDIF()

  IF(CMAKE_GENERATOR MATCHES "NMake")
    SET(NMAKE ON)
    MESSAGE(STATUS "Generating NMake project")
  ENDIF()

  IF(CMAKE_GENERATOR MATCHES "Ninja")
    SET(NINJA ON)
    MESSAGE(STATUS "Generating Ninja project")
  ENDIF()

  # If target and host CPU are the same
  IF("${HOST_CPU}" STREQUAL "${TARGET_CPU}" AND NOT CMAKE_CROSSCOMPILING)
    # x86-compatible CPU
    IF(HOST_CPU MATCHES "x86")
      IF(NOT CMAKE_SIZEOF_VOID_P)
        INCLUDE (CheckTypeSize)
        CHECK_TYPE_SIZE("void*"  CMAKE_SIZEOF_VOID_P)
      ENDIF()

      # Using 32 or 64 bits libraries
      IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
        SET(TARGET_CPU "x86_64")
      ELSE()
        SET(TARGET_CPU "x86")
      ENDIF()
    ELSEIF(HOST_CPU MATCHES "arm")
      SET(TARGET_CPU "arm")
    ELSE()
      SET(TARGET_CPU "unknown")
      MESSAGE(STATUS "Unknown architecture: ${HOST_CPU}")
    ENDIF()
    # TODO: add checks for PPC
  ELSE()
    MESSAGE(STATUS "Compiling on ${HOST_CPU} for ${TARGET_CPU}")
  ENDIF()

  # Use values from environment variables
  SET(PLATFORM_CFLAGS "$ENV{CFLAGS} $ENV{CPPFLAGS} ${PLATFORM_CFLAGS}")
  SET(PLATFORM_CXXFLAGS "$ENV{CXXFLAGS} $ENV{CPPFLAGS} ${PLATFORM_CXXFLAGS}")
  SET(PLATFORM_LINKFLAGS "$ENV{LDFLAGS} ${PLATFORM_LINKFLAGS}")

  # Remove -g and -O flag because we are managing them ourself
  STRING(REPLACE "-g" "" PLATFORM_CFLAGS ${PLATFORM_CFLAGS})
  STRING(REPLACE "-g" "" PLATFORM_CXXFLAGS ${PLATFORM_CXXFLAGS})
  STRING(REGEX REPLACE "-O[0-9s]" "" PLATFORM_CFLAGS ${PLATFORM_CFLAGS})
  STRING(REGEX REPLACE "-O[0-9s]" "" PLATFORM_CXXFLAGS ${PLATFORM_CXXFLAGS})

  # Strip spaces
  STRING(STRIP ${PLATFORM_CFLAGS} PLATFORM_CFLAGS)
  STRING(STRIP ${PLATFORM_CXXFLAGS} PLATFORM_CXXFLAGS)
  STRING(STRIP ${PLATFORM_LINKFLAGS} PLATFORM_LINKFLAGS)

  IF(NOT CMAKE_OSX_ARCHITECTURES)
    IF(TARGET_CPU STREQUAL "x86_64")
      SET(TARGET_X64 1)
      SET(TARGET_X86 1)
    ELSEIF(TARGET_CPU STREQUAL "x86")
      SET(TARGET_X86 1)
    ELSEIF(TARGET_CPU STREQUAL "arm64")
      SET(TARGET_ARM 1)
      SET(TARGET_ARM64 1)
    ELSEIF(TARGET_CPU STREQUAL "armv7s")
      SET(TARGET_ARM 1)
      SET(TARGET_ARMV7S 1)
    ELSEIF(TARGET_CPU STREQUAL "armv7")
      SET(TARGET_ARM 1)
      SET(TARGET_ARMV7 1)
    ELSEIF(TARGET_CPU STREQUAL "armv6")
      SET(TARGET_ARM 1)
      SET(TARGET_ARMV6 1)
    ELSEIF(TARGET_CPU STREQUAL "armv5")
      SET(TARGET_ARM 1)
      SET(TARGET_ARMV5 1)
    ELSEIF(TARGET_CPU STREQUAL "arm")
      SET(TARGET_ARM 1)
    ELSEIF(TARGET_CPU STREQUAL "mips")
      SET(TARGET_MIPS 1)
    ENDIF()

    IF(TARGET_ARM)
      IF(TARGET_ARM64)
        ADD_PLATFORM_FLAGS("-DHAVE_ARM64")
      ENDIF()

      IF(TARGET_ARMV7S)
        ADD_PLATFORM_FLAGS("-DHAVE_ARMV7S")
      ENDIF()

      IF(TARGET_ARMV7)
        ADD_PLATFORM_FLAGS("-DHAVE_ARMV7")
      ENDIF()

      IF(TARGET_ARMV6)
        ADD_PLATFORM_FLAGS("-HAVE_ARMV6")
      ENDIF()

      ADD_PLATFORM_FLAGS("-DHAVE_ARM")
    ENDIF()

    IF(TARGET_X86)
      ADD_PLATFORM_FLAGS("-DHAVE_X86")
    ENDIF()

    IF(TARGET_X64)
      ADD_PLATFORM_FLAGS("-DHAVE_X64 -DHAVE_X86_64")
    ENDIF()

    IF(TARGET_MIPS)
      ADD_PLATFORM_FLAGS("-DHAVE_MIPS")
    ENDIF()
  ENDIF()

  # Fix library paths suffixes for Debian MultiArch
  IF(LIBRARY_ARCHITECTURE)
    SET(CMAKE_LIBRARY_PATH /lib/${LIBRARY_ARCHITECTURE} /usr/lib/${LIBRARY_ARCHITECTURE} ${CMAKE_LIBRARY_PATH})
    IF(TARGET_X64)
      SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} /lib64 /usr/lib64)
    ELSEIF(TARGET_X86)
      SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} /lib32 /usr/lib32)
    ENDIF()
  ENDIF()

  IF(APPLE AND NOT IOS)
    SET(CMAKE_INCLUDE_PATH /opt/local/include ${CMAKE_INCLUDE_PATH})
    SET(CMAKE_LIBRARY_PATH /opt/local/lib ${CMAKE_LIBRARY_PATH})
  ENDIF()

  IF(WITH_LOGGING)
    ADD_PLATFORM_FLAGS("-DENABLE_LOGS")
  ENDIF()

  IF(MSVC)
    # From 2.8.12 (included) to 3.1.0 (excluded), the /Fd parameter to specify
    # compilation PDB was managed entirely by CMake and there was no way to access
    # or change it, so we need to remove it from CFLAGS and manage it ourself later
    IF(CMAKE_VERSION VERSION_GREATER "2.8.11.9" AND CMAKE_VERSION VERSION_LESS "3.1.0")
      SET(MANUALLY_MANAGE_PDB_FLAG ON)
    ELSE()
      SET(MANUALLY_MANAGE_PDB_FLAG OFF)
    ENDIF()

    IF(MSVC_VERSION EQUAL "1700" AND NOT MSVC11)
      SET(MSVC11 ON)
    ENDIF()

    # Ignore default include paths
    ADD_PLATFORM_FLAGS("/X")

    IF(MSVC14)
      ADD_PLATFORM_FLAGS("/Gy-")
      # /Ox is working with VC++ 2015, but custom optimizations don't exist
      SET(RELEASE_CFLAGS "/Ox /GF /GS- ${RELEASE_CFLAGS}")
      # without inlining it's unusable, use custom optimizations again
      SET(DEBUG_CFLAGS "/Od /Ob1 /GF- ${DEBUG_CFLAGS}")

      IF(WITH_INSTALL_RUNTIMES)
        SET(CMAKE_INSTALL_UCRT_LIBRARIES ON)
      ENDIF()
    ELSEIF(MSVC12)
      ADD_PLATFORM_FLAGS("/Gy-")
      # /Ox is working with VC++ 2013, but custom optimizations don't exist
      SET(RELEASE_CFLAGS "/Ox /GF /GS- ${RELEASE_CFLAGS}")
      # without inlining it's unusable, use custom optimizations again
      SET(DEBUG_CFLAGS "/Od /Ob1 /GF- ${DEBUG_CFLAGS}")
    ELSEIF(MSVC11)
      ADD_PLATFORM_FLAGS("/Gy-")
      # /Ox is working with VC++ 2012, but custom optimizations don't exist
      SET(RELEASE_CFLAGS "/Ox /GF /GS- ${RELEASE_CFLAGS}")
      # without inlining it's unusable, use custom optimizations again
      SET(DEBUG_CFLAGS "/Od /Ob1 /GF- ${DEBUG_CFLAGS}")
    ELSEIF(MSVC10)
      ADD_PLATFORM_FLAGS("/Gy-")
      # /Ox is working with VC++ 2010, but custom optimizations don't exist
      SET(RELEASE_CFLAGS "/Ox /GF /GS- ${RELEASE_CFLAGS}")
      # without inlining it's unusable, use custom optimizations again
      SET(DEBUG_CFLAGS "/Od /Ob1 /GF- ${DEBUG_CFLAGS}")
    ELSEIF(MSVC90)
      ADD_PLATFORM_FLAGS("/Gy-")
      # don't use a /O[012x] flag if you want custom optimizations
      SET(RELEASE_CFLAGS "/Ob2 /Oi /Ot /Oy /GT /GF /GS- ${RELEASE_CFLAGS}")
      # without inlining it's unusable, use custom optimizations again
      SET(DEBUG_CFLAGS "/Ob1 /GF- ${DEBUG_CFLAGS}")
    ELSEIF(MSVC80)
      ADD_PLATFORM_FLAGS("/Gy- /Wp64")
      # don't use a /O[012x] flag if you want custom optimizations
      SET(RELEASE_CFLAGS "/Ox /GF /GS- ${RELEASE_CFLAGS}")
      # without inlining it's unusable, use custom optimizations again
      SET(DEBUG_CFLAGS "/Od /Ob1 ${DEBUG_CFLAGS}")
    ELSE()
      MESSAGE(FATAL_ERROR "Can't determine compiler version ${MSVC_VERSION}")
    ENDIF()

    ADD_PLATFORM_FLAGS("/D_CRT_SECURE_NO_DEPRECATE /D_CRT_SECURE_NO_WARNINGS /D_CRT_NONSTDC_NO_WARNINGS /D_SCL_SECURE_NO_WARNINGS /D_WIN32 /DWIN32 /D_WINDOWS /wd4250")

    IF(WITH_PCH_MAX_SIZE)
      ADD_PLATFORM_FLAGS("/Zm1000")
    ENDIF()

    ADD_PLATFORM_FLAGS("/D_WIN32_WINNT=0x0501 /DWINVER=0x0501")

    IF(TARGET_X64)
      # Fix a bug with Intellisense
      ADD_PLATFORM_FLAGS("/D_WIN64")
      # Fix a compilation error for some big C++ files
      ADD_PLATFORM_FLAGS("/bigobj")
    ELSE()
      # Allows 32 bits applications to use 3 GB of RAM
      ADD_PLATFORM_LINKFLAGS("/LARGEADDRESSAWARE")
    ENDIF()

    # Exceptions are only set for C++
    IF(WITH_EXCEPTIONS)
      SET(PLATFORM_CXXFLAGS "${PLATFORM_CXXFLAGS} /EHa")
    ELSE()
      SET(PLATFORM_CXXFLAGS "${PLATFORM_CXXFLAGS} -DBOOST_NO_EXCEPTIONS -D_HAS_EXCEPTIONS=0 /wd4275 /wd4577")
    ENDIF()

    # RTTI is only set for C++
    IF(WITH_RTTI)
#      SET(PLATFORM_CXXFLAGS "${PLATFORM_CXXFLAGS} /GR")
    ELSE()
      SET(PLATFORM_CXXFLAGS "${PLATFORM_CXXFLAGS} /GR-")
    ENDIF()

    IF(WITH_SYMBOLS)
      SET(RELEASE_CFLAGS "/Zi ${RELEASE_CFLAGS}")
      SET(RELEASE_LINKFLAGS "/DEBUG ${RELEASE_LINKFLAGS}")
    ELSE()
      SET(RELEASE_LINKFLAGS "/RELEASE ${RELEASE_LINKFLAGS}")
    ENDIF()

    IF(WITH_STATIC_RUNTIMES)
      SET(RUNTIME_FLAG "/MT")
    ELSE()
      SET(RUNTIME_FLAG "/MD")
    ENDIF()

    IF(WITH_WARNINGS)
      SET(DEBUG_CFLAGS "/W4 ${DEBUG_CFLAGS}")
    ELSE()
      SET(DEBUG_CFLAGS "/W3 ${DEBUG_CFLAGS}")
    ENDIF()

    SET(DEBUG_CFLAGS "/Zi ${RUNTIME_FLAG}d /RTC1 /D_DEBUG /DDEBUG ${DEBUG_CFLAGS}")
    SET(RELEASE_CFLAGS "${RUNTIME_FLAG} /DNDEBUG ${RELEASE_CFLAGS}")
    SET(DEBUG_LINKFLAGS "/DEBUG /OPT:NOREF /OPT:NOICF /NODEFAULTLIB:msvcrt /PDBCOMPRESS ${MSVC_INCREMENTAL_YES_FLAG} ${DEBUG_LINKFLAGS}")
    SET(RELEASE_LINKFLAGS "/OPT:REF /OPT:ICF /INCREMENTAL:NO ${RELEASE_LINKFLAGS}")

    IF(MANUALLY_MANAGE_PDB_FLAG)
      SET(CMAKE_C_COMPILE_OBJECT "<CMAKE_C_COMPILER> /nologo /TC <FLAGS> <DEFINES> /Fo<OBJECT> -c <SOURCE>")
      SET(CMAKE_CXX_COMPILE_OBJECT "<CMAKE_CXX_COMPILER> /nologo /TP <FLAGS> <DEFINES> /Fo<OBJECT> -c <SOURCE>")
    ENDIF()
  ELSE()
    IF(WIN32)
      ADD_PLATFORM_FLAGS("-DWIN32 -D_WIN32")

      IF(CLANG)
        ADD_PLATFORM_FLAGS("-nobuiltininc")
      ENDIF()
    ENDIF()

    INIT_BUILD_FLAGS_MAC()
    INIT_BUILD_FLAGS_ANDROID()

    IF(NOT APPLE)
      IF(HOST_CPU STREQUAL "x86_64" AND TARGET_CPU STREQUAL "x86")
        ADD_PLATFORM_FLAGS("-m32 -march=i686")
      ENDIF()

      IF(HOST_CPU STREQUAL "x86" AND TARGET_CPU STREQUAL "x86_64")
        ADD_PLATFORM_FLAGS("-m64")
      ENDIF()
    ENDIF()

    # use c++0x standard to use std::unique_ptr and std::shared_ptr
    IF(NOT XCODE)
      SET(PLATFORM_CXXFLAGS "${PLATFORM_CXXFLAGS} -std=c++0x")
    ENDIF()

    ADD_PLATFORM_FLAGS("-D_REENTRANT -g -pipe")

    # hardening
    ADD_PLATFORM_FLAGS("-D_FORTIFY_SOURCE=2")

    IF(WITH_COVERAGE)
      ADD_PLATFORM_FLAGS("-fprofile-arcs -ftest-coverage")
    ENDIF()

    IF(WITH_WARNINGS)
      ADD_PLATFORM_FLAGS("-Wall")
    ELSE()
      # Check wrong formats in printf-like functions
      ADD_PLATFORM_FLAGS("-Wformat -Werror=format-security")

      # Don't display invalid or unused command lines arguments by default (often too verbose)
      ADD_PLATFORM_FLAGS("-Wno-invalid-command-line-argument -Wno-unused-command-line-argument")
    ENDIF()

    # never display these warnings because they are minor
    ADD_PLATFORM_FLAGS("-Wno-unused-parameter")

    # Fix "relocation R_X86_64_32 against.." error on x64 platforms
    ADD_PLATFORM_FLAGS("-fPIC")

    IF(NOT XCODE)
      ADD_PLATFORM_FLAGS("-MMD -MP")
    ENDIF()

    IF(WITH_VISIBILITY_HIDDEN AND NOT XCODE)
      ADD_PLATFORM_FLAGS("-fvisibility=hidden")
    ENDIF()

    IF(WITH_VISIBILITY_HIDDEN AND NOT XCODE)
      SET(PLATFORM_CXXFLAGS "${PLATFORM_CXXFLAGS} -fvisibility-inlines-hidden")
    ENDIF()

    IF(NOT XCODE)
      # Exceptions are only set for C++
      IF(WITH_EXCEPTIONS)
        SET(PLATFORM_CXXFLAGS "${PLATFORM_CXXFLAGS} -fexceptions")
      ELSE()
        SET(PLATFORM_CXXFLAGS "${PLATFORM_CXXFLAGS} -fno-exceptions -DBOOST_NO_EXCEPTIONS")
      ENDIF()

      # RTTI is only set for C++
      IF(WITH_RTTI)
        SET(PLATFORM_CXXFLAGS "${PLATFORM_CXXFLAGS} -frtti")
      ELSE()
        SET(PLATFORM_CXXFLAGS "${PLATFORM_CXXFLAGS} -fno-rtti")
      ENDIF()

      IF(WITH_CPP11 AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 5.0.0)
        SET(USE_CPP11 ON)
        SET(PLATFORM_CXXFLAGS "${PLATFORM_CXXFLAGS} -std=c++11")
      ENDIF()
    ELSE()
      IF(NOT WITH_EXCEPTIONS)
        SET(PLATFORM_CXXFLAGS "${PLATFORM_CXXFLAGS} -DBOOST_NO_EXCEPTIONS")
      ENDIF()
    ENDIF()

    # hardening
    ADD_PLATFORM_FLAGS("-fstack-protector --param=ssp-buffer-size=4")

    # If -fstack-protector or -fstack-protector-all enabled, enable too new warnings and fix possible link problems
    IF(WITH_WARNINGS)
      ADD_PLATFORM_FLAGS("-Wstack-protector")
    ENDIF()

    # Fix undefined reference to `__stack_chk_fail' error
    ADD_PLATFORM_LINKFLAGS("-lc")

    IF(NOT APPLE)
      ADD_PLATFORM_LINKFLAGS("-Wl,--no-undefined -Wl,--as-needed")
    ENDIF()

    IF(NOT APPLE)
      # hardening
      ADD_PLATFORM_LINKFLAGS("-Wl,-Bsymbolic-functions -Wl,-z,relro -Wl,-z,now")
    ENDIF()

    IF(NOT WITH_SYMBOLS)
      IF(APPLE)
        SET(RELEASE_LINKFLAGS "${RELEASE_LINKFLAGS} -Wl,-dead_strip")
      ELSE()
        SET(RELEASE_LINKFLAGS "${RELEASE_LINKFLAGS} -Wl,-s")
      ENDIF()
    ENDIF()

    IF(WITH_STATIC_RUNTIMES)
      ADD_PLATFORM_LINKFLAGS("-static")
    ENDIF()

    SET(DEBUG_CFLAGS "-D_DEBUG -DDEBUG ${DEBUG_CFLAGS}")
    SET(RELEASE_CFLAGS "-DNDEBUG -O3 ${RELEASE_CFLAGS}")
    SET(DEBUG_LINKFLAGS "${DEBUG_LINKFLAGS}")
    SET(RELEASE_LINKFLAGS "${RELEASE_LINKFLAGS}")
  ENDIF()

  INCLUDE(PCHSupport OPTIONAL)

  SET(BUILD_FLAGS_INIT ON)
ENDMACRO()

MACRO(SETUP_BUILD_FLAGS)
  IF(NOT BUILD_FLAGS_INIT)
    INIT_BUILD_FLAGS()
  ENDIF()

  SET(CMAKE_C_FLAGS ${PLATFORM_CFLAGS} CACHE STRING "" FORCE)
  SET(CMAKE_CXX_FLAGS ${PLATFORM_CXXFLAGS} CACHE STRING "" FORCE)
  SET(CMAKE_EXE_LINKER_FLAGS ${PLATFORM_LINKFLAGS} CACHE STRING "" FORCE)
  SET(CMAKE_MODULE_LINKER_FLAGS ${PLATFORM_LINKFLAGS} CACHE STRING "" FORCE)
  SET(CMAKE_SHARED_LINKER_FLAGS ${PLATFORM_LINKFLAGS} CACHE STRING "" FORCE)

  ## Debug
  SET(CMAKE_C_FLAGS_DEBUG ${DEBUG_CFLAGS} CACHE STRING "" FORCE)
  SET(CMAKE_CXX_FLAGS_DEBUG ${DEBUG_CFLAGS} CACHE STRING "" FORCE)
  SET(CMAKE_EXE_LINKER_FLAGS_DEBUG ${DEBUG_LINKFLAGS} CACHE STRING "" FORCE)
  SET(CMAKE_MODULE_LINKER_FLAGS_DEBUG ${DEBUG_LINKFLAGS} CACHE STRING "" FORCE)
  SET(CMAKE_SHARED_LINKER_FLAGS_DEBUG ${DEBUG_LINKFLAGS} CACHE STRING "" FORCE)

  ## Release
  SET(CMAKE_C_FLAGS_RELEASE ${RELEASE_CFLAGS} CACHE STRING "" FORCE)
  SET(CMAKE_CXX_FLAGS_RELEASE ${RELEASE_CFLAGS} CACHE STRING "" FORCE)
  SET(CMAKE_EXE_LINKER_FLAGS_RELEASE ${RELEASE_LINKFLAGS} CACHE STRING "" FORCE)
  SET(CMAKE_MODULE_LINKER_FLAGS_RELEASE ${RELEASE_LINKFLAGS} CACHE STRING "" FORCE)
  SET(CMAKE_SHARED_LINKER_FLAGS_RELEASE ${RELEASE_LINKFLAGS} CACHE STRING "" FORCE)

  SET(BUILD_FLAGS_SETUP ON)
ENDMACRO()

# Macro to create x_ABSOLUTE_PREFIX from x_PREFIX
MACRO(MAKE_ABSOLUTE_PREFIX NAME_RELATIVE NAME_ABSOLUTE)
  IF(IS_ABSOLUTE "${${NAME_RELATIVE}}")
    SET(${NAME_ABSOLUTE} ${${NAME_RELATIVE}})
  ELSE()
    IF(WIN32)
      SET(${NAME_ABSOLUTE} ${${NAME_RELATIVE}})
    ELSE()
      SET(${NAME_ABSOLUTE} ${CMAKE_INSTALL_PREFIX}/${${NAME_RELATIVE}})
    ENDIF()
  ENDIF()
ENDMACRO()

MACRO(SETUP_PREFIX_PATHS name)
  IF(NOT BUILD_FLAGS_SETUP)
    SETUP_BUILD_FLAGS()
  ENDIF()

  IF(UNIX)
    ## Allow override of install_prefix/etc path.
    IF(NOT ETC_PREFIX)
      SET(ETC_PREFIX "etc/${name}")
    ENDIF()
    MAKE_ABSOLUTE_PREFIX(ETC_PREFIX ETC_ABSOLUTE_PREFIX)

    ## Allow override of install_prefix/share path.
    IF(NOT SHARE_PREFIX)
      SET(SHARE_PREFIX "share/${name}")
    ENDIF()
    MAKE_ABSOLUTE_PREFIX(SHARE_PREFIX SHARE_ABSOLUTE_PREFIX)

    ## Allow override of install_prefix/sbin path.
    IF(NOT SBIN_PREFIX)
      SET(SBIN_PREFIX "sbin")
    ENDIF()
    MAKE_ABSOLUTE_PREFIX(SBIN_PREFIX SBIN_ABSOLUTE_PREFIX)

    ## Allow override of install_prefix/bin path.
    IF(NOT BIN_PREFIX)
      SET(BIN_PREFIX "bin")
    ENDIF()
    MAKE_ABSOLUTE_PREFIX(BIN_PREFIX BIN_ABSOLUTE_PREFIX)

    ## Allow override of install_prefix/include path.
    IF(NOT INCLUDE_PREFIX)
      SET(INCLUDE_PREFIX "include")
    ENDIF()
    MAKE_ABSOLUTE_PREFIX(INCLUDE_PREFIX INCLUDE_ABSOLUTE_PREFIX)

    ## Allow override of install_prefix/lib path.
    IF(NOT LIB_PREFIX)
      IF(LIBRARY_ARCHITECTURE)
        SET(LIB_PREFIX "lib/${LIBRARY_ARCHITECTURE}")
      ELSE()
        SET(LIB_PREFIX "lib")
      ENDIF()
    ENDIF()
    MAKE_ABSOLUTE_PREFIX(LIB_PREFIX LIB_ABSOLUTE_PREFIX)

    ## Allow override of install_prefix/lib/cgi-bin path.
    IF(NOT CGI_PREFIX)
      IF(LIBRARY_ARCHITECTURE)
        SET(CGI_PREFIX "lib/${LIBRARY_ARCHITECTURE}/cgi-bin")
      ELSE()
        SET(CGI_PREFIX "lib/cgi-bin")
      ENDIF()
    ENDIF()
    MAKE_ABSOLUTE_PREFIX(CGI_PREFIX CGI_ABSOLUTE_PREFIX)

    ## Allow override of install_prefix/lib path.
    IF(NOT PLUGIN_PREFIX)
      IF(LIBRARY_ARCHITECTURE)
        SET(PLUGIN_PREFIX "lib/${LIBRARY_ARCHITECTURE}/${name}")
      ELSE()
        SET(PLUGIN_PREFIX "lib/${name}")
      ENDIF()
    ENDIF()
    MAKE_ABSOLUTE_PREFIX(PLUGIN_PREFIX PLUGIN_ABSOLUTE_PREFIX)

    IF(NOT WWW_PREFIX)
      SET(WWW_PREFIX "/var/www")
    ENDIF()

    # Aliases for automake compatibility
    SET(prefix ${CMAKE_INSTALL_PREFIX})
    SET(exec_prefix ${BIN_ABSOLUTE_PREFIX})
    SET(libdir ${LIB_ABSOLUTE_PREFIX})
    SET(includedir ${INCLUDE_ABSOLUTE_PREFIX})
  ENDIF()
  IF(WIN32)
    IF(TARGET_X64)
      SET(LIB_SUFFIX "64")
    ENDIF()

    IF(WITH_UNIX_STRUCTURE)
      SET(ETC_PREFIX "etc/${name}")
      SET(SHARE_PREFIX "share/${name}")
      SET(SBIN_PREFIX "bin${LIB_SUFFIX}")
      SET(BIN_PREFIX "bin${LIB_SUFFIX}")
      SET(PLUGIN_PREFIX "bin${LIB_SUFFIX}")
    ELSE()
      SET(ETC_PREFIX ".")
      SET(SHARE_PREFIX ".")
      SET(SBIN_PREFIX ".")
      SET(BIN_PREFIX ".")
      SET(PLUGIN_PREFIX ".")
    ENDIF()

    SET(INCLUDE_PREFIX "include")
    SET(LIB_PREFIX "lib${LIB_SUFFIX}") # static libs
    SET(CGI_PREFIX "cgi-bin")
    SET(WWW_PREFIX "www")
    SET(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION ${BIN_PREFIX})
  ENDIF()

  SET(PREFIX_PATHS_SETUP ON)
ENDMACRO()

MACRO(SETUP_EXTERNAL)
  IF(NOT BUILD_FLAGS_SETUP)
    SETUP_BUILD_FLAGS()
  ENDIF()

  IF(NOT PREFIX_PATHS_SETUP)
    MESSAGE(FATAL_ERROR "You MUST call SETUP_PREFIX_PATHS before SETUP_EXTERNAL")
  ENDIF()

  IF(WIN32)
    FIND_PACKAGE(External REQUIRED)
  ELSE()
    FIND_PACKAGE(External QUIET)

    IF(APPLE)
      IF(WITH_STATIC_EXTERNAL)
        # Look only for static libraries because systems libraries are using Frameworks
        SET(CMAKE_FIND_LIBRARY_SUFFIXES .a)
      ELSE()
        SET(CMAKE_FIND_LIBRARY_SUFFIXES .dylib .so .a)
      ENDIF()
    ELSE()
      IF(WITH_STATIC_EXTERNAL)
        SET(CMAKE_FIND_LIBRARY_SUFFIXES .a .so)
      ELSE()
        SET(CMAKE_FIND_LIBRARY_SUFFIXES .so .a)
      ENDIF()
    ENDIF()

    IF(CMAKE_DL_LIBS)
      FIND_LIBRARY(DL_LIBRARY ${CMAKE_DL_LIBS})
      IF(DL_LIBRARY)
        SET(CMAKE_DL_LIBS ${DL_LIBRARY})
      ENDIF()
    ENDIF()
  ENDIF()

  # Android, iOS and Mac OS X have pthread, but no need to link to libpthread
  IF(ANDROID OR APPLE)
    SET(CMAKE_USE_PTHREADS_INIT 1)
    SET(Threads_FOUND TRUE)
  ELSE()
    SET(THREADS_HAVE_PTHREAD_ARG ON)
    FIND_PACKAGE(Threads)
    # TODO: replace all -l<lib> by absolute path to <lib> in CMAKE_THREAD_LIBS_INIT
  ENDIF()

  IF(WITH_STLPORT)
    FIND_PACKAGE(STLport REQUIRED)
    INCLUDE_DIRECTORIES(${STLPORT_INCLUDE_DIR})
  ENDIF()

  IF(MSVC)
    FIND_PACKAGE(MSVC REQUIRED)
    FIND_PACKAGE(WindowsSDK REQUIRED)
  ENDIF()
ENDMACRO()

# macro to define FIND_PACKAGE options with a different package name
MACRO(FIX_PACKAGE_OPTIONS OLDNAME NEWNAME)
  # append other options if needed
  SET(_OPTIONS COMPONENTS REQUIRED QUIETLY)

  # process each options
  FOREACH(_OPTION ${_OPTIONS})
    SET(OLD_OPTION ${OLDNAME}_FIND_${_OPTION})
    IF(DEFINED )
      SET(NEW_OPTION ${NEWNAME}_FIND_${_OPTION})
      SET(${NEW_OPTION} ${OLD_OPTION})
    ENDIF()
  ENDFOREACH()
ENDMACRO()

MACRO(FIND_PACKAGE_HELPER NAME INCLUDE)
  # Looks for a directory containing NAME.
  #
  # NAME is the name of the library, lowercase and uppercase can be mixed
  # It should be EXACTLY (same case) the same part as XXXX in FindXXXX.cmake
  #
  # INCLUDE is the file to check for includes
  #
  # Following parameters are optional variables and must be prefixed by:
  #
  # RELEASE is the list of libraries to check in release mode
  # DEBUG is the list of libraries to check in debug mode
  # SUFFIXES is the PATH_SUFFIXES to check for include file
  #
  # The first match will be used in the specified order and next matches will be ignored
  #
  # The following values are defined
  # NAME_INCLUDE_DIR - where to find NAME
  # NAME_LIBRARIES   - link against these to use NAME
  # NAME_FOUND       - True if NAME is available.

  SET(_PARAMS ${ARGN})

  SET(_RELEASE_LIBRARIES)
  SET(_DEBUG_LIBRARIES)
  SET(_SUFFIXES)

  SET(_IS_RELEASE OFF)
  SET(_IS_DEBUG OFF)
  SET(_IS_SUFFIXES OFF)
  SET(_IS_VERBOSE OFF)

  IF(_PARAMS)
    FOREACH(_PARAM ${_PARAMS})
      IF(_PARAM STREQUAL "RELEASE")
        SET(_IS_RELEASE ON)
        SET(_IS_DEBUG OFF)
        SET(_IS_SUFFIXES OFF)
      ELSEIF(_PARAM STREQUAL "DEBUG")
        SET(_IS_RELEASE OFF)
        SET(_IS_DEBUG ON)
        SET(_IS_SUFFIXES OFF)
      ELSEIF(_PARAM STREQUAL "SUFFIXES")
        SET(_IS_RELEASE OFF)
        SET(_IS_DEBUG OFF)
        SET(_IS_SUFFIXES ON)
      ELSEIF(_PARAM STREQUAL "QUIET")
        SET(_IS_RELEASE OFF)
        SET(_IS_DEBUG OFF)
        SET(_IS_SUFFIXES OFF)
        SET(${NAME}_FIND_QUIETLY ON)
      ELSEIF(_PARAM STREQUAL "VERBOSE")
        SET(_IS_VERBOSE ON)
      ELSEIF(_PARAM STREQUAL "REQUIRED")
        SET(_IS_RELEASE OFF)
        SET(_IS_DEBUG OFF)
        SET(_IS_SUFFIXES OFF)
        SET(${NAME}_FIND_REQUIRED ON)
      ELSE()
        IF(_IS_RELEASE)
          LIST(APPEND _RELEASE_LIBRARIES ${_PARAM})
        ELSEIF(_IS_DEBUG)
          LIST(APPEND _DEBUG_LIBRARIES ${_PARAM})
        ELSEIF(_IS_SUFFIXES)
          LIST(APPEND _SUFFIXES ${_PARAM})
        ELSE()
          MESSAGE(STATUS "parameter ${_PARAM} with no prefix")
        ENDIF()
      ENDIF()
    ENDFOREACH()
  ENDIF()

  # Fixes names if invalid characters are found
  IF("${NAME}" MATCHES "^[a-zA-Z0-9]+$")
    SET(_NAME_FIXED ${NAME})
  ELSE()
    # if invalid characters are detected, replace them by valid ones
    STRING(REPLACE "+" "p" _NAME_FIXED ${NAME})
  ENDIF()

  # Create uppercase and lowercase versions of NAME
  STRING(TOUPPER ${NAME} _UPNAME)
  STRING(TOLOWER ${NAME} _LOWNAME)

  STRING(TOUPPER ${_NAME_FIXED} _UPNAME_FIXED)
  STRING(TOLOWER ${_NAME_FIXED} _LOWNAME_FIXED)

  SET(_SUFFIXES ${_SUFFIXES} ${_LOWNAME} ${_LOWNAME_FIXED} ${NAME})

  IF(NOT WIN32 AND NOT IOS)
    FIND_PACKAGE(PkgConfig QUIET)
    SET(_MODULES ${_LOWNAME} ${_RELEASE_LIBRARIES})
    LIST(REMOVE_DUPLICATES _MODULES)
    IF(PKG_CONFIG_EXECUTABLE)
      PKG_SEARCH_MODULE(PKG_${_NAME_FIXED} QUIET ${_MODULES})
    ENDIF()
  ENDIF()

  SET(_INCLUDE_PATHS)
  SET(_LIBRARY_PATHS)

  # Check for root directories passed to CMake with -DXXX_DIR=...
  IF(DEFINED ${_UPNAME_FIXED}_DIR)
    SET(_TMP ${${_UPNAME_FIXED}_DIR})
    GET_FILENAME_COMPONENT(_TMP ${_TMP} ABSOLUTE)
    LIST(APPEND _INCLUDE_PATHS ${_TMP}/include ${_TMP})
    LIST(APPEND _LIBRARY_PATHS ${_TMP}/lib${LIB_SUFFIX})

    IF(_IS_VERBOSE)
      MESSAGE(STATUS "Using ${_UPNAME_FIXED}_DIR as root directory ${_TMP}")
    ENDIF()
  ENDIF()

  IF(DEFINED ${_UPNAME}_DIR)
    SET(_TMP ${${_UPNAME}_DIR})
    LIST(APPEND _INCLUDE_PATHS ${_TMP}/include ${_TMP})
    LIST(APPEND _LIBRARY_PATHS ${_TMP}/lib${LIB_SUFFIX})

    IF(_IS_VERBOSE)
      MESSAGE(STATUS "Using ${_UPNAME_FIXED}_DIR as root directory ${_TMP}")
    ENDIF()
  ENDIF()

  IF(UNIX)
    # Append UNIX standard include paths
    SET(_UNIX_INCLUDE_PATHS)

    # Append multiarch include paths
    IF(CMAKE_LIBRARY_ARCHITECTURE)
      LIST(APPEND _UNIX_INCLUDE_PATHS
        /usr/local/include/${CMAKE_LIBRARY_ARCHITECTURE}
        /usr/include/${CMAKE_LIBRARY_ARCHITECTURE})
    ENDIF()

    LIST(APPEND _UNIX_INCLUDE_PATHS
      /usr/local/include
      /usr/include
      /sw/include
      /opt/local/include
      /opt/csw/include
      /opt/include)
  ENDIF()

  IF(_IS_VERBOSE)
    MESSAGE(STATUS "Searching header ${INCLUDE} in: ${_INCLUDE_PATHS} with suffixes ${_SUFFIXES}")
  ENDIF()

  # Search for include directory
  FIND_PATH(${_UPNAME_FIXED}_INCLUDE_DIR
    NAMES ${INCLUDE}
    HINTS
      ${PKG_${_NAME_FIXED}_INCLUDE_DIRS}
      ${_INCLUDE_PATHS}
      $ENV{${_UPNAME}_DIR}/include
      $ENV{${_UPNAME_FIXED}_DIR}/include
      $ENV{${_UPNAME}_DIR}
      $ENV{${_UPNAME_FIXED}_DIR}
    PATHS
    ${_UNIX_INCLUDE_PATHS}
    PATH_SUFFIXES
    ${_SUFFIXES}
    DOC "Include path for ${NAME}"
  )

  IF(_IS_VERBOSE)
    IF(${_UPNAME_FIXED}_INCLUDE_DIR)
      MESSAGE(STATUS "${INCLUDE} found in ${${_UPNAME_FIXED}_INCLUDE_DIR}")
    ELSE()
      MESSAGE(STATUS "${INCLUDE} not found")
    ENDIF()
  ENDIF()

  # Append environment variables XXX_DIR
  LIST(APPEND _LIBRARY_PATHS
    $ENV{${_UPNAME}_DIR}/lib${LIB_SUFFIX}
    $ENV{${_UPNAME_FIXED}_DIR}/lib${LIB_SUFFIX})

  IF(UNIX)
    SET(_UNIX_LIBRARY_PATHS)

    # Append multiarch libraries paths
    IF(CMAKE_LIBRARY_ARCHITECTURE)
      LIST(APPEND _UNIX_LIBRARY_PATHS
        /usr/local/lib/${CMAKE_LIBRARY_ARCHITECTURE}
        /lib/${CMAKE_LIBRARY_ARCHITECTURE}
        /usr/lib/${CMAKE_LIBRARY_ARCHITECTURE})
    ENDIF()

    # Append UNIX standard libraries paths
    LIST(APPEND _UNIX_LIBRARY_PATHS
      /usr/local/lib
      /usr/lib
      /lib
      /usr/local/X11R6/lib
      /usr/X11R6/lib
      /sw/lib
      /opt/local/lib
      /opt/csw/lib
      /opt/lib
      /usr/freeware/lib${LIB_SUFFIX})
  ENDIF()

  IF(WITH_STLPORT)
    LIST(APPEND _RELEASE_LIBRARIES ${_LOWNAME}_stlport ${_LOWNAME_FIXED}_stlport ${NAME}_stlport ${_NAME_FIXED}_stlport)
    LIST(APPEND _DEBUG_LIBRARIES ${_LOWNAME}_stlportd ${_LOWNAME_FIXED}_stlportd ${NAME}_stlportd ${_NAME_FIXED}_stlportd)
  ENDIF()

  LIST(APPEND _RELEASE_LIBRARIES ${_LOWNAME} ${_LOWNAME_FIXED} ${NAME} ${_NAME_FIXED})
  LIST(APPEND _DEBUG_LIBRARIES ${_LOWNAME}d ${_LOWNAME_FIXED}d ${NAME}d ${_NAME_FIXED}d)

  # Under Windows, some libs may need the lib prefix
  IF(WIN32)
    SET(_LIBS ${_RELEASE_LIBRARIES})
    FOREACH(_LIB ${_LIBS})
      LIST(APPEND _RELEASE_LIBRARIES lib${_LIB})
    ENDFOREACH()

    SET(_LIBS ${_DEBUG_LIBRARIES})
    FOREACH(_LIB ${_LIBS})
      LIST(APPEND _DEBUG_LIBRARIES lib${_LIB})
    ENDFOREACH()
  ENDIF()

  LIST(REMOVE_DUPLICATES _RELEASE_LIBRARIES)
  LIST(REMOVE_DUPLICATES _DEBUG_LIBRARIES)

  # Search for release library
  FIND_LIBRARY(${_UPNAME_FIXED}_LIBRARY_RELEASE
    NAMES
    ${_RELEASE_LIBRARIES}
    HINTS ${PKG_${_NAME_FIXED}_LIBRARY_DIRS}
    PATHS
    ${_LIBRARY_PATHS}
    ${_UNIX_LIBRARY_PATHS}
    NO_CMAKE_SYSTEM_PATH
  )

  IF(_IS_VERBOSE)
    IF(${_UPNAME_FIXED}_LIBRARY_RELEASE)
      MESSAGE(STATUS "${NAME} release library found: ${${_UPNAME_FIXED}_LIBRARY_RELEASE}")
    ELSE()
      MESSAGE(STATUS "${NAME} release library not found")
    ENDIF()
  ENDIF()

  # Search for debug library
  FIND_LIBRARY(${_UPNAME_FIXED}_LIBRARY_DEBUG
    NAMES
    ${_DEBUG_LIBRARIES}
    HINTS ${PKG_${_NAME_FIXED}_LIBRARY_DIRS}
    PATHS
    ${_LIBRARY_PATHS}
    ${_UNIX_LIBRARY_PATHS}
    NO_CMAKE_SYSTEM_PATH
  )

  IF(_IS_VERBOSE)
    IF(${_UPNAME_FIXED}_LIBRARY_DEBUG)
      MESSAGE(STATUS "${NAME} debug library found: ${${_UPNAME_FIXED}_LIBRARY_DEBUG}")
    ELSE()
      MESSAGE(STATUS "${NAME} debug library not found")
    ENDIF()
  ENDIF()

  SET(${_UPNAME_FIXED}_FOUND OFF)

  IF(${_UPNAME_FIXED}_INCLUDE_DIR)
    # Set also _INCLUDE_DIRS
    SET(${_UPNAME_FIXED}_INCLUDE_DIRS ${${_UPNAME_FIXED}_INCLUDE_DIR})
  ENDIF()

  # Library has been found if at least only one library and include are found
  IF(${_UPNAME_FIXED}_LIBRARY_RELEASE AND ${_UPNAME_FIXED}_LIBRARY_DEBUG)
    # Release and debug libraries found
    SET(${_UPNAME_FIXED}_FOUND ON)
    SET(${_UPNAME_FIXED}_LIBRARIES optimized ${${_UPNAME_FIXED}_LIBRARY_RELEASE} debug ${${_UPNAME_FIXED}_LIBRARY_DEBUG})
    SET(${_UPNAME_FIXED}_LIBRARY ${${_UPNAME_FIXED}_LIBRARY_RELEASE})
  ELSEIF(${_UPNAME_FIXED}_LIBRARY_RELEASE)
    # Release library found
    SET(${_UPNAME_FIXED}_FOUND ON)
    SET(${_UPNAME_FIXED}_LIBRARIES ${${_UPNAME_FIXED}_LIBRARY_RELEASE})
    SET(${_UPNAME_FIXED}_LIBRARY ${${_UPNAME_FIXED}_LIBRARY_RELEASE})
  ELSEIF(${_UPNAME_FIXED}_LIBRARY_DEBUG)
    # Debug library found
    SET(${_UPNAME_FIXED}_FOUND ON)
    SET(${_UPNAME_FIXED}_LIBRARIES ${${_UPNAME_FIXED}_LIBRARY_DEBUG})
    SET(${_UPNAME_FIXED}_LIBRARY ${${_UPNAME_FIXED}_LIBRARY_DEBUG})
  ENDIF()

  IF(${_UPNAME_FIXED}_FOUND)
    IF(NOT ${NAME}_FIND_QUIETLY)
      MESSAGE(STATUS "Found ${NAME}: ${${_UPNAME_FIXED}_LIBRARIES}")
    ENDIF()
  ELSE()
    IF(${NAME}_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Error: Unable to find ${NAME}!")
    ENDIF()
    IF(NOT ${NAME}_FIND_QUIETLY)
      MESSAGE(STATUS "Warning: Unable to find ${NAME}!")
    ENDIF()
  ENDIF()

  MARK_AS_ADVANCED(${_UPNAME_FIXED}_LIBRARY_RELEASE ${_UPNAME_FIXED}_LIBRARY_DEBUG)
ENDMACRO()

MACRO(MESSAGE_VERSION_PACKAGE_HELPER NAME VERSION)
  MESSAGE(STATUS "Found ${NAME} ${VERSION}: ${ARGN}")
ENDMACRO()
