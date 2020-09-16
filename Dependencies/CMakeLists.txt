cmake_minimum_required (VERSION 3.0)

# Build Dependencies Directory
set(DEPENDENCIES_DIR "${PROJECT_SOURCE_DIR}/Dependencies")

# AngelScript
set(DEP_ANGELSCRIPT_INC_DIR "${DEPENDENCIES_DIR}" CACHE INTERNAL DEP_ANGELSCRIPT_INC_DIR)
file(GLOB_RECURSE DEP_ANGELSCRIPT_SRC_FILES_INTRNL "${DEPENDENCIES_DIR}/angelscript/*.cpp" "${DEPENDENCIES_DIR}/angelscript/*.h")
set(DEP_ANGELSCRIPT_SRC_FILES "${DEP_ANGELSCRIPT_SRC_FILES_INTRNL}" CACHE INTERNAL DEP_ANGELSCRIPT_SRC_FILES)
message("Found AngelScript:\n\tInclude:${DEP_ANGELSCRIPT_INC_DIR}\n\tSource:${DEP_ANGELSCRIPT_SRC_FILES}")
message(--------------------------------------------------------------------------------\n)

# GLAD
set(DEP_GLAD_INC_DIR    "${DEPENDENCIES_DIR}/glad/include"    CACHE INTERNAL DEP_GLAD_INC_DIR)
set(DEP_GLAD_SRC_FILES  "${DEPENDENCIES_DIR}/glad/src/glad.c" CACHE INTERNAL DEP_GLAD_SRC_FILES)
message("Found GLAD:\n\tInclude: ${DEP_GLAD_INC_DIR}\n\tSource: ${DEP_GLAD_SRC_FILES}")
message(--------------------------------------------------------------------------------\n)

# GLM
set(DEP_GLM_INC_DIR    "${DEPENDENCIES_DIR}/glm" CACHE INTERNAL DEP_GLM_INC_DIR)
message("Found GLM:\n\tInclude: ${DEP_GLM_INC_DIR}")
message(--------------------------------------------------------------------------------\n)

# ImFileSystem
set(DEP_IMFILESYSTEM_INC_DIR  "${DEPENDENCIES_DIR}/ImFileSystem")
file(GLOB DEP_IMFILESYSTEM_SRC_FILES_INTRNL	"${DEPENDENCIES_DIR}/ImFileSystem/*.cpp"	"${PROJECT_SOURCE_DIR}/Dependencies/ImFileSystem/*.h")
set(DEP_IMFILESYSTEM_SRC_FILES ${DEP_IMFILESYSTEM_SRC_FILES_INTRNL} CACHE INTERNAL DEP_IMFILESYSTEM_SRC_FILES)
message("Found ImFileSystem:\n\tInclude: ${DEP_IMFILESYSTEM_INC_DIR}\n\tSource: ${DEP_IMFILESYSTEM_SRC_FILES}")
message(--------------------------------------------------------------------------------\n)

# ImGUI
set(DEP_IMGUI_INC_DIR  "${DEPENDENCIES_DIR}/ImGui")
file(GLOB DEP_IMGUI_SRC_FILES_INTRNL	"${DEPENDENCIES_DIR}/ImGui/*.cpp"	"${PROJECT_SOURCE_DIR}/Dependencies/ImGui/*.h")
set(DEP_IMGUI_SRC_FILES "${DEP_IMGUI_SRC_FILES_INTRNL}" CACHE INTERNAL DEP_IMGUI_SRC_FILES)
message("Found ImGui:\n\tInclude: ${DEP_IMGUI_INC_DIR}\n\tSource: ${DEP_IMGUI_SRC_FILES}")
message(--------------------------------------------------------------------------------\n)

# ImGuizmo
set(DEP_IMGUIZMO_INC_DIR "${DEPENDENCIES_DIR}" CACHE INTRNLERNAL DEP_IMGUIZMO_INC_DIR)
file(GLOB DEP_IMGUIZMO_SRC_FILES_INTRNL "${DEPENDENCIES_DIR}/ImGuizmo/*.cpp")
set(DEP_IMGUIZMO_SRC_FILES ${DEP_IMGUIZMO_SRC_FILES_INTRNL} CACHE INTERNAL DEP_IMGUIZMO_SRC_FILES)
message("Found ImGuizmo:\n\tInclude: ${DEP_IMGUIZMO_INC_DIR}\n\tSource: ${DEP_IMGUIZMO_SRC_FILES}")
message(--------------------------------------------------------------------------------\n)

# ImTextEditor
set(DEP_IMTEXTEDITOR_INC_DIR  "${DEPENDENCIES_DIR}/ImTextEditor")
file(GLOB DEP_IMTEXTEDITOR_SRC_FILES_INTRNL	"${DEPENDENCIES_DIR}/ImTextEditor/*.cpp"	"${PROJECT_SOURCE_DIR}/Dependencies/ImTextEditor/*.h")
set(DEP_IMTEXTEDITOR_SRC_FILES ${DEP_IMTEXTEDITOR_SRC_FILES_INTRNL} CACHE INTERNAL DEP_IMTEXTEDITOR_SRC_FILES)
message("Found ImTextEditor:\n\tInclude: ${DEP_IMTEXTEDITOR_INC_DIR}\n\tSource: ${DEP_IMTEXTEDITOR_SRC_FILES}")

# JSON
set(DEP_JSON_INC_DIR "${DEPENDENCIES_DIR}" CACHE INTERNAL DEP_JSON_INC_DIR)
message("Found JSON:\n\tInclude: ${DEP_JSON_INC_DIR}")
message(--------------------------------------------------------------------------------\n)

# SOIL
set(DEP_SOIL_INC_DIR "${DEPENDENCIES_DIR}" CACHE INTERNAL DEP_SOIL_INC_DIR)
file(GLOB DEP_SOIL_SRC_FILES_INTRNL	"${DEPENDENCIES_DIR}/soil/*.c"	"${DEPENDENCIES_DIR}/soil/*.h")
set(DEP_SOIL_SRC_FILES ${DEP_SOIL_SRC_FILES_INTRNL} CACHE INTERNAL DEP_SOIL_SRC_FILES)
message("Found SOIL:\n\tSource:${DEP_SOIL_SRC_DIR}\n\tInclude:${DEP_SOIL_INC_FILES}")
message(--------------------------------------------------------------------------------\n)

# Tweeny
set(DEP_TWEENY_INC_DIR "${DEPENDENCIES_DIR}" CACHE INTERNAL DEP_TWEENY_INC_DIR)
message("Found Tweeny:\n\tInclude: ${DEP_TWEENY_INC_DIR}")
message(--------------------------------------------------------------------------------\n)

# TinySpline
set(DEP_TINYSPLINE_INC_DIR "${DEPENDENCIES_DIR}" CACHE INTERNAL DEP_TINYSPLINE_INC_DIR)
file(GLOB DEP_TINYSPLINE_SRC_FILES_INTRNL	"${DEPENDENCIES_DIR}/tinyspline/*.c"	"${DEPENDENCIES_DIR}/tinyspline/*.h")
set(DEP_TINYSPLINE_SRC_FILES ${DEP_TINYSPLINE_SRC_FILES_INTRNL} CACHE INTERNAL DEP_TINYSPLINE_SRC_FILES)
message("Found TinySpline:\n\tInclude: ${DEP_TINYSPLINE_INC_DIR}\n\tSource: ${DEP_TINYSPLINE_SRC_FILES}")
message(--------------------------------------------------------------------------------\n)


