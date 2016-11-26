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

      ChaiScriptInstance::ChaiScriptInstance(
          AssetDefinition* definition, Transform3D* transform
      ) : AssetInstance(definition,transform) {
        return;
      }

      ChaiScriptInstance::~ChaiScriptInstance() {
        return;
      }

      bool ChaiScriptInstance::load(string projectPath) {
        mProjectPath = projectPath;
        mAbsolutePath = projectPath+mDefinition->getAssetPath();
        cout << "ChaiScriptInstance: Script at " << mAbsolutePath << endl;
        return mAbsolutePath.size() != 0;
      }

      string ChaiScriptInstance::getProjectPath() {
        return mProjectPath;
      }

      void ChaiScriptInstance::update() {}

      string ChaiScriptInstance::getAbsolutePath() {
        return mAbsolutePath;
      }

} // End of Dream
