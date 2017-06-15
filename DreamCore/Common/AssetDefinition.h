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

#ifndef ASSETDEFINITION_H
#define ASSETDEFINITION_H


#include <iostream>
#include <map>
#include <string>
#include <json.hpp>
#include "Constants.h"

using namespace std;
using namespace nlohmann;

namespace Dream
{
    class AssetDefinition
    {
    public:
        static AssetDefinition* getAssetDefinitionByUuid(vector<AssetDefinition*>,string);

    private:
        json mJson;
        string mProjectPath;
    public:
        AssetDefinition(nlohmann::json);
        ~AssetDefinition(void);

        void setUuid(string);
        string getUuid();
        bool hasUuid(string);
        void setName(string);
        string getName();
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

        json& getJson();

    }; // End of AssetDefinition

} // End of Dream

#endif // End of ASSETDEFINITION_H
