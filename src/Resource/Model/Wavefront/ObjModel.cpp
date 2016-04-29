/*
* Dream::Resource::Model::Wavefront::ObjModel
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

#include "ObjModel.h"

namespace Dream {
	namespace Resource {
		namespace Model {
			namespace Wavefront {
				ObjModel::ObjModel(void) : Dream::Resource::Model::Model() {}
				ObjModel::ObjModel(nlohmann::json json) : Dream::Resource::Model::Model(json) {}
				ObjModel::~ObjModel(void) {}
				void ObjModel::generateAbsolutePaths(std::string projectDir,std::string projectUUID){
					mObjPath = "";
					mMtlPath = "";
				}
			} // End of Wavefront
		} // End of Model
	} // End of Resource
} // End of DreamFileFormats
