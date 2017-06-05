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

# Globals variables
# QT_QMAKE_EXECUTABLE could point to qmake
# USE_QT
# USE_QT4 or USE_QT5
# QT_BINARY_DIR
# QT_TRANSLATIONS_DIR
# QT_PLUGINS_DIR
# QT_TSS => QT_QMS
# QT_UIS => QT_UIS_HEADERS
# QT_QRCS => QT_QRCS_CPPS
# headers => QT_MOCS_CPPS
# QT_SOURCES
# QT_LANGS
# QT_SHARED_MODULES
# QT4_MODULES
# QT5_MODULES
# QT_MODULES_WANTED
# QT_MODULES_USED

# Check .desktop file under Gnome
SET(DESKTOP_FILE $ENV{GIO_LAUNCHED_DESKTOP_FILE})

# Check build directory
IF(NOT DESKTOP_FILE)
  SET(DESKTOP_FILE ${CMAKE_BINARY_DIR})
ENDIF()

# Force Debug configuration if launched from Qt Creator
IF(NOT CMAKE_BUILD_TYPE AND DESKTOP_FILE MATCHES "qtcreator")
  SET(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
ENDIF()

MACRO(INIT_QT)
  # Init all variables needed by Qt
  SET(QT_SOURCES)
  SET(QT_LANGS)
  SET(QT_MOCS)
  SET(QT_MOCS_CPPS)
  SET(QT_UIS)
  SET(QT_UIS_HEADERS)
  SET(QT_QRCS)
  SET(QT_QRCS_CPPS)
  SET(QT_TSS)
  SET(QT_QMS)

  # Regex filter for Qt files
  SET(QT_FILES_FILTER "\\.(ts|qrc|ui)$")

  INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR})
ENDMACRO()

MACRO(ADD_QT5_DEPENDENCIES)
  LIST(APPEND QT5_MODULES_WANTED Core LinguistTools Concurrent)

  FOREACH(_MODULE ${QT5_MODULES_WANTED})
    IF(_MODULE STREQUAL "WebKit")
      LIST(APPEND QT5_MODULES_WANTED Quick Multimedia Qml Sql Sensors Network Gui)

      # Depends on Positioning since Qt 5.3.0
      IF("${Qt5Core_VERSION_STRING}" VERSION_GREATER "5.2.9")
        LIST(APPEND QT5_MODULES_WANTED Positioning)
      ENDIF()

      # Depends on WebChannel since Qt 5.4.2
      IF("${Qt5Core_VERSION_STRING}" VERSION_GREATER "5.4.1")
        LIST(APPEND QT5_MODULES_WANTED WebChannel)
      ENDIF()
    ELSEIF(_MODULE STREQUAL "WebEngine")
      LIST(APPEND QT5_MODULES_WANTED WebEngineCore Quick Gui WebChannel Qml Core Network)
    ELSEIF(_MODULE STREQUAL "WebKitWidgets")
      LIST(APPEND QT5_MODULES_WANTED MultimediaWidgets OpenGL PrintSupport)
    ELSEIF(_MODULE STREQUAL "Multimedia")
      LIST(APPEND QT5_MODULES_WANTED Gui Network)
    ELSEIF(_MODULE STREQUAL "MultimediaWidgets")
      LIST(APPEND QT5_MODULES_WANTED Multimedia Widgets Gui OpenGL)
    ELSEIF(_MODULE STREQUAL "OpenGL")
      LIST(APPEND QT5_MODULES_WANTED Widgets Gui)
    ELSEIF(_MODULE STREQUAL "PrintSupport")
      LIST(APPEND QT5_MODULES_WANTED Widgets Gui)
    ELSEIF(_MODULE STREQUAL "Qml")
      LIST(APPEND QT5_MODULES_WANTED Network)
    ELSEIF(_MODULE STREQUAL "Quick")
      LIST(APPEND QT5_MODULES_WANTED Qml Network Gui)
    ENDIF()
  ENDFOREACH()

  # Remove obsolete modules for Qt 5.6+
  IF("${Qt5Core_VERSION_STRING}" VERSION_GREATER "5.5.9")
    LIST(REMOVE_ITEM QT5_MODULES_WANTED Declarative MultimediaQuick QuickParticles V8 WebKit WebKitWidgets)
  ELSEIF("${Qt5Core_VERSION_STRING}" VERSION_LESS "5.6")
    LIST(REMOVE_ITEM QT5_MODULES_WANTED WebEngineWidgets)
  ENDIF()

  LIST(REMOVE_DUPLICATES QT5_MODULES_WANTED)
ENDMACRO()

