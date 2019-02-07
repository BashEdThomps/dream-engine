/*
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


#include "ShaderRuntime.h"

#include "ShaderCache.h"
#include "ShaderDefinition.h"
#include "ShaderTasks.h"
#include "../GraphicsComponent.h"
#include "../Light/LightRuntime.h"
#include "../Material/MaterialRuntime.h"
#include "../Texture/TextureRuntime.h"
#include "../../../Utilities/File.h"
#include "../../../Scene/SceneObject/SceneObjectRuntime.h"
#include "../../../Project/ProjectRuntime.h"

using namespace glm;

namespace Dream
{


    ShaderRuntime::ShaderRuntime
    (ShaderDefinition* definition, ProjectRuntime* rt)
        : SharedAssetRuntime(definition,rt),
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
          mFragmentSource(""),
          mCompileFragmentTask(nullptr),
          mCompileVertexTask(nullptr),
          mLinkTask(nullptr)
    {
        #ifdef DREAM_LOG
        setLogClassName("ShaderRuntime");
        getLog()->trace( "Constructing Object" );
        #endif
        mRuntimeMatricies.reserve(MAX_RUNTIMES);
    }

    ShaderRuntime::~ShaderRuntime
    ()
    {
        deleteUniforms();
        #ifdef DREAM_LOG
        getLog()->trace( "Destroying Object" );
        #endif
        mProjectRuntime->getGraphicsComponent()->pushTask(new ShaderFreeTask(getShaderProgram()));
    }

    GLuint
    ShaderRuntime::getShaderProgram
    ()
    const
    {
        return mShaderProgram;
    }

    void
    ShaderRuntime::setShaderProgram
    (GLuint sp)
    {
        mShaderProgram = sp;
    }

    int
    ShaderRuntime::countMaterials
    ()
    {
       return static_cast<int>(mMaterials.size());
    }

    bool
    ShaderRuntime::setModelMatrix
    (const mat4& value, const string& name)
    {
        GLint location = getUniformLocation(name);

        if (location == UNIFORM_NOT_FOUND)
        {
            #ifdef DREAM_LOG
            getLog()->info( "Unable to find model matrix uinform {} in {}" , name, getNameAndUuidString()  );
            #endif
            return false;
        }

        glUniformMatrix4fv(location,1,GL_FALSE,value_ptr(value));

        return true;
    }

    bool
    ShaderRuntime::setViewMatrix
    (const mat4& value, const string& name)
    {
        GLint location = getUniformLocation(name);

        if (location == UNIFORM_NOT_FOUND)
        {
            #ifdef DREAM_LOG
            getLog()->info( "Unable to find view matrix uinform {} in {}" ,  name, getNameAndUuidString()  );
            #endif
            return false;
        }
        glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(value));
        return true;
    }

    bool
    ShaderRuntime::setProjectionMatrix
    (const mat4& value, const string& name)
    {
        GLint location = getUniformLocation(name);

        if (location == UNIFORM_NOT_FOUND)
        {
            #ifdef DREAM_LOG
            getLog()->info( "Unable to find projection matrix uinform {} in {}" ,  name, getNameAndUuidString()  );
            #endif
            return false;
        }
        glUniformMatrix4fv(location,1,GL_FALSE,value_ptr(value));
        #ifdef DREAM_LOG
        checkGLError();
        #endif
        return true;
    }

    bool
    ShaderRuntime::setViewerPosition
    (const vec3& value, const string& name)
    {
        GLint uCamPos = getUniformLocation(name);

        if (uCamPos == UNIFORM_NOT_FOUND)
        {
            #ifdef DREAM_LOG
            getLog()->info( "Unable to find viewer position uinform {} in {}" ,  name, getNameAndUuidString()  );
            #endif
            return false;
        }
        glUniform3fv(uCamPos,1,value_ptr(value));
        #ifdef DREAM_LOG
        checkGLError();
        #endif
        return true;
    }

    bool
    ShaderRuntime::useDefinition
    ()
    {
        compileVertex();
        compileFragment();
        linkProgram();
        return true;
    }

    bool
    ShaderRuntime::compileVertex
    ()
    {
        // 1. Open Shader Files into Memory
        string absVertexPath = getAssetFilePath(Constants::SHADER_VERTEX_FILE_NAME);
        File vertexReader(absVertexPath);
        setVertexSource(vertexReader.readString());
        #ifdef DREAM_LOG
        getLog()->trace("Loading Vertex Shader for {} from {}\n{}\n",
            mDefinition->getNameAndUuidString(),absVertexPath, mVertexSource
        );
        #endif
        // 2. Compile shaders
        mCompileVertexTask = new ShaderCompileVertexTask(this);
        mProjectRuntime->getGraphicsComponent()->pushTask(mCompileVertexTask);
        return true;
    }

    bool
    ShaderRuntime::compileFragment
    ()
    {
        // 1. Open Shader Files into Memory
        string absFragmentPath = getAssetFilePath(Constants::SHADER_FRAGMENT_FILE_NAME);
        File fragmentReader(absFragmentPath);
        setFragmentSource(fragmentReader.readString());
        #ifdef DREAM_LOG
        getLog()->trace("Loading Fragment Shader for {} from {}\n{}\n",
            mDefinition->getNameAndUuidString(),absFragmentPath, mFragmentSource
        );
        #endif
        // 2. Compile shaders
        mCompileFragmentTask = new ShaderCompileFragmentTask(this);
        mProjectRuntime->getGraphicsComponent()->pushTask(mCompileFragmentTask);
        return true;
    }

   bool
   ShaderRuntime::linkProgram
   ()
   {
       mLinkTask = new ShaderLinkTask(this);
       mProjectRuntime->getGraphicsComponent()->pushTask(mLinkTask);
       return mLoaded;
   }

   void
   ShaderRuntime::clearLinkTask
   ()
   {
       mLinkTask = nullptr;
   }

   void
   ShaderRuntime::clearCompileFragmentTask
   ()
   {
       mCompileFragmentTask = nullptr;
   }

   void
   ShaderRuntime::clearCompileVertexTask
   ()
   {
       mCompileVertexTask = nullptr;
   }

   void
   ShaderRuntime::use
   ()
   {
       mPointLightCount = 0;
       mSpotLightCount = 0;
       mDirectionalLightCount = 0;
        if (CurrentShaderProgram != mShaderProgram)
        {
            #ifdef DREAM_LOG
            getLog()->info("Switching Shader Program from {} to {} for {}",
                CurrentShaderProgram,mShaderProgram,getNameAndUuidString()
            );
            #endif
            glUseProgram(mShaderProgram);
            CurrentShaderProgram = mShaderProgram;
        }
    }

    // API Setters =============================================================

    void
    ShaderRuntime::bindVertexArray
    (GLuint vao)
    {
        if (CurrentVAO != vao)
        {
            glBindVertexArray(vao);
            CurrentVAO = vao;
        }
    }

    void
    ShaderRuntime::unbindVertexArray
    ()
    {
        bindVertexArray(0);
    }

    GLint
    ShaderRuntime::getUniformLocation
    (const string& name)
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

    void
    ShaderRuntime::addUniform
    (UniformType type, const string& name, int count, void* data)
    {
        for (auto uniform : mUniformVector)
        {
            if (uniform->getName() == name)
            {
                #ifdef DREAM_LOG
                getLog()->info("Updating uniform {}", uniform->getName());
                #endif
                uniform->setData(data);
                return;
            }
        }
        #ifdef DREAM_LOG
        getLog()->info("Creating uniform {}", name);
        #endif
        auto newUniform = new ShaderUniform(type,name,count,data);
        newUniform->setLocation(glGetUniformLocation(mShaderProgram, name.c_str()));
        mUniformVector.push_back(newUniform);
    }

    void
    ShaderRuntime::bindMaterial
    (MaterialRuntime* material)
    {
        GLuint id;
        if (material == nullptr)
        {

            #ifdef DREAM_LOG
            getLog()->error("Attempted to bind a null material, weird");
            #endif
            return;
        }

        auto diffuse = material->getDiffuseTexture();
        if (diffuse != nullptr)
        {
            id = diffuse->getGLID();
            if (CurrentTexture0 != id)
            {
                #ifdef DREAM_LOG
                getLog()->info("Found Diffuse Texture, binding {}",id);
                #endif
                glActiveTexture(GL_TEXTURE0);
                #ifdef DREAM_LOG
                checkGLError();
                #endif
                glBindTexture(GL_TEXTURE_2D, id);
                #ifdef DREAM_LOG
                checkGLError();
                #endif
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
                #ifdef DREAM_LOG
                getLog()->info("Found Specular Texture, binding {}",id);
                #endif
                glActiveTexture(GL_TEXTURE1);
                #ifdef DREAM_LOG
                checkGLError();
                #endif
                glBindTexture(GL_TEXTURE_2D, id);
                #ifdef DREAM_LOG
                checkGLError();
                #endif
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
                #ifdef DREAM_LOG
                getLog()->info("Found Normal Texture, binding {}",id);
                #endif
                glActiveTexture(GL_TEXTURE2);
                #ifdef DREAM_LOG
                checkGLError();
                #endif
                glBindTexture(GL_TEXTURE_2D, id);

                #ifdef DREAM_LOG
                checkGLError();
                #endif
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
                #ifdef DREAM_LOG
                getLog()->info("Found Normal Texture, binding {}",id);
                #endif
                glActiveTexture(GL_TEXTURE3);
                #ifdef DREAM_LOG
                checkGLError();
                #endif
                glBindTexture(GL_TEXTURE_2D, id);

                #ifdef DREAM_LOG
                checkGLError();
                #endif
                GLuint normalIndex = 3;
                addUniform(INT1, "material.displacement", 1, &normalIndex);
                CurrentTexture3 = id;
            }
        }

        float ignore = material->getIgnore() ? 1.0f : 0.0f;
        addUniform(FLOAT1, "material.ignore", 1, &ignore);
        #ifdef DREAM_LOG
        checkGLError();
        #endif
    }

    void
    ShaderRuntime::bindLight
    (LightRuntime* light)
    {
        #ifdef DREAM_LOG
        getLog()->debug("Binding light {} ({})",light->getNameAndUuidString(),light->getType());
        #endif
        DirLight dirData;
        SpotLight spotData;
        PointLight pointData;

        switch (light->getType())
        {
            case LT_DIRECTIONAL:
                if (mDirectionalLightCount == MAX_LIGHTS)
                {
                    #ifdef DREAM_LOG
                    getLog()->error("Max dir lights bound");
                    #endif
                    return;
                }
                #ifdef DREAM_LOG
                getLog()->debug("Binding dir light {}", mDirectionalLightCount);
                #endif
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
                    #ifdef DREAM_LOG
                    getLog()->error("Max point lights bound");
                    #endif
                    return;
                }

                #ifdef DREAM_LOG
                getLog()->debug("Binding point light {}", mPointLightCount);
                #endif
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
#ifdef DREAM_LOG
                    getLog()->error("Max spot lights bound");
#endif
                    return;
                }
#ifdef DREAM_LOG
                getLog()->debug("Binding spot light {}", mSpotLightCount);
#endif
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
#ifdef DREAM_LOG
                getLog()->error("Cannot bind light with type NONE");
#endif
                break;
        }
    }

    void
    ShaderRuntime::unbind
    ()
    {
        glUseProgram(0);
        CurrentShaderProgram = 0;
    }

    // GL Syncros ==============================================================
    void
    ShaderRuntime::syncUniforms
    ()
    {
        #ifdef DREAM_LOG
        getLog()->debug("Synchronising uniforms for {}",getNameAndUuidString());
        checkGLError();
        #endif
        GLuint prog = getShaderProgram();

        // Sync lighting uniforms

        if (mPointLightCountLocation != UNIFORM_NOT_FOUND)
        {
            glUniform1i(mPointLightCountLocation,mPointLightCount);
            #ifdef DREAM_LOG
            checkGLError();
            #endif
        }

        else if (mPointLightCount > 0)
        {
            #ifdef DREAM_LOG
            getLog()->info("Could not find Point Light Count Location Uniform in {}",getNameAndUuidString());
            #endif
        }

        if (mSpotLightCountLocation != UNIFORM_NOT_FOUND)
        {
            glUniform1i(mSpotLightCountLocation,mSpotLightCount);
            #ifdef DREAM_LOG
            checkGLError();
            #endif
        }
        else if (mSpotLightCount > 0)
        {
            #ifdef DREAM_LOG
            getLog()->info("Could not find Spot Light Count Location Uniform in {}",getNameAndUuidString());
            #endif
        }

        if (mDirectionalLightCountLocation != UNIFORM_NOT_FOUND)
        {
            glUniform1i(mDirectionalLightCountLocation,mDirectionalLightCount);

            #ifdef DREAM_LOG
            checkGLError();
            #endif
        }
        else if (mDirectionalLightCount > 0)
        {
            #ifdef DREAM_LOG
            getLog()->info("Could not find Directional Light Count Location Uniform in {}",getNameAndUuidString());
            #endif
        }

        // Sync user uniforms

        for (ShaderUniform* uniform : mUniformVector)
        {
            if (!uniform->getNeedsUpdate())
            {
                continue;
            }

            #ifdef DREAM_LOG
            getLog()->trace("Uniform {} needs update",uniform->getName());
            #endif

            if (uniform->getCount() == 0)
            {
                continue;
            }

            #ifdef DREAM_LOG
            getLog()->trace
            (
                "Sync Uinform {} -> prog: {}, name: {}, loc: {}, count: {}",
                getUuid(),
                prog,
                uniform->getName(),
                uniform->getLocation(),
                uniform->getCount()
            );
            #endif

            auto location = uniform->getLocation();
            if (location == UNIFORM_NOT_FOUND)
            {
                #ifdef DREAM_LOG
                getLog()->info( "Unable to find uniform location '{}' in {}" , uniform->getName() ,getNameAndUuidString());
                #endif
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
                #ifdef DREAM_LOG
                checkGLError();
                #endif
            }
            uniform->setNeedsUpdate(false);
        }
    }

    void
    ShaderRuntime::InvalidateState
    ()
    {
        CurrentTexture0 = 0;
        CurrentTexture1 = 0;
        CurrentTexture2 = 0;
        CurrentTexture3 = 0;
        CurrentTexture4 = 0;
        CurrentTexture5 = 0;
        CurrentShaderProgram = 0;
        CurrentVAO = 0;
        CurrentVBO = 0;
    }

    void
    ShaderRuntime::deleteUniforms
    ()
    {
       for (ShaderUniform* uniform : mUniformVector)
       {
           delete uniform;
       }
       mUniformVector.clear();
    }

    void
    ShaderRuntime::bindLightQueue
    (const vector<LightRuntime*>& lightQueue)
    {
        for (auto light : lightQueue)
        {
            bindLight(light);
        }
    }

    void
    ShaderRuntime::bindRuntimes
    (const vector<SceneObjectRuntime*>& runtimes)
    {
        static mat4 data[100];
        size_t nRuntimes = runtimes.size();
        for (size_t i = 0; i<nRuntimes; i++)
        {
            data[i] = runtimes[i]->getTransform().getMatrix();
        }

        GLint location =  getUniformLocation("model[0]");

        if (location == UNIFORM_NOT_FOUND)
        {
            #ifdef DREAM_LOG
            getLog()->warn( "Unable to find model matrix uinform model[0] in {}", getNameAndUuidString());
            #endif
            return;
        }
        glUniformMatrix4fv(location,runtimes.size(),GL_FALSE,(float*)&data[0]);
    }

    void
    ShaderRuntime::addMaterial
    (MaterialRuntime* material)
    {
        // not in map
        if (find(mMaterials.begin(), mMaterials.end(), material) == mMaterials.end())
        {
            #ifdef DREAM_LOG
            getLog()->debug(
                "Adding Material {} to shader {}",
                material->getName(),
                getNameAndUuidString()
            );
            #endif
            mMaterials.push_back(material);
        }
        #ifdef DREAM_LOG
        else
        {
            getLog()->debug
            (
                "Material {} already registered to shader {}",
                material->getName(),
                getNameAndUuidString()
            );
        }
        #endif
    }

    #ifdef DREAM_LOG
    void
    ShaderRuntime::logMaterials
    ()
    {
       getLog()->debug("Materials for {}",getNameAndUuidString());
       for (auto material : mMaterials)
       {
           getLog()->debug("\t{}",material->getName());
           material->logMeshes();
       }
    }
    #endif

    void
    ShaderRuntime::drawGeometryPass
    (Camera* camera)
    {
       for (auto material : mMaterials)
       {
           if (material->countMeshes() == 0) continue;
           bindMaterial(material);
           syncUniforms();
           material->drawGeometryPass(camera);
       }
    }

    void
    ShaderRuntime::drawShadowPass
    (ShaderRuntime* shadowPassShader)
    {
       for (auto material : mMaterials)
       {
           if (material->countMeshes() == 0) continue;
           material->drawShadowPass(shadowPassShader);
       }
    }

    bool
    ShaderRuntime::getRecompile
    ()
    const
    {
        return mRecompile;
    }

    void
    ShaderRuntime::setRecompile
    (bool recompile)
    {
        mRecompile = recompile;
    }

    string
    ShaderRuntime::getVertexSource
    () const
    {
        return mVertexSource;
    }

    void
    ShaderRuntime::setVertexSource
    (const string& vertexSource)
    {
        mVertexSource = vertexSource;
    }

    string
    ShaderRuntime::getFragmentSource
    ()
    const
    {
        return mFragmentSource;
    }

    void
    ShaderRuntime::setFragmentSource
    (const string& fragmentSource)
    {
        mFragmentSource = fragmentSource;
    }

    GLuint
    ShaderRuntime::getVertexShader
    () const
    {
        return mVertexShader;
    }

    void
    ShaderRuntime::setVertexShader
    (const GLuint& vertexShader)
    {
        mVertexShader = vertexShader;
    }

    GLuint
    ShaderRuntime::getFragmentShader
    () const
    {
        return mFragmentShader;
    }

    void
    ShaderRuntime::setFragmentShader
    (const GLuint& fragmentShader)
    {
        mFragmentShader = fragmentShader;
    }

    GLint
    ShaderRuntime::getPointLightCountLocation
    () const
    {
        return mPointLightCountLocation;
    }

    void
    ShaderRuntime::setPointLightCountLocation
    (const GLint& pointLightCountLocation)
    {
        mPointLightCountLocation = pointLightCountLocation;
    }

    GLint
    ShaderRuntime::getSpotLightCountLocation
    () const
    {
        return mSpotLightCountLocation;
    }

    void
    ShaderRuntime::setSpotLightCountLocation
    (const GLint& spotLightCountLocation)
    {
        mSpotLightCountLocation = spotLightCountLocation;
    }

    GLint
    ShaderRuntime::getDirectionalLightCountLocation
    () const
    {
        return mDirectionalLightCountLocation;
    }

    void
    ShaderRuntime::setDirectionalLightCountLocation
    (const GLint& directionalLightCountLocation)
    {
        mDirectionalLightCountLocation = directionalLightCountLocation;
    }

    const GLint ShaderRuntime::UNIFORM_NOT_FOUND = -1;
    const char* ShaderRuntime::UNIFORM_POINT_LIGHT_COUNT = "pointLightCount";
    const char* ShaderRuntime::UNIFORM_SPOT_LIGHT_COUNT = "spotLightCount";
    const char* ShaderRuntime::UNIFORM_DIRECTIONAL_LIGHT_COUNT = "directionalLightCount";
    const unsigned int ShaderRuntime::MAX_LIGHTS = 10;
    const size_t ShaderRuntime::MAX_RUNTIMES = 100;

    GLuint ShaderRuntime::CurrentTexture0 = 0;
    GLuint ShaderRuntime::CurrentTexture1 = 0;
    GLuint ShaderRuntime::CurrentTexture2 = 0;
    GLuint ShaderRuntime::CurrentTexture3 = 0;
    GLuint ShaderRuntime::CurrentTexture4 = 0;
    GLuint ShaderRuntime::CurrentTexture5 = 0;
    GLuint ShaderRuntime::CurrentShaderProgram = 0;
    GLuint ShaderRuntime::CurrentVAO = 0;
    GLuint ShaderRuntime::CurrentVBO = 0;
}
