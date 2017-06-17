/*
* Dream::Project
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

#include <string>
#include <memory>
#include <vector>
#include <json.hpp>

#include "../Scene/Scene.h"

using std::string;
using std::unique_ptr;
using std::vector;
using nlohmann::json;

namespace Dream
{
    class ArgumentParser;
    class FileReader;
    class ProjectRuntime;
    class ProjectDefinition;
    class AssetDefinition;
    class IWindowComponent;

    class Project
    {
        // Variables
    private:
        unique_ptr<ProjectRuntime>    mRuntime;
        unique_ptr<ProjectDefinition> mDefinition;

        vector<unique_ptr<Scene>>           mScenes;
        vector<unique_ptr<AssetDefinition>> mAssetDefinitions;

        Scene* mActiveSceneHandle;

        // Public Methods
    public:
        Project  (IWindowComponent* wcHandle = 0);
        ~Project ();

        ProjectRuntime* getRuntimeHandle();
        ProjectDefinition* getDefinitionHandle();

        bool openFromFileReader(string directory, FileReader fileReader);
        bool openFromArgumentParser(ArgumentParser parser);
        bool openFromDirectory(string directory);

        size_t countScenes();
        Scene* getSceneHandleByName(string name);
        Scene* getSceneHandleByUuid(string uuid);

        void setStartupSceneUuid(string uuid);
        string getStartupSceneUuid();
        Scene* getStartupSceneHandle();

        void addScene(json scene);
        void removeScene(const unique_ptr<Scene>& sceneHandle);

        void setActiveSceneHandle(Scene* sceneHandle);
        Scene* getActiveSceneHandle();
        bool hasActiveSceneHandle();

        void addAssetDefinition(json def);
        void removeAssetDefinition(const unique_ptr<AssetDefinition>& def);

        size_t countAssetDefinitions();
        AssetDefinition* getAssetDefinitionHandleByUuid(string);

        void setStartupSceneAsActive();

    }; // End of Project

} // End of Dream
