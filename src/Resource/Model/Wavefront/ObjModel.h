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

#include "../Model.h"

#define RESOURCE_FORMAT_WAVEFRONT  "obj"

namespace Dream {
	namespace Resource {
		namespace Model {
			namespace Wavefront {
				class ObjModel : public Dream::Resource::Model::Model {
				private:
					std::string mObjPath;
					std::string mMtlPath;
				public:
					ObjModel(void);
					ObjModel(nlohmann::json);
					~ObjModel(void);
					void generateAbsolutePaths(std::string,std::string);
				};// End of ObjModel
			} // End of Wavefront
		} // End of Model
	} // End of Obj
} // End of DreamFileFormats

#endif // End of OBJMODEL_H
