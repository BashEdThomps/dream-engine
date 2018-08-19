# Locate TinySpline
# TINY_SPLINE_LIB
# TINY_SPLINE_FOUND, if false, do not try to link to TINY_SPLINE
# TINY_SPLINE_INC_DIR, where to find the headers

FIND_PATH(TINY_SPLINE_INCLUDE_DIR tinysplinecpp.h
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

FIND_LIBRARY(TINY_SPLINE_LIB
  NAMES tinyspline
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

FIND_LIBRARY(TINY_SPLINE_CPP_LIB
  NAMES tinysplinecpp
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


SET(TINY_SPLINE_FOUND "NO")

IF(TINY_SPLINE_INCLUDE_DIR AND TINY_SPLINE_LIB)
    SET(TINY_SPLINE_FOUND "YES")
    message("Found TinySpline Headers " ${TINY_SPLINE_INCLUDE_DIR})
    message("Found TinySpline Libs    " ${TINY_SPLINE_LIB})
    message("Found TinySpline CPP Libs    " ${TINY_SPLINE_CPP_LIB})
ELSE()
    message(FATAL_ERROR "Unable to find TinySpline")
ENDIF()
