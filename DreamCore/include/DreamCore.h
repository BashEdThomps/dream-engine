#pragma once

#include "Common/Constants.h"
#include "Common/Directory.h"
#include "Common/Logger.h"
#include "Common/GLHeader.h"

// Animation
#include "Components/Animation/AnimationDefinition.h"
#include "Components/Animation/AnimationKeyframe.h"
#include "Components/Animation/AnimationRuntime.h"

// Audio
#include "Components/Audio/AudioCache.h"
#include "Components/Audio/AudioRuntime.h"
#include "Components/Audio/AudioDefinition.h"
#include "Components/Audio/AudioComponent.h"

// Graphics
#include "Components/Graphics/GraphicsComponent.h"
#include "Components/Graphics/Font/FontDefinition.h"
#include "Components/Graphics/Shader/ShaderDefinition.h"
#include "Components/Graphics/Shader/ShaderRuntime.h"
#include "Components/Graphics/Shader/ShaderCache.h"
#include "Components/Graphics/Model/ModelDefinition.h"
#include "Components/Graphics/Model/ModelRuntime.h"
#include "Components/Graphics/Model/ModelCache.h"
#include "Components/Graphics/Model/ModelMesh.h"
#include "Components/Graphics/Light/LightDefinition.h"
#include "Components/Graphics/Light/LightRuntime.h"
#include "Components/Graphics/Material/MaterialCache.h"
#include "Components/Graphics/Material/MaterialDefinition.h"
#include "Components/Graphics/Material/MaterialRuntime.h"
#include "Components/Graphics/Texture/TextureRuntime.h"
#include "Components/Graphics/Texture/TextureDefinition.h"
#include "Components/Graphics/Texture/TextureCache.h"
#include "Components/Graphics/ParticleEmitter/ParticleEmitterDefinition.h"
#include "Components/Graphics/ParticleEmitter/ParticleEmitterRuntime.h"

// Input
#include "Components/Input/InputComponent.h"

// Physics
#include "Components/Physics/PhysicsComponent.h"
#include "Components/Physics/PhysicsObjectDefinition.h"

// Path
#include "Components/Path/PathDefinition.h"
#include "Components/Path/PathRuntime.h"

// Scroller
#include "Components/Scroller/ScrollerDefinition.h"

// Script
#include "Components/Script/ScriptDefinition.h"
#include "Components/Script/ScriptComponent.h"

// Object Emitter
#include "Components/ObjectEmitter/ObjectEmitterDefinition.h"
#include "Components/ObjectEmitter/ObjectEmitterRuntime.h"

// Window
#include "Components/Window/WindowComponent.h"

// Scene
#include "Scene/SceneDefinition.h"
#include "Scene/SceneRuntime.h"
#include "Scene/Entity/EntityDefinition.h"
#include "Scene/Entity/EntityRuntime.h"

// Project
#include "Project/ProjectDirectory.h"
#include "Project/ProjectDefinition.h"
#include "Project/ProjectRuntime.h"
#include "Project/Project.h"

// Task Manager
#include "TaskManager/Task.h"
#include "TaskManager/TaskManager.h"
#include "TaskManager/TaskThread.h"
