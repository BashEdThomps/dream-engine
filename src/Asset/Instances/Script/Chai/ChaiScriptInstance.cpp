/*
* Dream::Plugins::Scripting::Chai::ChaiScriptInstance
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

#include "ChaiScriptInstance.h"

namespace Dream     {
namespace Asset     {
namespace Instances {
namespace Script    {
namespace Chai      {
    
	ChaiScriptInstance::ChaiScriptInstance(Dream::Asset::AssetDefinition* definition) :
		Dream::Asset::AssetInstance(definition) {
	}

	ChaiScriptInstance::~ChaiScriptInstance(void) {}
	
	bool ChaiScriptInstance::load(std::string projectPath) {
		mScript = new chaiscript::ChaiScript(chaiscript::Std_Lib::library());
		chaiscript::Boxed_Value result = mScript->eval_file(projectPath+mDefinition->getAssetPath());
		mScript->add(chaiscript::user_type<Dream::Plugins::Input::InputEvent>(), "InputEvent");
		return !result.is_null();
	}
	
	void ChaiScriptInstance::update() {
		mScript->eval("update()");
	}
	
	void ChaiScriptInstance::processInputs(std::vector<Plugins::Input::InputEvent> inputEventVector) {
		using namespace Dream::Plugins::Input;
		for(std::vector<InputEvent>::iterator it = inputEventVector.begin(); it != inputEventVector.end(); it++) {
			auto processFunction = mScript->eval<std::function<void(InputEvent)>>("onInputEvent");
			processFunction(*it);
		}
	}
	
} // End of ChaiScript
} // End of Script
} // End of Instance
} // End of Asset
} // End of Dream
