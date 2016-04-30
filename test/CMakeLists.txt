# Build the Dream::Unit Library and Test executable.

# Unit
file(GLOB TEST_UNIT_SRC "Unit/*.h" "Unit/*.cpp")

# Util
file(GLOB TEST_UTIL_SRC "Util/*.h" "Util/*.cpp")

# JSON
file(GLOB TEST_JSON_SRC "JSON/*.h" "JSON/*.cpp")

# Scene
file(GLOB TEST_SCENE_SRC "Scene/*.h" "Scene/*.cpp")

# Audio
file(GLOB TEST_AUDIO_SRC "Audio/*.h" "Audio/*.cpp")

# Video
file(GLOB TEST_VIDEO_SRC "Video/*.h" "Video/*.cpp")

# Physics
file(GLOB TEST_PHYSICS_SRC "Physics/*.h" "Physics/*.cpp")

# Resource
file(GLOB TEST_RESOURCE_SRC "Resource/*.h" "Resource/*.cpp")
file(GLOB TEST_RESOURCE_MODEL_SRC "Resource/Model/*.h" "Resource/Model/*.cpp")
file(GLOB TEST_RESOURCE_ANIMATION_SRC "Resource/Animation/*.h" "Resource/Animation/*.cpp")
#file(GLOB LUA_SRC "Lua/*.h" "Lua/*.cpp")
#file(GLOB OBJ_SRC "Obj*.h" "Obj*.cpp")

# Dream
file(GLOB TEST_DREAM_SRC "*.h" "*.cpp")

# Targets & Stuff
add_executable (
    DreamTest
    ${TEST_UNIT_SRC}
    ${TEST_JSON_SRC}
    ${TEST_UTIL_SRC}
    ${TEST_AUDIO_SRC}
    ${TEST_VIDEO_SRC}
    ${TEST_PHYSICS_SRC}
    ${TEST_RESOURCE_SRC}
    ${TEST_RESOURCE_MODEL_SRC}
    ${TEST_RESOURCE_ANIMATION_SRC}
    ${TEST_SCENE_SRC}
    ${TEST_DREAM_SRC}
)

add_custom_command(
    TARGET DreamTest
    PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/test/test_resources $<TARGET_FILE_DIR:DreamTest>
)

target_link_libraries(
    DreamTest
    DreamBase
    -lBulletDynamics
    -lBulletCollision
    -lLinearMath
    -lalut
    -lopenal
    /usr/lib/libv8_base.a
    /usr/lib/libv8_external_snapshot.a
    /usr/lib/libv8_libbase.a
    /usr/lib/libv8_libplatform.a
    /usr/lib/libv8_nosnapshot.a
    /usr/lib/libv8.so
    -lpthread
    -ldl
)
