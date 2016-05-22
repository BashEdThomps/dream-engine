/*
* Dream::Plugins::Shader::ShaderInstance
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

#include "ShaderInstance.h"

namespace Dream      {
namespace Asset      {
namespace Instances  {
namespace Shader     {
    
	ShaderInstance::ShaderInstance(Dream::Asset::AssetDefinition* definition) :
		Dream::Asset::AssetInstance(definition) {}

	ShaderInstance::~ShaderInstance(void) {}
	
	bool ShaderInstance::load(std::string projectPath) {
		Dream::Util::FileReader *vertexReader, *fragmentReader;
		
		std::string absVertexPath, absFragmentPath;
		absVertexPath   = projectPath+mDefinition->getAssetPath() + SHADER_VERTEX;
		absFragmentPath = projectPath+mDefinition->getAssetPath() + SHADER_FRAGMENT;
		
		vertexReader = new Dream::Util::FileReader(absVertexPath);
		vertexReader->readIntoStringStream();
		mVertexShaderSource = vertexReader->getContentsAsString();
		delete vertexReader;
		
		fragmentReader = new Dream::Util::FileReader(absFragmentPath);
		fragmentReader->readIntoStringStream();
		mFragmentShaderSource = fragmentReader->getContentsAsString();
		delete fragmentReader;
		
		std::cout << "ShaderInstance: Loading Shader..." << std::endl
		          << "\t  Vertex: " << absVertexPath     << std::endl
		          << std::endl << mVertexShaderSource    << std::endl
		          << "\tFragment: " << absFragmentPath   << std::endl
		          << std::endl << mFragmentShaderSource  << std::endl;
		
		return mVertexShaderSource.size() > 0 && mFragmentShaderSource.size() > 0;
	}
	
} // End of Shader
} // End of Instance
} // End of Asset
} // End of Dream
