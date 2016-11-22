/*
* ChaiScriptAPI
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

#include <iostream>
#include <chaiscript/chaiscript.hpp>
#include <chaiscript/chaiscript_stdlib.hpp>

#include "../../Dream.h"

namespace Dream {
  namespace Components {
    namespace Scripting {

      class ChaiScriptAPI {
      private:
        chaiscript::ChaiScript *mEngine;
        Dream* mDream;
      public:
        ChaiScriptAPI(Dream*);
        ~ChaiScriptAPI(void);

        bool init();
        void update(Scene*);
        void handleInputs(SceneObject*);
        bool loadScript(Components::Scripting::ChaiScriptInstance*);
        bool importScriptAssetByUUID(std::string);

        // API Exposure
        void initAPIs();
        void initAnimationInstanceAPI();
        void initAnimationComponentAPI();
        void initAssetInstanceAPI();
        void initAssetManagerAPI();
        void initAudioComponentAPI();
        void initCameraAPI();
        void initGlfwTimeAPI();
        void initInputEventAPI();
        void initComponentManagerAPI();
        void initProjectAPI();
        void initSceneObjectAPI();
        void initSceneAPI();
        void initVideoComponentAPI();

      }; // End of ChaiScripAPI
    } // End Scripting
  } // End of Components
} // End of Dream

#endif // End of CHAISCRIPTING_H