MACRO(USE_QT_MODULES)
  OPTION(WITH_QT5 "Use Qt 5 instead of Qt 4" ON)

  SET(QT_MODULES_WANTED ${ARGN})
  SET(QT_MODULES_USED)
  SET(USE_QT OFF)
  SET(USE_QT4 OFF)
  SET(USE_QT5 OFF)

  # Qt shared modules
  SET(QT_SHARED_MODULES AxBase AxContainer AxServer CLucene Core Declarative Designer Gui Help Multimedia Network OpenGL Qml Script ScriptTools Sql Svg Test WebKit Xml XmlPatterns)

  # Qt 4 modules
  SET(QT4_MODULES ${QT_SHARED_MODULES} Main)

  # Qt 5 modules
  SET(QT5_MODULES ${QT_SHARED_MODULES} Bluetooth Concurrent DBus LinguistTools Location MultimediaQuick MultimediaWidgets Nfc OpenGL OpenGLExtensions PlatformSupport Positioning PrintSupport Quick QuickParticles QuickTest QuickWidgets Sensors SerialBus SerialPort UiPlugin UiTools V8 WebChannel WebEngine WebEngineCore WebEngineWidgets WebKitWidgets WebSockets WebView Widgets 3DCore 3DInput 3DLogic 3DQuick 3DQuickInput 3DQuickRender 3DRender)

  # Use WinExtras only under Windows
  IF(WIN32)
    LIST(APPEND QT5_MODULES WinExtras)
  ENDIF()

  # make a list with all Qt 4 and 5 modules wanted
  SET(QT5_MODULES_WANTED)
  SET(QT4_MODULES_WANTED)

  # by default, modules are shared
  SET(_ONLY_QT4 OFF)
  SET(_ONLY_QT5 OFF)

  FOREACH(_MODULE ${QT_MODULES_WANTED})
    IF(_MODULE STREQUAL "QT4")
      SET(_ONLY_QT4 ON)
      SET(_ONLY_QT5 OFF)
    ELSEIF(_MODULE STREQUAL "QT5")
      SET(_ONLY_QT4 OFF)
      SET(_ONLY_QT5 ON)
    ELSEIF(NOT _ONLY_QT4 AND NOT _ONLY_QT5)
      # add module to both lists
      LIST(APPEND QT4_MODULES_WANTED ${_MODULE})
      LIST(APPEND QT5_MODULES_WANTED ${_MODULE})
    ELSEIF(_ONLY_QT4)
      # add module to Qt 4 list
      LIST(APPEND QT4_MODULES_WANTED ${_MODULE})
    ELSEIF(_ONLY_QT5)
      # add module to Qt 5 list
      LIST(APPEND QT5_MODULES_WANTED ${_MODULE})
    ENDIF()
  ENDFOREACH()

  IF(WITH_QT5)
    # Look for Qt 5 in some environment variables
    SET(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${QTDIR} $ENV{QTDIR})
    FIND_PACKAGE(Qt5Core QUIET)

    IF(Qt5Core_FOUND)
      # add default dependencies
      ADD_QT5_DEPENDENCIES()

      # To be sure a 2nd stage dependency is right
      ADD_QT5_DEPENDENCIES()

      # only search for existing Qt 5 modules
      FOREACH(_MODULE ${QT5_MODULES_WANTED})
        IF(QT5_MODULES MATCHES ${_MODULE})
          FIND_PACKAGE(Qt5${_MODULE} REQUIRED)

          IF("${Qt5${_MODULE}_FOUND}")
            LIST(APPEND QT_MODULES_USED ${_MODULE})
            SET(USE_QT5 ON)
            SET(USE_QT ON)
          ENDIF()
        ENDIF()
      ENDFOREACH()
    ELSE()
      SET(WITH_QT5 OFF)
    ENDIF()
  ENDIF()

  IF(NOT WITH_QT5)
    LIST(APPEND QT4_MODULES_WANTED Main Core)
    LIST(REMOVE_DUPLICATES QT4_MODULES_WANTED)

    FOREACH(_MODULE ${QT4_MODULES_WANTED})
      IF(QT4_MODULES MATCHES ${_MODULE})
        LIST(APPEND _COMPONENTS Qt${_MODULE})
      ENDIF()
    ENDFOREACH()

    FIND_PACKAGE(Qt4 COMPONENTS ${_COMPONENTS} REQUIRED)
    INCLUDE(${QT_USE_FILE})

    FOREACH(_MODULE ${QT4_MODULES_WANTED})
      STRING(TOUPPER ${_MODULE} _UP_MODULE_NAME)

      IF("${QT_USE_QT${_UP_MODULE_NAME}}")
        LIST(APPEND QT_MODULES_USED ${_MODULE})
        SET(USE_QT4 ON)
        SET(USE_QT ON)
      ENDIF()
    ENDFOREACH()
  ENDIF()

  IF(USE_QT5)
    # Check if we are using Qt static or shared libraries
    GET_TARGET_PROPERTY(_FILE Qt5::Core IMPORTED_LOCATION_RELEASE)

    SET(QT_VERSION "${Qt5Core_VERSION_STRING}")
    SET(_VERSION "${QT_VERSION}")

    IF(_FILE MATCHES "\\.(lib|a)$")
      SET(QT_STATIC ON)
      SET(_VERSION "${_VERSION} static version")
    ELSE()
      SET(QT_STATIC OFF)
      SET(_VERSION "${_VERSION} shared version")
    ENDIF()

    MESSAGE(STATUS "Found Qt ${_VERSION}")

    # These variables are not defined with Qt5 CMake modules
    SET(QT_DIR "${_qt5Core_install_prefix}")
    SET(QT_BINARY_DIR "${QT_DIR}/bin")
    SET(QT_LIBRARY_DIR "${QT_DIR}/lib")
    SET(QT_PLUGINS_DIR "${QT_DIR}/plugins")
    SET(QT_RESOURCES_DIR "${QT_DIR}/resources")
    SET(QT_TRANSLATIONS_DIR "${QT_DIR}/translations")

    # Fix wrong include directories with Qt 5 under Mac OS X
    INCLUDE_DIRECTORIES("${QT_DIR}/include")
  ENDIF()

  IF(USE_QT4)
    SET(QT_STATIC ${QT_IS_STATIC})
  ENDIF()
