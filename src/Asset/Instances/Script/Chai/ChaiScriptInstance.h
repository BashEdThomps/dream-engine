/*
* Dream::Asset::Instances::Script::Chai::ChaiScriptInstance
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

#ifndef CHAISCRIPTINSTANCE_H
#define CHAISCRIPTINSTANCE_H

#include <vector>
#include <iostream>
#include <chaiscript/chaiscript.hpp>
#include "../../../AssetInstance.h"
#include "../../../../Plugins/Scripting/Chai/ChaiScripting.h"

namespace Dream     {
namespace Asset     {
namespace Instances {
namespace Script    {
namespace Chai      {
    
    class ChaiScriptInstance : public AssetInstance {
		private:
			chaiscript::ChaiScript *mScript;
    public:
			ChaiScriptInstance(AssetDefinition*);
			~ChaiScriptInstance(void);
			bool load(std::string);
			void update();
			void processInputs(std::vector<Plugins::Input::InputEvent>);
			
			// API Exposure
			void initInputEventInChaiScript();
			void initCameraInChaiScript();
			void initGlfwTimeInChaiScript();
			void initSceneObjectInChaiScript();
    }; // End of ChaiScriptInstance
    
} // End of ChaiScript
} // End of Script
} // End of Instance
} // End of Asset
} // End of Dream

#endif // End of CHAISCRIPT_H
