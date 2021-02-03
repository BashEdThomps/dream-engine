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
#include "Components/Storage/StorageManager.h"
#include "Components/Storage/File.h"
#include "Components/Graphics/GraphicsComponent.h"
#include "Components/Graphics/Light/LightRuntime.h"
#include "Components/Graphics/Material/MaterialRuntime.h"
#include "Components/Graphics/Texture/TextureRuntime.h"
#include "Scene/Entity/EntityRuntime.h"
#include "Project/ProjectRuntime.h"

#define ERROR_BUF_SZ 4096

using std::make_shared;
using std::pair;
using glm::value_ptr;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::ivec2;
using glm::ivec3;
using glm::ivec4;
using glm::uvec2;
using glm::uvec3;
using glm::uvec4;
using std::unique_lock;

namespace octronic::dream
{
    ShaderRuntime::ShaderRuntime
    (ShaderDefinition* definition, ProjectRuntime* rt)
        : SharedAssetRuntime("ShaderRuntime",definition,rt),
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
          mCompileFragmentTask(this),
          mCompileVertexTask(this),
          mLinkTask(this),
          mFreeTask(nullptr)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_TRACE( "ShaderRuntime: Constructing Object" );
        mRuntimeMatricies.reserve(MAX_RUNTIMES);
    }

    ShaderRuntime::~ShaderRuntime
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        deleteUniforms();
        LOG_TRACE( "ShaderRuntime: Destroying Object" );
        mFreeTask = make_shared<ShaderFreeTask>();
        mFreeTask->setShaderProgram(mShaderProgram);
        mProjectRuntime->getGraphicsComponent()->pushDestructionTask(mFreeTask);
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mShaderProgram = sp;
    }

    int
    ShaderRuntime::countMaterials
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
       return static_cast<int>(mMaterials.size());
    }

    bool
    ShaderRuntime::setModelMatrix
    (const mat4& value, const string& name)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        GLint location = getUniformLocation(name);

        if (location == UNIFORM_NOT_FOUND)
        {
            LOG_INFO( "ShaderRuntime: Unable to find model matrix uinform {} in {}" , name, getNameAndUuidString()  );
            return false;
        }

        glUniformMatrix4fv(location,1,GL_FALSE,value_ptr(value));

        return true;
    }

    bool
    ShaderRuntime::setViewMatrix
    (const mat4& value, const string& name)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        GLint location = getUniformLocation(name);

        if (location == UNIFORM_NOT_FOUND)
        {
            LOG_INFO( "ShaderRuntime: Unable to find view matrix uinform {} in {}" ,  name, getNameAndUuidString()  );
            return false;
        }
        glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(value));
        return true;
    }

    bool
    ShaderRuntime::setProjectionMatrix
    (const mat4& value, const string& name)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        GLint location = getUniformLocation(name);

        if (location == UNIFORM_NOT_FOUND)
        {
            LOG_INFO( "ShaderRuntime: Unable to find projection matrix uinform {} in {}" ,  name, getNameAndUuidString()  );
            return false;
        }
        glUniformMatrix4fv(location,1,GL_FALSE,value_ptr(value));
        GLCheckError();
        return true;
    }

    bool
    ShaderRuntime::setViewerPosition
    (const Vector3& value, const string& name)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        GLint uCamPos = getUniformLocation(name);

        if (uCamPos == UNIFORM_NOT_FOUND)
        {
            LOG_INFO( "ShaderRuntime: Unable to find viewer position uinform {} in {}" ,  name, getNameAndUuidString()  );
            return false;
        }
        glUniform3fv(uCamPos,1,value_ptr(value.toGLM()));
        GLCheckError();
        return true;
    }

    bool
    ShaderRuntime::useDefinition
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if (!compileVertex()) return false;
        if (!compileFragment()) return false;
        if (!linkProgram()) return false;
        return true;
    }

    bool
    ShaderRuntime::compileVertex
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        // 1. Open Shader Files into Memory
        StorageManager* fm = mProjectRuntime->getStorageManager();
#if defined (GL_ES_VERSION_3_0)
        string absVertexPath = getAssetFilePath(Constants::SHADER_GLES_VERTEX_FILE_NAME);
#else
        string absVertexPath = getAssetFilePath(Constants::SHADER_GLSL_VERTEX_FILE_NAME);
