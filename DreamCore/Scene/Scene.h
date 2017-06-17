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
    class SceneDefinition;
    class IAssetInstance;
    class Transform3D;
    class AssetDefinition;
    class SceneObjectDefinition;

    class Scene
    {
    private:
        Project* mProjectHandle;
        unique_ptr<SceneObject> mRootSceneObject;
        unique_ptr<SceneDefinition> mDefinition;
        unique_ptr<SceneRuntime> mRuntime;

    public:
        Scene(Project* parent, json sceneJson);
        ~Scene();

        void flush();

        Project* getProjectHandle();

        int countChildrenOfSceneObject(SceneObject*);

        void setRootSceneObjectHandle(SceneObject*);
        SceneObject* getRootSceneObjectHandle();
        int countSceneObjects();

        SceneObject* getSceneObjectHandleByName(string);
        SceneObject* getSceneObjectHandleByUuid(string);

        void showScenegraph();

        string indent(int);

        void findDeleteFlaggedSceneObjects();
        void findDeleteFlaggedScripts();

        void createAllAssetInstances();
        void loadAllAssetInstances();

        void cleanUpSceneObjects();
        void cleanUpRuntime();

        SceneRuntime* getRuntimeHandle() const;
        SceneDefinition* getDefinitionHandle() const;

    }; // End of Scene

} // End of Dream
