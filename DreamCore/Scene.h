/*
* Dream::Scene
*
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

#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <vector>
#include <map>
#include <json.hpp>

#include "String.h"
#include "SceneObject.h"
#include "Constants.h"
#include "IAssetInstance.h"
#include "AssetDefinition.h"

#include "Lua/LuaScriptInstance.h"

#include "Components/Graphics/Camera.h"
#include "Components/Audio/AudioInstance.h"
#include "Components/Animation/AnimationInstance.h"
#include "Components/Graphics/SpriteInstance.h"
#include "Components/Graphics/ShaderInstance.h"
#include "Components/Graphics/AssimpModelInstance.h"
#include "Components/Graphics/LightInstance.h"
#include "Components/Graphics/FontInstance.h"
#include "Components/Physics/PhysicsObjectInstance.h"

using namespace std;
namespace Dream
{
    class AudioComponent;
    class LuaEngine;

    class Scene
    {
    private:
        nlohmann::json mJson;
        string mUuid;
        string mName;
        string mNotes;
        bool mActive;
        SceneObject mRootSceneObject;
        vector<SceneObject> mDeleteQueue;
        Transform3D mDefaultCameraTransform;
        float mCameraMovementSpeed;
        vector<float> mClearColour;
        vector<float> mAmbientLightColour;
        string mProjectPath;
        vector<AssetDefinition> mAssetDefinitions;
        vector<float> mGravity;
        bool mPhysicsDebug;
        shared_ptr<AudioComponent> mAudioComponent;
        shared_ptr<LuaEngine> mLuaEngine;

    public:
        Scene();
        Scene(
            nlohmann::json, string,
            vector<AssetDefinition>,
            shared_ptr<AudioComponent>,
            shared_ptr<LuaEngine>
        );
        ~Scene();

        string getUuid();
        void setUuid(string);

        string getName();
        void setName(string);

        string getNameAndUuidString();

        int countChildrenOfSceneObject(SceneObject&);

        bool isActive();
        void setActive(bool);

        void setRootSceneObject(SceneObject&);
        SceneObject getRootSceneObject();
        size_t getNumberOfSceneObjects();

        SceneObject getSceneObjectByName(string);
        SceneObject getSceneObjectByUuid(string);

        void showStatus();
        void showScenegraph();

        string indent(int);

        glm::vec3 getDefaultCameraTranslation();
        glm::vec3 getDefaultCameraRotation();

        void setCameraMovementSpeed(float);
        float getCameraMovementSpeed();

        vector<float> getClearColour();
        vector<float> getAmbientLightColour();

        void addToDeleteQueue(SceneObject);
        vector<SceneObject> getDeleteQueue();
        void clearDeleteQueue();
        void findDeletedSceneObjects();

        bool createAllAssetInstances();

        shared_ptr<IAssetInstance>        createAssetInstanceFromDefinitionUuid(SceneObject&, string);
        shared_ptr<IAssetInstance>        createAssetInstance(SceneObject&, AssetDefinition&);
        shared_ptr<AnimationInstance>     createAnimationInstance(SceneObject&, AssetDefinition&);
        shared_ptr<AudioInstance>         createAudioInstance(SceneObject&, AssetDefinition&);
        shared_ptr<AssimpModelInstance>   createModelInstance(SceneObject&, AssetDefinition&);
        shared_ptr<LuaScriptInstance>     createScriptInstance(SceneObject&, AssetDefinition&);
        shared_ptr<ShaderInstance>        createShaderInstance(SceneObject&, AssetDefinition&);
        shared_ptr<PhysicsObjectInstance> createPhysicsObjectInstance(SceneObject&, AssetDefinition&);
        shared_ptr<LightInstance>         createLightInstance(SceneObject&, AssetDefinition&);
        shared_ptr<FontInstance>          createFontInstance(SceneObject&, AssetDefinition&);
        shared_ptr<SpriteInstance>        createSpriteInstance(SceneObject&, AssetDefinition&);

        void setProjectPath(string);

        bool createAssetInstancesForSceneObject(SceneObject&);
        void findDeletedScripts();
        void removeFromLuaScriptMap(SceneObject&);

        string getNotes();
        void setNotes(string notes);

        AssetDefinition getAssetDefinitionByUuid(string);

        vector<float> getGravity();
        bool getPhysicsDebug();

        void loadPhysicsMetadata(nlohmann::json);
        void loadGraphicsMetadata(nlohmann::json);
        void loadSceneObjectMetadata(nlohmann::json, SceneObject*);
        void loadDefaultCameraTransform(nlohmann::json);
        void loadClearColour(nlohmann::json);
        void loadAmbientLightColour(nlohmann::json);
        nlohmann::json toJson();
        void cleanUpAssetInstances();
        void setDeleteFlagOnAllSceneObjects(bool bDelete);
        void setLoadedFlagOnAllSceneObjects(bool load);

    }; // End of Scene

} // End of Dream

#endif // End of SCENE_H
