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

#include <string>

#pragma once

#ifdef __APPLE__
    #include <nlohmann/json.hpp>
#else
    #include <json.hpp>
#endif

#include "../Common/Constants.h"
#include "../Common/DreamObject.h"
#include <memory>

using std::string;
using nlohmann::json;

namespace Dream
{
    class IAssetDefinition;
    class SceneObjectRuntime;

    class IAssetInstance : public DreamObject
    {
    protected:
        bool mLoaded;
        string mAbsolutePath;
        IAssetDefinition* mDefinition;
        SceneObjectRuntime* mSceneObjectRuntime;
    public:
        IAssetInstance
        (
            IAssetDefinition* asset,
            SceneObjectRuntime* runtime
        );
        virtual ~IAssetInstance();

        string getUuid();
        string getName();

        string getNameAndUuidString();

        virtual bool load(string) = 0;
        virtual void loadExtraAttributes(json) = 0;

        string getAbsolutePath();
        void setAbsolutePath(string);

        bool getLoadedFlag();
        void setLoadedFlag(bool);

        SceneObjectRuntime* getSceneObjectRuntime();

    }; // End of IAssetInstance

} // End of Dream
