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

#ifndef PROJECT_H
#define PROJECT_H

#include <vector>
#include "JSON/json.hpp"
#include "Scene/Scene.h"
#include "Resource/Resource.h"

#define PROJECT_JSON_UUID            "uuid"
#define PROJECT_JSON_NAME            "name"
#define PROJECT_JSON_AUTHOR          "author"
#define PROJECT_JSON_DESCRIPTION     "description"
#define PROJECT_JSON_SCENE_ARRAY     "scenes"
#define PROJECT_JSON_RESOURCE_ARRAY  "resources"
#define PROJECT_JSON_OPENAL_ENABLED  "openAL"
#define PROJECT_JSON_BULLET2_ENABLED "bullet2"
#define PROJECT_JSON_BULLET3_ENABLED "bullet3"
#define PROJECT_JSON_OPENGL_ENABLED  "openGL"
#define PROJECT_JSON_VULKAN_ENABLED  "vulkan"

namespace Dream {
	class Project {
	private:
		std::string mUUID;
		std::string mName;
		std::string mDescription;
		std::string mDirectory;
		std::string mAuthor;

		bool mOpenALEnabled;
		bool mBullet2Enabled;
		bool mBullet3Enabled;
		bool mOpenGLEnabled;
		bool mVulkanEnabled;

		std::vector<Dream::Scene::Scene*> mScenes;
		std::vector<Dream::Resource::Resource*> mResources;
	public:
		Project(void);
		Project(nlohmann::json);
		~Project(void);

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

		void addResource(Dream::Resource::Resource*);
		void removeResource(Dream::Resource::Resource*);
		int getNumberOfResources();

		std::string getDirectory();
		void setDirectory(std::string);

		void showStatus();

		bool isOpenALEnabled();
		bool isBullet2Enabled();
		bool isBullet3Enabled();
		bool isOpenGLEnabled();
		bool isVulkanEnabled();
	}; // End of Project
} // End of Dream

#endif // End of PROJECT_H
