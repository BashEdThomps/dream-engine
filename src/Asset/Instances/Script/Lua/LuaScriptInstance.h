/*
 * Lua::LuaScriptInstance
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

#ifndef LuaScriptInstance_h
#define LuaScriptInstance_h

#include <iostream>
extern "C"
{
	#include "lua.h"
}
#include "../../../../Scene/Scene.h"
#include "../../../AssetInstance.h"

#define ASSET_FORMAT_LUA "Lua"

namespace Dream     {
namespace Asset     {
namespace Instances {
namespace Script    {
namespace Lua    {
					
	class LuaScriptInstance : public Dream::Asset::AssetInstance {
	private:
		std::string mScriptContent;
	public:
		LuaScriptInstance(Dream::Asset::AssetDefinition*);
		~LuaScriptInstance(void);
		bool load(std::string);
		std::string getScriptContent();
		void update(Dream::Scene::Scene*);
		
	}; // End of LuaScriptInstance
					
} // End of LuaScript
} // End of Script
} // End of Instance
} // End of Asset
} // End of Dream

#endif /* LuaScriptInstance_h */
