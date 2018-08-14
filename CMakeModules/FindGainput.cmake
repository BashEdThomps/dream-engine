# Locate Gainput
# GAINPUT_LIB
# GAINPUT_FOUND, if false, do not try to link to GAINPUT
# GAINPUT_INC_DIR, where to find the headers

FIND_PATH(GAINPUT_INCLUDE_DIR gainput/gainput.h
    ~/Library/Framework
    /Library/Frameworks
    /usr/local/include
    /usr/include
    /sw/include
    /opt/local/include
    /opt/csw/include
    /opt/include
    /usr/freeware/include
    /devel
)

FIND_LIBRARY(GAINPUT_LIB
  NAMES gainput
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/lib/
  /usr/lib
  /sw/lib
  /opt/local/lib
  /opt/csw/lib
  /opt/lib
  /usr/freeware/lib64
)

SET(GAINPUT_FOUND "NO")

IF(GAINPUT_INCLUDE_DIR AND GAINPUT_LIB)
    SET(GAINPUT_FOUND "YES")
    message("Found Gainput Headers " ${GAINPUT_INCLUDE_DIR})
    message("Found Gainput Libs    " ${GAINPUT_LIB})
ELSE()
  message(FATAL_ERROR "Unable to find Gainput")
ENDIF()
