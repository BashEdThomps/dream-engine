/*
* ShaderInstance
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

#include "ShaderCache.h"
#include "ShaderDefinition.h"
#include "../../../Utilities/FileReader.h"

using glm::value_ptr;

namespace Dream
{

    const GLint ShaderInstance::UNIFORM_NOT_FOUND = -1;

    ShaderInstance::ShaderInstance
    (ShaderCache* cache, ShaderDefinition* definition,SceneObjectRuntime* transform)
        : IAssetInstance(definition,transform),
          ILoggable ("ShaderInstance"),
          mCacheHandle(cache)
    {
        auto log = getLog();
        log->trace( "Constructing Object" );
        mShaderProgram = 0;
    }

    ShaderInstance::~ShaderInstance
    ()
    {
        auto log = getLog();
        log->trace( "Destroying Object" );
    }

    GLuint
    ShaderInstance::getShaderProgram
    ()
    {
        return mShaderProgram;
    }

    bool
    ShaderInstance::setDiffuseColour
    (vec3 diffuse, string name)
    {
        auto log = getLog();
        GLint location = getUniformLocation(name);
        if (location == UNIFORM_NOT_FOUND)
        {
            log->error( "Unable to find diffuse colour uniform '{}' in {}", name,getNameAndUuidString() );
            return false;
        }

        glUniform3fv(location,1,value_ptr(diffuse));
        return true;

    }

    bool
    ShaderInstance::setSpecularColour
    (vec3 specular, string name)
    {
        auto log = getLog();
        GLint location = getUniformLocation(name);
        if (location == UNIFORM_NOT_FOUND)
        {
            log->error( "Unable to find specular colour uniform {} in {}", name, getNameAndUuidString() );
            return false;
        }

        glUniform3fv(location,1,value_ptr(specular));
        return true;
    }

    bool
    ShaderInstance::setPointLight
    (int index, vec3 position, vec3 colour)
    {
        return setPointLightColour(index,colour) &&
                setPointLightPosition(index,position);
    }

    bool
    ShaderInstance::setPointLightColour
    (int index, vec3 colour)
    {
        auto log = getLog();
        stringstream name;
        name << "pointLightColour_" << index;
        GLint location = getUniformLocation(name.str());

        if (location == UNIFORM_NOT_FOUND)
        {
            log->error( "Unable to find point light colour uinform {} in {}" , name.str(), getNameAndUuidString()  );
            return false;
        }

        glUniform3fv(location,1, value_ptr(colour));
        return true;
    }

    bool
    ShaderInstance::setPointLightPosition
    (int index, vec3 position)
    {
        auto log = getLog();
        stringstream name;
        name << "pointLightPos_" << index;
        GLint location = getUniformLocation(name.str());
        if (location == UNIFORM_NOT_FOUND)
        {
            log->error( "Unable to find point light position uinform {} in {}" ,  name.str(), getNameAndUuidString()  );
            return false;
        }

        glUniform3fv(location, 1, value_ptr(position));
        return true;
    }

    bool
    ShaderInstance::setAmbientLight
    (vec3 colour, float strength)
    {
        return setAmbientColour(colour) && setAmbientStrength(strength);
    }

    bool
    ShaderInstance::setAmbientColour
    (vec3 value, string name)
    {
        auto log = getLog();
        GLint location = getUniformLocation(name);

        if (location == UNIFORM_NOT_FOUND)
        {
            log->error( "Unable to find ambient colour uinform {} in {}" ,  name, getNameAndUuidString()  );
            return false;
        }

        glUniform3fv(location,1,value_ptr(value));
        return true;
    }

    bool
    ShaderInstance::setAmbientStrength
    (float value, string name)
    {
        auto log = getLog();

        GLint location = getUniformLocation(name);
        if (location == UNIFORM_NOT_FOUND)
        {
            log->error( "Unable to find ambient strength uniform {} in {}" ,  name, getNameAndUuidString()  );
            return false;
        }

        glUniform1f(location,value);
        return true;
    }

    bool
    ShaderInstance::setModelMatrix
    (mat4 value, string name)
    {
        auto log = getLog();
        GLint location =  getUniformLocation(name);

        if (location == UNIFORM_NOT_FOUND)
        {
            log->error( "Unable to find model matrix uinform {} in {}" , name, getNameAndUuidString()  );
            return false;
        }

        glUniformMatrix4fv(location,1,GL_FALSE,value_ptr(value));

        return true;
    }

    bool
    ShaderInstance::setViewMatrix
    (mat4 value, string name)
    {
        auto log = getLog();
        GLint location = getUniformLocation(name);

        if (location == UNIFORM_NOT_FOUND)
        {
            log->error( "Unable to find view matrix uinform {} in {}" ,  name, getNameAndUuidString()  );
            return false;
        }

        glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(value));

        return true;
    }

    bool
    ShaderInstance::setProjectionMatrix
    (mat4 value, string name)
    {
        auto log = getLog();
        GLint location = getUniformLocation(name);

        if (location == UNIFORM_NOT_FOUND)
        {
            log->error( "Unable to find projection matrix uinform {} in {}" ,  name, getNameAndUuidString()  );
            return false;
        }

        glUniformMatrix4fv(location,1,GL_FALSE,value_ptr(value));

        return true;
    }

    bool
    ShaderInstance::setViewerPosition
    (vec3 value, string name)
    {
        auto log = getLog();
        GLint uCamPos = getUniformLocation(name);

        if (uCamPos == UNIFORM_NOT_FOUND)
        {
            log->error( "Unable to find viewer position uinform {} in {}" ,  name, getNameAndUuidString()  );
            return false;
        }

        glUniform3fv(uCamPos,1,value_ptr(value));

        return true;
    }

    bool
    ShaderInstance::load
    (string projectPath)
    {
        auto log = getLog();
        mShaderProgram = mCacheHandle->getShader(mDefinitionHandle->getUuid());

        if (mShaderProgram == 0)
        {
            string mVertexShaderSource;
            string mFragmentShaderSource;
            GLuint mVertexShader = 0;
            GLuint mFragmentShader = 0;
            // 1. Open Shader Files into Memory
            FileReader *vertexReader, *fragmentReader;
            string absVertexPath, absFragmentPath;
            absVertexPath   = projectPath+mDefinitionHandle->getAssetPath() + Constants::SHADER_VERTEX;
            absFragmentPath = projectPath+mDefinitionHandle->getAssetPath() + Constants::SHADER_FRAGMENT;
            vertexReader = new FileReader(absVertexPath);
            vertexReader->readIntoString();
            mVertexShaderSource = vertexReader->getContentsAsString();
            delete vertexReader;
            fragmentReader = new FileReader(absFragmentPath);
            fragmentReader->readIntoString();
            mFragmentShaderSource = fragmentReader->getContentsAsString();
            delete fragmentReader;
            log->info(
                "Loading Shader {}\n Vertex: {}\n{}\n Fragment: {}\n{}\n",
                mDefinitionHandle->getNameAndUuidString(),
                absVertexPath,
                mVertexShaderSource,
                absFragmentPath,
                mFragmentShaderSource
            );
            // 2. Compile shaders
            GLint success;
            GLchar infoLog[512];
            // Vertex Shader
            mVertexShader = glCreateShader(GL_VERTEX_SHADER);
            const char *vSource = mVertexShaderSource.c_str();
            glShaderSource(mVertexShader, 1, &vSource, nullptr);
            glCompileShader(mVertexShader);
            // Print compile errors if any
            glGetShaderiv(mVertexShader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(mVertexShader, 512, nullptr, infoLog);
                log->error( "SHADER VERTEX COMPILATION FAILED\n {}" ,infoLog );
            }
            // Fragment Shader
            mFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            const char *fSource = mFragmentShaderSource.c_str();
            glShaderSource(mFragmentShader, 1, &fSource, nullptr);
            glCompileShader(mFragmentShader);
            // Print compile errors if any
            glGetShaderiv(mFragmentShader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(mFragmentShader, 512, nullptr, infoLog);
                log->error( "SHADER FRAGMENT COMPILATION FAILED\n {}" , infoLog );
            }
            // Shader Program
            mShaderProgram = glCreateProgram();
            glAttachShader(mShaderProgram, mVertexShader);
            glAttachShader(mShaderProgram, mFragmentShader);
            glLinkProgram(mShaderProgram);
            // Print linking errors if any
            glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(mShaderProgram, 512, nullptr, infoLog);
                log->error( "SHADER PROGRAM LINKING FAILED\n {}" , infoLog );
            }
            // Delete the shaders as they're linked into our program now and no longer necessery
            glDeleteShader(mVertexShader);
            glDeleteShader(mFragmentShader);
            mCacheHandle->putShader(mDefinitionHandle->getUuid(),mShaderProgram);
        }
        mLoaded = (mShaderProgram != 0);
        return mLoaded;
    }

    void
    ShaderInstance::use
    ()
    {
        auto log = spdlog::get("ShaderInstance");
        log->info("Using Shader Program {} for {}",mShaderProgram,getNameAndUuidString());
        glUseProgram(mShaderProgram);
    }

    void
    ShaderInstance::loadExtraAttributes
    (nlohmann::json)
    {
        // pass
        return;
    }

    // API Setters =============================================================

    void
    ShaderInstance::bindVertexArray
    (GLuint vao)
    {
        glBindVertexArray(vao);
    }

    void
    ShaderInstance::unbindVertexArray
    ()
    {
        bindVertexArray(0);
    }

    GLint
    ShaderInstance::getUniformLocation
    (string name)
    {
        return glGetUniformLocation(mShaderProgram,name.c_str());
    }

    void ShaderInstance::addUniform(ShaderUniform uniform)
    {
        auto inVector = find(begin(mUniformVector),end(mUniformVector),uniform);
        if (inVector != end(mUniformVector))
        {
            mUniformVector.erase(inVector);
        }
        mUniformVector.push_back(uniform);
    }

    void
    ShaderInstance::unbind
    ()
    {
        glUseProgram(0);
    }

    // GL Syncros ==============================================================
    void
    ShaderInstance::syncUniforms
    ()
    {
        auto log = getLog();
        log->info("Synchronising uniforms for {}",getNameAndUuidString());
        GLuint prog = getShaderProgram();

        for (ShaderUniform uniform : mUniformVector)
        {
            GLint location = getUniformLocation(uniform.getName());
            log->info(
                " Sync Uinform {} -> prog: {}, name: {}, loc: {}, count: {}",
                getUuid(),
                prog,
                uniform.getName(),
                location,
                uniform.getCount()
            );

            if (location == UNIFORM_NOT_FOUND)
            {
                log->error( "Unable to find uniform location '{}' in {}" , uniform.getName() ,getNameAndUuidString());
                continue;
            }

            switch (uniform.getType())
            {
                // Int
                case INT1:
                    if (uniform.getCount() > 0)
                    {
                        glUniform1iv(location,uniform.getCount(),static_cast<GLint*>(uniform.getData()));
                    }
                    break;
                case INT2:
                    if (uniform.getCount() > 0)
                    {
                        glUniform2iv(location,uniform.getCount(),static_cast<GLint*>(uniform.getData()));
                    }
                    break;
                case INT3:
                    if (uniform.getCount() > 0)
                    {
                        glUniform3iv(location,uniform.getCount(),static_cast<GLint*>(uniform.getData()));
                    }
                    break;
                case INT4:
                    if (uniform.getCount() > 0)
                    {
                        glUniform2iv(location,uniform.getCount(),static_cast<GLint*>(uniform.getData()));
                    }
                    break;

                    // Uint
                case UINT1:
                    if (uniform.getCount() > 0)
                    {
                        glUniform1uiv(location,uniform.getCount(),static_cast<GLuint*>(uniform.getData()));
                    }
                    break;
                case UINT2:
                    if (uniform.getCount() > 0)
                    {
                        glUniform2uiv(location,uniform.getCount(),static_cast<GLuint*>(uniform.getData()));
                    }
                    break;
                case UINT3:
                    if (uniform.getCount() > 0)
                    {
                        glUniform3uiv(location,uniform.getCount(),static_cast<GLuint*>(uniform.getData()));
                    }
                    break;
                case UINT4:
                    if (uniform.getCount() > 0)
                    {
                        glUniform4uiv(location,uniform.getCount(),static_cast<GLuint*>(uniform.getData()));
                    }
                    break;

                    // Float
                case FLOAT1:
                    if (uniform.getCount() > 0)
                    {
                        glUniform1fv(location,uniform.getCount(),static_cast<GLfloat*>(uniform.getData()));
                    }
                    break;
                case FLOAT2:
                    if (uniform.getCount() > 0)
                    {
                        glUniform2fv(location,uniform.getCount(),static_cast<GLfloat*>(uniform.getData()));
                    }
                    break;
                case FLOAT3:
                    if (uniform.getCount() > 0)
                    {
                        glUniform3fv(location,uniform.getCount(),static_cast<GLfloat*>(uniform.getData()));
                    }
                    break;
                case FLOAT4:
                    if (uniform.getCount() > 0)
                    {
                        glUniform4fv(location,uniform.getCount(),static_cast<GLfloat*>(uniform.getData()));
                    }
                    break;

                    // Double
                case DOUBLE1:
                    if (uniform.getCount() > 0)
                    {
                        glUniform1dv(location,uniform.getCount(),static_cast<GLdouble*>(uniform.getData()));
                    }
                    break;
                case DOUBLE2:
                    if (uniform.getCount() > 0)
                    {
                        glUniform2dv(location,uniform.getCount(),static_cast<GLdouble*>(uniform.getData()));
                    }
                    break;
                case DOUBLE3:
                    if (uniform.getCount() > 0)
                    {
                        glUniform3dv(location,uniform.getCount(),static_cast<GLdouble*>(uniform.getData()));
                    }
                    break;
                case DOUBLE4:
                    if (uniform.getCount() > 0)
                    {
                        glUniform4dv(location,uniform.getCount(),static_cast<GLdouble*>(uniform.getData()));
                    }
                    break;
            }
        }
    }



    ShaderUniform::ShaderUniform(UniformType type, string name, int count, void* data)
        : ILoggable ("ShaderUniform"),
          mType(type),
          mName(name),
          mData(data),
          mCount(count)

    {
        auto log = getLog();
        log->info("Constructing uniform {}, count {}",mName,count);
    }

    ShaderUniform::~ShaderUniform()
    {

    }

    bool ShaderUniform::operator==(const ShaderUniform& other) const
    {
       return getName().compare(other.getName()) == 0;
    }

    int ShaderUniform::getCount() const
    {
        return mCount;
    }

    void ShaderUniform::setCount(int count)
    {
        mCount = count;
    }

    void* ShaderUniform::getData() const
    {
        return mData;
    }

    void ShaderUniform::setData(void* data)
    {
        mData = data;
    }

    string ShaderUniform::getName() const
    {
        return mName;
    }

    void ShaderUniform::setName(const string& name)
    {
        mName = name;
    }

    UniformType ShaderUniform::getType() const
    {
        return mType;
    }

    void ShaderUniform::setType(const UniformType& type)
    {
        mType = type;
    }

} // End of Dream
