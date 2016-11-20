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

#include "ChaiScripting.h"

namespace Dream {
  namespace Components {
    namespace Scripting {

      ChaiScripting::ChaiScripting(void) : ScriptingComponentInterface() {}
      ChaiScripting::~ChaiScripting(void) {}

      bool ChaiScripting::init() {
        std::cout << "ChaiScripting: Initialising..." << std::endl;
        return true;
      }

      void ChaiScripting::update(Dream::Scene* scene) {
        handleInputs(scene->getRootSceneObject());
        std::vector<Dream::SceneObject*> scenegraph = scene->getScenegraphVector();
        std::vector<Dream::SceneObject*>::iterator sgIter;
        for (sgIter = scenegraph.begin(); sgIter != scenegraph.end(); sgIter++) {
          Dream::SceneObject *currentSceneObject = (*sgIter);
          if (currentSceneObject->hasScriptAssetInstance()) {
            try {
              Asset::Instances::Script::Chai::ChaiScriptInstance* script;
              script = dynamic_cast<Asset::Instances::Script::Chai::ChaiScriptInstance*>(currentSceneObject->getScriptAssetInstance());
              script->update();
            } catch (const std::exception &ex) {
              std::cerr << "ChaiScripting: Exception Running Update on "
                        << currentSceneObject->getNameUUIDString() << std::endl << ex.what() << std::endl;
            }
          }
        }
      }

      void ChaiScripting::handleInputs(Dream::SceneObject* root) {
        if (root->hasScriptAssetInstance()) {
          try {
            Asset::Instances::Script::Chai::ChaiScriptInstance* script;
            script = dynamic_cast<Asset::Instances::Script::Chai::ChaiScriptInstance*>(
                  root->getScriptAssetInstance()
                  );
            script->processInputs(Input::InputComponentInterface::sInputEventsVector);
          } catch (const std::exception &ex) {
            std::cerr << "ChaiScripting: Exception Handling Inputs"
                      << std::endl
                      << ex.what()
                      << std::endl;
          }
          Input::InputComponentInterface::clearInputEvents();
        }
      }

    } // End of Scripting
  } // End of Components
} // End of Dream
