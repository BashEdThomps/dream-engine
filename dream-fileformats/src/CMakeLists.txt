file(GLOB SRC "obj/*.h" "obj/*.cpp" )
add_library    (DreamFileFormats SHARED ${SRC})
