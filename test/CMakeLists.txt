# Build the Dream Test executable.

# Dream
file(GLOB_RECURSE TEST_DREAM_SRC "*.h" "*.cpp")
source_group(DremTest FILES ${TEST_DREAM_SRC})

# Find Required Packages
find_package(OPENAL REQUIRED)
find_package(BULLET REQUIRED)
find_package(GLFW   REQUIRED)
find_package(OPENGL REQUIRED)
find_package(GLM    REQUIRED)
find_package(GLEW   REQUIRED)
find_package(assimp REQUIRED)
find_package(SOIL   REQUIRED)
find_package(LUA    REQUIRED)


# Targets & Stuff
add_executable (DreamTest ${TEST_DREAM_SRC})

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
  ${OPENGL_LIBRARIES}
  ${assimp_LIBRARIES}
  ${SOIL_LIBRARIES}
  ${GLFW_LIBRARIES}
  ${GLEW_LIBRARIES}
  ${BULLET_LIBRARY}
  ${LUA_LIBRARIES}
)

include_directories(
  ${OPENAL_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${GLFW_INCLUDE_DIR}
  ${GLEW_INCLUDE_DIRS}
  ${assimp_INCLUDE_DIRS}
  ${SOIL_INCLUDE_DIR}
  ${GLM_INCLUDE_DIR}
  ${BULLET_INCLUDE_DIR}
  ${LUA_INCLUDE_DIR})
