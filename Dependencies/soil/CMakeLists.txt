cmake_minimum_required (VERSION 3.9)

project (soil C)

add_library (
	${PROJECT_NAME}
	SHARED
	SOIL.c
	image_helper.c
)

if (APPLE)
	target_link_libraries(
		${PROJECT_NAME}
		"-framework OpenGL"
		"-framework CoreFoundation"
		)
endif()