ENDMACRO()

MACRO(FILTER_QT_FILES FILE)
  IF(USE_QT)
    IF(${FILE} MATCHES "\\.ts$")
      STRING(REGEX REPLACE "^.*_([a-z-]*)\\.ts$" "\\1" _LANG ${FILE})
      LIST(APPEND QT_LANGS ${_LANG})
      LIST(APPEND QT_TSS ${FILE})
    ELSEIF(${FILE} MATCHES "\\.qrc$")
      LIST(APPEND QT_QRCS ${FILE})
    ELSEIF(${FILE} MATCHES "\\.ui$")
      LIST(APPEND QT_UIS ${FILE})
    ENDIF()
  ENDIF()
ENDMACRO()

MACRO(COMPILE_QT_RESOURCES)
  IF(QT_QRCS)
    # Generate .cpp from .qrc
    IF(USE_QT5)
      QT5_ADD_RESOURCES(QT_QRCS_CPPS ${QT_QRCS})
    ELSEIF(USE_QT4)
      QT4_ADD_RESOURCES(QT_QRCS_CPPS ${QT_QRCS})
    ENDIF()
  ENDIF()
ENDMACRO()

MACRO(COMPILE_QT_UIS)
  IF(QT_UIS)
    # Generate .h from .ui
    IF(USE_QT5)
      QT5_WRAP_UI(QT_UIS_HEADERS ${QT_UIS})
    ELSEIF(USE_QT4)
      QT4_WRAP_UI(QT_UIS_HEADERS ${QT_UIS})
    ENDIF()

    SOURCE_GROUP("ui" FILES ${QT_UIS})
  ENDIF()
ENDMACRO()

MACRO(COMPILE_QT_HEADERS _TARGET)
  IF(USE_QT)
    # CMake supports automoc since version 2.8.6
    IF(CMAKE_VERSION VERSION_GREATER "2.8.5" AND CMAKE_AUTOMOC)
      SET(QT_MOCS_CPPS "${CMAKE_CURRENT_BINARY_DIR}/${_TARGET}_automoc.cpp")
      SET_SOURCE_FILES_PROPERTIES(${QT_MOCS_CPPS} PROPERTIES GENERATED TRUE)
    ELSE()
      SET(_FILES "${ARGN}")
      IF(_FILES)
        # Generate .cpp from .h witout notice messages
        IF(USE_QT5)
          QT5_WRAP_CPP(QT_MOCS_CPPS ${_FILES} OPTIONS -nn)
        ELSEIF(USE_QT4)
          QT4_WRAP_CPP(QT_MOCS_CPPS ${_FILES})
        ENDIF()
      ENDIF()
    ENDIF()
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
  ENDIF()
ENDMACRO()

IF(CMAKE_VERSION VERSION_GREATER "2.8.2")
  include(CMakeParseArguments)
ENDIF()

