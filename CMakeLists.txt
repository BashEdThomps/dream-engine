cmake_minimum_required (VERSION 3.2.2)
project (dream)
# DreamUnit
add_subdirectory (dream-unit/src)
add_subdirectory (dream-unit/test)

#add_subdirectory (dream-collision-world)
#add_subdirectory (dream-animation)
#add_subdirectory (dream-scenegraph)
#add_subdirectory (dream)

enable_testing()
add_test(NAME DreamUnitTest COMMAND dream-unit/test/Test)
