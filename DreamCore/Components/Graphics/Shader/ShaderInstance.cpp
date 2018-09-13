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
#include "../Model/AssimpMaterial.h"
#include "../Light/LightInstance.h"

using namespace glm;

namespace Dream
{
    const GLint ShaderInstance::UNIFORM_NOT_FOUND = -1;

    ShaderInstance::ShaderInstance
    (shared_ptr<ShaderCache> cache,
     shared_ptr<ShaderDefinition> definition,
     shared_ptr<SceneObjectRuntime> transform)
        : IAssetInstance(definition,transform),
          mPointLightCount(0),
          mPointLightCountLocation(UNIFORM_NOT_FOUND),
          mSpotLightCount(0),
          mSpotLightCountLocation(UNIFORM_NOT_FOUND),
          mDirectionalLightCount(0),
          mDirectionalLightCountLocation(UNIFORM_NOT_FOUND),
          mCache(cache)
    {

        setLogClassName("ShaderInstance");
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
        if (mCache == nullptr)
        {
            return false;
        }
        mShaderProgram = mCache->getShader(mDefinition->getUuid());

        if (mShaderProgram == 0)
        {
            string mVertexShaderSource;
            string mFragmentShaderSource;
            GLuint mVertexShader = 0;
            GLuint mFragmentShader = 0;
            // 1. Open Shader Files into Memory
            FileReader *vertexReader, *fragmentReader;
            string absVertexPath, absFragmentPath;
            absVertexPath   = projectPath+mDefinition->getAssetPath() + Constants::SHADER_VERTEX;
            absFragmentPath = projectPath+mDefinition->getAssetPath() + Constants::SHADER_FRAGMENT;
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
                        mDefinition->getNameAndUuidString(),
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
            mCache->putShader(mDefinition->getUuid(),mShaderProgram);
        }

        mLoaded = (mShaderProgram != 0);

        if (mLoaded)
        {
            mPointLightCountLocation       =  glGetUniformLocation(mShaderProgram, UNIFORM_POINT_LIGHT_COUNT);
            mSpotLightCountLocation        =  glGetUniformLocation(mShaderProgram, UNIFORM_SPOT_LIGHT_COUNT);
            mDirectionalLightCountLocation =  glGetUniformLocation(mShaderProgram, UNIFORM_DIRECTIONAL_LIGHT_COUNT);
        }

        return mLoaded;
    }

    void
    ShaderInstance::use
    ()
    {
        mPointLightCount = 0;
        mSpotLightCount = 0;
        mDirectionalLightCount = 0;
        GLint currentShader = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM,&currentShader);
        if (static_cast<GLuint>(currentShader) != mShaderProgram)
        {
            auto log = spdlog::get("ShaderInstance");
            log->debug("Switching Shader Program from {} to {} for {}",
                          currentShader,mShaderProgram,getNameAndUuidString());
            glUseProgram(mShaderProgram);
        }
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

    void ShaderInstance::addUniform(UniformType type, string name, int count, void* data)
    {
        auto log = getLog();
        for (auto uniform : mUniformVector)
        {
            if (uniform->getName().compare(name) == 0)
            {
                log->trace("Updating uniform {}", uniform->getName());
                uniform->setData(data);
                return;
            }
        }
        log->info("Creating uniform {}", name);
        auto newUniform = make_shared<ShaderUniform>(type,name,count,data);
        newUniform->setLocation(glGetUniformLocation(mShaderProgram, name.c_str()));
        mUniformVector.push_back(newUniform);
    }

    void ShaderInstance::bindMaterial(shared_ptr<AssimpMaterial> material)
    {
        checkGLError();
        auto log = getLog();
        GLuint id;
        if (material == nullptr)
        {
            return;
        }
        if (material->mDiffuseTexture != nullptr)
        {
            id = material->mDiffuseTexture->id;
            log->info("Found Diffuse Texture, binding {}",id);
            glActiveTexture(GL_TEXTURE0);
            checkGLError();
            glBindTexture(GL_TEXTURE_2D, id);
            checkGLError();
            GLuint diffuseIndex = 0;
            addUniform(INT1, "material.diffuse", 1, &diffuseIndex);
        }

        if (material->mSpecularTexture != nullptr)
        {
            id =  material->mSpecularTexture->id;
            log->info("Found Specular Texture, binding {}",id);
            glActiveTexture(GL_TEXTURE1);
            checkGLError();
            glBindTexture(GL_TEXTURE_2D, id);
            checkGLError();
            GLuint specularIndex = 1;
            addUniform(INT1, "material.specular", 1, &specularIndex);
        }

        if (material->mNormalTexture != nullptr)
        {
            id =  material->mNormalTexture->id;
            log->info("Found Normal Texture, binding {}",id);
            glActiveTexture(GL_TEXTURE2);
            checkGLError();
            glBindTexture(GL_TEXTURE_2D, id);
            checkGLError();
            GLuint normalIndex = 2;
            addUniform(INT1, "material.normalMap", 1, &normalIndex);
        }

        addUniform(FLOAT1, "material.shininess", 1, &material->mShininessStrength);

    }