function(QT5FIXED_CREATE_TRANSLATION _qm_files)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs OPTIONS)

    cmake_parse_arguments(_LUPDATE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    set(_lupdate_files ${_LUPDATE_UNPARSED_ARGUMENTS})
    set(_lupdate_options ${_LUPDATE_OPTIONS})

    set(_my_sources)
    set(_my_tsfiles)
    foreach(_file ${_lupdate_files})
        get_filename_component(_ext ${_file} EXT)
        get_filename_component(_abs_FILE ${_file} ABSOLUTE)
        if(_ext MATCHES "ts")
            list(APPEND _my_tsfiles ${_abs_FILE})
        else()
            list(APPEND _my_sources ${_abs_FILE})
        endif()
    endforeach()
    foreach(_ts_file ${_my_tsfiles})
        if(_my_sources)
          # make a list file to call lupdate on, so we don't make our commands too
          # long for some systems
          get_filename_component(_ts_name ${_ts_file} NAME_WE)
          set(_ts_lst_file "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${_ts_name}_lst_file")
          set(_lst_file_srcs)
          foreach(_lst_file_src ${_my_sources})
              set(_lst_file_srcs "${_lst_file_src}\n${_lst_file_srcs}")
          endforeach()

#          get_directory_property(_inc_DIRS INCLUDE_DIRECTORIES)
#          foreach(_pro_include ${_inc_DIRS})
#              get_filename_component(_abs_include "${_pro_include}" ABSOLUTE)
#              set(_lst_file_srcs "-I${_pro_include}\n${_lst_file_srcs}")
#          endforeach()

          file(WRITE ${_ts_lst_file} "${_lst_file_srcs}")
        endif()
        add_custom_command(OUTPUT ${_ts_file}
            COMMAND ${Qt5_LUPDATE_EXECUTABLE}
            ARGS ${_lupdate_options} "@${_ts_lst_file}" -ts ${_ts_file}
            DEPENDS ${_my_sources} ${_ts_lst_file} VERBATIM)
    endforeach()
    qt5_add_translation(${_qm_files} ${_my_tsfiles})
    set(${_qm_files} ${${_qm_files}} PARENT_SCOPE)
endfunction()

MACRO(COMPILE_QT_TRANSLATIONS)
  IF(QT_TSS)
    SET_SOURCE_FILES_PROPERTIES(${QT_TSS} PROPERTIES OUTPUT_LOCATION "${CMAKE_BINARY_DIR}/translations")

    IF(WITH_UPDATE_TRANSLATIONS)
      SET(_TRANS ${ARGN} ${QT_UIS})
      IF(USE_QT5)
        QT5FIXED_CREATE_TRANSLATION(QT_QMS ${_TRANS} ${QT_TSS})
      ELSEIF(USE_QT4)
        QT4_CREATE_TRANSLATION(QT_QMS ${_TRANS} ${QT_TSS})
      ENDIF()
    ELSE()
      IF(USE_QT5)
        QT5_ADD_TRANSLATION(QT_QMS ${QT_TSS})
      ELSEIF(USE_QT4)
        QT4_ADD_TRANSLATION(QT_QMS ${QT_TSS})
      ENDIF()
    ENDIF()

    SOURCE_GROUP("translations" FILES ${QT_TSS})
  ENDIF()
ENDMACRO()

MACRO(SET_QT_SOURCES)
  IF(USE_QT)
    # Qt generated files
    SET(QT_SOURCES ${QT_MOCS_CPPS} ${QT_UIS_HEADERS} ${QT_QRCS_CPPS} ${QT_QMS})

    IF(QT_SOURCES)
      SOURCE_GROUP("generated" FILES ${QT_SOURCES})
      SET_SOURCES_FLAGS(${QT_SOURCES})
    ENDIF()
  ENDIF()
ENDMACRO()

MACRO(LINK_QT_LIBRARY _TARGET _NAME)
  IF(WIN32)
    SET(_PREFIX "Qt5")
    SET(_EXT "lib")
  ELSE()
    SET(_PREFIX "libQt5")
    SET(_EXT "a")
  ENDIF()
  SET(_LIB "${QT_LIBRARY_DIR}/${_PREFIX}${_NAME}.${_EXT}")
  IF(EXISTS ${_LIB})
    TARGET_LINK_LIBRARIES(${_TARGET} optimized ${_LIB})
  ENDIF()
  SET(_LIB "${QT_LIBRARY_DIR}/${_PREFIX}${_NAME}d.${_EXT}")
  IF(EXISTS ${_LIB})
    TARGET_LINK_LIBRARIES(${_TARGET} debug ${_LIB})
  ENDIF()
ENDMACRO()

MACRO(LINK_QT_PLUGIN _TARGET _TYPE _NAME)
  IF(WIN32)
    SET(_PREFIX "")
    SET(_EXT "lib")
  ELSE()
    SET(_PREFIX "lib")
    SET(_EXT "a")
  ENDIF()
  SET(_LIB "${QT_PLUGINS_DIR}/${_TYPE}/${_PREFIX}${_NAME}.${_EXT}")
  IF(EXISTS ${_LIB})
    TARGET_LINK_LIBRARIES(${_TARGET} optimized ${_LIB})
  ENDIF()
  SET(_LIB "${QT_PLUGINS_DIR}/${_TYPE}/${_PREFIX}${_NAME}d.${_EXT}")
  IF(EXISTS ${_LIB})
    TARGET_LINK_LIBRARIES(${_TARGET} debug ${_LIB})
  ENDIF()
ENDMACRO()

MACRO(LINK_QT_LIBRARIES _TARGET)
  IF(USE_QT)
    IF(USE_QT5)
      IF("${Qt5Core_VERSION_STRING}" VERSION_LESS "5.3" OR CMAKE_VERSION VERSION_LESS "2.8.12")
        QT5_USE_MODULES(${_TARGET} ${QT_MODULES_USED})
      ELSE()
        FOREACH(_MODULE ${QT_MODULES_USED})
          IF(TARGET "Qt5::${_MODULE}")
            TARGET_LINK_LIBRARIES(${_TARGET} "Qt5::${_MODULE}")
          ENDIF()
        ENDFOREACH()
      ENDIF()

      GET_TARGET_PROPERTY(_TYPE ${_TARGET} TYPE)

      # Link to QtMain
      IF(_TYPE STREQUAL "EXECUTABLE" AND CMAKE_VERSION VERSION_LESS "2.8.11")
        TARGET_LINK_LIBRARIES(${_TARGET} ${Qt5Core_QTMAIN_LIBRARIES})
      ENDIF()

      IF(QT_STATIC)
        ADD_DEFINITIONS(-DQT_STATICPLUGIN)

        FOREACH(_MODULE ${QT_MODULES_USED})
          IF(_MODULE STREQUAL "Core")
            IF(APPLE)
              # pcre is needed since Qt 5.5
              SET(PCRE_LIBRARY "${QT_LIBRARY_DIR}/libqtpcre.a")

              IF(NOT EXISTS ${PCRE_LIBRARY})
                FIND_LIBRARY(PCRE_LIBRARY pcre16 pcre)
              ENDIF()

              IF(NOT EXISTS ${PCRE_LIBRARY})
                MESSAGE(FATAL_ERROR "PCRE is required since Qt 5.5")
              ENDIF()

              FIND_LIBRARY(FOUNDATION_FRAMEWORK Foundation)
              FIND_LIBRARY(CARBON_FRAMEWORK Carbon)
              FIND_LIBRARY(SECURITY_FRAMEWORK Security)

              TARGET_LINK_LIBRARIES(${_TARGET}
                ${PCRE_LIBRARY}
                ${FOUNDATION_FRAMEWORK}
                ${CARBON_FRAMEWORK}
                ${SECURITY_FRAMEWORK})
            ELSEIF(WIN32)
              SET(PCRE_LIB "${QT_LIBRARY_DIR}/qtpcre.lib")

              IF(EXISTS ${PCRE_LIB})
                TARGET_LINK_LIBRARIES(${_TARGET} ${PCRE_LIB})
              ENDIF()
            ELSEIF(UNIX)
              # pcre is needed since Qt 5.5
              SET(PCRE_LIBRARY "${QT_LIBRARY_DIR}/libqtpcre.a")

              IF(NOT EXISTS ${PCRE_LIBRARY})
                FIND_LIBRARY(PCRE_LIBRARY pcre16 pcre)
              ENDIF()

              IF(NOT EXISTS ${PCRE_LIBRARY})
                MESSAGE(FATAL_ERROR "PCRE is required since Qt 5.5")
              ENDIF()

              SET(QT_LIBRARIES ${QT_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} -ldl -lrt)

              TARGET_LINK_LIBRARIES(${_TARGET} ${PCRE_LIBRARY} -ldl -lrt)
            ENDIF()
          ENDIF()
          IF(_MODULE STREQUAL "Network")
            FIND_PACKAGE(OpenSSL REQUIRED)
            FIND_PACKAGE(MyZLIB REQUIRED)
            TARGET_LINK_LIBRARIES(${_TARGET} ${OPENSSL_LIBRARIES} ${ZLIB_LIBRARIES})

            IF(WIN32)
              TARGET_LINK_LIBRARIES(${_TARGET}
                ${WINSDK_LIBRARY_DIR}/Crypt32.lib
                ${WINSDK_LIBRARY_DIR}/WS2_32.Lib
                ${WINSDK_LIBRARY_DIR}/IPHlpApi.Lib)
            ENDIF()
          ENDIF()
          IF(_MODULE STREQUAL "Gui")
            FIND_PACKAGE(MyPNG REQUIRED)
            FIND_PACKAGE(JPEG REQUIRED)

            TARGET_LINK_LIBRARIES(${_TARGET} ${PNG_LIBRARIES} ${JPEG_LIBRARIES})

            LINK_QT_LIBRARY(${_TARGET} PrintSupport)
            LINK_QT_LIBRARY(${_TARGET} PlatformSupport)

            IF(WIN32)
              TARGET_LINK_LIBRARIES(${_TARGET}
                ${WINSDK_LIBRARY_DIR}/Imm32.lib
                ${WINSDK_LIBRARY_DIR}/OpenGL32.lib
                ${WINSDK_LIBRARY_DIR}/WinMM.Lib)

              LINK_QT_PLUGIN(${_TARGET} printsupport windowsprintersupport)
              LINK_QT_PLUGIN(${_TARGET} platforms qwindows)
            ELSEIF(APPLE)
              # Cups needs .dylib
              SET(OLD_CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES})
              SET(CMAKE_FIND_LIBRARY_SUFFIXES .dylib)
              FIND_LIBRARY(CUPS_LIBRARY cups)
              SET(CMAKE_FIND_LIBRARY_SUFFIXES ${OLD_CMAKE_FIND_LIBRARY_SUFFIXES})

              FIND_LIBRARY(IOKIT_FRAMEWORK IOKit)
              FIND_LIBRARY(COCOA_FRAMEWORK Cocoa)
              FIND_LIBRARY(SYSTEMCONFIGURATION_FRAMEWORK SystemConfiguration)
              FIND_LIBRARY(OPENGL_FRAMEWORK NAMES OpenGL)

              TARGET_LINK_LIBRARIES(${_TARGET}
                ${CUPS_LIBRARY}
                ${COCOA_FRAMEWORK}
                ${SYSTEMCONFIGURATION_FRAMEWORK}
                ${IOKIT_FRAMEWORK}
                ${OPENGL_FRAMEWORK})

              LINK_QT_PLUGIN(${_TARGET} printsupport cocoaprintersupport)
              LINK_QT_PLUGIN(${_TARGET} platforms qcocoa)
            ELSE()
              # order is very important there
              LINK_QT_PLUGIN(${_TARGET} platforms qxcb)
              LINK_QT_PLUGIN(${_TARGET} xcbglintegrations qxcb-glx-integration)

              LINK_QT_LIBRARY(${_TARGET} XcbQpa)
              LINK_QT_LIBRARY(${_TARGET} PlatformSupport)

              TARGET_LINK_LIBRARIES(${_TARGET} -lX11-xcb -lXi -lSM -lICE -lxcb -lGL -lxcb-glx)

              IF(EXISTS "${QT_LIBRARY_DIR}/libxcb-static.a")
                TARGET_LINK_LIBRARIES(${_TARGET} "${QT_LIBRARY_DIR}/libxcb-static.a")
              ENDIF()

              TARGET_LINK_LIBRARIES(${_TARGET} -lfontconfig)

              LINK_QT_LIBRARY(${_TARGET} DBus)
            ENDIF()

            LINK_QT_PLUGIN(${_TARGET} imageformats qgif)
            LINK_QT_PLUGIN(${_TARGET} imageformats qicns)
            LINK_QT_PLUGIN(${_TARGET} imageformats qico)
            LINK_QT_PLUGIN(${_TARGET} imageformats qjpeg)
            LINK_QT_PLUGIN(${_TARGET} imageformats qmng)
            LINK_QT_PLUGIN(${_TARGET} imageformats qwebp)

            # harfbuzz is needed since Qt 5.3
            IF(WIN32)
              SET(HB_LIB "${QT_LIBRARY_DIR}/qtharfbuzzng.lib")
            ELSE()
              SET(HB_LIB "${QT_LIBRARY_DIR}/libqtharfbuzzng.a")
            ENDIF()

            IF(EXISTS ${HB_LIB})
              TARGET_LINK_LIBRARIES(${_TARGET} ${HB_LIB})
            ENDIF()

            # freetype is needed since Qt 5.5
            IF(WIN32)
              SET(FREETYPE_LIBRARY "${QT_LIBRARY_DIR}/qtfreetype.lib")
            ELSE()
              SET(FREETYPE_LIBRARY "${QT_LIBRARY_DIR}/libqtfreetype.a")

              IF(NOT EXISTS ${FREETYPE_LIBRARY})
                FIND_PACKAGE(Freetype)
              ENDIF()
            ENDIF()

            IF(EXISTS ${FREETYPE_LIBRARY})
              TARGET_LINK_LIBRARIES(${_TARGET} ${FREETYPE_LIBRARY})
            ENDIF()
          ENDIF()
          IF(_MODULE STREQUAL "Multimedia")
            LINK_QT_PLUGIN(${_TARGET} mediaservice qtmedia_audioengine)

            IF(WIN32)
              LINK_QT_PLUGIN(${_TARGET} audio qtaudio_windows)
              LINK_QT_PLUGIN(${_TARGET} mediaservice dsengine)
              LINK_QT_PLUGIN(${_TARGET} mediaservice wmfengine)

              TARGET_LINK_LIBRARIES(${_TARGET} ${WINSDK_LIBRARY_DIR}/strmiids.lib)
            ELSEIF(APPLE)
              LINK_QT_PLUGIN(${_TARGET} audio qtaudio_coreaudio)

              FIND_LIBRARY(COREAUDIO_FRAMEWORK CoreAudio)
              FIND_LIBRARY(AUDIOUNIT_FRAMEWORK AudioUnit)
              FIND_LIBRARY(AUDIOTOOLBOX_FRAMEWORK AudioToolbox)

              TARGET_LINK_LIBRARIES(${_TARGET}
                ${AUDIOUNIT_FRAMEWORK}
                ${COREAUDIO_FRAMEWORK}
                ${AUDIOTOOLBOX_FRAMEWORK})
            ENDIF()
          ENDIF()
          IF(_MODULE STREQUAL "Widgets")
            LINK_QT_PLUGIN(${_TARGET} accessible qtaccessiblewidgets)
          ENDIF()
          IF(_MODULE STREQUAL "Sql")
            LINK_QT_PLUGIN(${_TARGET} sqldrivers qsqlite)
          ENDIF()
          IF(_MODULE STREQUAL "Svg")
            LINK_QT_PLUGIN(${_TARGET} imageformats qsvg)
            LINK_QT_PLUGIN(${_TARGET} iconengines qsvgicon)
          ENDIF()
          IF(_MODULE STREQUAL "Qml")
            IF(APPLE)
              LINK_QT_PLUGIN(${_TARGET} qmltooling qmldbg_tcp)
            ENDIF()
          ENDIF()
        ENDFOREACH()
      ENDIF()
    ENDIF()
    IF(USE_QT4)
      TARGET_LINK_LIBRARIES(${_TARGET} ${QT_LIBRARIES})
      IF(APPLE)
        FIND_LIBRARY(SECURITY_FRAMEWORK Security)
        TARGET_LINK_LIBRARIES(${_TARGET} ${SECURITY_FRAMEWORK})
      ENDIF()
    ENDIF()
  ENDIF()
