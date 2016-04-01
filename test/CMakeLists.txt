# Build the DreamUnit Library and Test executable.
#file(GLOB DREAM_SRC "*.h" "*.cpp")
#file(GLOB LUA_SRC "Lua/*.h" "Lua/*.cpp")
#file(GLOB OBJ_SRC "Obj*.h" "Obj*.cpp")
file(GLOB SCENEGRAPH_SRC "Scenegraph*.h" "Scenegraph*.cpp")
file(GLOB UNIT_SRC "Unit/*.h" "Unit/*.cpp")
file(GLOB ANIMATION_SRC "Animation/*.h" "Animation/*.cpp")

add_executable (DreamTest TestMain.cpp ${UNIT_SRC} ${ANIMATION_SRC} ${SCENEGRAPH_SRC}) #${DREAM_SRC} ${LUA_SRC} ${OBJ_SRC} )

target_link_libraries(DreamTest Dream)
