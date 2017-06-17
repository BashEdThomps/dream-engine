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
#include <json.hpp>

#include "../Common/IDefinition.h"

using std::string;
using std::vector;
using nlohmann::json;

namespace Dream
{
    class Project;

    class AssetDefinition : public IDefinition
    {
    private:
        string mProjectPath;
        Project *mProjectHandle;

    public:
        AssetDefinition(Project*, json);
        ~AssetDefinition();

        void setType(string);
        string getType();

        void setFormat(string);
        string getFormat();

        string getNameAndUuidString();

        string getAssetTypeDirectory();

        bool isTypeAnimation();
        bool isTypeAudio();
        bool isTypeModel();
        bool isTypeScript();
        bool isTypeShader();
        bool isTypePhysicsObject();
        bool isTypeLight();
        bool isTypeSprite();
        bool isTypeFont();

        bool isScriptFormatLua();
        bool isModelFormatAssimp();

        bool isAudioFormatWav();
        bool isAudioFormatOgg();

        string getAssetPath();

        void setProjectPath(string);
        string getProjectPath();

        void showStatus();

        Project *getProjectHandle();

    }; // End of AssetDefinition

} // End of Dream
