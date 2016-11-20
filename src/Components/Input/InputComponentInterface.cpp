/*
* Dream::Components::Input::InputInterface
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

#include "InputComponentInterface.h"

namespace Dream {
  namespace Components {
    namespace Input {

      std::vector<InputEvent> InputComponentInterface::sInputEventsVector = std::vector<InputEvent>();

      InputComponentInterface::InputComponentInterface() : Dream::Components::ComponentInterface() {}

      void InputComponentInterface::pushEvent(InputEvent event) {
        sInputEventsVector.push_back(event);
      }

      void InputComponentInterface::clearInputEvents() {
        sInputEventsVector.clear();
      }

      std::vector<InputEvent> InputComponentInterface::getInputEventVector() {
        return sInputEventsVector;
      }

    } // End of Input
  } // End of Components
} // End of Dream
