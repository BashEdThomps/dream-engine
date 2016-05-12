# Build the Dream Library and Executable.
set(CMAKE_VERBOSE_MAKEFILE ON)

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

# PluginInterfaces
file(GLOB PLUGIN_INTERFACES_SRC "PluginInterfaces/*.h" "PluginInterfaces/*.cpp")

# Plugins::Audio::OpenAL
file(GLOB PLUGINS_AUDIO_OPENAL_SRC "Plugins/Audio/OpenAL/*.h" "Plugins/Audio/OpenAL/*.cpp")

# Plugins::Video::OpenGL
file(GLOB PLUGINS_VIDEO_OPENGL_SRC "Plugins/Video/OpenGL/*.h" "Plugins/Video/OpenGL/*.cpp")

# Plugins::Physics::Bullet
file (GLOB PLUGINS_PHYSICS_BULLET_SRC "Plugins/Physics/Bullet/*.h" "Plugins/Physics/Bullet/*.cpp")


# Scripting
file (GLOB PLUGINS_SCRIPTING_CHAI_SRC "Plugins/Scripting/Chai/*.h" "Plugins/Scripting/Chai/*.cpp")
file (GLOB PLUGINS_SCRIPTING_V8_SRC "Plugins/Scripting/v8/*.h" "Plugins/Scripting/v8/*.cpp")

# Dream
file(GLOB DREAM_SRC "*.h" "*.cpp")
list(REMOVE_ITEM DREAM_SRC "Main.cpp")

# Find Required Libraries
find_package(OPENAL COMPONENTS OpenAL REQUIRED)
if (NOT OPENAL_FOUND)
  message(FATAL_ERROR "Package OpenAL not found")
endif()

find_package(BULLET COMPONENTS Bullet REQUIRED)
if (NOT BULLET_FOUND)
  message(FATAL_ERROR "Package Bullet not found")
endif()

find_package(V8 COMPONENTS V8 REQUIRED)
if (NOT V8_FOUND)
  message(FATAL_ERROR "Package v8 not found")
endif()

find_package(GLFW COMPONENTS GLFW REQUIRED)
if (NOT GLFW_FOUND)
  message(FATAL_ERROR "Package GLFW Not Found")
endif()

find_package(CHAISCRIPT COMPONENTS Chaiscript REQUIRED)
if (NOT CHAISCRIPT_FOUND)
  message(FATAL_ERROR "Package ChaiScript was not found")
endif()

# Targets
add_library (
    DreamBase SHARED
    ${JSON_SRC}
    ${UTIL_SRC}
    ${UNIT_SRC}
    ${PLUGINS_AUDIO_OPENAL_SRC}
    ${PLUGINS_VIDEO_OPENGL_SRC}
    ${PLUGINS_PHYSICS_BULLET_SRC}
    ${PLUGINS_SCRIPTING_CHAI_SRC}
    ${PLUGINS_SCRIPTING_V8_SRC}
    ${PLUGIN_INTERFACES_SRC}
    ${RESOURCE_SRC}
    ${RESOURCE_ANIMATION_SRC}
    ${RESOURCE_AUDIO_SRC}
    ${RESOURCE_AUDIO_OGG_SRC}
    ${RESOURCE_AUDIO_WAV_SRC}
    ${RESOURCE_MODEL_SRC}
    ${RESOURCE_MODEL_WAVEFRONT_SRC}
    ${RESOURCE_SCRIPT_SRC}
    ${RESOURCE_SCRIPT_JS_SRC}
    ${RESOURCE_SCRIPT_CHAI_SRC}
    ${SCENE_SRC}
    ${DREAM_SRC}
)

target_link_libraries(
  DreamBase
  -lpthread
  -ldl
  ${OPENAL_LIBRARY}
  ${GLFW_LIBRARIES}
  ${V8_LIBRARY}
  ${BULLET_LIBRARY}
)

include_directories(
  ${CHAISCRIPT_INCLUDE_DIR}
  ${OPENAL_INCLUDE_DIR}
  ${GLFW_INCLUDE_DIR}
  ${V8_INCLUDE_DIR}
  ${BULLET_INCLUDE_DIR}
)

add_executable (Dream Main.cpp)

target_link_libraries(
  Dream
  DreamBase
  -lpthread
  -ldl
  ${OPENAL_LIBRARY}
  ${GLFW_LIBRARIES}
  ${V8_LIBRARY}
  ${BULLET_LIBRARY}
)
