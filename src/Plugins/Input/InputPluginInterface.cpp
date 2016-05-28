/*
* Dream::Plugins::Input::InputInterface
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

#include "InputPluginInterface.h"

namespace Dream   {
namespace Plugins {
namespace Input   {
	
	static std::vector<InputEvent> sInputEventVector;
	
	InputPluginInterface::InputPluginInterface  ()
	  : Dream::Plugins::PluginInterface() {}
	
	void InputPluginInterface::pushEvent(InputEvent event) {
		sInputEventVector.push_back(event);
	}

	void InputPluginInterface::clearInputEvents() {
		sInputEventVector.clear();
	}
	
	std::vector<InputEvent> InputPluginInterface::getInputEventVector() {
		return sInputEventVector;
	}
	
	
} // End of Input
} // End of Plugins
} // End of Dream
