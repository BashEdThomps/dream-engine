/*
* Dream::Asset::Model::Wavefront::ObjModel
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

#include "ObjModelInstance.h"

#define TINYOBJLOADER_IMPLEMENTATION

namespace Dream      {
namespace Asset      {
namespace Instances  {
namespace Model      {
namespace WaveFront  {

	ObjModelInstance::ObjModelInstance(Dream::Asset::AssetDefinition* definition) :
		Dream::Asset::AssetInstance(definition) {
	}

	ObjModelInstance::~ObjModelInstance(void) {}
	
	bool ObjModelInstance::load(Dream::Util::FileReader *file) {
		std::string path = file->getPath();
		std::cout << "ObjModelInstance: Loading Model from " << path << std::endl;
		
		if(!tinyobj::LoadObj(mShapes, mMaterials, mError, path.c_str())) {
			std::cerr << mError << std::endl;
			return false;
		}
		
		std::cout << "ObjModelInstance: Loaded Instance"      << std::endl;
    	std::cout << "# of shapes    : " << mShapes.size()    << std::endl;
    	std::cout << "# of materials : " << mMaterials.size() << std::endl;
	
		return true;
	}
    
} // End of WaveFront
} // End of Model
} // End of Instance
} // End of Asset
} // End of DreamFileFormats
