file(GLOB TEST_SRC "*.h" "*.cpp")
add_executable (DreamFileFormatsTest ${TEST_SRC})
target_link_libraries(DreamFileFormatsTest DreamUnit DreamFileFormats)
