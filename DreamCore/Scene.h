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

#include "Lua/ILuaExposable.h"
#include "Lua/LuaScriptInstance.h"

#include "Components/Graphics/Camera.h"
#include "Components/Audio/IAudioInstance.h"
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
    class IAudioComponent;

    class Scene : public ILuaExposable
    {
    public:
        string mUuid;
        string mName;
        SceneObject *mRootSceneObject;
        vector<SceneObject*> mScenegraphVector;
        vector<SceneObject*> mDeleteQueue;
        Transform3D* mDefaultCameraTransform;
        float mCameraMovementSpeed;
        vector<float> mClearColour;
        vector<float> mAmbientLightColour;
        map<SceneObject*,LuaScriptInstance*> mLuaScriptMap;
        string mProjectPath;
        vector<AssetDefinition*>* mAssetDefinitions;
        vector<float> mGravity;
        bool mPhysicsDebug;
        IAudioComponent* mAudioComponent;
    public:
        Scene(nlohmann::json, string, vector<AssetDefinition*>*, IAudioComponent* audioComponent);
        ~Scene();
        string getUuid();
        void setUuid(string);
        string getNameAndUuidString();
        string getName();
        void setName(string);
        void loadSceneObjects(nlohmann::json,SceneObject*);
        void loadDefaultCameraTransform(nlohmann::json);
        int countChildrenOfSceneObject(SceneObject*);
        void setRootSceneObject(SceneObject*);
        SceneObject* getRootSceneObject();
        size_t getNumberOfSceneObjects();
        bool hasSceneObect(SceneObject*);
        SceneObject* getSceneObjectByName(string);
        SceneObject* getSceneObjectByUuid(string);
        void showStatus();
        void showScenegraph();
        string indent(int);
        void generateScenegraphVector();
        bool isScenegraphVectorEmpty();
        vector<SceneObject*> getScenegraphVector();
        glm::vec3 getDefaultCameraTranslation();
        glm::vec3 getDefaultCameraRotation();
        void setCameraMovementSpeed(float);
        float getCameraMovementSpeed();
        void loadClearColour(nlohmann::json);
        vector<float> getClearColour();

        void loadAmbientLightColour(nlohmann::json);
        vector<float> getAmbientLightColour();

        void addToDeleteQueue(SceneObject*);
        vector<SceneObject*> getDeleteQueue();
        void clearDeleteQueue();
        void destroyDeleteQueue();
        void findDeletedSceneObjects();

        bool createAllAssetInstances();
        IAssetInstance* createAssetInstanceFromDefinitionUuid(SceneObject*, string);
        IAssetInstance* createAssetInstance(SceneObject*, AssetDefinition*);
        AnimationInstance* createAnimationInstance(SceneObject*, AssetDefinition*);
        IAudioInstance* createAudioInstance(SceneObject*, AssetDefinition*);
        AssimpModelInstance* createModelInstance(SceneObject*, AssetDefinition*);
        LuaScriptInstance* createScriptInstance(SceneObject*, AssetDefinition*);
        ShaderInstance* createShaderInstance(SceneObject*, AssetDefinition*);
        PhysicsObjectInstance* createPhysicsObjectInstance(SceneObject*, AssetDefinition*);
        LightInstance* createLightInstance(SceneObject*, AssetDefinition*);
        FontInstance* createFontInstance(SceneObject*, AssetDefinition*);
        SpriteInstance* createSpriteInstance(SceneObject*, AssetDefinition*);

        void setProjectPath(string);

        map<SceneObject*,LuaScriptInstance*> *getLuaScriptMap();
        void insertIntoLuaScriptMap(SceneObject*,LuaScriptInstance*);
        bool createAssetInstancesForSceneObject(SceneObject*);
        void findDeletedScripts();
        void removeFromLuaScriptMap(SceneObject*);

        AssetDefinition* getAssetDefinitionByUuid(string);

        void loadPhysics(nlohmann::json);
        vector<float> getGravity();
        bool getPhysicsDebug();

        void exposeLuaApi(lua_State*);

    }; // End of Scene

} // End of Dream

#endif // End of SCENE_H
