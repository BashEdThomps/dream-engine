/*
* Dream::Project::Project
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

#ifndef PROJECT_H
#define PROJECT_H

#include <unistd.h>
#include <vector>
#include "../ExternalLibs/JSON/json.hpp"
#include "../Scene/Scene.h"
#include "../Asset/AssetDefinition.h"
#include "../Util/StringUtils.h"
#include "../Asset/AssetManager.h"
#include "../Components/ComponentManager.h"
#include "../Scene/Time.h"

#define PROJECT_UUID            "uuid"
#define PROJECT_NAME            "name"
#define PROJECT_AUTHOR          "author"
#define PROJECT_DESCRIPTION     "description"
#define PROJECT_SCENE_ARRAY     "scenes"
#define PROJECT_ASSET_ARRAY     "assets"
#define PROJECT_STARTUP_SCENE   "startupScene"

#define PROJECT_CHAI_ENABLED    "chai"
#define PROJECT_OPENAL_ENABLED  "openAL"
#define PROJECT_BULLET2_ENABLED "bullet2"
#define PROJECT_OPENGL_ENABLED  "openGL"
#define PROJECT_VULKAN_ENABLED  "vulkan"

namespace Dream {
namespace Project {
	class Project {
	// Static Variables
	private:
		static Scene::Time* sTime;
	// Instance Variables
	private:
		std::string mUUID;
		std::string mName;
		std::string mDescription;
		std::string mAuthor;
		std::string mProjectPath;
		std::string mStartupScene;
		Asset::AssetManager    *mAssetManager;
		Components::ComponentManager *mComponentManager;
		bool mChaiEnabled;
		bool mOpenALEnabled;
		bool mBullet2Enabled;
		bool mOpenGLEnabled;
		bool mVulkanEnabled;
    bool mDone;
		std::vector<Dream::Scene::Scene*> mScenes;
		std::vector<Dream::Asset::AssetDefinition*> mAssetDefinitions;
		Scene::Scene *mActiveScene;
		
		// Static Methods
	public:
		static Scene::Time* getTime();
	// Instance Methods
	public:
		Project(void);
		Project(std::string, nlohmann::json);
		~Project(void);

		void setMetadata(nlohmann::json);
		void setComponentFlags(nlohmann::json);
		void loadScenesFromJson(nlohmann::json);
		void loadAssetsDefinitionsFromJson(nlohmann::json);

		void        setUUID(std::string);
		std::string getUUID();

		void        setName(std::string);
		std::string getName();

		void        setDescription(std::string);
		std::string getDescription(void);

		void        setAuthor(std::string);
		std::string getAuthor();
		
		void        setStartupSceneUUID(std::string);
		std::string getStartupSceneUUID();
		Dream::Scene::Scene* getStartupScene();
		
		void addScene(Dream::Scene::Scene*);
		void removeScene(Dream::Scene::Scene*);
		int  getNumberOfScenes();
		
		Dream::Scene::Scene* getSceneByName(std::string);
		Dream::Scene::Scene* getSceneByUUID(std::string);
		

		void addAssetDefinition(Dream::Asset::AssetDefinition*);
		void removeAssetDefinition(Dream::Asset::AssetDefinition*);
		int  getNumberOfAssetDefinitions();
		Dream::Asset::AssetDefinition* getAssetDefinitionByUUID(std::string);

		std::string getProjectPath();
		void        setProjectPath(std::string);

		void showStatus();
		
		bool isChaiEnabled();
		bool isOpenALEnabled();
		bool isBullet2Enabled();
		bool isOpenGLEnabled();
		bool isVulkanEnabled();
		
		void          setActiveScene(Scene::Scene*);
		Scene::Scene *getActiveScene();
		bool          hasActiveScene();
		bool          initActiveScene();
		
		bool createAssetManager();
		Asset::AssetManager* getAssetManager();
		
		bool createComponentManager();
		Components::ComponentManager* getComponentManager();
		
		bool loadScene(Scene::Scene*);
		bool run();
		
	}; // End of Project
	
} // End of Project
} // End of Dream

#endif // End of PROJECT_H
