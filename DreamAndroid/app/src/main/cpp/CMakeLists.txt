cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -fno-rtti")

if (${ANDROID_PLATFORM_LEVEL} LESS 12)
  message(FATAL_ERROR "OpenGL 2 is not supported before API level 11 (currently using ${ANDROID_PLATFORM_LEVEL}).")
  return()
elseif (${ANDROID_PLATFORM_LEVEL} LESS 18)
  set(OPENGL_LIB GLESv2)
else ()
  set(OPENGL_LIB GLESv3)
endif (${ANDROID_PLATFORM_LEVEL} LESS 12)

set(dependencies_dir /Users/Ashley/Desktop/dream/Dependencies)

# Angelscript
set (angelscript_src_dir ${dependencies_dir}/angelscript-2.35.0)
set (angelscript_output_dir ${CMAKE_BINARY_DIR}/build_angelscript)
file(MAKE_DIRECTORY ${angelscript_output_dir})
add_subdirectory(${angelscript_src_dir}/angelscript/projects/cmake ${angelscript_output_dir} EXCLUDE_FROM_ALL)
include_directories(${angelscript_src_dir}/angelscript/include)
include_directories(${angelscript_src_dir}/add_on)

# Assimp
set (assimp_src_dir ${dependencies_dir}/assimp-5.0.1/)
set (assimp_output_dir ${CMAKE_BINARY_DIR}/build_assimp)
file(MAKE_DIRECTORY ${assimp_output_dir})
add_subdirectory(${assimp_src_dir} ${assimp_output_dir} EXCLUDE_FROM_ALL)
include_directories(${assimp_src_dir}/include)

# Bullet
set (bullet_src_dir ${dependencies_dir}/bullet3-3.08/)
set (bullet_output_dir ${CMAKE_BINARY_DIR}/build_bullet)
file(MAKE_DIRECTORY ${bullet_output_dir})
add_subdirectory(${bullet_src_dir} ${bullet_output_dir} EXCLUDE_FROM_ALL)
include_directories(${bullet_src_dir}/src)

# Freetype
set (freetype_src_dir ${dependencies_dir}/freetype2-VER-2-10-2/)
set (freetype_output_dir ${CMAKE_BINARY_DIR}/build_freetype)
file(MAKE_DIRECTORY ${freetype_output_dir})
add_subdirectory(${freetype_src_dir} ${freetype_output_dir} EXCLUDE_FROM_ALL)
include_directories(${freetype_src_dir}/include)

# GLM
set (glm_src_dir ${dependencies_dir}/glm-0.9.9.8/)
set (glm_output_dir ${CMAKE_BINARY_DIR}/build_glm)
file(MAKE_DIRECTORY ${glm_output_dir})
add_subdirectory(${glm_src_dir} ${glm_output_dir} EXCLUDE_FROM_ALL)
include_directories(${glm_src_dir})

# JSON
set(json_src_dir ${dependencies_dir}/json/)
include_directories(${json_src_dir})

# Lua
set (lua_src_dir ${dependencies_dir}/lua-5.3.6/)
set (lua_output_dir ${CMAKE_BINARY_DIR}/build_lua)
file(MAKE_DIRECTORY ${lua_output_dir})
add_subdirectory(${lua_src_dir} ${lua_output_dir} EXCLUDE_FROM_ALL)
include_directories(${lua_src_dir}/src)

# ogg
set (ogg_src_dir ${dependencies_dir}/ogg-1.3.4/)
set (ogg_output_dir ${CMAKE_BINARY_DIR}/build_ogg)
file(MAKE_DIRECTORY ${ogg_output_dir})
add_subdirectory(${ogg_src_dir} ${ogg_output_dir} EXCLUDE_FROM_ALL)
include_directories(${ogg_src_dir}/include)

# STB
set (stb_src_dir ${dependencies_dir}/stb/)
include_directories(${stb_src_dir})

# sol
set(sol_src_dir ${dependencies_dir}/sol/)
include_directories(${sol_src_dir})

# Spdlog
set (spdlog_src_dir ${dependencies_dir}/spdlog-1.8.2/)
set (spdlog_output_dir ${CMAKE_BINARY_DIR}/build_spdlog)
file(MAKE_DIRECTORY ${spdlog_output_dir})
add_subdirectory(${spdlog_src_dir} ${spdlog_output_dir} EXCLUDE_FROM_ALL)
include_directories(${spdlog_src_dir}/include)

# Tweeny
set (tweeny_src_dir ${dependencies_dir}/tweeny-3.1.1/)
set (tweeny_output_dir ${CMAKE_BINARY_DIR}/build_tweeny)
file(MAKE_DIRECTORY ${tweeny_output_dir})
add_subdirectory(${tweeny_src_dir} ${tweeny_output_dir} EXCLUDE_FROM_ALL)
include_directories(${tweeny_src_dir}/include)

# TinySpline
set (tinyspline_src_dir ${dependencies_dir}/tinyspline/)
set (tinyspline_output_dir ${CMAKE_BINARY_DIR}/build_tinyspline)
file(MAKE_DIRECTORY ${tinyspline_output_dir})
add_subdirectory(${tinyspline_src_dir} ${tinyspline_output_dir} EXCLUDE_FROM_ALL)
include_directories(${tinyspline_src_dir})

# Vorbis
set (vorbis_src_dir ${dependencies_dir}/vorbis-1.3.7/)
set (vorbis_output_dir ${CMAKE_BINARY_DIR}/build_vorbis)
file(MAKE_DIRECTORY ${vorbis_output_dir})
add_subdirectory(${vorbis_src_dir} ${vorbis_output_dir} EXCLUDE_FROM_ALL)
include_directories(${vorbis_src_dir}/include)

# DreamCore
set (DreamCore_src_dir /Users/Ashley/Desktop/dream/DreamCore)
set (DreamCore_output_dir ${CMAKE_BINARY_DIR}/build_DreamCore)
file(MAKE_DIRECTORY ${DreamCore_output_dir})
add_subdirectory(${DreamCore_src_dir} ${DreamCore_output_dir})
include_directories(${DreamCore_src_dir}/include)

add_library(
      dream_android
      SHARED
      DreamJNI.cpp
)

target_link_libraries(
      dream_android
      ${OPENGL_LIB}
      android
      EGL
      log
      m
)
