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
#include "Components/Graphics/Sprite/SpriteInstance.h"
#include "Components/Graphics/Shader/ShaderInstance.h"
#include "Components/Graphics/Model/AssimpModelInstance.h"
#include "Components/Graphics/Light/LightInstance.h"
#include "Components/Graphics/Font/FontInstance.h"
#include "Components/Physics/PhysicsObjectInstance.h"

using namespace std;

namespace Dream
{
    class Project;

    enum SceneState
    {
        NOT_LOADED,
        LOADED,
        RUNNING,
        DONE,
        CLEANED_UP
    };

    class Scene
    {
    private:
        Project* mProjectHandle;
        nlohmann::json mJson;
        SceneObject *mRootSceneObjectHandle;
        vector<SceneObject*> mDeleteQueue;
        string mProjectPath;
        SceneState mState;

    public:
        Scene(Project* parent, nlohmann::json projJson);
        ~Scene();

        void flush();

        string getUuid();
        void setUuid(string);

        string getName();
        void setName(string);

        string getNameAndUuidString();

        Project* getProjectHandle();

        int countChildrenOfSceneObject(SceneObject*);

        void setRootSceneObject(SceneObject*);
        SceneObject* getRootSceneObject();
        int getNumberOfSceneObjects();

        SceneObject* getSceneObjectByName(string);
        SceneObject* getSceneObjectByUuid(string);

        void showStatus();
        void showScenegraph();

        string indent(int);
        Transform3D getDefaultCameraTransform();

        void setCameraMovementSpeed(float);
        float getCameraMovementSpeed();

        vector<float> getClearColour();
        vector<float> getAmbientLightColour();

        void addToDeleteQueue(SceneObject*);
        vector<SceneObject*> getDeleteQueue();
        void clearDeleteQueue();
        void destroyDeleteQueue();

        void findDeleteFlaggedSceneObjects();
        void findDeleteFlaggedScripts();

        void createAllAssetInstances();
        void loadAllAssetInstances();

        string getNotes();
        void setNotes(string notes);

        AssetDefinition* getAssetDefinitionByUuid(string);

        vector<float> getGravity();
        bool getPhysicsDebug();

        void loadSceneObjectJsonData(nlohmann::json, SceneObject*);

        nlohmann::json toJson();

        void cleanUpSceneObjects();
        void cleanUp();

        bool hasName(string);
        bool hasUuid(string);

        SceneState getState();
        void setState(SceneState);
        bool isState(SceneState);

    }; // End of Scene

} // End of Dream

#endif // End of SCENE_H