    void ShaderInstance::bindLight(shared_ptr<LightInstance> light)
    {
        auto log = getLog();
        log->info("Binding light {} ({})",light->getNameAndUuidString(),light->getType());
        DirLight dirData;
        SpotLight spotData;
        PointLight pointData;

        /*

        DirLight;
            vec3 direction;
            vec3 ambient;
            vec3 diffuse;
            vec3 specular;

         PointLight;
            vec3 position;
            float constant;
            float linear;
            float quadratic;
            vec3 ambient;
            vec3 diffuse;
            vec3 specular;

        SpotLight;
            vec3 position;
            vec3 direction;
            float cutOff;
            float outerCutOff;
            float constant;
            float linear;
            float quadratic;
            vec3 ambient;
            vec3 diffuse;
            vec3 specular;
        */


        switch (light->getType())
        {
            case LT_DIRECTIONAL:
                if (mDirectionalLightCount == MAX_LIGHTS)
                {
                    log->error("Max dir lights bound");
                    return;
                }
                log->info("Binding dir light {}", mDirectionalLightCount);
                dirData = light->getDirectionalLightData();
                addUniform(FLOAT3,"dirLights["+ std::to_string(mDirectionalLightCount)+"].direction",1, &dirData.direction);
                addUniform(FLOAT3,"dirLights["+ std::to_string(mDirectionalLightCount)+"].ambient",1,   &dirData.ambient);
                addUniform(FLOAT3,"dirLights["+ std::to_string(mDirectionalLightCount)+"].diffuse",1,   &dirData.diffuse);
                addUniform(FLOAT3,"dirLights["+ std::to_string(mDirectionalLightCount)+"].specular",1,  &dirData.specular);
                mDirectionalLightCount++;
                break;

            case LT_POINT:
                if (mPointLightCount == MAX_LIGHTS)
                {
                    log->error("Max point lights bound");
                    return;
                }
                log->info("Binding point light {}", mPointLightCount);
                pointData = light->getPointLightData();
                addUniform(FLOAT3,"pointLights["+std::to_string(mPointLightCount)+"].ambient",1,   &pointData.ambient);
                addUniform(FLOAT3,"pointLights["+std::to_string(mPointLightCount)+"].diffuse",1,   &pointData.diffuse);
                addUniform(FLOAT3,"pointLights["+std::to_string(mPointLightCount)+"].specular",1,  &pointData.specular);
                addUniform(FLOAT3,"pointLights["+std::to_string(mPointLightCount)+"].position",1,  &pointData.position);
                addUniform(FLOAT1,"pointLights["+std::to_string(mPointLightCount)+"].constant",1,  &pointData.constant);
                addUniform(FLOAT1,"pointLights["+std::to_string(mPointLightCount)+"].linear",1,    &pointData.linear);
                addUniform(FLOAT1,"pointLights["+std::to_string(mPointLightCount)+"].quadratic",1, &pointData.quadratic);
                mPointLightCount++;
                break;

            case LT_SPOTLIGHT:
                if (mSpotLightCount == MAX_LIGHTS)
                {
                    log->error("Max spot lights bound");
                    return;
                }
                log->info("Binding spot light {}", mSpotLightCount);
                spotData = light->getSpotLightData();
                addUniform(FLOAT3,"spotLights["+std::to_string(mSpotLightCount)+"].ambient",1,     &spotData.ambient);
                addUniform(FLOAT3,"spotLights["+std::to_string(mSpotLightCount)+"].diffuse",1,     &spotData.diffuse);
                addUniform(FLOAT3,"spotLights["+std::to_string(mSpotLightCount)+"].specular",1,    &spotData.specular);
                addUniform(FLOAT3,"spotLights["+std::to_string(mSpotLightCount)+"].position",1,    &spotData.position);
                addUniform(FLOAT1,"spotLights["+std::to_string(mSpotLightCount)+"].constant",1,    &spotData.constant);
                addUniform(FLOAT1,"spotLights["+std::to_string(mSpotLightCount)+"].linear",1,      &spotData.linear);
                addUniform(FLOAT1,"spotLights["+std::to_string(mSpotLightCount)+"].quadratic",1,   &spotData.quadratic);
                addUniform(FLOAT1,"spotLights["+std::to_string(mSpotLightCount)+"].cutOff",1,      &spotData.cutOff);
                addUniform(FLOAT1,"spotLights["+std::to_string(mSpotLightCount)+"].outerCutOff",1, &spotData.outerCutOff);
                mSpotLightCount++;
                break;

            case LT_NONE:
                log->error("Cannot bind light with type NONE");
                break;
        }
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

        // Sync lighting uniforms

        if (mPointLightCountLocation != UNIFORM_NOT_FOUND)
        {
            glUniform1i(mPointLightCountLocation,mPointLightCount);
            checkGLError();
        }
        else
        {
            log->error("Could not find Point Light Location Uniform");
        }

        if (mSpotLightCountLocation != UNIFORM_NOT_FOUND)
        {
            glUniform1i(mSpotLightCountLocation,mSpotLightCount);
            checkGLError();
        }
        else
        {
            log->error("Could not find Spot Light Location Uniform");
        }

        if (mDirectionalLightCountLocation != UNIFORM_NOT_FOUND)
        {
            glUniform1i(mDirectionalLightCountLocation,mDirectionalLightCount);
            checkGLError();
        }
        else
        {
            log->error("Could not find Directional Light Location Uniform");
        }

        // Sync user uniforms

        for (shared_ptr<ShaderUniform> uniform : mUniformVector)
        {
            if (uniform->getCount() == 0)
            {
                continue;
            }

            log->trace(
                        "Sync Uinform {} -> prog: {}, name: {}, loc: {}, count: {}",
                        getUuid(),
                        prog,
                        uniform->getName(),
                        uniform->getLocation(),
                        uniform->getCount()
                        );

            auto location = uniform->getLocation();
            if (location == UNIFORM_NOT_FOUND)
            {
                log->warn( "Unable to find uniform location '{}' in {}" , uniform->getName() ,getNameAndUuidString());
                continue;
            }
            else
            {
                switch (uniform->getType())
                {
                    // Int
                    case INT1:
                        glUniform1i(location,*static_cast<GLint*>(uniform->getData()));
                        break;
                    case INT2:
                        glUniform2iv(location,uniform->getCount(),value_ptr(*static_cast<ivec2*>(uniform->getData())));
                        break;
                    case INT3:
                        glUniform3iv(location,uniform->getCount(),value_ptr(*static_cast<ivec3*>(uniform->getData())));
                        break;
                    case INT4:
                        glUniform4iv(location,uniform->getCount(),value_ptr(*static_cast<ivec4*>(uniform->getData())));
                        break;

                    // Uint
                    case UINT1:
                        glUniform1ui(location,*static_cast<GLuint*>(uniform->getData()));
                        break;
                    case UINT2:
                        glUniform2uiv(location,uniform->getCount(),value_ptr(*static_cast<uvec2*>(uniform->getData())));
                        break;
                    case UINT3:
                        glUniform3uiv(location,uniform->getCount(),value_ptr(*static_cast<uvec3*>(uniform->getData())));
                        break;
                    case UINT4:
                        glUniform4uiv(location,uniform->getCount(),value_ptr(*static_cast<uvec4*>(uniform->getData())));
                        break;

                    // float
                    case FLOAT1:
                        glUniform1f(location,*static_cast<GLfloat*>(uniform->getData()));
                        break;
                    case FLOAT2:
                        glUniform2fv(location,uniform->getCount(),glm::value_ptr(*static_cast<vec2*>(uniform->getData())));
                        break;
                    case FLOAT3:
                        glUniform3fv(location,uniform->getCount(),glm::value_ptr(*static_cast<vec3*>(uniform->getData())));
                        break;
                    case FLOAT4:
                        glUniform4fv(location,uniform->getCount(),glm::value_ptr(*static_cast<vec4*>(uniform->getData())));
                        break;
                }
                checkGLError();
            }
        }
    }



