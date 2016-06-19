/*
* Dream::Asset::AssetInstance
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

#ifndef ASSETINSTANCE_H
#define ASSETINSTANCE_H

#include <iostream>

#include "../Util/FileReader.h"
#include "AssetDefinition.h"
#include "../Scene/SceneObject.h"

#define ASSET_UUID "uuid"
#define ASSET_NAME "name"

namespace Dream {
	
namespace Scene {
	class SceneObject;
}
	
namespace Asset {
	
	class AssetInstance {
	protected:
		AssetDefinition    *mDefinition;
		Scene::SceneObject *mParent;
	public:
		AssetInstance(AssetDefinition*);
		virtual ~AssetInstance(void) {};
		
		std::string getUUID();
		std::string getName();
		
		std::string getNameAndUUIDString();
		
		virtual bool load(std::string) = 0;
		virtual void loadExtraAttributes(nlohmann::json) = 0;
		
		void setParentSceneObject(Scene::SceneObject*);
		Scene::SceneObject* getParentSceneObject();
	}; // End of AssetInstance
	
} // End of Asset
} // End of Dream

#endif // End of ASSETINSTANCE_H
