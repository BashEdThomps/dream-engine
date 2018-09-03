/*
* Dream::AssetDefinition
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
#include <map>
#include <string>
#include <vector>

#pragma once

#ifdef __APPLE__
    #include <nlohmann/json.hpp>
#else
    #include <json.hpp>
#endif

#include "../Common/IDefinition.h"
#include "../Common/ILoggable.h"
#include "../Common/Constants.h"
#include "Transform3D.h"

using std::string;
using std::vector;
using nlohmann::json;

namespace Dream
{
    class ProjectDefinition;

    class IAssetDefinition : public IDefinition, ILoggable
    {

    public:
        IAssetDefinition(shared_ptr<ProjectDefinition>, json);
        static AssetType getAssetType(json);

        virtual ~IAssetDefinition();

        shared_ptr<ProjectDefinition> getProject();
        void showStatus();

        void setType(string);
        string getType();

        void setFormat(string);
        string getFormat();

        string getAssetTypeDirectory();

        bool isTypePath();
        bool isTypeAudio();
        bool isTypeModel();
        bool isTypeScript();
        bool isTypeShader();
        bool isTypePhysicsObject();
        bool isTypeLight();
        bool isTypeSprite();
        bool isTypeFont();

        string getAssetPath();

        void setProjectPath(string);
        string getProjectPath();

    protected:
        shared_ptr<ProjectDefinition> mProjectDefinition;

    }; // End of AssetDefinition

} // End of Dream
