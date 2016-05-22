/*
* Dream::Asset::Instance::Shader::ShaderInstance
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

#ifndef SHADERINSTANCE_H
#define SHADERINSTANCE_H

#include <iostream>
#include "../../AssetInstance.h"

#define SHADER_VERTEX   "vertex"
#define SHADER_FRAGMENT "fragment"

namespace Dream      {
namespace Asset      {
namespace Instances  {
namespace Shader     {
    
    class ShaderInstance : public Dream::Asset::AssetInstance {
		private:
			std::string mVertexShaderSource;
			std::string mFragmentShaderSource;
    public:
			ShaderInstance(Dream::Asset::AssetDefinition*);
			~ShaderInstance(void);
			bool load(std::string);
		
    }; // End of ShaderInstance
    
} // End of Shader
} // End of Instance
} // End of Asset
} // End of Dream

#endif // End of SHADERINSTANCE_H
