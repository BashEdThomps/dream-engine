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
#include "../Light/LightInstance.h"
#include "../Material/MaterialInstance.h"
#include "../Texture/TextureInstance.h"
#include "../../../Scene/SceneObject/SceneObjectRuntime.h"
#include "../../../Utilities/File.h"

using namespace glm;

namespace Dream
{
    const GLint ShaderInstance::UNIFORM_NOT_FOUND = -1;

    ShaderInstance::ShaderInstance
    (ShaderDefinition* definition)
        : IAssetInstance(definition,nullptr),
          mPointLightCount(0),
          mPointLightCountLocation(UNIFORM_NOT_FOUND),
          mSpotLightCount(0),
          mSpotLightCountLocation(UNIFORM_NOT_FOUND),
          mDirectionalLightCount(0),
          mDirectionalLightCountLocation(UNIFORM_NOT_FOUND),
          mShaderProgram(0),
          mNeedsRebind(true),
          mMaterialLocation(UNIFORM_NOT_FOUND),
          mVertexShader(0),
          mFragmentShader(0),
          mRecompile(false),
          mVertexSource(""),
          mFragmentSource("")

    {

        setLogClassName("ShaderInstance");
        auto log = getLog();
        log->trace( "Constructing Object" );
        mShaderProgram = 0;
        mInstanceMatricies.reserve(MAX_INSTANCES);
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

    int
    ShaderInstance::countMaterials
    ()
    {
       return static_cast<int>(mMaterials.size());
    }

    bool
    ShaderInstance::setModelMatrix
    (mat4 value, string name)
    {
        auto log = getLog();
        GLint location =  getUniformLocation(name);

        if (location == UNIFORM_NOT_FOUND)
        {
            log->info( "Unable to find model matrix uinform {} in {}" , name, getNameAndUuidString()  );
            return false;
        }

        glUniformMatrix4fv(location,1,GL_FALSE,value_ptr(value));

        return true;
    }

    /*
    bool
    ShaderInstance::setInstanceModelMatricies
    (vector<mat4> value, string name)
    {
        auto log = getLog();
        GLint location =  getUniformLocation("model[0]");

        if (location == UNIFORM_NOT_FOUND)
        {
            log->warn( "Unable to find model matrix uinform {} in {}" , name, getNameAndUuidString()  );
            return false;
        }
        glUniformMatrix4fv(location,value.size(),GL_FALSE,(float*)&value[0]);
        return true;
    }
    */


    bool
    ShaderInstance::setViewMatrix
    (mat4 value, string name)
    {
        auto log = getLog();
        GLint location = getUniformLocation(name);

        if (location == UNIFORM_NOT_FOUND)
        {
            log->info( "Unable to find view matrix uinform {} in {}" ,  name, getNameAndUuidString()  );
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
            log->info( "Unable to find projection matrix uinform {} in {}" ,  name, getNameAndUuidString()  );
            return false;
        }
        glUniformMatrix4fv(location,1,GL_FALSE,value_ptr(value));
        checkGLError();
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
            log->info( "Unable to find viewer position uinform {} in {}" ,  name, getNameAndUuidString()  );
            return false;
        }
        glUniform3fv(uCamPos,1,value_ptr(value));
        checkGLError();
        return true;
    }

    bool
    ShaderInstance::load
    (string projectPath)
    {
        mProjectPath = projectPath;
        if(!compileFragment())
        {
            return false;
        }
        if (!compileVertex())
        {
            return false;
        }
        return linkProgram();
    }

    bool
   ShaderInstance::compileVertex
   ()
   {
       auto log = getLog();
        // 1. Open Shader Files into Memory
        string absVertexPath = mProjectPath+mDefinition->getAssetPath() + Constants::SHADER_VERTEX;
        File vertexReader(absVertexPath);
        mVertexSource = vertexReader.readString();
        log->trace(
            "Loading Vertex Shader for {} from {}\n Vertex: {}\n",
            mDefinition->getNameAndUuidString(),
            absVertexPath,
            mVertexSource
        );
        // 2. Compile shaders
        GLint success;
        GLchar infoLog[512];
        // Vertex Shader
        mVertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char *vSource = mVertexSource.c_str();
        glShaderSource(mVertexShader, 1, &vSource, nullptr);
        glCompileShader(mVertexShader);
        // Print compile errors if any
        glGetShaderiv(mVertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(mVertexShader, 512, nullptr, infoLog);
            log->error( "VERTEX SHADER COMPILATION FAILED\n {}" ,infoLog );
            glDeleteShader(mVertexShader);
            mVertexShader = 0;
            return false;
        }
        return true;
   }

