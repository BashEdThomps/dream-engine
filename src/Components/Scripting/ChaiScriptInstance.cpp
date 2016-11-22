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

#include "ChaiScriptInstance.h"

namespace Dream {
  namespace Components {
    namespace Scripting {

      ChaiScriptInstance::ChaiScriptInstance(AssetDefinition* definition) : AssetInstance(definition) {}

      ChaiScriptInstance::~ChaiScriptInstance() {}

      bool ChaiScriptInstance::load(std::string projectPath) {
        mProjectPath = projectPath;
        mAbsolutePath = projectPath+mDefinition->getAssetPath();
        std::cout << "ChaiScriptInstance: Script at " << mAbsolutePath << std::endl;
        return mAbsolutePath.size() != 0;
      }

      std::string ChaiScriptInstance::getProjectPath() {
        return mProjectPath;
      }

      void ChaiScriptInstance::update() {}

      std::string ChaiScriptInstance::getAbsolutePath() {
        return mAbsolutePath;
      }

    } // End of Script
  } // End of Components
} // End of Dream
