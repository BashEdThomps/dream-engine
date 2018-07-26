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
        log->info( "ShaderInstance: Constructing Object" );
        mShaderProgram = 0;
    }

    ShaderInstance::~ShaderInstance
    ()
    {
        auto log = getLog();
        log->info( "ShaderInstance: Destroying Object" );
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
            log->error( "ShaderInstance: Unable to find uniform for {}", name );
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
            log->error( "ShaderInstance: Unable to find uniform for ", name );
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
            log->error( "ShaderInstance: Unable to find uinform " , name.str() );
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
            log->error( "ShaderInstance: Unable to find uinform " ,  name.str() );
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
            log->error( "ShaderInstance: Unable to find uinform " ,  name );
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
            log->error( "ShaderInstance: Unable to find uinform " ,  name );
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
            log->error( "ShaderInstance: Unable to find uinform {}" , name );
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
            log->error( "ShaderInstance: Unable to find uinform {}" ,  name );
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
            log->error( "ShaderInstance: Unable to find uinform {}" ,  name );
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
            log->error( "ShaderInstance: Unable to find uinform {}" ,  name );
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
               "ShaderInstance: Loading Shader {}\n",
               "ShaderInstance: Vertex: {}\n{}\n",
               "ShaderInstance: Fragment: {}\n{}\n",
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
                log->error( "ShaderInstance: SHADER VERTEX COMPILATION FAILED\n {}" ,infoLog );
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
                log->error( "ShaderInstance: SHADER FRAGMENT COMPILATION FAILED\n {}" , infoLog );
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
                log->error( "ShaderInstance: SHADER PROGRAM LINKING FAILED\n {}" , infoLog );
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
        glUseProgram(mShaderProgram);
        syncUniforms();
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
    ShaderInstance::syncUniforms
    ()
    {
        syncUniform1f();
    }

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

    void
    ShaderInstance::setUniform1f
    (string location, GLfloat value)
    {
        for (pair<string,GLfloat> obj : mUniform1fMap)
        {
            if (obj.first == location)
            {
                obj.second = value;
                return;
            }
        }
        mUniform1fMap.insert(pair<string,GLfloat>(location,value));
    }

    void
    ShaderInstance::unbind
    ()
    {
        glUseProgram(0);
    }

    // GL Syncros ==============================================================

    // 1f
    void
    ShaderInstance::syncUniform1f
    ()
    {
        auto log = getLog();
        GLuint prog = getShaderProgram();

        for (pair<string,GLfloat> it : mUniform1fMap)
        {
            string name = it.first;
            GLfloat val = it.second;
            GLint location = getUniformLocation(name);
            log->info(
                "ShaderInstance: {}\n"
                " Sync Uinform1f -> \n"
                " prog: {}\n"
                " name: {}'n"
                " loc: {}\n"
                " val: {}\n",
                getUuid(),prog,name,location,val
            );

            if (location == UNIFORM_NOT_FOUND)
            {
                log->error( "ShaderProgram: unable to find uniform " , name );
                continue;
            }

            glUniform1f(location,val);
        }
    }

} // End of Dream
