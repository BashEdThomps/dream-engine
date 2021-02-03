cmake_minimum_required (VERSION 3.0)

project (glad C)  

add_library (
    glad
    SHARED
    src/glad.c
)

target_include_directories(glad PUBLIC "include")
set_target_properties(glad PROPERTIES PUBLIC_HEADER include)
set_target_properties(glad PROPERTIES OUTPUT_NAME "glad") # rename the library output name
