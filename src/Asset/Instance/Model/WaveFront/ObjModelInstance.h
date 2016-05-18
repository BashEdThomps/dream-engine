/*
* Model
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

#ifndef OBJMODEL_H
#define OBJMODEL_H

#include "../../../Instance/AssetInstance.h"
#include "tiny_obj_loader.h"

#define ASSET_FORMAT_WAVEFRONT  "obj"
#define ASSET_FORMAT_MTL        "mtl"
#define ASSET_FORMAT_OBJ        "obj"

namespace Dream     {
namespace Asset     {
namespace Instance  {
namespace Model     {
namespace WaveFront {
	
	class ObjModelInstance : public Dream::Asset::Instance::AssetInstance {
	private:
		std::string mObjPath;
		std::string mMtlPath;
	public:
		ObjModelInstance(Dream::Asset::AssetDefinition*);
		~ObjModelInstance(void);
		
		void generateAbsolutePaths(std::string,std::string);
	}; // End of ObjModel
	
} // End of WaveFront
} // End of Model
} // End of Instance
} // End of Asset
} // End of Dream

#endif // End of OBJMODEL_H
