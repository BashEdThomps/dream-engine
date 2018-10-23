/*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#pragma once

#include "../Common/DreamObject.h"

#include "../Components/IAssetDefinition.h"
#include "../Components/Path/PathDefinition.h"
#include "../Components/Audio/AudioDefinition.h"
#include "../Components/Graphics/Light/LightDefinition.h"
#include "../Components/Graphics/Model/ModelCache.h"
#include "../Components/Graphics/Model/ModelDefinition.h"
#include "../Components/Graphics/Font/FontDefinition.h"
#include "../Components/Graphics/Shader/ShaderDefinition.h"
#include "../Components/Scripting/ScriptDefinition.h"
#include "../Components/Physics/PhysicsObjectDefinition.h"
#include "../Components/Graphics/Texture/TextureDefinition.h"
#include "../Components/Graphics/Material/MaterialDefinition.h"
#include "../Components/Graphics/ParticleEmitter/ParticleEmitterDefinition.h"

#include "../Components/Time.h"
#include "../Components/Transform3D.h"

#include "../Components/Path/PathComponent.h"
#include "../Components/Audio/AudioComponent.h"
#include "../Components/Audio/Ogg/OggAudioInstance.h"
#include "../Components/Audio/Wav/WavAudioInstance.h"
#include "../Components/Graphics/GraphicsComponent.h"
#include "../Components/Graphics/Shader/ShaderInstance.h"
#include "../Components/Scripting/IScriptComponent.h"
#include "../Components/Scripting/Lua/LuaComponent.h"
#include "../Components/Physics/PhysicsComponent.h"

#include "../Components/Graphics/Camera.h"
#include "../Components/Graphics/BoundingBox.h"
#include "../Components/Graphics/Model/ModelInstance.h"

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

#include "../deps/spdlog/spdlog.h"
#include "../deps/spdlog/sinks/stdout_color_sinks.h"
