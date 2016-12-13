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

#ifndef ASSETINSTANCE_H
#define ASSETINSTANCE_H

#include <iostream>

#include "FileReader.h"
#include "AssetDefinition.h"
#include "Transform3D.h"

#define ASSET_UUID "uuid"
#define ASSET_NAME "name"

namespace Dream {

  class AssetInstance {
  protected:
    bool mLoaded;
    string mAbsolutePath;
    AssetDefinition *mDefinition;
    Transform3D *mTransform;
  public:
    AssetInstance(AssetDefinition*, Transform3D*);
    virtual ~AssetInstance();

    string getUUID();
    string getName();

    string getNameAndUUIDString();

    virtual bool load(string) = 0;
    virtual void loadExtraAttributes(nlohmann::json) = 0;

    Transform3D* getTransform();

    string getAbsolutePath();
    void setAbsolutePath(string);

    bool isLoaded();
    void setLoaded(bool);

  }; // End of AssetInstance
} // End of Dream

#endif // End of ASSETINSTANCE_H
