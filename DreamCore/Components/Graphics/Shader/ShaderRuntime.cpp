#include "ShaderRuntime.h"

#include "ShaderDefinition.h"
#include "ShaderTasks.h"
#include "Storage/StorageManager.h"
#include "Storage/File.h"
#include "Components/Graphics/GraphicsComponent.h"
#include "Components/Graphics/Material/MaterialRuntime.h"
#include "Components/Graphics/Texture/TextureRuntime.h"
#include "Entity/EntityRuntime.h"
#include "Project/ProjectRuntime.h"
#include "UniformType.h"

#define ERROR_BUF_SZ 4096

using std::make_shared;
using std::make_unique;
using std::pair;
using std::static_pointer_cast;

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
using glm::mat2;
using glm::mat3;
using glm::mat4;
using glm::mat2x3;
using glm::mat2x4;
using glm::mat3x2;
using glm::mat3x4;
using glm::mat4x2;
using glm::mat4x3;

namespace octronic::dream
{
  ShaderRuntime::ShaderRuntime
  (ProjectRuntime& rt,
   ShaderDefinition& definition)
    : SharedAssetRuntime(rt, definition),
      mShaderProgram(0),
      mNeedsRebind(true),
      mVertexCompilationFailed(false),
      mFragmentCompilationFailed(false),
      mLinkingFailed(false),
      mVertexSource(""),
      mVertexShader(0),
      mFragmentSource(""),
      mFragmentShader(0)
  {
    LOG_TRACE( "ShaderRuntime: Constructing Object" );
    mRuntimeMatricies.reserve(MAX_RUNTIMES);
    mCompileFragmentTask = make_shared<ShaderCompileFragmentTask>(getProjectRuntime(), *this);
    mCompileVertexTask = make_shared<ShaderCompileVertexTask>(getProjectRuntime(), *this);
    mLinkTask = make_shared<ShaderLinkTask>(getProjectRuntime(), *this);
    mFreeTask = make_shared<ShaderFreeTask>(getProjectRuntime());
  }

  ShaderRuntime::~ShaderRuntime
  ()
  {
    LOG_TRACE( "ShaderRuntime: Destroying Object" );
    mFreeTask->setShaderProgram(mShaderProgram);
    auto& gfxComp = getProjectRuntime().getGraphicsComponent();
    auto& gfxDestructionQueue = gfxComp.getDestructionTaskQueue();
    gfxDestructionQueue.pushTask(mFreeTask);
  }

  // Loading =================================================================

  bool
  ShaderRuntime::loadFromDefinition
  ()
  {
    if (!readVertexSource() || !readFragmentSource())
    {
      return false;
    }
    return true;
  }

  bool
  ShaderRuntime::readVertexSource
  ()
  {
    // 1. Open Shader Files into Memory
    auto& sm = getProjectRuntime().getStorageManager();
    auto& projectDir = getProjectRuntime().getProjectDirectory();
    auto& shaderDef = static_cast<ShaderDefinition&>(getDefinition());

#if defined (GL_ES_VERSION_3_0)

    string absVertexPath = projectDir.getAssetAbsolutePath(shaderDef,Constants::SHADER_GLES_VERTEX_FILE_NAME);
#else
    string absVertexPath = projectDir.getAssetAbsolutePath(shaderDef,Constants::SHADER_GLSL_VERTEX_FILE_NAME);
#endif
    auto& vertexReader = sm.openFile(absVertexPath);
    if (vertexReader.exists())
    {
      setVertexSource(vertexReader.readString());
      LOG_TRACE("ShaderRuntime: Loading Vertex Shader for {} from {}\n{}\n",
                getDefinition().getNameAndUuidString(),absVertexPath,
                mVertexSource);
      sm.closeFile(vertexReader);
      return true;
    }
    LOG_ERROR("ShaderRuntime: Vertex Shader file does not exist");
    sm.closeFile(vertexReader);
    return false;
  }

  bool
  ShaderRuntime::readFragmentSource
  ()
  {
    // 1. Open Shader Files into Memory
    auto& sm = getProjectRuntime().getStorageManager();
    auto& projectDir = getProjectRuntime().getProjectDirectory();
    auto& shaderDef = static_cast<ShaderDefinition&>(getDefinition());
#if defined (GL_ES_VERSION_3_0)
    string absFragmentPath = projectDir.getAssetAbsolutePath(shaderDef,Constants::SHADER_GLES_FRAGMENT_FILE_NAME);
#else
    string absFragmentPath = projectDir.getAssetAbsolutePath(shaderDef,Constants::SHADER_GLSL_FRAGMENT_FILE_NAME);
#endif
    auto& fragmentReader = sm.openFile(absFragmentPath);
    if (fragmentReader.exists())
    {
      setFragmentSource(fragmentReader.readString());
      LOG_TRACE("ShaderRuntime: Loading Fragment Shader for {} from {}\n{}\n",
                getDefinition().getNameAndUuidString(),absFragmentPath,
                mFragmentSource);

      // 2. Push a Fragment Compile Task
      sm.closeFile(fragmentReader);
      return true;
    }
    LOG_ERROR("ShaderRuntime: Fragment Shader file does not exist");
    sm.closeFile(fragmentReader);
    return false;
  }

