#pragma once


// Common
#include "Common/Constants.h"
#include "Common/Logger.h"
#include "Common/GLHeader.h"

// Animation
#include "Components/Animation/AnimationDefinition.h"
#include "Components/Animation/AnimationKeyframe.h"
#include "Components/Animation/AnimationRuntime.h"

// Audio
#include "Components/Audio/AudioRuntime.h"
#include "Components/Audio/AudioDefinition.h"
#include "Components/Audio/AudioComponent.h"
#include "Components/Audio/AudioLoader.h"
#include "Components/Audio/OggLoader.h"
#include "Components/Audio/WavLoader.h"

// Graphics
#include "Components/Graphics/GraphicsComponent.h"

#include "Components/Graphics/Shader/ShaderDefinition.h"
#include "Components/Graphics/Shader/ShaderRuntime.h"

#include "Components/Graphics/Model/ModelDefinition.h"
#include "Components/Graphics/Model/ModelRuntime.h"
#include "Components/Graphics/Model/ModelMesh.h"

#include "Components/Graphics/Material/MaterialDefinition.h"
#include "Components/Graphics/Material/MaterialRuntime.h"

#include "Components/Graphics/Texture/TextureRuntime.h"
#include "Components/Graphics/Texture/TextureDefinition.h"

#include "Components/Graphics/Font/FontDefinition.h"
#include "Components/Graphics/Font/FontRuntime.h"

// Storage
#include "Storage/ProjectDirectory.h"
#include "Storage/Directory.h"
#include "Storage/File.h"
#include "Storage/StorageManager.h"


// Input
#include "Components/Input/InputComponent.h"

// Physics
#include "Components/Physics/PhysicsComponent.h"
#include "Components/Physics/PhysicsObjectDefinition.h"

// Path
#include "Components/Path/PathDefinition.h"
#include "Components/Path/PathRuntime.h"

// Script
#include "Components/Script/ScriptDefinition.h"
#include "Components/Script/ScriptComponent.h"

// Window
#include "Components/Window/WindowComponent.h"

// Scene
#include "Scene/SceneDefinition.h"
#include "Scene/SceneRuntime.h"
#include "Entity/EntityDefinition.h"
#include "Entity/EntityRuntime.h"

// Project
#include "Project/ProjectDefinition.h"
#include "Project/ProjectRuntime.h"
#include "Project/Project.h"

// Task Manager
#include "Task/Task.h"

// Cache post-include
#include "Components/Cache.h"
