# Build the DreamUnit Library and Test executable.
project        (DreamUnit)
add_library    (${PROJECT_NAME} SHARED src/Unit.cpp src/Unit.h)
#add_executable (DreamUnitTest test/TestMain.cpp src/Unit.h)
