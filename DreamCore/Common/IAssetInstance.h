/*
* Dream::IAssetInstance
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

#pragma once

#include <iostream>

#include "AssetDefinition.h"
#include "Constants.h"
#include "Transform3D.h"
#include "../Utilities/FileReader.h"

namespace Dream
{
    class IAssetInstance
    {
    protected:
        bool mLoaded;
        string mAbsolutePath;
        AssetDefinition *mDefinitionHandle;
        Transform3D *mTransformHandle;
        string mUuid;
    public:
        IAssetInstance(AssetDefinition*, Transform3D*);
        virtual ~IAssetInstance();

        string getUuid();
        string getName();

        string getNameAndUuidString();

        virtual bool load(string) = 0;
        virtual void loadExtraAttributes(nlohmann::json) = 0;

        Transform3D* getTransform();

        string getAbsolutePath();
        void setAbsolutePath(string);

        bool getLoadedFlag();
        void setLoadedFlag(bool);

    }; // End of IAssetInstance

} // End of Dream
