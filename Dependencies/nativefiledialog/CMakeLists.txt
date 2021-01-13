cmake_minimum_required (VERSION 3.9)

project (NativeFileDialog C)  # project here actually means solution in premake

add_library (
	${PROJECT_NAME}
	SHARED
	src/nfd_zenity.c
	src/nfd_gtk.c
	src/nfd_common.c
	src/nfd_cocoa.m
)
