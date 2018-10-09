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
#include "../../../Scene/SceneObject/SceneObjectRuntime.h"
#include "../../../Utilities/FileReader.h"
#include "../Model/Material/Material.h"
#include "../Light/LightInstance.h"

using namespace glm;

namespace Dream
{
    const GLint ShaderInstance::UNIFORM_NOT_FOUND = -1;

    ShaderInstance::ShaderInstance
    (ShaderDefinition* definition,
     SceneObjectRuntime* transform)
        : IAssetInstance(definition,transform),
          mPointLightCount(0),
          mPointLightCountLocation(UNIFORM_NOT_FOUND),
          mSpotLightCount(0),
          mSpotLightCountLocation(UNIFORM_NOT_FOUND),
          mDirectionalLightCount(0),
          mDirectionalLightCountLocation(UNIFORM_NOT_FOUND),
          mNeedsRebind(true)
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
        glDeleteProgram(mShaderProgram);
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
        log->debug(
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
        if (CurrentShaderProgram != mShaderProgram)
        {
            auto log = spdlog::get("ShaderInstance");
            log->info(
                "Switching Shader Program from {} to {} for {}",
                CurrentShaderProgram,
                mShaderProgram,
                getNameAndUuidString()
            );
            glUseProgram(mShaderProgram);
            CurrentShaderProgram = mShaderProgram;
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
        if (CurrentVAO != vao)
        {
            glBindVertexArray(vao);
            CurrentVAO = vao;
        }
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
                log->info("Updating uniform {}", uniform->getName());
                uniform->setData(data);
                return;
            }
        }
        log->info("Creating uniform {}", name);
        auto newUniform = make_shared<ShaderUniform>(type,name,count,data);
        newUniform->setLocation(glGetUniformLocation(mShaderProgram, name.c_str()));
        mUniformVector.push_back(newUniform);
    }

    void ShaderInstance::bindMaterial(const shared_ptr<Material>& material)
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
            if (CurrentTexture0 != id)
            {
                log->info("Found Diffuse Texture, binding {}",id);
                glActiveTexture(GL_TEXTURE0);
                checkGLError();
                glBindTexture(GL_TEXTURE_2D, id);
                checkGLError();
                GLuint diffuseIndex = 0;
                addUniform(INT1, "material.diffuse", 1, &diffuseIndex);
                CurrentTexture0 = id;
            }
        }

        if (material->mSpecularTexture != nullptr)
        {
            id =  material->mSpecularTexture->id;
            if (CurrentTexture1 != id)
            {
                log->info("Found Specular Texture, binding {}",id);
                glActiveTexture(GL_TEXTURE1);
                checkGLError();
                glBindTexture(GL_TEXTURE_2D, id);
                checkGLError();
                GLuint specularIndex = 1;
                addUniform(INT1, "material.specular", 1, &specularIndex);
                addUniform(FLOAT1, "material.shininess", 1, &material->mShininessStrength);
                CurrentTexture1 = id;
            }
        }

        if (material->mNormalTexture != nullptr)
        {
            id =  material->mNormalTexture->id;
            if (CurrentTexture2 != id)
            {
                log->info("Found Normal Texture, binding {}",id);
                glActiveTexture(GL_TEXTURE2);
                checkGLError();
                glBindTexture(GL_TEXTURE_2D, id);
                checkGLError();
                GLuint normalIndex = 2;
                addUniform(INT1, "material.normalMap", 1, &normalIndex);
                CurrentTexture2 = id;
            }
        }

        if (material->mDisplacementTexture != nullptr)
        {
            id =  material->mDisplacementTexture->id;
            if (CurrentTexture3 != id)
            {
                log->info("Found Normal Texture, binding {}",id);
                glActiveTexture(GL_TEXTURE3);
                checkGLError();
                glBindTexture(GL_TEXTURE_2D, id);
                checkGLError();
                GLuint normalIndex = 3;
                addUniform(INT1, "material.depthMap", 1, &normalIndex);
                CurrentTexture3 = id;
            }
        }
    }

    void ShaderInstance::bindLight(LightInstance* light)
    {
        auto log = getLog();
        log->debug("Binding light {} ({})",light->getNameAndUuidString(),light->getType());
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
                log->debug("Binding dir light {}", mDirectionalLightCount);
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
                log->debug("Binding point light {}", mPointLightCount);
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
                log->debug("Binding spot light {}", mSpotLightCount);
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
        CurrentShaderProgram = 0;
    }

    // GL Syncros ==============================================================
    void
    ShaderInstance::syncUniforms
    ()
    {
        auto log = getLog();
        log->debug("Synchronising uniforms for {}",getNameAndUuidString());
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

        for (const shared_ptr<ShaderUniform>& uniform : mUniformVector)
        {
            if (!uniform->getNeedsUpdate())
            {
                continue;
            }

            log->trace("Uniform {} needs update",uniform->getName());

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
            uniform->setNeedsUpdate(false);
        }
    }

    void
    ShaderInstance::InvalidateState
    ()
    {
        CurrentTexture0 = 0;
        CurrentTexture1 = 0;
        CurrentTexture2 = 0;
        CurrentTexture3 = 0;
        CurrentShaderProgram = 0;
        CurrentVAO = 0;
        CurrentVBO = 0;
    }

    void
    ShaderInstance::bindLightQueue
    (vector<LightInstance*> lightQueue)
    {
        for (size_t i=0; i < lightQueue.size(); i++)
        {
            bindLight(lightQueue.at(i));
        }
    }

    void
    ShaderInstance::bindInstances
    (vector<SceneObjectRuntime*> instances)
    {
        for (size_t i=0; i<instances.size(); i++)
        {
            auto instance = instances.at(i);
            setModelMatrix(instance->getTransform()->asMat4(), "model["+std::to_string(i)+"]");
        }
    }

    void
    ShaderInstance::addMaterial
    (const shared_ptr<Material> material)
    {
        auto log = getLog();
        // not in map
        if (find(mMaterials.begin(), mMaterials.end(), material) == mMaterials.end())
        {
            log->debug(
                "Adding Material {} to shader {}",
                material->mName.C_Str(),
                getNameAndUuidString()
            );
            mMaterials.push_back(material);
        }
        else
        {
            log->debug
            (
                "Material {} already registered to shader {}",
                material->mName.C_Str(),
                getNameAndUuidString()
            );
        }
    }

    void
    ShaderInstance::logMaterials
    ()
    {
       auto log = getLog();
       log->debug("Materials for {}",getNameAndUuidString());
       for (auto material : mMaterials)
       {
           log->debug("\t{}",material->mName.C_Str());
           material->logMeshes();
       }
    }

    void
    ShaderInstance::draw
    ()
    {
       for (auto material : mMaterials)
       {
           bindMaterial(material);
           material->draw(this);
       }
    }

    const char* ShaderInstance::UNIFORM_POINT_LIGHT_COUNT = "pointLightCount";
    const char* ShaderInstance::UNIFORM_SPOT_LIGHT_COUNT = "spotLightCount";
    const char* ShaderInstance::UNIFORM_DIRECTIONAL_LIGHT_COUNT = "directionalLightCount";
    const unsigned int ShaderInstance::MAX_LIGHTS = 10;

    GLuint ShaderInstance::CurrentTexture0 = 0;
    GLuint ShaderInstance::CurrentTexture1 = 0;
    GLuint ShaderInstance::CurrentTexture2 = 0;
    GLuint ShaderInstance::CurrentTexture3 = 0;
    GLuint ShaderInstance::CurrentShaderProgram = 0;
    GLuint ShaderInstance::CurrentVAO = 0;
    GLuint ShaderInstance::CurrentVBO = 0;

} // End of Dream
