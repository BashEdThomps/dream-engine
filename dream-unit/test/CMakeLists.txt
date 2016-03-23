# Build the DreamUnit Library and Test executable.
file(GLOB TEST_SRC "*.h" "*.cpp")

add_executable (Test ${TEST_SRC})
target_link_libraries(Test DreamUnit)
