cmake_minimum_required (VERSION 3.0)
project(DreamTool)

# Targets ######################################################################

include_directories(${DreamCore_SOURCE_DIR}/include)
include_directories(${DreamOpenAL_SOURCE_DIR})

add_executable (
    ${PROJECT_NAME}
	Main.cpp
	DreamToolContext.cpp
	DreamToolWindow.cpp

	Model/Model.cpp
	Model/PreferencesModel.cpp
	Model/TemplatesModel.cpp
	Model/ModelDefinitionBatchImporter.cpp

	Widgets/DreamToolWidget.cpp
	Widgets/GL/GLWidget.cpp
	Widgets/GL/LightViewer.cpp
	Widgets/GL/SelectionHighlighter.cpp
	Widgets/GL/Grid.cpp
	Widgets/GL/AnimationViewer.cpp
	Widgets/GL/PathViewer.cpp
	Widgets/GL/Cursor.cpp

	Widgets/ImGui/SceneStateWindow.cpp
	Widgets/ImGui/ProjectBrowser.cpp
	Widgets/ImGui/ScriptDebugWindow.cpp
	Widgets/ImGui/MenuBar.cpp
	Widgets/ImGui/ImGuiWidget.cpp
	Widgets/ImGui/GamepadStateWindow.cpp
	Widgets/ImGui/GLPreviewWindowComponent.cpp
	Widgets/ImGui/PropertiesWindow.cpp
	Widgets/ImGui/ScriptEditorWindow.cpp
	Widgets/ImGui/ToolPropertiesWindow.cpp
	Widgets/ImGui/ShaderEditorWindow.cpp
	Widgets/ImGui/RenderingDebugWindow.cpp
	Widgets/ImGui/CacheContentWindow.cpp
	Widgets/ImGui/TaskManagerWindow.cpp
)

target_include_directories(
	${PROJECT_NAME}
	PRIVATE .)

if (WIN32)
    target_link_libraries(
        ${PROJECT_NAME}
        DreamCore
		DreamOpenAL
    )
else()
    target_link_libraries(
        ${PROJECT_NAME}
        DreamCore
		DreamOpenAL
		ImGui
		ImGuizmo
		ImTextEditor
		nfd
        -lpthread
        -ldl
    )
endif()

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/Resources"
    "${CMAKE_CURRENT_BINARY_DIR}"
)
