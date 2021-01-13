cmake_minimum_required (VERSION 3.9)

project (ImGui C)  # project here actually means solution in premake

add_library (
	${PROJECT_NAME}
	SHARED
	imgui_widgets.cpp
	imgui.cpp
	examples/imgui_impl_opengl3.cpp
	examples/imgui_impl_glfw.cpp
	imgui_draw.cpp
)

target_include_directories(
	${PROJECT_NAME}
	PRIVATE
	.
)

target_link_libraries(
	${PROJECT_NAME}
	glfw
	glad
)
