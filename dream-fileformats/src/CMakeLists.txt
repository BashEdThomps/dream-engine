file(GLOB SRC "TestMain.cpp" "obj/*.h" "obj/*.cpp" )
add_library    (DreamFileFormats SHARED ${SRC})
