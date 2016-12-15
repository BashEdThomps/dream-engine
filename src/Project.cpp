/*
* Project
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

#include "Project.h"

namespace Dream {

    Project::Project(AssetManager* assetManager) {
        setAssetManager(assetManager);
    }

    Project::Project(AssetManager* assetManager, string projectPath, nlohmann::json jsonProject) {
        setAssetManager(assetManager);
        setProjectPath(projectPath);
        setMetadata(jsonProject);
        mAssetManager->loadAssetDefinitionsFromJson(jsonProject[PROJECT_ASSET_ARRAY]);
        loadScenesFromJson(jsonProject[PROJECT_SCENE_ARRAY]);
        showStatus();
    }

    void Project::setMetadata(nlohmann::json jsonProject) {
        if (jsonProject [PROJECT_UUID].is_null()) {
            cerr << "Project: UUID is nullptr." << endl;
        } else {
            setUuid(jsonProject[PROJECT_UUID]);
        }

        if (jsonProject[PROJECT_NAME].is_null()) {
            cerr << "Project: Name is nullptr." << endl;
        } else {
            setName(jsonProject[PROJECT_NAME]);
        }

        if (jsonProject[PROJECT_AUTHOR].is_null()) {
            cerr << "Project: Author is nullptr." << endl;
        } else {
            setAuthor(jsonProject[PROJECT_AUTHOR]);
        }

        if (jsonProject[PROJECT_DESCRIPTION].is_null()) {
            cerr << "Project: Descriptiont is nullptr." << endl;
        } else {
            setDescription(jsonProject[PROJECT_DESCRIPTION]);
        }

        if (jsonProject[PROJECT_STARTUP_SCENE].is_null()) {
            cerr << "Project: Startup Scene is nullptr." << endl;
        } else {
            setStartupSceneUuid(jsonProject[PROJECT_STARTUP_SCENE]);
        }

        if (!jsonProject[PROJECT_WINDOW_SIZE].is_null()) {
            nlohmann::json windowSizeJson = jsonProject[PROJECT_WINDOW_SIZE];
            setWindowWidth(windowSizeJson[PROJECT_WIDTH]);
            setWindowHeight(windowSizeJson[PROJECT_HEIGHT]);
        }

    }

    void Project::showStatus() {
        if (DEBUG) {
            cout << "Project: "  << endl;
            cout << "         UUID: " << getUuid() << endl;
            cout << "         Name: " << getName() << endl;
            cout << "       Author: " << getAuthor() << endl;
            cout << "  Description: " << getDescription() << endl;
            cout << "Startup Scene: " << getStartupSceneUuid() << endl;
            cout << " Window Width: " << getWindowWidth() << endl;
            cout << "Window Height: " << getWindowHeight() << endl;
        }
    }

    Project::~Project() {}

    void Project::setName(string name) {
        mName = name;
    }

    void Project::setUuid(string uuid) {
        mUuid = uuid;
    }

    string Project::getUuid() {
        return mUuid;
    }

    void Project::loadScenesFromJson(nlohmann::json jsonSceneArray) {

        if (DEBUG) {
            cout << "Project: Loading Scenes from JSON Array" << endl;
        }
        for (nlohmann::json::iterator it = jsonSceneArray.begin(); it != jsonSceneArray.end(); ++it) {

            if (DEBUG) {
                cout << "Project: Creating Scene" << endl;
            }
            Scene *nextScene = new Scene((*it));
            nextScene->showStatus();
            addScene(nextScene);
        }
    }

    void Project::setStartupSceneUuid(string sceneUuid) {
        mStartupScene = sceneUuid;
    }

    string Project::getStartupSceneUuid() {
        return mStartupScene;
    }

    Scene* Project::getStartupScene() {
        return getSceneByUuid(getStartupSceneUuid());
    }

    string Project::getName() {
        return mName;
    }

    void Project::setDescription(string description) {
        mDescription = description;
    }

    string Project::getDescription(void) {
        return mDescription;
    }

    void Project::setAuthor(string author) {
        mAuthor = author;
    }

    string Project::getAuthor() {
        return mAuthor;
    }

    void Project::addScene(Scene *scene) {
        mScenes.push_back(scene);
    }

    size_t Project::getNumberOfScenes() {
        return mScenes.size();
    }

    Scene* Project::getSceneByUuid(string uuid) {
        Scene* retval = nullptr;
        for(vector<Scene*>::iterator it = mScenes.begin(); it != mScenes.end(); ++it) {
            if ((*it)->getUuid().compare(uuid) == 0) {
                retval = (*it);
                break;
            }
        }
        return retval;
    }

    Scene* Project::getSceneByName(string name) {
        Scene* retval = nullptr;
        for(vector<Scene*>::iterator it = mScenes.begin(); it != mScenes.end(); ++it) {
            if ((*it)->getName().compare(name) == 0) {
                retval = (*it);
                break;
            }
        }
        return retval;
    }

    string Project::getProjectPath() {
        return mProjectPath;
    }

    void Project::setProjectPath(string dir) {
        mProjectPath = dir;
    }

    void Project::setActiveScene(Scene* scene) {
        mActiveScene = scene;
    }

    Scene* Project::getActiveScene() {
        return mActiveScene;
    }

    bool Project::hasActiveScene() {
        return getActiveScene() != nullptr;
    }

    void Project::setAssetManager(AssetManager* assetManager) {
        mAssetManager = assetManager;
    }

    int Project::getWindowWidth() {
        return mWindowWidth;
    }

    void Project::setWindowWidth(int width) {
        mWindowWidth = width;
    }

    int Project::getWindowHeight() {
        return mWindowHeight;
    }

    void Project::setWindowHeight(int height) {
        mWindowHeight = height;
    }

} // End of Dream
