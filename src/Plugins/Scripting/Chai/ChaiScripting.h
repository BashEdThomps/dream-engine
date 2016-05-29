/*
* Dream::Scripting::ChaiScripting
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

#ifndef CHAISCRIPTING_H
#define CHAISCRIPTING_H

#include "../ScriptingPluginInterface.h"
#include "../../../Asset/Instances/Script/ChaiScript/ChaiScriptInstance.h"

#include <chaiscript/chaiscript.hpp>
#include <chaiscript/chaiscript_stdlib.hpp>

namespace Dream     {
namespace Plugins   {
namespace Scripting {
namespace Chai      {
	
	class ChaiScripting : public ScriptingPluginInterface {
	private:
		chaiscript::ChaiScript *mChai;
	public:
		ChaiScripting(void);
		~ChaiScripting(void);

		chaiscript::ChaiScript* getChaiEngine();

		bool init();
		void update(Dream::Scene::Scene*);
	}; // End of ChaiScripting
	
} // End of Chai
} // End Scripting
} // End of Plugins
} // End of Dream

#endif // End of CHAISCRIPTING_H
