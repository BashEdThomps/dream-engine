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

#ifndef LuaScripting_h
#define LuaScripting_h

#include "lua.hpp"

#include "../ScriptingPluginInterface.h"
#include "../../../Asset/Instances/Script/Lua/LuaScriptInstance.h"

namespace Dream     {
namespace Plugins   {
namespace Scripting {
namespace Lua    {
				
	class LuaScripting : public ScriptingPluginInterface {
	private:
		lua_State	*mState;
	public:
		LuaScripting(void);
		~LuaScripting(void);
					
		bool init();
		void update(Dream::Scene::Scene*);
	}; // End of LuaScripting
				
} // End of Lua
} // End Scripting
} // End of Plugins
} // End of Dream

#endif /* LuaScripting_h */
