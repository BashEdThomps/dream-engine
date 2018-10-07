# Locate NanoVG
# NANOVG_LIB
# NANOVG_FOUND, if false, do not try to link to NANOVG
# NANOVG_INC_DIR, where to find the headers

FIND_PATH(NANOVG_INCLUDE_DIR nanovg/nanovg.h
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

FIND_LIBRARY(NANOVG_LIB
  NAMES nanovg
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


SET(NANOVG_FOUND "NO")

IF(NANOVG_INCLUDE_DIR AND NANOVG_LIB)
    SET(NANOVG_FOUND "YES")
    message("Found NanoVG Headers " ${NANOVG_INCLUDE_DIR})
    message("Found NanoVG Libs    " ${NANOVG_LIB})
ELSE()
    message(FATAL_ERROR "Unable to find NanoVG")
ENDIF()
