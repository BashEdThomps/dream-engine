# Build the DreamUnit Library and Test executable.

add_library (DreamUnit src/unit.c)
add_executable (DreamUnitTest src/unit.c src/test/test_main.c)

target_include_directories (DreamUnit PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
