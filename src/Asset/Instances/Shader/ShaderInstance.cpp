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
			mIsVertexShaderCompiled    = 0;
			mVertexShaderCompilerLog   = NULL;
			mFragmentShader            = 0;
			mIsFragmentShaderCompiled  = 0;
			mFragmentShaderCompilerLog = NULL;
			mIsShaderLinked            = 0;
			mShaderLinkerLog           = NULL;
	}

	ShaderInstance::~ShaderInstance(void) {
		if (mVertexShaderCompilerLog){
			free(mVertexShaderCompilerLog);
		}
		
		if (mFragmentShaderCompilerLog) {
			free (mFragmentShaderCompilerLog);
		}
		
		if (mShaderLinkerLog) {
			free (mShaderLinkerLog);
		}
		
		if (mShaderProgram && mVertexShader) {
			glDetachShader(mShaderProgram, mVertexShader);
		}
		
		if (mShaderProgram && mFragmentShader) {
			glDetachShader(mShaderProgram, mFragmentShader);
		}
		
		if (mShaderProgram) {
			glDeleteProgram(mShaderProgram);
		}
		
		if (mVertexShader) {
			glDeleteShader(mVertexShader);
		}
		
		if (mFragmentShader) {
			glDeleteShader(mFragmentShader);
		}
	}
	
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
		
		std::cout << "ShaderInstance: Loaded Shader..." << std::endl
		          << "\t  Vertex: " << absVertexPath     << std::endl
		          << std::endl << mVertexShaderSource    << std::endl
		          << "\tFragment: " << absFragmentPath   << std::endl
		          << std::endl << mFragmentShaderSource  << std::endl;
		
		return mVertexShaderSource.size() > 0 && mFragmentShaderSource.size() > 0;
	}
	
	bool ShaderInstance::compileShaderProgram() {
		if (!compileVertexShader()){
			std::cerr << "ShaderInstance: Error compiling vertex shader" << std::endl
			          << mVertexShaderCompilerLog << std::endl;
			return false;
		}
		
		if (!compileFragmentShader()){
			std::cerr << "ShaderInstance: Error compiling fragment shader" << std::endl
			          << mFragmentShaderCompilerLog << std::endl;
			return false;
		}
		
		/* If we reached this point it means the vertex and fragment shaders compiled and are syntax error free. */
		/* We must link them together to make a GL shader program */
		/* GL shader programs are monolithic. It is a single piece made of 1 vertex shader and 1 fragment shader. */
		/* Assign our program handle a "name" */
		mShaderProgram = glCreateProgram();
		glAttachShader(mShaderProgram, mVertexShader);
		glAttachShader(mShaderProgram, mFragmentShader);
	
		return true;
	}
	
	bool ShaderInstance::compileVertexShader() {
		size_t maxLogLength;
		mVertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(mVertexShader, 1, (const GLchar**)mVertexShaderSource.c_str(), 0);
		glCompileShader(mVertexShader);
		
		glGetShaderiv(mVertexShader, GL_COMPILE_STATUS, &mIsVertexShaderCompiled);
		if(mIsVertexShaderCompiled == FALSE) {
			glGetShaderiv(mVertexShader, GL_INFO_LOG_LENGTH, &maxLogLength);
			mVertexShaderCompilerLog = (char *)malloc(maxLogLength);
			glGetShaderInfoLog(mVertexShader, maxLogLength, &maxLogLength, mVertexShaderCompilerLog);
			return false;
		}
		return true;
	}
	
	bool ShaderInstance::compileFragmentShader() {
		size_t maxLogLength;
		mFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(mFragmentShader, 1, (const GLchar**)mFragmentShaderSource.c_str(), 0);
		glCompileShader(mFragmentShader);
		
		glGetShaderiv(mFragmentShader, GL_COMPILE_STATUS, &mIsFragmentShaderCompiled);
		if(mIsFragmentShaderCompiled == FALSE) {
			glGetShaderiv(mFragmentShader, GL_INFO_LOG_LENGTH, &maxLogLength);
			mFragmentShaderCompilerLog = (char *)malloc(maxLogLength);
			glGetShaderInfoLog(mFragmentShader, maxLogLength, &maxLogLength, mFragmentShaderCompilerLog);
			return false;
		}
		return true;
	}
	
	GLuint ShaderInstance::getShaderProgram() {
		return mShaderProgram;
	}
	
	bool ShaderInstance::linkShader() {
		size_t maxLogLength;
		/* Link our program */
		/* At this stage, the vertex and fragment programs are inspected, optimized and a binary code is generated for the shader. */
		/* The binary code is uploaded to the GPU, if there is no error. */
		glLinkProgram(mShaderProgram);
		
		/* Again, we must check and make sure that it linked. If it fails, it would mean either there is a mismatch between the vertex */
		/* and fragment shaders. It might be that you have surpassed your GPU's abilities. Perhaps too many ALU operations or */
		/* too many texel fetch instructions or too many interpolators or dynamic loops. */
		glGetProgramiv(mShaderProgram, GL_LINK_STATUS, (int *)&mIsShaderLinked);
		if(mIsShaderLinked == FALSE) {
			/* Noticed that glGetProgramiv is used to get the length for a shader program, not glGetShaderiv. */
			glGetProgramiv(mShaderProgram, GL_INFO_LOG_LENGTH, &maxLogLength);
			
			/* The maxLength includes the NULL character */
			mShaderLinkerLog = (char *)malloc(maxLogLength);
			
			/* Notice that glGetProgramInfoLog, not glGetShaderInfoLog. */
			glGetProgramInfoLog(mShaderProgram, maxLogLength, &maxLogLength, mShaderLinkerLog);
			return false;
		}
		return true;
	}
	
	void ShaderInstance::useShader() {
		glUseProgram(mShaderProgram);
	}
	
	void ShaderInstance::bindAttributeLocation(GLuint location, GLchar* variable) {
		glBindAttribLocation(mShaderProgram,location,variable);
	}
	
} // End of Shader
} // End of Instance
} // End of Asset
} // End of Dream