    ShaderUniform::ShaderUniform(UniformType type, string name, int count, void* data)
        : DreamObject ("ShaderUniform"),
          mType(type),
          mName(name),
          mCount(count)

    {
        auto log = getLog();
        log->info("Constructing uniform {}, count {}",mName,count);
        switch (type)
        {
            case Dream::INT1:
                mData = new GLint[count];
                memcpy(mData,data,sizeof(GLint)*static_cast<unsigned long>(count));
                break;
            case Dream::INT2:
                mData = new ivec2[count];
                memcpy(mData,data,sizeof(ivec2)*static_cast<unsigned long>(count));
                break;
            case Dream::INT3:
                mData = new ivec3[count];
                memcpy(mData,data,sizeof(ivec3)*static_cast<unsigned long>(count));
                break;
            case Dream::INT4:
                mData = new ivec4[count];
                memcpy(mData,data,sizeof(ivec4)*static_cast<unsigned long>(count));
                break;

            case Dream::UINT1:
                mData = new GLint[count];
                memcpy(mData,data,sizeof(GLuint)*static_cast<unsigned long>(count));
                break;
            case Dream::UINT2:
                mData = new uvec2[count];
                memcpy(mData,data,sizeof(uvec2)*static_cast<unsigned long>(count));
                break;
            case Dream::UINT3:
                mData = new uvec3[count];
                memcpy(mData,data,sizeof(uvec3)*static_cast<unsigned long>(count));
                break;
            case Dream::UINT4:
                mData = new uvec4[count];
                memcpy(mData,data,sizeof(uvec4)*static_cast<unsigned long>(count));
                break;

            case Dream::FLOAT1:
                mData = new GLint[count];
                memcpy(mData,data,sizeof(GLfloat)*static_cast<unsigned long>(count));
                break;
            case Dream::FLOAT2:
                mData = new vec2[count];
                memcpy(mData,data,sizeof(vec2)*static_cast<unsigned long>(count));
                break;
            case Dream::FLOAT3:
                mData = new vec3[count];
                memcpy(mData,data,sizeof(vec3)*static_cast<unsigned long>(count));
                break;
            case Dream::FLOAT4:
                mData = new vec4[count];
                memcpy(mData,data,sizeof(vec4)*static_cast<unsigned long>(count));
                break;

        }
    }

