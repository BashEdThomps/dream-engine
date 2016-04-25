# Build the Dream Library and Executable.

# Unit
file(GLOB UNIT_SRC "Unit/*.h" "Unit/*.cpp")

# Util
file(GLOB UTIL_SRC "Util/*.h" "Util/*.cpp")

# JSON
file(GLOB JSON_SRC "JSON/json.hpp")

# Scene
file(GLOB SCENE_SRC "Scene/*.h" "Scene/*.cpp")

# Resource
file(GLOB RESOURCE_SRC "Resource/*.h" "Resource/*.cpp")

file(GLOB RESOURCE_ANIMATION_SRC "Resource/Animation/*.h" "Resource/Animation/*.cpp")


file(GLOB RESOURCE_MODEL_SRC "Resource/Model/*.h" "Resource/Model/*.cpp")
file(GLOB RESOURCE_MODEL_WAVEFRONT_SRC "Resource/Model/Wavefront/*.h" "Resource/Model/Wavefront/*.cpp")

file(GLOB RESOURCE_SCRIPT_SRC "Resource/Script/*.h" "Resource/Script/*.cpp")
file(GLOB RESOURCE_SCRIPT_JS_SRC "Resource/Script/JavaScript/*.h" "Resource/Script/JavaScript/*.cpp")
#file(GLOB OBJ_SRC "Obj*.h" "Obj*.cpp")

# Audio
file(GLOB AUDIO_SRC "Audio/*.h" "Audio/*.cpp")

# Video
file(GLOB VIDEO_SRC "Video/*.h" "Video/*.cpp")

# Physics
file (GLOB PHYSICS_SRC "Physics/*.h" "Physics/*.cpp")


# Scripting
file (GLOB SCRIPTING_SRC "Scripting/*.h" "Scripting/*.cpp")

# Dream
file(GLOB DREAM_SRC "*.h" "*.cpp")
list(REMOVE_ITEM DREAM_SRC "Main.cpp")
# Targets
add_library (
    Dream SHARED
    ${JSON_SRC}
    ${UTIL_SRC}
    ${UNIT_SRC}
    ${AUDIO_SRC}
    ${VIDEO_SRC}
    ${PHYSICS_SRC}
    ${SCRIPTING_SRC}

    ${RESOURCE_SRC}
    ${RESOURCE_ANIMATION_SRC}
    ${RESOURCE_MODEL_SRC}
    ${RESOURCE_MODEL_WAVEFRONT_SRC}
    ${RESOURCE_SCRIPT_SRC}
    ${RESOURCE_SCRIPT_JS_SRC}

    ${SCENE_SRC}
    ${DREAM_SRC}
)

# Bullet Headers
include_directories(/usr/include/bullet)

add_executable (DreamMain Main.cpp)

target_link_libraries(
    DreamMain
    Dream
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
