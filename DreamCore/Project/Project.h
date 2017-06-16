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

#include "Project.h"

#include <string>
#include <memory>
#include <vector>
#include <json.hpp>

using std::string;
using std::unique_ptr;
using std::vector;
using nlohmann::json;

namespace Dream
{
    class ArgumentParser;
    class FileReader;
    class ProjectRuntime;
    class ProjectJsonData;
    class Scene;
    class AssetDefinition;
    class IWindowComponent;

    class Project
    {
        // Variables
    private:
        unique_ptr<ProjectRuntime> mRuntime;
        unique_ptr<ProjectJsonData> mJsonData;
        string mProjectPath;
        vector<Scene*> mScenes;
        vector<AssetDefinition*> mAssetDefinitions;
        Scene *mActiveSceneHandle;

        // Public Methods
    public:
        Project(IWindowComponent* = 0);
        ~Project();

        bool initRuntime();
        ProjectRuntime *getRuntimeHandle();
        ProjectJsonData *getJsonDataHandle();

        void destroyAllScenes();
        void destroyAllAssetDefinitions();

        void loadMetadataFromJson(nlohmann::json);
        void loadScenesFromJson(nlohmann::json);
        bool loadFromFileReader(string projectPath, FileReader*);
        bool loadFromArgumentParser(ArgumentParser*);
        bool loadFromDirectory(string);

        void updateAll();
        void updateLogic();
        void updateGraphics();
        void updateFlush();

        void setUuid(string);
        string getUuid();

        void setName(string);
        string getName();

        void setDescription(string);
        string getDescription(void);

        void setAuthor(string);
        string getAuthor();

        void setStartupSceneUuid(string);

        string getStartupSceneUuid();
        Scene* getStartupScene();

        void addScene(Scene*);
        void removeScene(Scene*);
        size_t getNumberOfScenes();
        Scene* getSceneByName(string);
        Scene* getSceneByUuid(string);
        vector<Scene*> getSceneList();

        string getProjectPath();
        void setProjectPath(string);

        void showStatus();

        void setActiveScene(Scene*);
        Scene *getActiveScene();
        bool hasActiveScene();

        int getWindowWidth();
        void setWindowWidth(int);
        int getWindowHeight();
        void setWindowHeight(int);

        void addAssetDefinition(AssetDefinition*);
        void removeAssetDefinition(AssetDefinition*);
        size_t getNumberOfAssetDefinitions();
        AssetDefinition* getAssetDefinitionByUuid(string);
        vector<AssetDefinition*> getAssetDefinitions();

        nlohmann::json getJson();

        bool loadActiveScene();
        bool loadSceneByUuid(string);
        void setStartupSceneActive();

        void cleanUpActiveScene();

        // Private Methods
    private:
        void loadAssetDefinitionsFromJson(nlohmann::json);

    }; // End of Project

} // End of Dream
