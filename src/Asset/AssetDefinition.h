/*
* Dream::Asset::AssetDefinition
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

#include "../JSON/json.hpp"

#define ASSET_UUID   "uuid"
#define ASSET_NAME   "name"
#define ASSET_TYPE   "type"
#define ASSET_FORMAT "format"

#define DIR_PATH_SEP        "/"
#define ASSET_DIR        "asset"
#define ASSET_DIR_MODEL  "model"
#define ASSET_DIR_AUDIO  "audio"
#define ASSET_DIR_SCRIPT "script"

#define ASSET_TYPE_MODEL     "Model"
#define ASSET_TYPE_AUDIO     "Audio"
#define ASSET_TYPE_SCRIPT    "Script"
#define ASSET_TYPE_ANIMATION "Animation"


#define ASSET_DEF_FORMAT_SCRIPT_JAVASCRIPT "js"
#define ASSET_DEF_FORMAT_SCRIPT_CHAISCRIPT "chai"
#define ASSET_DEF_FORMAT_MODEL_WAVEFRONT   "obj"
#define ASSET_DEF_FORMAT_AUDIO_WAV         "wav"
#define ASSET_DEF_FORMAT_AUDIO_OGG         "ogg"
#define ASSET_DEF_FORMAT_ANIMATION_DREAM   "dream_anim"

namespace Dream {
namespace Asset {
	
	class AssetDefinition {
		std::map<std::string,std::string> mAttributes;
	public:
		AssetDefinition(void);
		AssetDefinition(nlohmann::json);
		~AssetDefinition(void);
		std::pair<std::string,std::string> mapPair(std::string,std::string);
		void setUUID(std::string);
		std::string getUUID();
		void setName(std::string);
		std::string getName();
		void setType(std::string);
		std::string getType();
		void setFormat(std::string);
		std::string getFormat();
		
		std::string getAttribute(std::string);
		void addAttribute(std::string,std::string);
		
		bool isTypeAnimation();
		bool isTypeAudio();
		bool isTypeModel();
		bool isTypeScript();
		
		bool isScriptFormatJavaScript();
		bool isScriptFormatChaiScript();
		
		bool isModelFormatWaveFront();
		
		bool isAudioFormatWav();
		bool isAudioFormatOgg();
		
		bool isAnimationFormatDream();
		
	}; // End of AssetDefinition
	
} // End of Asset
} // End of Dream

#endif // End of ASSETDEFINITION_H
