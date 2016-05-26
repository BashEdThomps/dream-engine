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
		Dream::Asset::AssetInstance(definition) {
			mShaderProgram             = 0;
			mVertexShader              = 0;
			mFragmentShader            = 0;
	}

	ShaderInstance::~ShaderInstance(void) {
		
	}
	
	GLuint ShaderInstance::getShaderProgram() {
		return mShaderProgram;
	}
	
	bool ShaderInstance::load(std::string projectPath) {
		
		// 1. Open Shader Files into Memory
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
		
		// 2. Compile shaders
		GLint success;
		GLchar infoLog[512];
		
		// Vertex Shader
		mVertexShader = glCreateShader(GL_VERTEX_SHADER);
		const char *vSource = mVertexShaderSource.c_str();
		glShaderSource(mVertexShader, 1, &vSource, NULL);
		glCompileShader(mVertexShader);
		
		// Print compile errors if any
		glGetShaderiv(mVertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(mVertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		
		// Fragment Shader
		mFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		const char *fSource = mFragmentShaderSource.c_str();
		glShaderSource(mFragmentShader, 1, &fSource, NULL);
		glCompileShader(mFragmentShader);
		
		// Print compile errors if any
		glGetShaderiv(mFragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(mFragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		
		// Shader Program
		mShaderProgram = glCreateProgram();
		glAttachShader(mShaderProgram, mVertexShader);
		glAttachShader(mShaderProgram, mFragmentShader);
		glLinkProgram(mShaderProgram);
		
		// Print linking errors if any
		glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(mShaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		
		// Delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(mVertexShader);
		glDeleteShader(mFragmentShader);
		
		return mVertexShader != 0 && mFragmentShader != 0;
	}
	
	void ShaderInstance::use() {
		glUseProgram(mShaderProgram);
	}
	
} // End of Shader
} // End of Instance
} // End of Asset
} // End of Dream
