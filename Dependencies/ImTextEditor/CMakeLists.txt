cmake_minimum_required (VERSION 3.9)

project (ImTextEditor CXX)

add_library (
	${PROJECT_NAME}
	SHARED
	TextEditor.cpp
)

target_link_libraries(
	${PROJECT_NAME}
	ImGui
	)
