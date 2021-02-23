cmake_minimum_required (VERSION 3.0)

project(
    DreamCore
    LANGUAGES CXX
    VERSION 1.0.0
    DESCRIPTION "DreamCore Library"
)


include(GNUInstallDirs)

# Targets #####################################################################

add_library(
    DreamCore
    SHARED
	# Common
	Common/Constants.cpp
	Common/Uuid.cpp
	# Math
	Math/Matrix.cpp
	Math/Quaternion.cpp
	Math/Transform.cpp
	Math/Vector.cpp
	# Base
	Base/Definition.cpp
	Base/Runtime.cpp
	Base/RuntimeTasks.cpp
	# Components
	Components/AssetRuntime.cpp
	Components/AssetDefinition.cpp
	Components/Component.cpp
	Components/DiscreteAssetRuntime.cpp
	Components/Event.cpp
	Components/SharedAssetRuntime.cpp
	Components/Time.cpp
	# Components/Animation
	Components/Animation/AnimationTasks.cpp
	Components/Animation/AnimationKeyframe.cpp
	Components/Animation/AnimationRuntime.cpp
	Components/Animation/AnimationEasing.cpp
	Components/Animation/AnimationDefinition.cpp
	# Components/Audio
	Components/Audio/AudioComponent.cpp
	Components/Audio/AudioDefinition.cpp
	Components/Audio/AudioTasks.cpp
	Components/Audio/AudioRuntime.cpp
	Components/Audio/AudioLoader.cpp
	Components/Audio/OggLoader.cpp
	Components/Audio/WavLoader.cpp
	# Components/Graphics
	Components/Graphics/Camera.cpp
	Components/Graphics/Frustum.cpp
	Components/Graphics/GraphicsComponent.cpp
	Components/Graphics/GraphicsComponentTasks.cpp
	# Components/Graphics/Font
	Components/Graphics/Font/FontDefinition.cpp
	Components/Graphics/Font/FontRuntime.cpp
	Components/Graphics/Font/FontTasks.cpp
	# Components/Graphics/Material
	Components/Graphics/Material/MaterialRuntime.cpp
	Components/Graphics/Material/MaterialDefinition.cpp
	# Components/Graphics/Model
	Components/Graphics/Model/ModelAnimation.cpp
	Components/Graphics/Model/ModelRuntime.cpp
	Components/Graphics/Model/ModelTasks.cpp
	Components/Graphics/Model/ModelDefinition.cpp
	Components/Graphics/Model/ModelMesh.cpp
	# Components/Graphics/Shader
	Components/Graphics/Shader/ShaderTasks.cpp
	Components/Graphics/Shader/ShaderDefinition.cpp
	Components/Graphics/Shader/ShaderRuntime.cpp
	Components/Graphics/Shader/ShaderUniform.cpp
	# Components/Graphics/Texture
	Components/Graphics/Texture/TextureRuntime.cpp
	Components/Graphics/Texture/TextureTasks.cpp
	Components/Graphics/Texture/TextureDefinition.cpp
	# Components/Input
	Components/Input/InputComponent.cpp
	Components/Input/InputTasks.cpp
	Components/Input/Joystick.cpp
	# Components/Path
	Components/Path/PathTasks.cpp
	Components/Path/PathRuntime.cpp
	Components/Path/PathDefinition.cpp
	# Components/Physics
	Components/Physics/PhysicsComponent.cpp
	Components/Physics/PhysicsTasks.cpp
	Components/Physics/PhysicsObjectDefinition.cpp
	Components/Physics/PhysicsObjectRuntime.cpp
	Components/Physics/PhysicsMotionState.cpp
	Components/Physics/PhysicsDebugDrawer.cpp
	# Components/Script
	Components/Script/ScriptTasks.cpp
	Components/Script/ScriptRuntime.cpp
	Components/Script/ScriptComponent.cpp
	Components/Script/ScriptDefinition.cpp
	# Components/Window
	Components/Window/WindowComponent.cpp
	# Project
	Project/Project.cpp
	Project/ProjectDefinition.cpp
	Project/ProjectRuntime.cpp
	# Scene
	Scene/SceneRuntime.cpp
	Scene/SceneDefinition.cpp
	# Entity
	Entity/EntityRuntime.cpp
	Entity/EntityDefinition.cpp
	Entity/BoundingBox.cpp
	# Storage
	Storage/File.cpp
	Storage/StorageManager.cpp
	Storage/Directory.cpp
	Storage/ProjectDirectory.cpp
	# Task
	Task/Task.cpp
)

IF (WIN32)
    target_link_libraries(
        DreamCore
    )
ELSEIF(ANDROID)
	target_link_libraries(
		${PROJECT_NAME}
		android
		EGL
		GLESv3
		assimp
		BulletCollision
		BulletDynamics
		freetype
		LinearMath
		lua
		ogg
		spdlog
		tinyspline
		vorbis
		vorbisfile
	)
ELSEIF (UNIX AND NOT APPLE) # Linux
    target_link_libraries(
         DreamCore
         -lpthread
         -ldl
    )
ELSEIF(APPLE)
    target_link_libraries(
        DreamCore
        -lpthread
        -ldl
		assimp
		BulletCollision
		BulletDynamics
		freetype
		glad
		lua
		LinearMath
		ogg
		spdlog
		tinyspline
		vorbis
		vorbisfile
        "-framework CoreFoundation"
    )
ENDIF()

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER include/DreamCore.h)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
	.
	${glm_output_dir}
	${bullet_src_dir}/src
)

link_directories(
	${glad_output_dir}
	${glm_output_dir}
	${freetype_output_dir}
	${lua_output_dir}
	${spdlog_output_dir}
	${tweeny_output_dir}
	${ogg_output_dir}
)

install(
	TARGETS ${PROJECT_NAME}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