    bool
   ShaderInstance::compileFragment
   ()
   {
       auto log = getLog();
        // 1. Open Shader Files into Memory
        string absFragmentPath = mProjectPath+mDefinition->getAssetPath() + Constants::SHADER_FRAGMENT;
        File fragmentReader(absFragmentPath);
        mFragmentSource = fragmentReader.readString();
        log->trace(
            "Loading Fragment Shader for {} from {}\n {}\n",
            mDefinition->getNameAndUuidString(),
            absFragmentPath,
            mFragmentSource
        );
        // 2. Compile shaders
        GLint success;
        GLchar infoLog[512];
        // Fragment Shader
        mFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char *fSource = mFragmentSource.c_str();
        glShaderSource(mFragmentShader, 1, &fSource, nullptr);
        glCompileShader(mFragmentShader);
        // Print compile errors if any
        glGetShaderiv(mFragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(mFragmentShader, 512, nullptr, infoLog);
            log->error( "FRAGMENT SHADER COMPILATION FAILED\n {}" , infoLog );
            glDeleteShader(mFragmentShader);
            mFragmentShader = 0;
            return false;
        }
        return true;
   }

    bool
   ShaderInstance::linkProgram
   ()
   {
       auto log = getLog();
       if (mVertexShader != 0 && mFragmentShader != 0)
       {
           GLint success;

            // Create Shader Program
            mShaderProgram = glCreateProgram();
            if (mShaderProgram == 0)
            {
                log->error("Unable to create shader program");
                return false;
            }

            glAttachShader(mShaderProgram, mVertexShader);
            glAttachShader(mShaderProgram, mFragmentShader);
            glLinkProgram(mShaderProgram);

            // Print linking errors if any
            glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &success);
            GLchar infoLog[512];
            if (!success)
            {
                glGetProgramInfoLog(mShaderProgram, 512, nullptr, infoLog);
                log->error( "SHADER PROGRAM LINKING FAILED\n {}" , infoLog );
                glDeleteProgram(mShaderProgram);
                return false;
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
        auto iter = mUinformCache.find(name);
        if (iter == mUinformCache.end())
        {
            GLint loc = glGetUniformLocation(mShaderProgram,name.c_str());
            mUinformCache.insert(pair<string,GLint>(name,loc));
            return loc;
        }
        return (*iter).second;
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

    void
    ShaderInstance::bindMaterial
    (MaterialInstance* material)
    {
        auto log = getLog();
        GLuint id;
        if (material == nullptr)
        {
            log->error("Attempted to bind a null material, weird");
            return;
        }

        auto diffuse = material->getDiffuseTexture();
        if (diffuse != nullptr)
        {
            id = diffuse->getGLID();
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

        auto diffuseColour = material->getColorDiffuse();
        vec3 glmDiffuse(diffuseColour.r,diffuseColour.g,diffuseColour.b);
        addUniform(FLOAT3, "material.diffuseColor", 1, &glmDiffuse);

        auto specular = material->getSpecularTexture();
        if (specular != nullptr)
        {
            id =  specular->getGLID();
            if (CurrentTexture1 != id)
            {
                log->info("Found Specular Texture, binding {}",id);
                glActiveTexture(GL_TEXTURE1);
                checkGLError();
                glBindTexture(GL_TEXTURE_2D, id);
                checkGLError();
                GLuint specularIndex = 1;
                addUniform(INT1, "material.specular", 1, &specularIndex);

                CurrentTexture1 = id;
            }
        }

        auto spec = material->getColorSpecular();
        vec3 glmSpec(spec.r,spec.g,spec.b);
        addUniform(FLOAT3, "material.specularColor", 1, &glmSpec);
        float ss = material->getShininessStrength();
        addUniform(FLOAT1, "material.shininess", 1, &ss);

        auto normal = material->getNormalTexture();
        if (normal != nullptr)
        {
            id =  normal->getGLID();
            if (CurrentTexture2 != id)
            {
                log->info("Found Normal Texture, binding {}",id);
                glActiveTexture(GL_TEXTURE2);
                checkGLError();
                glBindTexture(GL_TEXTURE_2D, id);
                checkGLError();
                GLuint normalIndex = 2;
                addUniform(INT1, "material.normal", 1, &normalIndex);
                CurrentTexture2 = id;
            }
        }

        auto displacement = material->getDisplacementTexture();
        if (displacement != nullptr)
        {
            id = displacement->getGLID();
            if (CurrentTexture3 != id)
            {
                log->info("Found Normal Texture, binding {}",id);
                glActiveTexture(GL_TEXTURE3);
                checkGLError();
                glBindTexture(GL_TEXTURE_2D, id);
                checkGLError();
                GLuint normalIndex = 3;
                addUniform(INT1, "material.displacement", 1, &normalIndex);
                CurrentTexture3 = id;
            }
        }
        checkGLError();
    }

    void ShaderInstance::bindLight(LightInstance* light)
    {
        auto log = getLog();
        log->debug("Binding light {} ({})",light->getNameAndUuidString(),light->getType());
        DirLight dirData;
        SpotLight spotData;
        PointLight pointData;

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
        checkGLError();
        GLuint prog = getShaderProgram();

        // Sync lighting uniforms

        if (mPointLightCountLocation != UNIFORM_NOT_FOUND)
        {
            glUniform1i(mPointLightCountLocation,mPointLightCount);
            checkGLError();
        }
        else if (mPointLightCount > 0)
        {
            log->info("Could not find Point Light Count Location Uniform in {}",getNameAndUuidString());
        }

        if (mSpotLightCountLocation != UNIFORM_NOT_FOUND)
        {
            glUniform1i(mSpotLightCountLocation,mSpotLightCount);
            checkGLError();
        }
        else if (mSpotLightCount > 0)
        {
            log->info("Could not find Spot Light Count Location Uniform in {}",getNameAndUuidString());
        }

        if (mDirectionalLightCountLocation != UNIFORM_NOT_FOUND)
        {
            glUniform1i(mDirectionalLightCountLocation,mDirectionalLightCount);
            checkGLError();
        }
        else if (mDirectionalLightCount > 0)
        {
            log->info("Could not find Directional Light Count Location Uniform in {}",getNameAndUuidString());
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

            log->trace
            (
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
                log->info( "Unable to find uniform location '{}' in {}" , uniform->getName() ,getNameAndUuidString());
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
        auto log = getLog();
        // TODO - Bind as single uniform?
        for (size_t i=0; i<instances.size(); i++)
        {
            if (i>=MAX_INSTANCES)
            {
                log->info("Maximum number of instances reached");
                break;
            }
            auto instance = instances.at(i);
            setModelMatrix(instance->getTransform().getMatrix(), "model["+std::to_string(i)+"]");
        }
    }

    void
    ShaderInstance::addMaterial
    (MaterialInstance* material)
    {
        auto log = getLog();
        // not in map
        if (find(mMaterials.begin(), mMaterials.end(), material) == mMaterials.end())
        {
            log->debug(
                "Adding Material {} to shader {}",
                material->getName(),
                getNameAndUuidString()
            );
            mMaterials.push_back(material);
        }
        else
        {
            log->debug
            (
                "Material {} already registered to shader {}",
                material->getName(),
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
           log->debug("\t{}",material->getName());
           material->logMeshes();
       }
    }

    void
    ShaderInstance::draw
    (Camera* camera)
    {
       for (auto material : mMaterials)
       {
           bindMaterial(material);
           material->draw(camera);
       }
    }

    bool
    ShaderInstance::getRecompile
    ()
    const
    {
        return mRecompile;
    }

    void
    ShaderInstance::setRecompile
    (bool recompile)
    {
        mRecompile = recompile;
    }

    string
    ShaderInstance::getVertexSource
    () const
    {
        return mVertexSource;
    }

    void
    ShaderInstance::setVertexSource
    (string vertexSource)
    {
        mVertexSource = vertexSource;
    }

    string
    ShaderInstance::getFragmentSource
    ()
    const
    {
        return mFragmentSource;
    }

    void
    ShaderInstance::setFragmentSource
    (string fragmentSource)
    {
        mFragmentSource = fragmentSource;
    }

    const char* ShaderInstance::UNIFORM_POINT_LIGHT_COUNT = "pointLightCount";
    const char* ShaderInstance::UNIFORM_SPOT_LIGHT_COUNT = "spotLightCount";
    const char* ShaderInstance::UNIFORM_DIRECTIONAL_LIGHT_COUNT = "directionalLightCount";
    const unsigned int ShaderInstance::MAX_LIGHTS = 10;
    const size_t ShaderInstance::MAX_INSTANCES = 100;

    GLuint ShaderInstance::CurrentTexture0 = 0;
    GLuint ShaderInstance::CurrentTexture1 = 0;
    GLuint ShaderInstance::CurrentTexture2 = 0;
    GLuint ShaderInstance::CurrentTexture3 = 0;
    GLuint ShaderInstance::CurrentShaderProgram = 0;
    GLuint ShaderInstance::CurrentVAO = 0;
    GLuint ShaderInstance::CurrentVBO = 0;

} // End of Dream
