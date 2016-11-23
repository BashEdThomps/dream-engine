/*
* InputComponent
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

#include "InputComponent.h"

namespace Dream {
  namespace Components {
    namespace Input {

      InputComponent::InputComponent()
        : Dream::Components::ComponentInterface() {

      }

      InputComponent::~InputComponent() {

      }

      void InputComponent::pushEvent(InputEvent event) {
        mInputEventsVector.push_back(event);
      }

      void InputComponent::clearInputEvents() {
        mInputEventsVector.clear();
      }

      std::vector<InputEvent> InputComponent::getInputEventVector() {
        return mInputEventsVector;
      }

      void InputComponent::onKeyboardEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
        InputEvent keyboardEvent(INPUT_TYPE_KEYBOARD);
        keyboardEvent.setKey(key);
        keyboardEvent.setScancode(scancode);
        keyboardEvent.setAction(action);
        keyboardEvent.setMods(mods);
        pushEvent(keyboardEvent);
      }

      void InputComponent::onMouseButtonEvent(GLFWwindow* window, int button, int action, int mods) {
        InputEvent mouseEvent(INPUT_TYPE_MOUSE_BUTTON);
        mouseEvent.setButton(button);
        mouseEvent.setAction(action);
        mouseEvent.setMods(mods);
        pushEvent(mouseEvent);
      }

      void InputComponent::onMouseScrollEvent(GLFWwindow* window, double xoffset, double yoffset) {
        InputEvent mouseEvent(INPUT_TYPE_MOUSE_SCROLL);
        mouseEvent.setXScrollOffset(xoffset);
        mouseEvent.setYScrollOffset(yoffset);
        pushEvent(mouseEvent);
      }

      void InputComponent::onMousePositionEvent(GLFWwindow* window, double xpos, double ypos) {
        InputEvent mouseEvent(INPUT_TYPE_MOUSE_POSITION);
        mouseEvent.setXPosition(xpos);
        mouseEvent.setYPosition(ypos);
        pushEvent(mouseEvent);
      }

      bool InputComponent::init() {
        if (!mWindow) {
          std::cerr << "GLFWInput: Unable to initialise without a window reference."
                    << std::endl;
          return false;
        }
        std::cout << "GLFWInput: Initialising." << std::endl;
        /* Find a way to C++ these
            glfwSetKeyCallback         (mWindow, InputComponent::onKeyboardEvent);
            glfwSetMouseButtonCallback (mWindow, InputComponent::onMouseButtonEvent);
            glfwSetScrollCallback      (mWindow, InputComponent::onMouseScrollEvent);
            glfwSetCursorPosCallback   (mWindow, InputComponent::onMousePositionEvent);
        */
        std::cout << "GLFWInput: Init Complete." << std::endl;
        return true;
      }

      void InputComponent::setWindow(GLFWwindow* window) {
        mWindow = window;
      }

      void InputComponent::update(Dream::Scene *scene) {
        glfwPollEvents();
      }

    } // End of Input
  } // End of Components
} // End of Dream
