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
file(GLOB RESOURCE_SRC                 "Resource/*.h"                   "Resource/*.cpp"                   )
file(GLOB RESOURCE_ANIMATION_SRC       "Resource/Animation/*.h"         "Resource/Animation/*.cpp"         )
file(GLOB RESOURCE_AUDIO_SRC           "Resource/Audio/*.h"             "Resource/Audio/*.cpp"             )
file(GLOB RESOURCE_AUDIO_OGG_SRC       "Resource/Audio/Ogg/*.h"         "Resource/Audio/Ogg/*.cpp"         )
file(GLOB RESOURCE_AUDIO_WAV_SRC       "Resource/Audio/Wav/*.h"         "Resource/Audio/Wav/*.cpp"         )
file(GLOB RESOURCE_MODEL_SRC           "Resource/Model/*.h"             "Resource/Model/*.cpp"             )
file(GLOB RESOURCE_MODEL_WAVEFRONT_SRC "Resource/Model/Wavefront/*.h"   "Resource/Model/Wavefront/*.cpp"   )
file(GLOB RESOURCE_SCRIPT_SRC          "Resource/Script/*.h"            "Resource/Script/*.cpp"            )
file(GLOB RESOURCE_SCRIPT_JS_SRC       "Resource/Script/JavaScript/*.h" "Resource/Script/JavaScript/*.cpp" )

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
    DreamBase SHARED
    ${JSON_SRC}
    ${UTIL_SRC}
    ${UNIT_SRC}
    ${AUDIO_SRC}
    ${VIDEO_SRC}
    ${PHYSICS_SRC}
    ${SCRIPTING_SRC}

    ${RESOURCE_SRC}
    ${RESOURCE_ANIMATION_SRC}
    ${RESOURCE_AUDIO_SRC}
    ${RESOURCE_AUDIO_OGG_SRC}
    ${RESOURCE_AUDIO_WAV_SRC}
    ${RESOURCE_MODEL_SRC}
    ${RESOURCE_MODEL_WAVEFRONT_SRC}
    ${RESOURCE_SCRIPT_SRC}
    ${RESOURCE_SCRIPT_JS_SRC}

    ${SCENE_SRC}
    ${DREAM_SRC}
)

# Bullet Headers
include_directories(/usr/local/include/bullet)
include_directories(/usr/local/include/v8)
link_directories(/usr/local/lib)

add_executable (Dream Main.cpp)

target_link_libraries(
    Dream
    DreamBase
    ${BULLET_DYNAMICS}
    ${BULLET_COLLISION}
    ${LINEAR_MATH}
    ${OPENAL}
    ${V8_BASE}
    ${V8_EXT_SNAP}
    ${V8_LIBBASE}
    ${V8_LIBPLATFORM}
    ${V8_NOSNAPSHOT}
    ${V8}
    ${PTHREAD}
    ${DL}
)