ENDMACRO()

MACRO(INSTALL_QT_TRANSLATIONS _TARGET)
  IF(QT_QMS)
    IF(APPLE AND RESOURCES_DIR)
      ADD_CUSTOM_COMMAND(TARGET ${_TARGET} PRE_BUILD COMMAND mkdir -p ${RESOURCES_DIR}/translations)

      # Copying all Qt translations to bundle
      FOREACH(_QM ${QT_QMS})
        ADD_CUSTOM_COMMAND(TARGET ${_TARGET} POST_BUILD COMMAND cp ARGS ${_QM} ${RESOURCES_DIR}/translations)
      ENDFOREACH()
    ELSE()
      # Install all applications Qt translations
      INSTALL(FILES ${QT_QMS} DESTINATION ${SHARE_PREFIX}/translations)
    ENDIF()

    IF(WIN32 OR APPLE)
      IF(USE_QT5)
        SET(_QM_BASE "qtbase")
      ELSEIF(USE_QT4)
        SET(_QM_BASE "qt")
      ENDIF()
      SET(PAK_FILES)
      # Copy Qt standard translations
      FOREACH(_LANG ${QT_LANGS})
        SET(LANG_FILE "${QT_TRANSLATIONS_DIR}/${_QM_BASE}_${_LANG}.qm")
        IF(EXISTS ${LANG_FILE})
          IF(WIN32)
            INSTALL(FILES ${LANG_FILE} DESTINATION ${SHARE_PREFIX}/translations)
          ELSEIF(APPLE AND RESOURCES_DIR)
            ADD_CUSTOM_COMMAND(TARGET ${_TARGET} POST_BUILD COMMAND cp ARGS ${LANG_FILE} ${RESOURCES_DIR}/translations)
          ENDIF()
        ENDIF()
        # Install WebEngine translations
        IF(USE_QT5)
          FOREACH(_MODULE ${QT_MODULES_USED})
            IF(_MODULE STREQUAL "WebEngine")
              SET(PAK_MASK "${QT_TRANSLATIONS_DIR}/qtwebengine_locales/${_LANG}*.pak")
              FILE(GLOB PAK_FILE ${PAK_MASK})
              IF(PAK_FILE)
                LIST(APPEND PAK_FILES ${PAK_FILE})
              ENDIF()
            ENDIF()
          ENDFOREACH()
        ENDIF()
      ENDFOREACH()
      IF(PAK_FILES)
        INSTALL(FILES ${PAK_FILES} DESTINATION ${SHARE_PREFIX}/translations/qtwebengine_locales)
      ENDIF()
    ENDIF()
  ENDIF()
