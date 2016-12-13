/*
* AssetInstance
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

#include "Uuid.h"
#include "AssetInstance.h"

namespace Dream {
  AssetInstance::AssetInstance(
      AssetDefinition* definition,
      Transform3D* transform ) {
    mDefinition = definition;
    mTransform = transform;
    mLoaded = false;
    mName = mDefinition->getName();
    mUuid = Uuid::generateUuid();
  }

  AssetInstance::~AssetInstance() {

  }

  string AssetInstance::getName() {
    return mDefinition->getName();
  }

  string AssetInstance::getUuid() {
    return mUuid;
  }

  string AssetInstance::getNameAndUuidString() {
    return getName() + " (" + getUuid() + ")";
  }

  Transform3D* AssetInstance::getTransform() {
    return mTransform;
  }

  string AssetInstance::getAbsolutePath() {
    return mAbsolutePath;
  }

  void AssetInstance::setAbsolutePath(string path) {
    mAbsolutePath = path;
  }

  bool AssetInstance::getLoadedFlag() {
    return mLoaded;
  }

  void AssetInstance::setLoadedFlag(bool loaded) {
    mLoaded = loaded;
  }

} // End of Dream
