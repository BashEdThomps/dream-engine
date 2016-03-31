file(GLOB TEST_SRC "*.h" "*.cpp")

add_executable (DreamScenegraphTest ${TEST_SRC})
target_link_libraries(DreamScenegraphTest DreamUnit)