#endif
        File* vertexReader = fm->openFile(absVertexPath);
        if (vertexReader->exists())
        {
			setVertexSource(vertexReader->readString());
			LOG_TRACE("ShaderRuntime: Loading Vertex Shader for {} from {}\n{}\n",
				mDefinition->getNameAndUuidString(),absVertexPath, mVertexSource
			);
			// 2. Push a Vertex Compile Task
			mCompileVertexTask.clearState();
			mProjectRuntime->getGraphicsComponent()->pushTask(&mCompileVertexTask);
            fm->closeFile(vertexReader);
            vertexReader = nullptr;
			return true;
        }
        LOG_ERROR("ShaderRuntime: Vertex Shader file does not exist");
        fm->closeFile(vertexReader);
        vertexReader = nullptr;
        return false;
    }

    bool
    ShaderRuntime::compileFragment
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        // 1. Open Shader Files into Memory
        StorageManager* fm = mProjectRuntime->getStorageManager();
#if defined (GL_ES_VERSION_3_0)
        string absFragmentPath = getAssetFilePath(Constants::SHADER_GLES_FRAGMENT_FILE_NAME);
#else
        string absFragmentPath = getAssetFilePath(Constants::SHADER_GLSL_FRAGMENT_FILE_NAME);
#endif
        File* fragmentReader = fm->openFile(absFragmentPath);
        if (fragmentReader->exists())
        {
			setFragmentSource(fragmentReader->readString());
			LOG_TRACE("ShaderRuntime: Loading Fragment Shader for {} from {}\n{}\n",
				mDefinition->getNameAndUuidString(),absFragmentPath, mFragmentSource
			);

			// 2. Push a Fragment Compile Task
			mCompileFragmentTask.clearState();
			mProjectRuntime->getGraphicsComponent()->pushTask(&mCompileFragmentTask);
            fm->closeFile(fragmentReader);
            fragmentReader = nullptr;
			return true;
        }
        LOG_ERROR("ShaderRuntime: Fragment Shader file does not exist");
        fm->closeFile(fragmentReader);
        fragmentReader = nullptr;
        return false;
    }

   bool
   ShaderRuntime::linkProgram
   ()
   {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
       // Push a shader Link Task
       mLinkTask.clearState();

       mLinkTask.dependsOn(&mCompileVertexTask);
       mLinkTask.dependsOn(&mCompileFragmentTask);
       mProjectRuntime->getGraphicsComponent()->pushTask(&mLinkTask);
       return true;
   }

    bool
    ShaderRuntime::use
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if (mShaderProgram != 0)
        {
            mPointLightCount = 0;
            mSpotLightCount = 0;
            mDirectionalLightCount = 0;
            if (CurrentShaderProgram != mShaderProgram)
            {
                 LOG_INFO("ShaderRuntime: Switching Shader Program from {} to {} for {}",
                    CurrentShaderProgram,mShaderProgram,getNameAndUuidString()
                 );
                 glUseProgram(mShaderProgram);
                 CurrentShaderProgram = mShaderProgram;
            }
            return true;
        }
        return false;
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        for (auto uniform : mUniformVector)
        {
            if (uniform->getName() == name)
            {
                LOG_INFO("ShaderRuntime: Updating uniform {}", uniform->getName());
                uniform->setData(data);
                return;
            }
        }
        LOG_INFO("ShaderRuntime: Creating uniform {}", name);
        auto newUniform = new ShaderUniform(type,name,count,data);
        newUniform->setLocation(glGetUniformLocation(mShaderProgram, name.c_str()));
        mUniformVector.push_back(newUniform);
    }

    void
    ShaderRuntime::bindMaterial
    (MaterialRuntime* material)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        GLuint id;
        if (material == nullptr)
        {

            LOG_ERROR("ShaderRuntime: Attempted to bind a null material, weird");
            return;
        }

        auto diffuse = material->getDiffuseTexture();
        if (diffuse != nullptr)
        {
            id = diffuse->getGLID();
            if (CurrentTexture0 != id)
            {
                LOG_INFO("ShaderRuntime: Found Diffuse Texture, binding {}",id);
                glActiveTexture(GL_TEXTURE0);
                GLCheckError();
                glBindTexture(GL_TEXTURE_2D, id);
                GLCheckError();
                GLuint diffuseIndex = 0;
                addUniform(INT1, "material.diffuse", 1, &diffuseIndex);
                CurrentTexture0 = id;
            }
        }

        auto diffuseColour = material->getColorDiffuse();
        Vector3 glmDiffuse(diffuseColour.r,diffuseColour.g,diffuseColour.b);
        addUniform(FLOAT3, "material.diffuseColor", 1, &glmDiffuse);

        auto specular = material->getSpecularTexture();
        if (specular != nullptr)
        {
            id =  specular->getGLID();
            if (CurrentTexture1 != id)
            {
                LOG_INFO("ShaderRuntime: Found Specular Texture, binding {}",id);
                glActiveTexture(GL_TEXTURE1);
                GLCheckError();
                glBindTexture(GL_TEXTURE_2D, id);
                GLCheckError();
                GLuint specularIndex = 1;
                addUniform(INT1, "material.specular", 1, &specularIndex);
                CurrentTexture1 = id;
            }
        }

        auto spec = material->getColorSpecular();
        Vector3 glmSpec(spec.r,spec.g,spec.b);
        addUniform(FLOAT3, "material.specularColor", 1, &glmSpec);
        float ss = material->getShininessStrength();
        addUniform(FLOAT1, "material.shininess", 1, &ss);

        auto normal = material->getNormalTexture();
        if (normal != nullptr)
        {
            id =  normal->getGLID();
            if (CurrentTexture2 != id)
            {
                LOG_INFO("ShaderRuntime: Found Normal Texture, binding {}",id);
                glActiveTexture(GL_TEXTURE2);
                GLCheckError();
                glBindTexture(GL_TEXTURE_2D, id);

                GLCheckError();
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
                LOG_INFO("ShaderRuntime: Found Normal Texture, binding {}",id);
                glActiveTexture(GL_TEXTURE3);
                GLCheckError();
                glBindTexture(GL_TEXTURE_2D, id);

                GLCheckError();
                GLuint normalIndex = 3;
                addUniform(INT1, "material.displacement", 1, &normalIndex);
                CurrentTexture3 = id;
            }
        }

        float ignore = material->getIgnore() ? 1.0f : 0.0f;
        addUniform(FLOAT1, "material.ignore", 1, &ignore);
        GLCheckError();
    }

    void
    ShaderRuntime::bindLight
    (LightRuntime* light)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_DEBUG("ShaderRuntime: Binding light {} ({})",light->getNameAndUuidString(),light->getType());
        DirLight dirData;
        SpotLight spotData;
        PointLight pointData;

        switch (light->getType())
        {
            case LT_DIRECTIONAL:
                if (mDirectionalLightCount == MAX_LIGHTS)
                {
                    LOG_ERROR("ShaderRuntime: Max dir lights bound");
                    return;
                }
                LOG_DEBUG("ShaderRuntime: Binding dir light {}", mDirectionalLightCount);
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
                    LOG_ERROR("ShaderRuntime: Max point lights bound");
                    return;
                }

                LOG_DEBUG("ShaderRuntime: Binding point light {}", mPointLightCount);
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
                    LOG_ERROR("ShaderRuntime: Max spot lights bound");
                    return;
                }
                LOG_DEBUG("ShaderRuntime: Binding spot light {}", mSpotLightCount);
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
                LOG_INFO("ShaderRuntime: Cannot bind light with type NONE");
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_DEBUG("ShaderRuntime: Synchronising uniforms for {}",getNameAndUuidString());
        GLCheckError();
        GLuint prog = getShaderProgram();

        // Sync lighting uniforms

        if (mPointLightCountLocation != UNIFORM_NOT_FOUND)
        {
            glUniform1i(mPointLightCountLocation,mPointLightCount);
            GLCheckError();
        }

        else if (mPointLightCount > 0)
        {
            LOG_INFO("ShaderRuntime: Could not find Point Light Count Location Uniform in {}",getNameAndUuidString());
        }

        if (mSpotLightCountLocation != UNIFORM_NOT_FOUND)
        {
            glUniform1i(mSpotLightCountLocation,mSpotLightCount);
            GLCheckError();
        }
        else if (mSpotLightCount > 0)
        {
            LOG_INFO("ShaderRuntime: Could not find Spot Light Count Location Uniform in {}",getNameAndUuidString());
        }

        if (mDirectionalLightCountLocation != UNIFORM_NOT_FOUND)
        {
            glUniform1i(mDirectionalLightCountLocation,mDirectionalLightCount);

            GLCheckError();
        }
        else if (mDirectionalLightCount > 0)
        {
            LOG_INFO("ShaderRuntime: Could not find Directional Light Count Location Uniform in {}",getNameAndUuidString());
        }

        // Sync user uniforms

        for (ShaderUniform* uniform : mUniformVector)
        {
            if (!uniform->getNeedsUpdate())
            {
                continue;
            }

            LOG_TRACE("ShaderRuntime: Uniform {} needs update",uniform->getName());

            if (uniform->getCount() == 0)
            {
                continue;
            }

            LOG_TRACE
            (
                "ShaderRuntime: Sync Uinform {} -> prog: {}, name: {}, loc: {}, count: {}",
                getUuid(),
                prog,
                uniform->getName(),
                uniform->getLocation(),
                uniform->getCount()
            );

            auto location = uniform->getLocation();
            if (location == UNIFORM_NOT_FOUND)
            {
                LOG_INFO( "ShaderRuntime: Unable to find uniform location '{}' in {}" , uniform->getName() ,getNameAndUuidString());
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
                GLCheckError();
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        for (auto light : lightQueue)
        {
            bindLight(light);
        }
    }

    void
    ShaderRuntime::bindRuntimes
    (const vector<EntityRuntime*>& runtimes)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        static mat4 data[100];
        size_t nRuntimes = runtimes.size();
        nRuntimes = (nRuntimes > MAX_RUNTIMES ? MAX_RUNTIMES : nRuntimes);
        for (size_t i = 0; i<nRuntimes; i++)
        {
            data[i] = runtimes[i]->getTransform().getMatrix();
        }

        GLint location =  getUniformLocation("model[0]");

        if (location == UNIFORM_NOT_FOUND)
        {
            LOG_WARN( "ShaderRuntime: Unable to find model matrix uinform model[0] in {}", getNameAndUuidString());
            return;
        }
        glUniformMatrix4fv(location,runtimes.size(),GL_FALSE,(float*)&data[0]);
    }

    void
    ShaderRuntime::addMaterial
    (MaterialRuntime* material)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        // not in map
        if (find(mMaterials.begin(), mMaterials.end(), material) == mMaterials.end())
        {
            LOG_DEBUG(
                "ShaderRuntime: Adding Material {} to shader {}",
                material->getName(),
                getNameAndUuidString()
            );
            mMaterials.push_back(material);
        }
        else
        {
            LOG_DEBUG
            (
                "ShaderRuntime: Material {} already registered to shader {}",
                material->getName(),
                getNameAndUuidString()
            );
        }
    }

    void
    ShaderRuntime::logMaterials
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
       LOG_DEBUG("ShaderRuntime: Materials for {}",getNameAndUuidString());
       for (auto material : mMaterials)
       {
           LOG_DEBUG("\t{}",material->getName());
           material->logMeshes();
       }
    }

    void
    ShaderRuntime::drawGeometryPass
    (Camera* camera)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mDirectionalLightCountLocation = directionalLightCountLocation;
    }

    void
    ShaderRuntime::setFontPositionUniform(const vec2& pos)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
       GLuint location = getUniformLocation("uModel");
       mat4 model(1.f);
       model = glm::translate(model,vec3(pos.x,pos.y,0.f));

       if (location == UNIFORM_NOT_FOUND)
       {
            LOG_ERROR( "ShaderRuntime: Unable to find Font model matrix uinform \"uModel\" in shader {}" ,getNameAndUuidString()  );
            assert(false);
            return;
       }

       glUniformMatrix4fv(location,1,GL_FALSE,value_ptr(model));
    }

    void
    ShaderRuntime::setFontColorUniform(const Vector3& color)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
       GLuint location = getUniformLocation("uColor");
       vec3 glm_val = color.toGLM();
       if (location == UNIFORM_NOT_FOUND)
       {
            LOG_ERROR( "ShaderRuntime: Unable to find Font model matrix uinform \"uColor\" in shader {}" ,getNameAndUuidString());
            assert(false);
            return;
       }
       glUniform3fv(location,1,value_ptr(glm_val));
    }


    void ShaderRuntime::setFontProjection(const mat4& proj)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        GLuint location = getUniformLocation("uProjection");

       if (location == UNIFORM_NOT_FOUND)
       {
            LOG_ERROR( "ShaderRuntime: Unable to find Font projection matrix uinform \"uColor\" in shader {}" ,getNameAndUuidString());
            assert(false);
            return;
       }

       glUniformMatrix4fv(location,1,GL_FALSE,value_ptr(proj));

    }

    bool ShaderRuntime::performFragmentCompilation()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_TRACE("ShaderRuntime: {}",__FUNCTION__);
        GLint success;
        GLchar infoLog[ERROR_BUF_SZ];
        // Fragment Shader
        setFragmentShader(glCreateShader(GL_FRAGMENT_SHADER));
        string fs = getFragmentSource().c_str();

        const char *fSource = fs.c_str();
        glShaderSource(getFragmentShader(), 1, &fSource, nullptr);
        glCompileShader(getFragmentShader());
        // Print compile errors if any
        glGetShaderiv(getFragmentShader(), GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(getFragmentShader(), ERROR_BUF_SZ, nullptr, infoLog);
            LOG_CRITICAL( "ShaderRuntime:\n"
                       "\tFRAGMENT SHADER COMPILATION FAILED\n"
                       "\tShaderRuntime: {}\n"
                       "\t{}",
                       getNameAndUuidString(),
                       infoLog );
            glDeleteShader(getFragmentShader());
            setFragmentShader(0);
            return false;
        }
        LOG_TRACE("ShaderRuntime: Fragment compile successful");
        return true;
    }

	bool ShaderRuntime::performVertexCompilation()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_TRACE("ShaderRuntime: {}",__FUNCTION__);
        GLint success;
        GLchar infoLog[ERROR_BUF_SZ];
        // Vertex Shader
        setVertexShader(glCreateShader(GL_VERTEX_SHADER));
        string vs = getVertexSource().c_str();

        const char *vSource = vs.c_str();
        glShaderSource(getVertexShader(), 1, &vSource, nullptr);
        glCompileShader(getVertexShader());
        // Print compile errors if any
        glGetShaderiv(getVertexShader(), GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(getVertexShader(), ERROR_BUF_SZ, nullptr, infoLog);
            LOG_CRITICAL( "ShaderCompileVertexTask:\n"
                       "\tVERTEX SHADER COMPILATION FAILED\n"
                       "\tShaderRuntime: {}\n"
                       "\t{}",
                       getNameAndUuidString(),
                       infoLog );
            glDeleteShader(getVertexShader());
            setVertexShader(0);
            return false;
        }
        LOG_TRACE("ShaderRuntime: Vertex compile successful");
        return true;
    }

	bool ShaderRuntime::performLinking()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_TRACE("ShaderRuntime: {}",__FUNCTION__);
        if (getVertexShader() != 0 && getFragmentShader() != 0)
        {
            GLint success;

            // Create Shader Program
            setShaderProgram(glCreateProgram());
            if (getShaderProgram() == 0)
            {
                LOG_CRITICAL( "ShaderRuntime: Linking:\n"
                           "\tSHADER LINKING FAILED\n"
                           "\tShaderRuntime: {}\n",
                           getNameAndUuidString());

                return false;
            }

            glAttachShader(getShaderProgram(), getVertexShader());
            glAttachShader(getShaderProgram(), getFragmentShader());
            glLinkProgram(getShaderProgram());

            // Print linking errors if any
            glGetProgramiv(getShaderProgram(), GL_LINK_STATUS, &success);
            GLchar infoLog[ERROR_BUF_SZ];
            if (!success)
            {
                glGetProgramInfoLog(getShaderProgram(), ERROR_BUF_SZ, nullptr, infoLog);
                LOG_CRITICAL("ShaderLinkTask: SHADER PROGRAM LINKING FAILED\n {}" , infoLog );
                glDeleteProgram(getShaderProgram());
                setShaderProgram(0);
                return false;
            }

            // Delete the shaders as they're linked into our program now and no longer necessery
            glDeleteShader(getVertexShader());
            glDeleteShader(getFragmentShader());

            setLoaded(getShaderProgram() != 0);

            if (getLoaded())
            {
                setPointLightCountLocation(
                            glGetUniformLocation(getShaderProgram(),
                                                 ShaderRuntime::UNIFORM_POINT_LIGHT_COUNT));

                setSpotLightCountLocation(
                            glGetUniformLocation(getShaderProgram(),
                                                 ShaderRuntime::UNIFORM_SPOT_LIGHT_COUNT));

                setDirectionalLightCountLocation(
                            glGetUniformLocation(getShaderProgram(),
                                                 ShaderRuntime::UNIFORM_DIRECTIONAL_LIGHT_COUNT));
            }
        }
        LOG_TRACE("ShaderRuntime: Linking successful");
        return true;
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