  bool
  ShaderRuntime::performFragmentCompilation
  ()
  {
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
      mFragmentCompilationFailed = true;
      return false;
    }
    LOG_TRACE("ShaderRuntime: Fragment compile successful");
    return true;
  }

  bool
  ShaderRuntime::performVertexCompilation
  ()
  {
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
      LOG_CRITICAL( "ShaderRuntime:\n"
                       "\tVERTEX SHADER COMPILATION FAILED\n"
                       "\tShaderRuntime: {}\n"
                       "\t{}",
                    getNameAndUuidString(),
                    infoLog );
      glDeleteShader(getVertexShader());
      setVertexShader(0);
      mVertexCompilationFailed = true;
      return false;
    }
    LOG_TRACE("ShaderRuntime: Vertex compile successful");
    return true;
  }

  bool
  ShaderRuntime::performLinking
  ()
  {
    LOG_TRACE("ShaderRuntime: {}",__FUNCTION__);
    if (getVertexShader() != 0 && getFragmentShader() != 0)
    {
      GLint success;

      // Create Shader Program
      setShaderProgram(glCreateProgram());
      if (getShaderProgram() == 0)
      {
        LOG_CRITICAL("ShaderRuntime: Linking:\n"
                             "\tSHADER LINKING FAILED\n"
                             "\tShaderRuntime: {}\n",
                     getNameAndUuidString());
        mLinkingFailed = true;
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
        LOG_CRITICAL("ShaderLinkTask: {} SHADER PROGRAM LINKING FAILED\n {}" , getNameAndUuidString(), infoLog );
        glDeleteProgram(getShaderProgram());
        setShaderProgram(0);
        mLinkingFailed = true;
        return false;
      }

      // Delete the shaders as they're linked into our program now and no longer necessery
      glDeleteShader(getVertexShader());
      mVertexShader = 0;
      glDeleteShader(getFragmentShader());
      mFragmentShader = 0;

      setLoaded(getShaderProgram() != 0);

      if (getLoaded())
      {
        LOG_TRACE("ShaderRuntime: Linking successful");
      }
    }
    return mLoaded;
  }

  size_t
  ShaderRuntime::countMaterials
  ()
  const
  {
    return mMaterials.size();
  }

  // Uniforms ================================================================

  void
  ShaderRuntime::setModelMatrixUniform
  (mat4 value)
  {
    addUniform(UNIFORM_TYPE_MATRIX4, UNIFORM_MODEL_MATRIX,1, &value);
  }

  void
  ShaderRuntime::setViewMatrixUniform
  (mat4 value)
  {
    addUniform(UNIFORM_TYPE_MATRIX4, UNIFORM_VIEW_MATRIX,1, &value);
  }

  void
  ShaderRuntime::setProjectionMatrixUniform
  (mat4 value)
  {
    addUniform(UNIFORM_TYPE_MATRIX4, UNIFORM_PROJECTION_MATRIX,1, &value);
  }

  void
  ShaderRuntime::setCameraPositionUniform
  (vec3 value)
  {
    addUniform(UNIFORM_TYPE_FLOAT3, UNIFORM_CAMERA_POSITION,1, &value);
  }

  void
  ShaderRuntime::setColorUniform
  (vec4 color)
  {
    addUniform(UNIFORM_TYPE_FLOAT4, UNIFORM_COLOR, 1, &color);
  }


  void
  ShaderRuntime::setEquirectangularMapUniform
  (GLint map)
  {
    addUniform(UNIFORM_TYPE_INT1, UNIFORM_EQUIRECTANGULAR_MAP, 1, &map);
  }

  void
  ShaderRuntime::setEnvironmentMapUniform
  (GLint map)
  {
    addUniform(UNIFORM_TYPE_INT1, UNIFORM_ENVIRONMENT_MAP, 1, &map);
  }

  void
  ShaderRuntime::setRoughnessUniform
  (float r)
  {
    addUniform(UNIFORM_TYPE_FLOAT1, UNIFORM_ROUGHNESS_VALUE, 1, &r);
  }

  void ShaderRuntime::setIrradianceTextureUniform(GLuint t)
  {
    GLuint tex_id = 5;
    addUniform(UNIFORM_TYPE_INT1, UNIFORM_IRRADIANCE_TEXTURE, 1, &tex_id);
    setTexture(GL_TEXTURE5, GL_TEXTURE_CUBE_MAP, t);
  }

  void ShaderRuntime::setPreFilterTextureUniform(GLuint t)
  {
    GLuint tex_id = 6;
    addUniform(UNIFORM_TYPE_INT1, UNIFORM_PREFILTER_TEXTURE, 1, &tex_id);
    setTexture(GL_TEXTURE6, GL_TEXTURE_CUBE_MAP, t);
  }

  void ShaderRuntime::setBrdfLutTextureUniform(GLuint t)
  {
    GLuint tex_id = 7;
    addUniform(UNIFORM_TYPE_INT1, UNIFORM_BRDF_LUT_TEXTURE, 1, &tex_id);
    setTexture(GL_TEXTURE7, GL_TEXTURE_2D, t);
  }

  GLint
  ShaderRuntime::getUniformLocation
  (const string& name)
  {
    assert(!name.empty());
    auto iter = mUniformLocationCache.find(name);
    if (iter == mUniformLocationCache.end())
    {
      GLint loc = glGetUniformLocation(mShaderProgram,name.c_str());
      mUniformLocationCache.insert(pair<string,GLint>(name,loc));
      return loc;
    }
    return (*iter).second;
  }

  void
  ShaderRuntime::addUniform
  (UniformType type, const string& name, int count, void* data)
  {
    for (auto& uniform : mUniformVector)
    {
      if (uniform->getName() == name)
      {
        LOG_INFO("ShaderRuntime: Updating uniform {}", uniform->getName());
        uniform->setData(data);
        return;
      }
    }

    LOG_INFO("ShaderRuntime: Creating uniform {}", name);
    GLint location = getUniformLocation(name);
    if (location == UNIFORM_NOT_FOUND)
    {
      LOG_ERROR("ShaderRuntime: Uniform {} not found in shader {}", name, getNameAndUuidString());
    }

    auto& newUniform = mUniformVector.emplace_back(make_unique<ShaderUniform>(type,name,count,data));
    newUniform->setLocation(location);
  }

  void
  ShaderRuntime::syncUniforms
  ()
  {
    LOG_DEBUG("ShaderRuntime: Synchronising uniforms for {}",getNameAndUuidString());
    GLuint prog = getShaderProgram();

    // Sync user uniforms
    for (auto& uniform : mUniformVector)
    {
      if (!uniform->getNeedsUpdate())
      {
        LOG_TRACE("ShaderRuntime: Uniform {} does not need update",uniform->getName());
        continue;
      }
      LOG_TRACE("ShaderRuntime: Uniform {} needs update",uniform->getName());

      if (uniform->getCount() == 0)
      {
        continue;
      }
      LOG_TRACE("ShaderRuntime: Sync Uinform {} -> prog: {}, name: {}, loc: {}, count: {}",
                getUuid(),
                prog, uniform->getName(),
                uniform->getLocation(),
                uniform->getCount());

      auto location = uniform->getLocation();
      if (location == UNIFORM_NOT_FOUND)
      {
        LOG_INFO("ShaderRuntime: Unable to find uniform location '{}' in {}",
                 uniform->getName(), getNameAndUuidString());
        continue;
      }

      assert(uniform->getData() != nullptr);

      switch (uniform->getType())
      {
        // int =====================================================
        case UNIFORM_TYPE_INT1:
        {
          GLint d = *(GLint*)uniform->getData();
          glUniform1i(location,d);
          GLCheckError();
          break;
        }
        case UNIFORM_TYPE_INT2:
          glUniform2iv(location,uniform->getCount(),static_cast<GLint*>(uniform->getData()));
          GLCheckError();
          break;
        case UNIFORM_TYPE_INT3:
          glUniform3iv(location,uniform->getCount(),static_cast<GLint*>(uniform->getData()));
          GLCheckError();
          break;
        case UNIFORM_TYPE_INT4:
          glUniform4iv(location,uniform->getCount(),static_cast<GLint*>(uniform->getData()));
          GLCheckError();
          break;
          // uint ====================================================
        case UNIFORM_TYPE_UINT1:
          glUniform1ui(location,*static_cast<GLuint*>(uniform->getData()));
          GLCheckError();
          break;
        case UNIFORM_TYPE_UINT2:
          glUniform2uiv(location,uniform->getCount(),static_cast<GLuint*>(uniform->getData()));
          GLCheckError();
          break;
        case UNIFORM_TYPE_UINT3:
          glUniform3uiv(location,uniform->getCount(),static_cast<GLuint*>(uniform->getData()));
          GLCheckError();
          break;
        case UNIFORM_TYPE_UINT4:
          glUniform4uiv(location,uniform->getCount(),static_cast<GLuint*>(uniform->getData()));
          GLCheckError();
          break;
          // float ===================================================
        case UNIFORM_TYPE_FLOAT1:
          glUniform1f(location,*static_cast<GLfloat*>(uniform->getData()));
          GLCheckError();
          break;
        case UNIFORM_TYPE_FLOAT2:
          glUniform2fv(location,uniform->getCount(),static_cast<GLfloat*>(uniform->getData()));
          GLCheckError();
          break;
        case UNIFORM_TYPE_FLOAT3:
          glUniform3fv(location,uniform->getCount(),static_cast<GLfloat*>(uniform->getData()));
          GLCheckError();
          break;
        case UNIFORM_TYPE_FLOAT4:
          glUniform4fv(location,uniform->getCount(),static_cast<GLfloat*>(uniform->getData()));
          GLCheckError();
          break;
          // Matrix ==================================================
        case UNIFORM_TYPE_MATRIX2:
          glUniformMatrix2fv(location, uniform->getCount(), GL_FALSE, static_cast<GLfloat*>(uniform->getData()));
          GLCheckError();
          break;
        case UNIFORM_TYPE_MATRIX3:
          glUniformMatrix3fv(location, uniform->getCount(), GL_FALSE, static_cast<GLfloat*>(uniform->getData()));
          GLCheckError();
          break;
        case UNIFORM_TYPE_MATRIX4:
          glUniformMatrix4fv(location, uniform->getCount(), GL_FALSE, static_cast<GLfloat*>(uniform->getData()));
          GLCheckError();
          break;
        case UNIFORM_TYPE_MATRIX2X3:
          glUniformMatrix2x3fv(location, uniform->getCount(), GL_FALSE, static_cast<GLfloat*>(uniform->getData()));
          GLCheckError();
          break;
        case UNIFORM_TYPE_MATRIX3X2:
          glUniformMatrix3x2fv(location, uniform->getCount(), GL_FALSE, static_cast<GLfloat*>(uniform->getData()));
          GLCheckError();
          break;
        case UNIFORM_TYPE_MATRIX2X4:
          glUniformMatrix2x4fv(location, uniform->getCount(), GL_FALSE, static_cast<GLfloat*>(uniform->getData()));
          GLCheckError();
          break;
        case UNIFORM_TYPE_MATRIX4X2:
          glUniformMatrix4x2fv(location, uniform->getCount(), GL_FALSE, static_cast<GLfloat*>(uniform->getData()));
          GLCheckError();
          break;
        case UNIFORM_TYPE_MATRIX3X4:
          glUniformMatrix3x4fv(location, uniform->getCount(), GL_FALSE, static_cast<GLfloat*>(uniform->getData()));
          GLCheckError();
          break;
        case UNIFORM_TYPE_MATRIX4X3:
          glUniformMatrix4x3fv(location, uniform->getCount(), GL_FALSE, static_cast<GLfloat*>(uniform->getData()));
          GLCheckError();
          break;

      }
      uniform->setNeedsUpdate(false);
    }
  }

  bool ShaderRuntime::checkUniformValue(ShaderUniform& uf)
  {
    static char namebuf[256];
    void* data = nullptr;
    int element_size = 0;

    if (uf.getLocation() == ShaderRuntime::UNIFORM_NOT_FOUND) {
      LOG_ERROR("ShaderRuntime: Check Error: Uniform not found {}",uf.getName());
      return false;
    }

    switch(uf.getType())
    {
      // int =====================================================
      case UNIFORM_TYPE_INT1:
      {
        element_size = 1;
        data = malloc(sizeof(GLint)*uf.getCount());
        for (unsigned int i=0; i<uf.getCount(); i++)
        {
          if (uf.getCount() == 1)snprintf(namebuf, 256, "%s", uf.getName().c_str());
          else                    snprintf(namebuf, 256, "%s[%d]", uf.getName().c_str(), i);
          glGetUniformiv(mShaderProgram,getUniformLocation(namebuf), (GLint*)data + i * element_size);
        }
        break;
      }
      case UNIFORM_TYPE_INT2:
      {
        element_size = 2;
        data = malloc(uf.getCount()*element_size);
        for (unsigned int i=0; i<uf.getCount(); i++)
        {
          if (uf.getCount() == 1)snprintf(namebuf, 256, "%s", uf.getName().c_str());
          else                    snprintf(namebuf, 256, "%s[%d]", uf.getName().c_str(), i);
          glGetUniformiv(mShaderProgram,getUniformLocation(namebuf), (GLint*)data + i * element_size);
        }
        break;
      }
      case UNIFORM_TYPE_INT3:
      {
        element_size = 3;
        data = malloc(uf.getCount()*element_size);
        for (unsigned int i=0; i<uf.getCount(); i++)
        {
          if (uf.getCount() == 1)snprintf(namebuf, 256, "%s", uf.getName().c_str());
          else                    snprintf(namebuf, 256, "%s[%d]", uf.getName().c_str(), i);
          glGetUniformiv(mShaderProgram,getUniformLocation(namebuf), (GLint*)data + i * element_size);
        }
        break;
      }
      case UNIFORM_TYPE_INT4:
      {
        element_size = 4;
        data = malloc(sizeof(GLint) * uf.getCount()*element_size);
        for (unsigned int i=0; i<uf.getCount(); i++)
        {
          if (uf.getCount() == 1)snprintf(namebuf, 256, "%s", uf.getName().c_str());
          else                    snprintf(namebuf, 256, "%s[%d]", uf.getName().c_str(), i);
          glGetUniformiv(mShaderProgram,getUniformLocation(namebuf), (GLint*)data + i * element_size);
        }
        break;
      }
        // uint ====================================================
      case UNIFORM_TYPE_UINT1:
      {
        element_size = 1;
        data = malloc(sizeof(GLuint) * uf.getCount());
        for (unsigned int i=0; i<uf.getCount(); i++)
        {
          if (uf.getCount() == 1)snprintf(namebuf, 256, "%s", uf.getName().c_str());
          else                    snprintf(namebuf, 256, "%s[%d]", uf.getName().c_str(), i);
          glGetUniformuiv(mShaderProgram,getUniformLocation(namebuf), (GLuint*)data + i * element_size);
        }
        break;
      }
      case UNIFORM_TYPE_UINT2:
      {
        element_size = 2;
        data = malloc(sizeof(GLuint) * uf.getCount() * element_size);
        for (unsigned int i=0; i<uf.getCount(); i++)
        {
          if (uf.getCount() == 1)snprintf(namebuf, 256, "%s", uf.getName().c_str());
          else                    snprintf(namebuf, 256, "%s[%d]", uf.getName().c_str(), i);
          glGetUniformuiv(mShaderProgram,getUniformLocation(namebuf), (GLuint*)data + i * element_size);
        }
        break;
      }
      case UNIFORM_TYPE_UINT3:
      {
        element_size = 3;
        data = malloc(sizeof(GLuint) * uf.getCount() * element_size);
        for (unsigned int i=0; i<uf.getCount(); i++)
        {
          if (uf.getCount() == 1)snprintf(namebuf, 256, "%s", uf.getName().c_str());
          else                    snprintf(namebuf, 256, "%s[%d]", uf.getName().c_str(), i);
          glGetUniformuiv(mShaderProgram,getUniformLocation(namebuf), (GLuint*)data + i * element_size);
        }
        break;
      }
      case UNIFORM_TYPE_UINT4:
      {
        element_size = 4;
        data = malloc(sizeof(GLuint) * uf.getCount() * element_size);
        for (unsigned int i=0; i<uf.getCount(); i++)
        {
          if (uf.getCount() == 1)snprintf(namebuf, 256, "%s", uf.getName().c_str());
          else                    snprintf(namebuf, 256, "%s[%d]", uf.getName().c_str(), i);
          glGetUniformuiv(mShaderProgram,getUniformLocation(namebuf), (GLuint*)data + i * element_size);
        }
        break;
      }
        // float ===================================================
      case UNIFORM_TYPE_FLOAT1:
      {
        element_size = 1;
        data = malloc(uf.getCount());
        for (unsigned int i=0; i<uf.getCount(); i++)
        {
          if (uf.getCount() == 1)snprintf(namebuf, 256, "%s", uf.getName().c_str());
          else                    snprintf(namebuf, 256, "%s[%d]", uf.getName().c_str(), i);
          glGetUniformfv(mShaderProgram,getUniformLocation(namebuf), (GLfloat*)data + i * element_size);
        }
        break;
      }
      case UNIFORM_TYPE_FLOAT2:
      {
        element_size = 2;
        data = malloc(sizeof(GLfloat) * uf.getCount() * element_size);
        for (unsigned int i=0; i<uf.getCount(); i++)
        {
          if (uf.getCount() == 1)snprintf(namebuf, 256, "%s", uf.getName().c_str());
          else                    snprintf(namebuf, 256, "%s[%d]", uf.getName().c_str(), i);
          glGetUniformfv(mShaderProgram,getUniformLocation(namebuf), (GLfloat*)data + i * element_size);
        }
        break;
      }
      case UNIFORM_TYPE_FLOAT3:
      {
        element_size = 3;
        data = malloc(sizeof(GLfloat)*uf.getCount() * element_size);
        for (unsigned int i=0; i<uf.getCount(); i++)
        {
          if (uf.getCount() == 1)snprintf(namebuf, 256, "%s", uf.getName().c_str());
          else                    snprintf(namebuf, 256, "%s[%d]", uf.getName().c_str(), i);
          glGetUniformfv(mShaderProgram,getUniformLocation(namebuf), (GLfloat*)data + i * element_size);
        }
        break;
      }
      case UNIFORM_TYPE_FLOAT4:
      {
        element_size = 4;
        data = malloc(sizeof(GLfloat) * uf.getCount() * element_size);
        for (unsigned int i=0; i<uf.getCount(); i++)
        {
          if (uf.getCount() == 1)snprintf(namebuf, 256, "%s", uf.getName().c_str());
          else                    snprintf(namebuf, 256, "%s[%d]", uf.getName().c_str(), i);
          glGetUniformfv(mShaderProgram,getUniformLocation(namebuf), (GLfloat*)data + i * element_size);
        }
        break;
      }
        // Matrix ==================================================
      case UNIFORM_TYPE_MATRIX2:
      {
        data = malloc(sizeof(mat2) * uf.getCount());
        for (unsigned int i=0; i<uf.getCount(); i++)
        {
          if (uf.getCount() == 1)snprintf(namebuf, 256, "%s", uf.getName().c_str());
          else                    snprintf(namebuf, 256, "%s[%d]", uf.getName().c_str(), i);
          glGetUniformfv(mShaderProgram,getUniformLocation(namebuf), (GLfloat*)data + sizeof(mat2) * i);
        }
        break;
      }
      case UNIFORM_TYPE_MATRIX3:
      {
        data = malloc(sizeof(mat3) * uf.getCount());
        for (unsigned int i=0; i<uf.getCount(); i++)
        {
          if (uf.getCount() == 1)snprintf(namebuf, 256, "%s", uf.getName().c_str());
          else                    snprintf(namebuf, 256, "%s[%d]", uf.getName().c_str(), i);
          glGetUniformfv(mShaderProgram,getUniformLocation(namebuf), (GLfloat*)data + sizeof(mat3) * i);
        }
        break;
      }
      case UNIFORM_TYPE_MATRIX4:
      {
        data = malloc(sizeof(mat4) * uf.getCount());
        for (unsigned int i=0; i<uf.getCount(); i++)
        {
          if (uf.getCount() == 1)snprintf(namebuf, 256, "%s", uf.getName().c_str());
          else                    snprintf(namebuf, 256, "%s[%d]", uf.getName().c_str(), i);
          glGetUniformfv(mShaderProgram,getUniformLocation(namebuf), (GLfloat*)data + sizeof(mat4) * i);
        }
        break;
      }
      case UNIFORM_TYPE_MATRIX2X3:
      {
        data = malloc(sizeof(mat2x3) * uf.getCount());
        for (unsigned int i=0; i<uf.getCount(); i++)
        {
          if (uf.getCount() == 1)snprintf(namebuf, 256, "%s", uf.getName().c_str());
          else                    snprintf(namebuf, 256, "%s[%d]", uf.getName().c_str(), i);
          glGetUniformfv(mShaderProgram,getUniformLocation(namebuf), (GLfloat*)data + sizeof(mat2x3) * i);
        }
        break;
      }
      case UNIFORM_TYPE_MATRIX3X2:
      {
        data = malloc(sizeof(mat3x2) * uf.getCount());
        for (unsigned int i=0; i<uf.getCount(); i++)
        {
          if (uf.getCount() == 1)snprintf(namebuf, 256, "%s", uf.getName().c_str());
          else                    snprintf(namebuf, 256, "%s[%d]", uf.getName().c_str(), i);
          glGetUniformfv(mShaderProgram,getUniformLocation(namebuf), (GLfloat*)data + sizeof(mat3x2) * i);
        }
        break;
      }
      case UNIFORM_TYPE_MATRIX2X4:
      {
        data = malloc(sizeof(mat2x4) * uf.getCount());
        for (unsigned int i=0; i<uf.getCount(); i++)
        {
          if (uf.getCount() == 1)snprintf(namebuf, 256, "%s", uf.getName().c_str());
          else                    snprintf(namebuf, 256, "%s[%d]", uf.getName().c_str(), i);
          glGetUniformfv(mShaderProgram,getUniformLocation(namebuf), (GLfloat*)data + sizeof(mat2x4) * i);
        }
        break;
      }
      case UNIFORM_TYPE_MATRIX4X2:
      {
        data = malloc(sizeof(mat4x2) * uf.getCount());
        for (unsigned int i=0; i<uf.getCount(); i++)
        {
          if (uf.getCount() == 1)snprintf(namebuf, 256, "%s", uf.getName().c_str());
          else                    snprintf(namebuf, 256, "%s[%d]", uf.getName().c_str(), i);
          glGetUniformfv(mShaderProgram,getUniformLocation(namebuf), (GLfloat*)data + sizeof(mat4x2) * i);
        }
        break;
      }
      case UNIFORM_TYPE_MATRIX3X4:
      {
        data = malloc(sizeof(mat3x4) * uf.getCount());
        for (unsigned int i=0; i<uf.getCount(); i++)
        {
          if (uf.getCount() == 1)snprintf(namebuf, 256, "%s", uf.getName().c_str());
          else                    snprintf(namebuf, 256, "%s[%d]", uf.getName().c_str(), i);
          glGetUniformfv(mShaderProgram,getUniformLocation(namebuf), (GLfloat*)data + sizeof(mat3x4) * i);
        }
        break;
      }
      case UNIFORM_TYPE_MATRIX4X3:
      {
        data = malloc(sizeof(mat4x3) * uf.getCount());
        for (unsigned int i=0; i<uf.getCount(); i++)
        {
          if (uf.getCount() == 1)snprintf(namebuf, 256, "%s", uf.getName().c_str());
          else                    snprintf(namebuf, 256, "%s[%d]", uf.getName().c_str(), i);
          glGetUniformfv(mShaderProgram,getUniformLocation(namebuf), (GLfloat*)data + sizeof(mat4x3) * i);
        }
        break;
      }
    }

    if (data == nullptr) return false;

    GLCheckError();
    int ret = memcmp(uf.getData(), data, uf.getDataSize());
    free(data);
    return ret == 0;
  }

  void
  ShaderRuntime::deleteUniforms
  ()
  {
    mUniformVector.clear();
    mUniformLocationCache.clear();
  }

  void
  ShaderRuntime::setShadowTextureUniform
  (GLint shadow)
  {
    addUniform(UNIFORM_TYPE_INT1,UNIFORM_SHADOW_TEXTURE ,1, &shadow);
  }

  void
  ShaderRuntime::setShadowSpaceMatrixUniform
  (mat4 ssm)
  {
    addUniform(UNIFORM_TYPE_MATRIX4, UNIFORM_SHADOW_SPACE_MATRIX, 1, & ssm);
  }

  void ShaderRuntime::setLightPositionsUniform(vec3* v, GLuint count)
  {
    addUniform(UNIFORM_TYPE_FLOAT3, UNIFORM_LIGHT_POSITIONS, count, (void*)v);
  }

  void ShaderRuntime::setLightColorsUniform(vec3* v, GLuint count)
  {
    addUniform(UNIFORM_TYPE_FLOAT3, UNIFORM_LIGHT_COLORS, count, (void*)v);
  }


  void ShaderRuntime::setTexture(GLenum pos, GLenum target, GLuint texture)
  {
    if (CurrentTextures[pos] != texture)
    {
      glActiveTexture(pos);
      glBindTexture(target, texture);
      GLCheckError();
      ShaderRuntime::CurrentTextures[pos] = texture;
    }
  }

  // Binding/Unbinding =======================================================

  bool
  ShaderRuntime::use
  ()
  {
    if (mShaderProgram != 0)
    {
      if (CurrentShaderProgram != mShaderProgram)
      {
        LOG_INFO("ShaderRuntime: Switching Shader Program from {} to {} for {}",
                 CurrentShaderProgram,mShaderProgram,getNameAndUuidString());
        glUseProgram(mShaderProgram);
        CurrentShaderProgram = mShaderProgram;
      }
      return true;
    }
    else
    {
      LOG_ERROR("ShaderRuntime: Unable to use shader {}, glProgram == 0");
    }
    return false;
  }

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

  void
  ShaderRuntime::bindMaterial
  (MaterialRuntime& material)
  {
    auto albedoOpt = material.getAlbedoTexture();
    if (albedoOpt)
    {
      TextureRuntime& albedo = albedoOpt.value().get();
      if (albedo.getLoaded())
      {
        GLuint id = albedo.getTextureID();
        if (CurrentTextures[GL_TEXTURE0] != id)
        {
          LOG_INFO("ShaderRuntime: Found Albedo Texture, binding {}",id);
          GLuint albedoIndex = 0;
          addUniform(UNIFORM_TYPE_INT1, UNIFORM_MATERIAL_ALBEDO, 1, &albedoIndex);
          setTexture(GL_TEXTURE0, GL_TEXTURE_2D, id);
        }
      }
    }

    auto normalOpt = material.getNormalTexture();
    if (normalOpt)
    {
      TextureRuntime& normal = normalOpt.value();
      if (normal.getLoaded())
      {
        GLuint id  =  normal.getTextureID();
        if (CurrentTextures[GL_TEXTURE1] != id)
        {
          LOG_INFO("ShaderRuntime: Found Normal Texture, binding {}",id);
          GLuint normalIndex = 1;
          addUniform(UNIFORM_TYPE_INT1, UNIFORM_MATERIAL_NORMAL, 1, &normalIndex);
          setTexture(GL_TEXTURE1, GL_TEXTURE_2D, id);
        }
      }
    }

    auto metallicOpt = material.getMetallicTexture();
    if (metallicOpt)
    {
      TextureRuntime& metallic = metallicOpt.value().get();
      if (metallic.getLoaded())
      {
        GLuint id = metallic.getTextureID();
        if (CurrentTextures[GL_TEXTURE2] != id)
        {
          LOG_INFO("ShaderRuntime: Found Metallic Texture, binding {}",id);
          GLuint metallicIndex = 2;
          addUniform(UNIFORM_TYPE_INT1, UNIFORM_MATERIAL_METALLIC, 1, &metallicIndex);
          setTexture(GL_TEXTURE2, GL_TEXTURE_2D, id);
        }
      }
    }

    auto roughnessOpt = material.getRoughnessTexture();
    if (roughnessOpt)
    {
      TextureRuntime& roughness = roughnessOpt.value();
      if (roughness.getLoaded())
      {
        GLuint id = roughness.getTextureID();
        if (CurrentTextures[GL_TEXTURE3] != id)
        {
          LOG_INFO("ShaderRuntime: Found Roughness Texture, binding {}",id);
          GLuint roughnessIndex = 3;
          addUniform(UNIFORM_TYPE_INT1, UNIFORM_MATERIAL_ROUGHNESS, 1, &roughnessIndex);
          setTexture(GL_TEXTURE3, GL_TEXTURE_2D, id);
        }
      }
    }

    auto aoOpt = material.getAoTexture();
    if (aoOpt)
    {
      TextureRuntime& ao = aoOpt.value().get();
      if (ao.getLoaded())
      {
        GLuint id = ao.getTextureID();
        if (CurrentTextures[GL_TEXTURE4] != id)
        {
          LOG_INFO("ShaderRuntime: Found AO Texture, binding {}",id);
          GLuint aoIndex = 4;
          addUniform(UNIFORM_TYPE_INT1, UNIFORM_MATERIAL_AO, 1, &aoIndex);
          setTexture(GL_TEXTURE4, GL_TEXTURE_2D, id);
        }
      }
    }
  }

  void
  ShaderRuntime::unbind
  ()
  {
    glUseProgram(0);
    CurrentShaderProgram = 0;
  }

  void
  ShaderRuntime::bindRuntimes
  (const vector<reference_wrapper<EntityRuntime>>& runtimes)
  {
    static mat4 data[100];
    size_t nRuntimes = runtimes.size();
    nRuntimes = (nRuntimes > MAX_RUNTIMES ? MAX_RUNTIMES : nRuntimes);
    for (size_t i = 0; i<nRuntimes; i++)
    {
      auto& rt = runtimes[i].get();
      data[i] = rt.getTransform().getMatrix();
    }

    addUniform(UNIFORM_TYPE_MATRIX4, UNIFORM_MODEL_MATRIX_ARRAY, nRuntimes, data);
  }

  void
  ShaderRuntime::addMaterial
  (MaterialRuntime& mtl)
  {
    // not in map
    auto iter = find_if(mMaterials.begin(), mMaterials.end(),
                        [&](reference_wrapper<MaterialRuntime>& next){return next.get() == mtl; });

    if (iter == mMaterials.end())
    {
      LOG_DEBUG("ShaderRuntime: Adding Material {} to shader {}", mtl.getName(), getNameAndUuidString());
      mMaterials.push_back(mtl);
    }
    else
    {
      LOG_DEBUG("ShaderRuntime: Material {} already registered to shader {}", mtl.getName(), getNameAndUuidString());
    }
  }

  void
  ShaderRuntime::logMaterials
  ()
  const
  {
    LOG_DEBUG("ShaderRuntime: Materials for {}",getNameAndUuidString());
    for (auto& mtlWrap : mMaterials)
    {
      auto& mtl = mtlWrap.get();
      LOG_DEBUG("\t{}",mtl.getName());
      mtl.logMeshes();
    }
  }

  vector<reference_wrapper<MaterialRuntime>>
  ShaderRuntime::getMaterialsVector
  ()
  const
  {
    return mMaterials;
  }

  // Drawing =================================================================

  void
  ShaderRuntime::drawShadowPass
  (ShaderRuntime& shadowPassShader)
  {
    LOG_TRACE("ShaderRuntime: Rendering ShadowPass {}",getNameAndUuidString());

    for (auto& mtlWrap : mMaterials)
    {
      auto& mtl = mtlWrap.get();
      if (mtl.countMeshes() == 0) continue;
      mtl.drawShadowPass(shadowPassShader);
    }
  }

  // Misc ====================================================================

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

  vector<reference_wrapper<ShaderUniform>>
  ShaderRuntime::getUniformsVector
  ()
  const
  {
    vector<reference_wrapper<ShaderUniform>> ret;
    for (auto& uf : mUniformVector)
    {
      ret.push_back(*uf);
    }
    return ret;
  }

  bool
  ShaderRuntime::hasVertexSource
  ()
  const
  {
    return !mVertexSource.empty();
  }

  bool
  ShaderRuntime::hasFragmentSource
  ()
  const
  {
    return !mFragmentSource.empty();
  }

  bool
  ShaderRuntime::hasVertexCompilationFailed
  ()
  const
  {
    return mVertexCompilationFailed;
  }

  bool
  ShaderRuntime::hasFragmentCompilationFailed
  ()
  const
  {
    return mFragmentCompilationFailed;
  }

  bool
  ShaderRuntime::hasLinkingFailed
  ()
  const
  {
    return mLinkingFailed;
  }

  bool
  ShaderRuntime::hasValidGLID
  ()
  const
  {
    return mShaderProgram > 0;
  }

  // Tasks ===================================================================

  void
  ShaderRuntime::pushTasks
  ()
  {
    auto& tq = getProjectRuntime().getTaskQueue();
    auto& gfxComp = getProjectRuntime().getGraphicsComponent();
    auto& gfxTaskQueue = gfxComp.getTaskQueue();
    auto& gfxDestQueue = gfxComp.getDestructionTaskQueue();

    if (mReloadFlag)
    {
      deleteUniforms();
      mFreeTask->setShaderProgram(mShaderProgram);

      mVertexSource = "";
      mFragmentSource = "";
      mReloadFlag = false;
      mLoaded = false;
      mLoadError = false;
      mFragmentCompilationFailed = false;
      mVertexCompilationFailed = false;
      mLinkingFailed = false;
      mShaderProgram = 0;
      gfxDestQueue.pushTask(mFreeTask);
      mLoadFromDefinitionTask->setState(TASK_STATE_QUEUED);
      mCompileVertexTask->setState(TASK_STATE_QUEUED);
      mCompileFragmentTask->setState(TASK_STATE_QUEUED);
      mLinkTask->setState(TASK_STATE_QUEUED);
    }
    else if (!mLoaded && !mLoadError)
    {
      if (mLoadFromDefinitionTask->hasState(TASK_STATE_QUEUED))
      {
        tq.pushTask(mLoadFromDefinitionTask);
      }
      else
      {
        if (mCompileFragmentTask->hasState(TASK_STATE_QUEUED))
        {
          gfxTaskQueue.pushTask(mCompileFragmentTask);
        }

        if (mCompileVertexTask->hasState(TASK_STATE_QUEUED))
        {
          gfxTaskQueue.pushTask(mCompileVertexTask);
        }
        else if (mCompileFragmentTask->hasState(TASK_STATE_COMPLETED) &&
                 mCompileVertexTask->hasState(TASK_STATE_COMPLETED) &&
                 mLinkTask->hasState(TASK_STATE_QUEUED))
        {
          gfxTaskQueue.pushTask(mLinkTask);
        }
      }
    }
  }

  // Statics =================================================================

  void
  ShaderRuntime::InvalidateState
  ()
  {
    CurrentTextures.clear();
    CurrentShaderProgram = 0;
    CurrentVAO = 0;
    CurrentVBO = 0;
  }

  const GLint ShaderRuntime::UNIFORM_NOT_FOUND               = -1;
  const char* ShaderRuntime::UNIFORM_MODEL_MATRIX            = "uModelMatrix";
  const char* ShaderRuntime::UNIFORM_VIEW_MATRIX             = "uViewMatrix";
  const char* ShaderRuntime::UNIFORM_PROJECTION_MATRIX       = "uProjectionMatrix";
  const char* ShaderRuntime::UNIFORM_CAMERA_POSITION         = "uCameraPosition";
  const char* ShaderRuntime::UNIFORM_COLOR                   = "uColor";
  const char* ShaderRuntime::UNIFORM_SHADOW_TEXTURE          = "uShadowTexture";
  const char* ShaderRuntime::UNIFORM_SHADOW_SPACE_MATRIX     = "uShadowSpaceMatrix";
  const char* ShaderRuntime::UNIFORM_MODEL_MATRIX_ARRAY      = "uModelMatrixArray[0]";

  // PBR Textures
  const char* ShaderRuntime::UNIFORM_MATERIAL_ALBEDO         = "uAlbedoTexture";
  const char* ShaderRuntime::UNIFORM_MATERIAL_NORMAL         = "uNormalTexture";
  const char* ShaderRuntime::UNIFORM_MATERIAL_METALLIC       = "uMetallicTexture";
  const char* ShaderRuntime::UNIFORM_MATERIAL_ROUGHNESS      = "uRoughnessTexture";
  const char* ShaderRuntime::UNIFORM_MATERIAL_AO             = "uAOTexture";

  // PBR Environment
  const char* ShaderRuntime::UNIFORM_EQUIRECTANGULAR_MAP     = "uEquirectangularMap";
  const char* ShaderRuntime::UNIFORM_ENVIRONMENT_MAP         = "uEnvironmentMap";
  const char* ShaderRuntime::UNIFORM_ROUGHNESS_VALUE         = "uRoughness";
  const char* ShaderRuntime::UNIFORM_IRRADIANCE_TEXTURE      = "uIrradianceTexture";
  const char* ShaderRuntime::UNIFORM_PREFILTER_TEXTURE       = "uPreFilterTexture";
  const char* ShaderRuntime::UNIFORM_BRDF_LUT_TEXTURE        = "uBrdfLutTexture";
  const char* ShaderRuntime::UNIFORM_LIGHT_POSITIONS         = "uLightPositions";
  const char* ShaderRuntime::UNIFORM_LIGHT_COLORS            = "uLightColors";

  const size_t ShaderRuntime::MAX_RUNTIMES = 100;

  map<GLenum,GLuint> ShaderRuntime::CurrentTextures;
  GLuint ShaderRuntime::CurrentShaderProgram = 0;
  GLuint ShaderRuntime::CurrentVAO = 0;
  GLuint ShaderRuntime::CurrentVBO = 0;
}