ENDMACRO()

MACRO(INSTALL_QT_EXECUTABLE _NAME)
  IF(WIN32)
    SET(_EXT "exe")
  ENDIF()
  SET(_BIN "${QT_BINARY_DIR}/${_NAME}.${_EXT}")
  IF(EXISTS ${_BIN})
    INSTALL(FILES ${_BIN} DESTINATION ${BIN_PREFIX})
  ENDIF()
ENDMACRO()

MACRO(INSTALL_LIBRARY _NAME)
  IF(WIN32)
    SET(_PREFIX "")
    SET(_EXT "dll")
  ELSE()
    SET(_PREFIX "lib")
    SET(_EXT "so")
  ENDIF()
  SET(_LIB_MASK "${QT_BINARY_DIR}/${_PREFIX}${_NAME}.${_EXT}")
  FILE(GLOB _LIBS ${_LIB_MASK})
  IF(_LIBS)
    INSTALL(FILES ${_LIBS} DESTINATION ${BIN_PREFIX})
  ENDIF()
ENDMACRO()

MACRO(INSTALL_MISC _SRC _DST)
  SET(_MISC_MASK "${_SRC}")
  FILE(GLOB _MISC ${_MISC_MASK})
  IF(_MISC)
    INSTALL(FILES ${_MISC} DESTINATION ${BIN_PREFIX}/${_DST})
  ENDIF()
