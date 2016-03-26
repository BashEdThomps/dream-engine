file(GLOB SRC "*.cpp" )
file(GLOB HEADERS "*.h")
add_library (DreamUnit SHARED ${SRC} ${HEADERS})

# Expose DreamUnit's public includes to other subprojects through cache variable.
set (DREAM_UNIT_INCLUDE_DIRS ${PROJECT_SOURCE_DIR} CACHE INTERNAL "DeamUnit: Include Directories" FORCE)
