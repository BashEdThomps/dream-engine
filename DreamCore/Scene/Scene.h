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
#include <memory>
#include <json.hpp>

using std::string;
using std::vector;
using std::map;
using std::unique_ptr;
using nlohmann::json;

namespace Dream
{
    class Project;
    class SceneObject;
    class SceneRuntime;
    class SceneJsonData;
    class IAssetInstance;
    class Transform3D;
    class AssetDefinition;

    class Scene
    {
    private:
        Project* mProjectHandle;
        unique_ptr<SceneObject> mRootSceneObject;
        unique_ptr<SceneJsonData> mJsonData;
        unique_ptr<SceneRuntime> mRuntime;

    public:
        Scene(Project* parent, json sceneJson);
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

        void findDeleteFlaggedSceneObjects();
        void findDeleteFlaggedScripts();

        void createAllAssetInstances();
        void loadAllAssetInstances();

        string getNotes();
        void setNotes(string notes);

        AssetDefinition* getAssetDefinitionByUuid(string);

        bool getPhysicsDebug();

        void loadSceneObjectJsonData(nlohmann::json, SceneObject*);

        nlohmann::json getJson();

        void cleanUpSceneObjects();
        void cleanUp();

        bool hasName(string);
        bool hasUuid(string);

        SceneRuntime* getRuntime() const;

    }; // End of Scene

} // End of Dream
