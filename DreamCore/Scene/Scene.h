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

#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <json.hpp>

#include "SceneObject.h"
#include "SceneRuntime.h"

#include "../Common/Constants.h"
#include "../Common/IAssetInstance.h"
#include "../Common/AssetDefinition.h"
#include "../Components/Graphics/Camera.h"
#include "../Components/Audio/AudioInstance.h"
#include "../Components/Animation/AnimationInstance.h"
#include "../Components/Graphics/Sprite/SpriteInstance.h"
#include "../Components/Graphics/Shader/ShaderInstance.h"
#include "../Components/Graphics/Model/AssimpModelInstance.h"
#include "../Components/Graphics/Light/LightInstance.h"
#include "../Components/Graphics/Font/FontInstance.h"
#include "../Components/Physics/PhysicsObjectInstance.h"
#include "../Lua/LuaScriptInstance.h"
#include "../Utilities/String.h"

using namespace std;

namespace Dream
{
    class Project;
    class Scene
    {
    private:
        Project* mProjectHandle;
        nlohmann::json mJson;
        unique_ptr<SceneObject> mRootSceneObject;
        SceneRuntime mRuntime;

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

        nlohmann::json getJson();

        void cleanUpSceneObjects();
        void cleanUp();

        bool hasName(string);
        bool hasUuid(string);

        SceneRuntime getRuntime() const;

    }; // End of Scene

} // End of Dream
