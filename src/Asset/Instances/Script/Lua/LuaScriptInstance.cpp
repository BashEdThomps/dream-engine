/*
 * Dream::Plugins::Scripting::Lua::LuaScriptInstance
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

#include "LuaScriptInstance.h"

namespace Dream     {
namespace Asset     {
namespace Instances {
namespace Script    {
namespace Lua    {
					
	LuaScriptInstance::LuaScriptInstance(Dream::Asset::AssetDefinition* definition) :
		Dream::Asset::AssetInstance(definition) {}
					
	LuaScriptInstance::~LuaScriptInstance(void) {}
					
	bool LuaScriptInstance::load(std::string projectPath) {
		Util::FileReader *reader;
		std::string absPath = projectPath+mDefinition->getAssetPath();
		reader = new Util::FileReader(absPath);
		reader->readIntoStringStream();
		mScriptContent = reader->getContentsAsString();
		delete reader;
		std::cout << "LuaScriptInstance: Read Script for " << mDefinition->getName() << std::endl
		          << "\tFrom " << absPath << std::endl
		          << mScriptContent << std::endl;
		return !mScriptContent.empty();
	}
	
	std::string LuaScriptInstance::getScriptContent() {
		return mScriptContent;
	}
	
	void LuaScriptInstance::update(Dream::Scene::Scene *scene) {
		std::cout << "LuaScriptInstance: Executing Update" << std::endl;
	}
	
} // End of Lua
} // End of Script
} // End of Instance
} // End of Asset
} // End of Dream
