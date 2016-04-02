# Build the Dream::Unit Library and Test executable.

# Unit
file(GLOB TEST_UNIT_SRC "Unit/*.h" "Unit/*.cpp")

# Scene
file(GLOB TEST_SCENE_SRC "Scene/*.h" "Scene/*.cpp")
file(GLOB TEST_SCENE_SCENEGRAPH_SRC "Scene/Scenegraph*.h" "Scene/Scenegraph*.cpp")

# Resource
file(GLOB TEST_RESOURCE_SRC "Resource/*.h" "Resource/*.cpp")
file(GLOB TEST_RESOURCE_ANIMATION_SRC "Resource/Animation/*.h" "Resource/Animation/*.cpp")
#file(GLOB LUA_SRC "Lua/*.h" "Lua/*.cpp")
#file(GLOB OBJ_SRC "Obj*.h" "Obj*.cpp")

# Dream
file(GLOB TEST_DREAM_SRC "*.h" "*.cpp")

# Targets
add_executable (DreamTest ${TEST_UNIT_SRC} ${TEST_RESOURCE_SRC} ${TEST_RESOURCE_ANIMATION_SRC} ${TEST_SCENE_SRC} ${TEST_SCENE_SCENEGRAPH_SRC} ${TEST_DREAM_SRC})
target_link_libraries(DreamTest Dream)
