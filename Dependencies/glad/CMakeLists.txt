cmake_minimum_required (VERSION 2.6)
project (glad C)  # project here actually means solution in premake

add_library (
    glad
    SHARED
    src/glad.c
)

target_include_directories(glad PUBLIC "include")
set_target_properties(glad PROPERTIES PUBLIC_HEADER include)
set_target_properties(glad PROPERTIES OUTPUT_NAME "glad") # rename the library output name
