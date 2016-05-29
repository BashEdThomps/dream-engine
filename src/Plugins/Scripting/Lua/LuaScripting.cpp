/*
 * Dream::Scripting::LuaScripting
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


#include "LuaScripting.h"

namespace Dream     {
namespace Plugins   {
namespace Scripting {
namespace Lua    {
				
	LuaScripting::LuaScripting(void) : ScriptingPluginInterface() {
	}
				
	LuaScripting::~LuaScripting(void) {
		if (mState) {
			lua_close(mState);
		}
	}
				
	bool LuaScripting::init() {
		mState = luaL_newstate();
		return mState != NULL;
	}
				
	void LuaScripting::update(Dream::Scene::Scene* scene) {
		Dream::Scene::SceneObject* root = scene->getRootSceneObject();
		if (root->hasScriptAssetInstance()) try {
			std::cout << "LuaScripting: Invoking RootSceneObject Update Script" << std::endl;
			Asset::Instances::Script::Lua::LuaScriptInstance* script;
			script = dynamic_cast<Asset::Instances::Script::Lua::LuaScriptInstance*>(root->getScriptAssetInstance());
			script->update(scene);
		} catch (std::exception ex) {
			std::cerr << "LuaScripting: Could not cast Root Scene Object's Script AssetInstance to LuaScriptInstance"
								<< std::endl
								<< ex.what()
								<< std::endl;
		}
	}
				
} // End of Lua
} // End of Scripting
} // End of Plugins
} // End of Dream
