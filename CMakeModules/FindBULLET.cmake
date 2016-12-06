# Locate Bullet
# This module defines
# BULLET_LIBS
# BULLET_FOUND, if false, do not try to link to BULLET
# BULLET_INC_DIR, where to find the headers

FIND_PATH(BULLET_INCLUDE_DIR btBulletDynamicsCommon.h
    ~/Library/Framework
    /Library/Frameworks
    /usr/local/include/bullet
    /usr/include/bullet
    /sw/include/bullet
    /opt/local/include/bullet
    /opt/csw/include/bullet
    /opt/include/bullet
    /usr/freeware/include/bullet
    /devel
)

FIND_LIBRARY(BULLET_DYNAMICS_LIB
  NAMES BulletDynamics
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

FIND_LIBRARY(BULLET_COLLISION_LIB
  NAMES BulletCollision
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/lib
  /usr/lib
  /sw/lib
  /opt/local/lib
  /opt/csw/lib
  /opt/lib
  /usr/freeware/lib64
)

#FIND_LIBRARY(BULLET_INVERSE_DYNAMICS_LIB
#  NAMES BulletInverseDynamics
#  PATHS
#  ~/Library/Frameworks
#  /Library/Frameworks
#  /usr/local/lib
#  /usr/lib
#  /sw/lib
#  /opt/local/lib
#  /opt/csw/lib
#  /opt/lib
#  /usr/freeware/lib64
#)

FIND_LIBRARY(BULLET_SOFT_BODY_LIB
  NAMES BulletSoftBody
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/lib
  /usr/lib
  /sw/lib
  /opt/local/lib
  /opt/csw/lib
  /opt/lib
  /usr/freeware/lib64
)

FIND_LIBRARY(LINEARMATH_LIB
  NAMES LinearMath
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/lib
  /usr/lib
  /sw/lib
  /opt/local/lib
  /opt/csw/lib
  /opt/lib
  /usr/freeware/lib64
)

message("Bullet Headers "        ${BULLET_INCLUDE_DIR})
message("LinearMath "            ${LINEARMATH_LIB})
message("BulletCollision "       ${BULLET_COLLISION_LIB})
message("BulletDynamics "        ${BULLET_DYNAMICS_LIB})
#message("BulletInverseDynamics " ${BULLET_INVERSE_DYNAMICS_LIB})
message("BulletSoftBody "        ${BULLET_SOFT_BODY_LIB})

SET(BULLET_FOUND "NO")

IF(
  BULLET_INCLUDE_DIR          AND
  LINEARMATH_LIB              AND
  BULLET_DYNAMICS_LIB         AND
#  BULLET_INVERSE_DYNAMICS_LIB AND
  BULLET_COLLISION_LIB        AND
  BULLET_SOFT_BODY_LIB
)
  SET(BULLET_FOUND "YES")

  #SET(BULLET_INCLUDE_DIR ${_BULLET_INCLUDE_DIR})
  SET(BULLET_LIBRARY
    ${LINEARMATH_LIB}
    ${BULLET_COLLISION_LIB}
    ${BULLET_DYNAMICS_LIB}
    ${BULLET_SOFT_BODY_LIB}
#    ${BULLET_INVERSE_DYNAMICS_LIB}
  )
    message("Found Bullet Headers " ${BULLET_INCLUDE_DIR})
    message("Found Bullet Libs " ${BULLET_LIS})
ELSE()
  message(FATAL_ERROR "Unable to find Bullet")
ENDIF(
  BULLET_INCLUDE_DIR          AND
  LINEARMATH_LIB              AND
  BULLET_DYNAMICS_LIB         AND
#  BULLET_INVERSE_DYNAMICS_LIB AND
  BULLET_COLLISION_LIB        AND
  BULLET_SOFT_BODY_LIB
)
