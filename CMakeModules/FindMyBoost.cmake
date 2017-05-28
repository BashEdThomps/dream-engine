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

SET(BOOST_LIBRARIES)

FIND_PACKAGE_HELPER(Boost boost/version.hpp QUIET)

IF(BOOST_INCLUDE_DIR)
  IF(NOT BOOST_VERSION)
    PARSE_VERSION_OTHER(${BOOST_INCLUDE_DIR}/boost/version.hpp BOOST_VERSION)

    IF(BOOST_VERSION)
      # Hack because patch is base 100 and minor version is base 1000
      CONVERT_NUMBER_VERSION(${BOOST_VERSION}0 1000 _VERSIONS)
      LIST(GET _VERSIONS 0 BOOST_VERSION_PATCH)
      LIST(GET _VERSIONS 1 BOOST_VERSION_MINOR)
      LIST(GET _VERSIONS 2 BOOST_VERSION_MAJOR)
      SET(BOOST_VERSION "${BOOST_VERSION_MAJOR}.${BOOST_VERSION_MINOR}.${BOOST_VERSION_PATCH}")
    ENDIF()
  ENDIF()

  SET(BOOST_FOUND ON)
ELSE()
  MESSAGE(FATAL_ERROR "Unable to find Boost")
ENDIF()

FOREACH(COMPONENT ${MyBoost_FIND_COMPONENTS})
  SET(_NAME Boost${COMPONENT})
  STRING(TOUPPER ${_NAME} _UPNAME)

  SET(${_NAME}_FIND_REQUIRED ${MyBoost_FIND_REQUIRED})

  IF(MSVC15)
    SET(_COMPILER "vc150")
  ELSEIF(MSVC14)
    SET(_COMPILER "vc140")
  ELSEIF(MSVC13)
    SET(_COMPILER "vc130")
  ELSEIF(MSVC12)
    SET(_COMPILER "vc120")
  ELSEIF(MSVC11)
    SET(_COMPILER "vc110")
  ELSEIF(MSVC10)
    SET(_COMPILER "vc100")
  ELSEIF(MSVC90)
    SET(_COMPILER "vc90")
  ELSEIF(MSVC80)
    SET(_COMPILER "vc80")
  ELSEIF(MINGW)
    SET(_COMPILER "mingw")
  ELSE()
    SET(_COMPILER "unknown")
  ENDIF()

  SET(_VERSION "${BOOST_VERSION_MAJOR}_${BOOST_VERSION_MINOR}")

  IF(DEFINED BOOST_DIR)
    SET(${_UPNAME}_DIR ${BOOST_DIR})
  ENDIF()

  FIND_PACKAGE_HELPER(${_NAME} boost/version.hpp
    RELEASE boost_${COMPONENT}-${_COMPILER}-mt-${_VERSION} boost_${COMPONENT}-mt boost_${COMPONENT}
    DEBUG boost_${COMPONENT}-${_COMPILER}-mt-gd-${_VERSION} boost_${COMPONENT}-mt-gd boost_${COMPONENT}-gd
    QUIET)

  IF(${_UPNAME}_FOUND)
    LIST(APPEND BOOST_LIBRARIES ${${_UPNAME}_LIBRARIES})
  ENDIF()
ENDFOREACH()

MESSAGE_VERSION_PACKAGE_HELPER(Boost ${BOOST_VERSION} ${BOOST_LIBRARIES})
