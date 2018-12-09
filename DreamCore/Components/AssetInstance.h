/*
* Dream::AssetInstance
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

#include "../Common/DreamObject.h"
#include <string>

using namespace std;

namespace Dream
{
    class AssetDefinition;

    class AssetInstance : public DreamObject
    {
    protected:
        bool mLoaded;
        AssetDefinition* mDefinition;
    public:
        AssetInstance
        (
            AssetDefinition* asset
        );
        virtual ~AssetInstance();

        string getUuid();
        string getName();

        string getNameAndUuidString();
        virtual string getAssetFilePath(string fmt = "") = 0;
        virtual string getAssetDirectoryPath() = 0;

        virtual bool load() = 0;

        bool getLoadedFlag();
        void setLoadedFlag(bool);
    };
}