    ShaderUniform::~ShaderUniform()
    {
        auto log = getLog();
        log->trace("Destructing {} {}", mName, mCount);
        switch (mType)
        {
            case Dream::INT1:
                delete[] static_cast<GLint*>(mData);
                break;
            case Dream::INT2:
                delete[] static_cast<ivec2*>(mData);
                break;
            case Dream::INT3:
                delete[] static_cast<ivec3*>(mData);
                break;
            case Dream::INT4:
                delete[] static_cast<ivec4*>(mData);
                break;

            case Dream::UINT1:
                delete[] static_cast<GLuint*>(mData);
                break;
            case Dream::UINT2:
                delete[] static_cast<uvec2*>(mData);
                break;
            case Dream::UINT3:
                delete[] static_cast<uvec3*>(mData);
                break;
            case Dream::UINT4:
                delete[] static_cast<uvec4*>(mData);
                break;

            case Dream::FLOAT1:
                delete[] static_cast<GLfloat*>(mData);
                break;
            case Dream::FLOAT2:
                delete[] static_cast<vec2*>(mData);
                break;
            case Dream::FLOAT3:
                delete[] static_cast<vec3*>(mData);
                break;
            case Dream::FLOAT4:
                delete[] static_cast<vec4*>(mData);
                break;
        }
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

    GLint ShaderUniform::getLocation() const
    {
        return mLocation;
    }

    void ShaderUniform::setLocation(GLint location)
    {
        mLocation = location;
    }

    void* ShaderUniform::getData() const
    {
        return mData;
    }

    void ShaderUniform::setData(void* data)
    {
        switch (mType)
        {
            case Dream::INT1:
                memcpy(mData,data,sizeof(GLint)*static_cast<unsigned long>(mCount));
                break;
            case Dream::INT2:
                memcpy(mData,data,sizeof(ivec2)*static_cast<unsigned long>(mCount));
                break;
            case Dream::INT3:
                memcpy(mData,data,sizeof(ivec3)*static_cast<unsigned long>(mCount));
                break;
            case Dream::INT4:
                memcpy(mData,data,sizeof(ivec4)*static_cast<unsigned long>(mCount));
                break;

            case Dream::UINT1:
                memcpy(mData,data,sizeof(GLuint)*static_cast<unsigned long>(mCount));
                break;
            case Dream::UINT2:
                memcpy(mData,data,sizeof(uvec2)*static_cast<unsigned long>(mCount));
                break;
            case Dream::UINT3:
                memcpy(mData,data,sizeof(uvec3)*static_cast<unsigned long>(mCount));
                break;
            case Dream::UINT4:
                memcpy(mData,data,sizeof(uvec4)*static_cast<unsigned long>(mCount));
                break;

            case Dream::FLOAT1:
                memcpy(mData,data,sizeof(GLfloat)*static_cast<unsigned long>(mCount));
                break;
            case Dream::FLOAT2:
                memcpy(mData,data,sizeof(vec2)*static_cast<unsigned long>(mCount));
                break;
            case Dream::FLOAT3:
                memcpy(mData,data,sizeof(vec3)*static_cast<unsigned long>(mCount));
                break;
            case Dream::FLOAT4:
                memcpy(mData,data,sizeof(vec4)*static_cast<unsigned long>(mCount));
                break;

        }

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
    const char* ShaderInstance::UNIFORM_POINT_LIGHT_COUNT = "pointLightCount";
    const char* ShaderInstance::UNIFORM_SPOT_LIGHT_COUNT = "spotLightCount";
    const char* ShaderInstance::UNIFORM_DIRECTIONAL_LIGHT_COUNT = "directionalLightCount";
    const unsigned int ShaderInstance::MAX_LIGHTS = 10;

} // End of Dream
