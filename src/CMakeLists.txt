# Build the Dream Library and Executable.
#file(GLOB DREAM_SRC "*.h" "*.cpp")
#file(GLOB LUA_SRC "Lua/*.h" "Lua/*.cpp")
#file(GLOB OBJ_SRC "Obj*.h" "Obj*.cpp")
file(GLOB SCENEGRAPH_SRC "Scenegraph*.h" "Scenegraph*.cpp")
file(GLOB UNIT_SRC "Unit/*.h" "Unit/*.cpp")
file(GLOB ANIMATION_SRC "Animation/*.h" "Animation/*.cpp")

#add_executable (Dream ${UNIT_SRC}) # ${DREAM_SRC} ${LUA_SRC} ${OBJ_SRC} ${SCENEGRAPH_SRC} )
add_library (Dream SHARED ${UNIT_SRC} ${ANIMATION_SRC} ${SCENEGRAPH_SRC}) #${ROOT_SRC} ${LUA_SRC} ${OBJ_SRC}  )
