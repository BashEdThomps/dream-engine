/*
* LuaScriptInstance
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

#ifndef LUASCRIPTINSTANCE_H
#define LUASCRIPTINSTANCE_H

#include <vector>
#include <iostream>
#include "../IAssetInstance.h"
#include "../Constants.h"

namespace Dream
{
      class LuaScriptInstance : public IAssetInstance
      {
      public:
        LuaScriptInstance(AssetDefinition*,Transform3D*);
        ~LuaScriptInstance(void);
        bool load(string);
        void update();
        void loadExtraAttributes(nlohmann::json) {}
      }; // End of LuaScriptInstance
} // End of Dream
#endif // End of LUASCRIPT_H
