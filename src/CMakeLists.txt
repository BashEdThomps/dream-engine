# Build the Dream Library and Executable.
#file(GLOB DREAM_SRC "*.h" "*.cpp")
#file(GLOB LUA_SRC "lua/*.h" "lua/*.cpp")
#file(GLOB OBJ_SRC "obj/*.h" "obj/*.cpp")
#file(GLOB SCENEGRAPH_SRC "scenegraph/*.h" "scenegraph/*.cpp")
file(GLOB UNIT_SRC "unit/*.h" "unit/*.cpp")
file(GLOB ANIMATION_SRC "animation/*.h" "animation/*.cpp")

#add_executable (Dream ${UNIT_SRC}) # ${DREAM_SRC} ${LUA_SRC} ${OBJ_SRC} ${SCENEGRAPH_SRC} )
add_library (Dream SHARED ${UNIT_SRC} ${ANIMATION_SRC}) #${ROOT_SRC} ${LUA_SRC} ${OBJ_SRC} ${SCENEGRAPH_SRC} )
