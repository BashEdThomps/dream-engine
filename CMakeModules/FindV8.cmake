# Locate V8
# This module defines
# V8_LIBRARY
# V8_FOUND, if false, do not try to link to V8
# V8_INCLUDE_DIR, where to find the headers

FIND_PATH(V8_INCLUDE_DIR v8.h
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/include/v8
  /usr/include/v8
  /sw/include/v8
  /opt/local/include/v8
  /opt/csw/include/v8
  /opt/include/v8
  /usr/freeware/include/v8
  /devel
)
FIND_LIBRARY(V8
  NAMES v8 v8.ia32 v8.x64 libv8
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

FIND_LIBRARY(V8_BASE_LIBRARY
  NAMES v8_base v8_base.ia32 v8_base.x64 libv8_base
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

FIND_LIBRARY(V8_LIBBASE_LIBRARY
  NAMES v8_libbase v8_libbase.ia32 v8_libbase.x64 libv8_libbase
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

FIND_LIBRARY(V8_LIBPLATFORM_LIBRARY
  NAMES v8_libplatform v8_libplatform.ia32 v8_libplatform.x64 libv8_libplatform
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
FIND_LIBRARY(V8_NOSNAPSHOT_LIBRARY
  NAMES v8_nosnapshot libv8_nosnapshot
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

# icuuc and icui18n build fine on all platforms
FIND_LIBRARY(V8_ICUUC_LIBRARY
  NAMES icuuc libicuuc
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

FIND_LIBRARY(V8_ICUI18N_LIBRARY
  NAMES icui18n libicui18n
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

message("V8 Include Path " ${V8_INCLUDE_DIR})
message("V8 Lib " ${V8})
message("V8 libplatform Lib " ${V8_LIBPLATFORM_LIBRARY})
message("V8 Base Lib " ${V8_BASE_LIBRARY})
message("V8 Libbase Lib " ${V8_LIBBASE_LIBRARY})
message("V8 NoSnapshot Lib " ${V8_NOSNAPSHOT_LIBRARY})
message("V8 ICUUC Lib " ${V8_ICUUC_LIBRARY})
message("V8 ICU I18N Lib " ${V8_ICUI18N_LIBRARY})

SET(V8_FOUND "NO")

IF(
  V8                     AND
  V8_BASE_LIBRARY        AND
  V8_LIBBASE_LIBRARY     AND
  V8_LIBPLATFORM_LIBRARY AND
  V8_NOSNAPSHOT_LIBRARY  AND
  V8_ICUUC_LIBRARY       AND
  V8_ICUI18N_LIBRARY     AND
  V8_INCLUDE_DIR)
  SET(V8_FOUND "YES")
  message("Found v8 Libraries")
  set(
    V8_LIBRARY
    ${V8}
    ${V8_BASE_LIBRARY}
    ${V8_LIBBASE_LIBRARY}
    ${V8_LIBPLATFORM_LIBRARY}
    ${V8_NOSNAPSHOT_LIBRARY}
    ${V8_ICUUC_LIBRARY}
    ${V8_ICUI18N_LIBRARY}
  )
ELSE()
  message(FATAL_ERROR "Unable to find v8 Libraries")
ENDIF(
  V8                     AND
  V8_BASE_LIBRARY        AND
  V8_LIBBASE_LIBRARY     AND
  V8_LIBPLATFORM_LIBRARY AND
  V8_NOSNAPSHOT_LIBRARY  AND
  V8_ICUUC_LIBRARY       AND
  V8_ICUI18N_LIBRARY     AND
  V8_INCLUDE_DIR
)
