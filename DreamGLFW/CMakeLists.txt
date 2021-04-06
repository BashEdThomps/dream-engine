cmake_minimum_required (VERSION 3.0)
project(DreamGLFW)

include_directories(${DreamCore_SOURCE_DIR}/include)
include_directories(${DreamOpenAL_SOURCE_DIR})

# Targets #####################################################################

add_executable (
  ${PROJECT_NAME}
  DefaultPrintListener.cpp
  GLFWWindowComponent.cpp
  Main.cpp
  )

if (WIN32)
  target_link_libraries(
    ${PROJECT_NAME}
    DreamCore
    DreamOpenAL
    )
elseif(UNIX AND NOT APPLE) # Linux
  target_link_libraries(
    ${PROJECT_NAME}
    DreamCore
    DreamOpenAL
    -lpthread
    -ldl
    )
elseif(APPLE)
  target_link_libraries(
    ${PROJECT_NAME}
    DreamCore
    DreamOpenAL
    glfw
    -lpthread
    -ldl
    "-framework IOKit"
    "-framework AppKit"
    "-framework OpenGL"
    "-framework CoreFoundation"
    "-framework Carbon"
    )
endif()
