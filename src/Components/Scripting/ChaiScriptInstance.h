/*
* ChaiScriptInstance
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
#include "../AssetInstance.h"
#include "ChaiScripting.h"

namespace Dream {
  namespace Components {
    namespace Script {
      class ChaiScriptInstance : public AssetInstance {
      private:
        chaiscript::ChaiScript *mScript;
        std::string             mProjectPath;
      public:
        ChaiScriptInstance(AssetDefinition*);
        ~ChaiScriptInstance(void);
        bool load(std::string);
        void update();
        void processInputs(std::vector<Components::Input::InputEvent>);
        bool importScriptAssetByUUID(std::string);
        std::string getProjectPath();
        // API Exposure
        void initAPIs();
        void initAnimationInstanceAPI();
        void initAnimationComponentAPI();
        void initAssetInstanceAPI();
        void initAssetManagerAPI();
        void initAudioComponentAPI();
        void initCameraAPI();
        void initChaiScriptAPI();
        void initGlfwTimeAPI();
        void initInputEventAPI();
        void initComponentManagerAPI();
        void initProjectAPI();
        void initSceneObjectAPI();
        void initSceneAPI();
        void initVideoComponentAPI();
        void loadExtraAttributes(nlohmann::json) {}
      }; // End of ChaiScriptInstance

    } // End of Scripting
  } // End of Components
} // End of Dream
#endif // End of CHAISCRIPT_H
