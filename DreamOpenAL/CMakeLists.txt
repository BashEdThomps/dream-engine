cmake_minimum_required (VERSION 3.0)

project(
    DreamOpenAL
    LANGUAGES CXX
    VERSION 1.0.0
    DESCRIPTION "Dream OpenAL Audio Component"
)


include(GNUInstallDirs)
include_directories(${DreamCore_SOURCE_DIR})
include_directories(${DreamCore_SOURCE_DIR}/include)

# Targets #####################################################################

add_library(
    DreamOpenAL
    SHARED
	OpenALAudioComponent.cpp
	OpenALAudioRuntime.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
#set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER .)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
	.
)

if (APPLE)
	target_link_libraries(
		${PROJECT_NAME}
		DreamCore
		"-framework OpenAL"
	)
else()
	target_link_libraries(
		${PROJECT_NAME}
		DreamCore
	)
endif()

#install(
#	TARGETS ${PROJECT_NAME}
#	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#   PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
#)

