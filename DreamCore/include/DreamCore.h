#pragma once

#include "../Common/Constants.h"
#include "../Common/DreamObject.h"

#include "../Components/IAssetDefinition.h"
#include "../Components/Path/PathDefinition.h"
#include "../Components/Audio/AudioDefinition.h"
#include "../Components/Graphics/Font/FontDefinition.h"
#include "../Components/Graphics/Light/LightDefinition.h"
#include "../Components/Graphics/Model/AssimpCache.h"
#include "../Components/Graphics/Model/ModelDefinition.h"
#include "../Components/Graphics/Shader/ShaderDefinition.h"
#include "../Components/Graphics/Sprite/SpriteDefinition.h"
#include "../Components/Physics/PhysicsObjectDefinition.h"
#include "../Components/Lua/ScriptDefinition.h"

#include "../Components/Time.h"
#include "../Components/Transform3D.h"

#include "../Components/Path/PathComponent.h"
#include "../Components/Audio/AudioComponent.h"
#include "../Components/Graphics/GraphicsComponent.h"
#include "../Components/Lua/LuaComponent.h"
#include "../Components/Physics/PhysicsComponent.h"

#include "../Components/Graphics/Camera.h"
#include "../Components/Graphics/BoundingBox.h"
#include "../Components/Graphics/Model/AssimpModelInstance.h"

#include "../Components/Window/IWindowComponent.h"

#include "../Scene/SceneDefinition.h"
#include "../Scene/SceneRuntime.h"
#include "../Scene/SceneState.h"

#include "../Scene/SceneObject/SceneObjectDefinition.h"
#include "../Scene/SceneObject/SceneObjectRuntime.h"

#include "../Project/Project.h"
#include "../Project/ProjectDefinition.h"
#include "../Project/ProjectRuntime.h"

#include "../Utilities/ArgumentParser.h"
#include "../Utilities/Uuid.h"