ENDMACRO()

MACRO(INSTALL_QT_LIBRARY _NAME)
  IF(WIN32)
    SET(_PREFIX "Qt5")
    SET(_EXT "dll")
  ELSE()
    SET(_PREFIX "libQt5")
    SET(_EXT "so")
  ENDIF()
  SET(_LIB "${QT_BINARY_DIR}/${_PREFIX}${_NAME}.${_EXT}")
  IF(EXISTS ${_LIB})
    INSTALL(FILES ${_LIB} DESTINATION ${BIN_PREFIX})
  ENDIF()
ENDMACRO()

MACRO(INSTALL_QT_PLUGIN _TYPE _NAME)
  IF(WIN32)
    SET(_PREFIX "")
    SET(_EXT "dll")
  ELSE()
    SET(_PREFIX "lib")
    SET(_EXT "so")
  ENDIF()
  SET(_LIB "${QT_PLUGINS_DIR}/${_TYPE}/${_PREFIX}${_NAME}.${_EXT}")
  IF(EXISTS ${_LIB})
    INSTALL(FILES ${_LIB} DESTINATION ${BIN_PREFIX}/${_TYPE})
  ENDIF()
ENDMACRO()

MACRO(INSTALL_QT_LIBRARIES)
  IF(WIN32 AND USE_QT)
    IF(NOT QT_STATIC)
      # Install Qt libraries
      FOREACH(_MODULE ${QT_MODULES_USED})
        IF(USE_QT5)
          SET(_MODULE_NAME "Qt5${_MODULE}")
        ELSEIF(USE_QT4)
          SET(_MODULE_NAME "Qt${_MODULE}4")
        ENDIF()

        # Library
        IF(EXISTS ${QT_BINARY_DIR}/${_MODULE_NAME}.dll)
          INSTALL(FILES "${QT_BINARY_DIR}/${_MODULE_NAME}.dll" DESTINATION ${BIN_PREFIX})
        ENDIF()

        # Plugins
        IF(USE_QT4)
          IF(_MODULE STREQUAL "Gui")
            INSTALL_QT_PLUGIN(imageformats qgif4)
            INSTALL_QT_PLUGIN(imageformats qico4)
            INSTALL_QT_PLUGIN(imageformats qjpeg4)
          ENDIF()

          IF(_MODULE STREQUAL "Sql")
            INSTALL_QT_PLUGIN(sqldrivers qsqlite4)
          ENDIF()

          IF(_MODULE STREQUAL "Svg")
            INSTALL_QT_PLUGIN(imageformats qsvg4)
            INSTALL_QT_PLUGIN(iconengines qsvgicon4)
          ENDIF()
        ENDIF()

        IF(USE_QT5)
          IF(_MODULE STREQUAL "Core")
            INSTALL_LIBRARY(icuin*)
            INSTALL_LIBRARY(icuuc*)
            INSTALL_LIBRARY(icudt*)
            INSTALL_QT_PLUGIN(platforms qwindows)
          ENDIF()

          IF(_MODULE STREQUAL "Gui")
            INSTALL_QT_PLUGIN(imageformats qgif)
            INSTALL_QT_PLUGIN(imageformats qicns)
            INSTALL_QT_PLUGIN(imageformats qico)
            INSTALL_QT_PLUGIN(imageformats qjpeg)
            INSTALL_QT_PLUGIN(imageformats qmng)
            INSTALL_QT_PLUGIN(imageformats qwebp)
            INSTALL_QT_PLUGIN(printsupport windowsprintersupport)
          ENDIF()

          IF(_MODULE STREQUAL "Multimedia")
            INSTALL_QT_PLUGIN(mediaservice qtmedia_audioengine)

            IF(WIN32)
              INSTALL_QT_PLUGIN(audio qtaudio_windows)
              INSTALL_QT_PLUGIN(mediaservice dsengine)
              INSTALL_QT_PLUGIN(mediaservice wmfengine)
            ELSEIF(APPLE)
              INSTALL_QT_PLUGIN(audio qtaudio_coreaudio)
            ENDIF()
          ENDIF()

          IF(_MODULE STREQUAL "WebEngine")
            INSTALL_QT_EXECUTABLE(QtWebEngineProcess)
            INSTALL_MISC(${QT_RESOURCES_DIR}/* "")
            IF(WIN32)
              INSTALL_MISC(${QT_BINARY_DIR}/qtwebengine/*.dll qtwebengine)
            ENDIF()
          ENDIF()

          IF(_MODULE STREQUAL "Widgets")
            INSTALL_QT_PLUGIN(accessible qtaccessiblewidgets)
          ENDIF()

          # Quick and Qml need V8
          IF(_MODULE MATCHES "^(Quick|Qml$)")
            INSTALL_QT_LIBRARY(V8)
          ENDIF()

          IF(_MODULE STREQUAL "Sql")
            INSTALL_QT_PLUGIN(sqldrivers qsqlite)
          ENDIF()

          IF(_MODULE STREQUAL "Svg")
            INSTALL_QT_PLUGIN(imageformats qsvg)
            INSTALL_QT_PLUGIN(iconengines qsvgicon)
          ENDIF()
        ENDIF()
      ENDFOREACH()

      # Install zlib DLL if found in Qt directory
      INSTALL_LIBRARY(zlib1)

      # Install OpenSSL from Qt directory
      INSTALL_LIBRARY(libeay*)
      INSTALL_LIBRARY(ssleay*)
    ENDIF()
  ENDIF()
ENDMACRO()

MACRO(INSTALL_QT_MISC _TARGET)
# Not needed anymore because qt_menu.nib is included in Qt resources
#  IF(USE_QT4 AND QT_STATIC)
    # Copying qt_menu.nib to bundle
#    IF(APPLE AND MAC_RESOURCES_DIR)
#      ADD_CUSTOM_COMMAND(TARGET ${_TARGET} POST_BUILD COMMAND cp -R ARGS ${MAC_RESOURCES_DIR}/qt_menu.nib ${RESOURCES_DIR})
#    ENDIF()
#  ENDIF()
ENDMACRO()
