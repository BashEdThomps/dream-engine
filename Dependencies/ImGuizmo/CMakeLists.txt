cmake_minimum_required (VERSION 3.9)

project (ImGuizmo CXX)

add_library (
	${PROJECT_NAME}
	SHARED
	ImGuizmo.cpp
)

target_link_libraries(
	${PROJECT_NAME}
	ImGui
	)
