# Build the Dream Test executable.

# Dream
file(GLOB_RECURSE TEST_DREAM_SRC "*.h" "*.cpp")

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

# Targets & Stuff
add_executable (
    DreamTest
    ${TEST_DREAM_SRC}
)

add_custom_command(
    TARGET DreamTest
    PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/test/test_assets $<TARGET_FILE_DIR:DreamTest>
)


target_link_libraries(
  DreamTest
  DreamBase
  -lpthread
  -ldl
  ${OPENAL_LIBRARY}
  ${GLFW_LIBRARIES}
  ${V8_LIBRARY}
  ${BULLET_LIBRARY}
)

include_directories(
  ${OPENAL_INCLUDE_DIR}
  ${GLFW_INCLUDE_DIR}
  ${V8_INCLUDE_DIR}
  ${BULLET_INCLUDE_DIR}
)
