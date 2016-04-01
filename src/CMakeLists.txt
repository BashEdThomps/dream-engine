# Build the Dream Library and Executable.

# Unit
file(GLOB UNIT_SRC "Unit/*.h" "Unit/*.cpp")

# JSON
file(GLOB JSON_SRC "JSON/json.hpp")

# Scene
file(GLOB SCENE_SRC "Scene/*.h" "Scene/*.cpp")
file(GLOB SCENE_SCENEGRAPH_SRC "Scene/Scenegraph/*.h" "Scene/Scenegraph/*.cpp")

# Resource
file(GLOB RESOURCE_ANIMATION_SRC "Resource/Animation/*.h" "Resource/Animation/*.cpp")
#file(GLOB LUA_SRC "Lua/*.h" "Lua/*.cpp")
#file(GLOB OBJ_SRC "Obj*.h" "Obj*.cpp")

# Dream
file(GLOB DREAM_SRC "*.h" "*.cpp")
list(REMOVE_ITEM DREAM_SRC "Main.cpp")
# Targets
add_library (Dream SHARED ${JSON_SRC} ${UNIT_SRC} ${RESOURCE_ANIMATION_SRC} ${SCENE_SRC} ${SCENE_SCENEGRAPH_SRC} ${DREAM_SRC})
add_executable (DreamMain Main.cpp)
target_link_libraries(DreamMain Dream)
