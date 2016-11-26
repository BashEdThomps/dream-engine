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
#include "../../AssetInstance.h"

namespace Dream {
      class ChaiScriptInstance : public AssetInstance {
      private:
        string mProjectPath;
        string mAbsolutePath;
      public:
        ChaiScriptInstance(AssetDefinition*,Transform3D*);
        ~ChaiScriptInstance(void);
        bool load(string);
        void update();
        string getProjectPath();
        string getAbsolutePath();
        void loadExtraAttributes(nlohmann::json) {}
      }; // End of ChaiScriptInstance

} // End of Dream
#endif // End of CHAISCRIPT_H
