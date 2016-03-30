cmake_minimum_required (VERSION 2.8.0)
project (dream)

# DreamUnit
add_subdirectory (dream-unit/src)
add_subdirectory (dream-unit/test)

#Dream FileFormats
add_subdirectory(dream-fileformats/src)
add_subdirectory(dream-fileformats/test)

# Dream Animation
add_subdirectory (dream-animation/src)
add_subdirectory (dream-animation/test)

# Dream Scenegraph
#add_subdirectory (dream-scenegraph/src)
#add_subdirectory (dream-scenegraph/test)
#add_subdirectory (dream-scenegraph/gl_test)

# Dream
add_subdirectory (dream/src)
add_subdirectory (dream/test)

# Unit Testing
enable_testing()
add_test(NAME DreamUnitTest COMMAND dream-unit/test/DreamUnitTest)
add_test(NAME DreamFileFormatsTest COMMAND dream-fileformats/test/DreamFileFormatsTest)
add_test(NAME DreamAnimationTest COMMAND dream-animation/test/DreamAnimationTest)
#add_test(NAME DreamScenegraphTest COMMAND dream-scenegraph/test/Test)
add_test(NAME DreamTest COMMAND dream/test/DreamTest)

# Another Day Maybe
#add_test(NAME DreamScenegraphGLTest COMMAND dream-scenegraph/test/GLTest)
#add_test(NAME DreamGLTest COMMAND dream/test/GLTest)
