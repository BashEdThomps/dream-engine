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

#ifndef PROJECT_H
#define PROJECT_H

#include <vector>
#include "../JSON/json.hpp"
#include "../Scene/Scene.h"
#include "../Resource/Resource.h"
#include "../Resource/Instance/ResourceInstance.h"
#include "../Resource/Instance/Model/WaveFront/ObjModelInstance.h"
#include "../Resource/Instance/Script/JavaScript/JavaScriptInstance.h"
#include "../Resource/Instance/Script/ChaiScript/ChaiScriptInstance.h"
#include "../Resource/Instance/Audio/Ogg/OggAudioInstance.h"
#include "../Resource/Instance/Audio/Wav/WavAudioInstance.h"
#include "../Resource/Instance/Animation/AnimationInstance.h"

#define PROJECT_UUID            "uuid"
#define PROJECT_NAME            "name"
#define PROJECT_AUTHOR          "author"
#define PROJECT_DESCRIPTION     "description"
#define PROJECT_SCENE_ARRAY     "scenes"
#define PROJECT_RESOURCE_ARRAY  "resources"
#define PROJECT_V8_ENABLED      "v8"
#define PROJECT_CHAI_ENABLED    "chai"
#define PROJECT_OPENAL_ENABLED  "openAL"
#define PROJECT_BULLET2_ENABLED "bullet2"
#define PROJECT_BULLET3_ENABLED "bullet3"
#define PROJECT_OPENGL_ENABLED  "openGL"
#define PROJECT_VULKAN_ENABLED  "vulkan"

namespace Dream {
	class Project {
	private:
		std::string mUUID;
		std::string mName;
		std::string mDescription;
		std::string mAuthor;
		std::string mProjectPath;

		bool mChaiEnabled;
		bool mV8Enabled;
		bool mOpenALEnabled;
		bool mBullet2Enabled;
		bool mBullet3Enabled;
		bool mOpenGLEnabled;
		bool mVulkanEnabled;

		std::vector<Dream::Scene::Scene*> mScenes;
		std::vector<Dream::Resource::Resource*> mResources;
	public:
		Project(void);
		Project(std::string, nlohmann::json);
		~Project(void);

		void enablePluginFlags(nlohmann::json);
		void loadScenesFromJson(nlohmann::json);
		void loadResourcesFromJson(nlohmann::json);

		void        setUUID(std::string);
		std::string getUUID();

		void        setName(std::string);
		std::string getName();

		void        setDescription(std::string);
		std::string getDescription(void);

		void        setAuthor(std::string);
		std::string getAuthor();

		void addScene(Dream::Scene::Scene*);
		void removeScene(Dream::Scene::Scene*);
		int getNumberOfScenes();
		Dream::Scene::Scene* getSceneByName(std::string);
		Dream::Scene::Scene* getSceneByUUID(std::string);

		void addResource(Dream::Resource::Resource*);
		void removeResource(Dream::Resource::Resource*);
		int getNumberOfResources();

		std::string getProjectPath();
		void        setProjectPath(std::string);

		void showStatus();

		bool isChaiEnabled();
		bool isV8Enabled();
		bool isOpenALEnabled();
		bool isBullet2Enabled();
		bool isBullet3Enabled();
		bool isOpenGLEnabled();
		bool isVulkanEnabled();
	}; // End of Project
} // End of Dream

#endif // End of PROJECT_H
