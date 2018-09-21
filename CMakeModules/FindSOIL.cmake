# Locate SOIL
# SOIL_LIB
# SOIL_FOUND, if false, do not try to link to SOIL
# SOIL_INC_DIR, where to find the headers

FIND_PATH(SOIL_INCLUDE_DIR SOIL/SOIL.h
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

FIND_LIBRARY(SOIL_LIB
  NAMES SOIL
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

SET(SOIL_FOUND "NO")

IF(SOIL_INCLUDE_DIR AND SOIL_LIB)
    SET(SOIL_FOUND "YES")
    message("Found SOIL Headers " ${SOIL_INCLUDE_DIR})
    message("Found SOIL Libs    " ${SOIL_LIB})
ELSE()
    message(FATAL_ERROR "Unable to find SOIL")
ENDIF()
