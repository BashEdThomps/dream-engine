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
#include "JSON/json.hpp"

namespace Dream {

	Project::Project(void) {

	}

	Project::Project(std::string json) {

	}

	Project::~Project(void) {
	}

	void Project::setName(std::string name) {
		mName = name;
	}

	std::string Project::getName() {
		return mName;
	}

	void Project::setDescription(std::string description) {
		mDescription = description;
	}

	std::string Project::getDescription(void) {
		return mDescription;
	}

	void Project::setAuthor(std::string author) {
		mAuthor = author;
	}

	std::string Project::getAuthor() {
		return mAuthor;
	}

	void Project::addScene(Dream::Scene::Scene *scene) {
		mScenes.push_back(scene);
	}

	void Project::addResource(Dream::Resource::Resource* resource) {
		mResources.push_back(resource);
	}

	void Project::removeResource(Dream::Resource::Resource*) {

	}

	int Project::getNumberOfResources() {
		return mResources.size();
	}

	int Project::getNumberOfScenes() {
		return mScenes.size();
	}
} // End of Dream
