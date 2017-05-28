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

FIND_PACKAGE_HELPER(JsonCpp json/json.h QUIET)

IF(JSONCPP_FOUND)
  SET(JSONCPP_DEFINITIONS)

  PARSE_VERSION_OTHER(${JSONCPP_INCLUDE_DIR}/json/version.h JSONCPP_VERSION_MAJOR JSONCPP_VERSION_MINOR JSONCPP_VERSION_PATCH)
  SET(JSONCPP_VERSION "${JSONCPP_VERSION_MAJOR}.${JSONCPP_VERSION_MINOR}.${JSONCPP_VERSION_PATCH}")

  MESSAGE_VERSION_PACKAGE_HELPER(JsonCpp ${JSONCPP_VERSION} ${JSONCPP_LIBRARIES})
ENDIF()
