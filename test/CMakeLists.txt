# Build the Dream::Unit Library and Test executable.

# Unit
file(GLOB TEST_UNIT_SRC "Unit/*.h" "Unit/*.cpp")

# Util
file(GLOB TEST_UTIL_SRC "Util/*.h" "Util/*.cpp")

# JSON
file(GLOB TEST_JSON_SRC "JSON/*.h" "JSON/*.cpp")

# Scene
file(GLOB TEST_SCENE_SRC "Scene/*.h" "Scene/*.cpp")

# Resource
file(GLOB TEST_RESOURCE_SRC "Resource/*.h" "Resource/*.cpp")
file(GLOB TEST_RESOURCE_ANIMATION_SRC "Resource/Animation/*.h" "Resource/Animation/*.cpp")
#file(GLOB LUA_SRC "Lua/*.h" "Lua/*.cpp")
#file(GLOB OBJ_SRC "Obj*.h" "Obj*.cpp")

# Dream
file(GLOB TEST_DREAM_SRC "*.h" "*.cpp")

# Targets & Stuff
add_executable (DreamTest ${TEST_UNIT_SRC} ${TEST_JSON_SRC} ${TEST_UTIL_SRC} ${TEST_RESOURCE_SRC} ${TEST_RESOURCE_ANIMATION_SRC} ${TEST_SCENE_SRC} ${TEST_DREAM_SRC})
add_custom_command(TARGET DreamTest PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/test/test_resources $<TARGET_FILE_DIR:DreamTest>)
target_link_libraries(DreamTest Dream)
