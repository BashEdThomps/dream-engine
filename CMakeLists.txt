cmake_minimum_required (VERSION 2.8.0)
project (dream)

# Dre;aa
add_subdirectory (src)
add_subdirectory (test)

# Unit Testing
enable_testing()
add_test(NAME DreamTest COMMAND DreamTest)
