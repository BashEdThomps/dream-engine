#include "GLWidget.h"
#include "DreamToolContext.h"
#include <DreamCore.h>
#include <glm/gtc/matrix_transform.hpp>

using octronic::dream::Project;
using octronic::dream::CameraRuntime;
using octronic::dream::ProjectRuntime;
using octronic::dream::SceneRuntime;

namespace octronic::dream::tool
{
  GLWidget::GLWidget
  (DreamToolContext& project, bool visible)
    : DreamToolWidget (project, visible),
      mLineWidth(1.f),
      mShaderProgram(0),
      mModelUniform(-1),
      mViewUniform(-1),
      mProjectionUniform(-1),
      mLineVao(0),
      mTriangleVao(0),
      mPointVao(0),
      mLineVbo(0),
      mTriangleVbo(0),
      mPointVbo(0)
  {
  }

  void GLWidget::init()
  {
    setShaderSources();
    initShader();
    initLineBuffers();
    initPointBuffers();
    initTriangleBuffers();
  }

  GLWidget::~GLWidget()
  {
    LOG_DEBUG("GLWidget: Destroying");
    if (mLineVao       > 0) glDeleteVertexArrays(1,&mLineVao);
    if (mTriangleVao   > 0) glDeleteVertexArrays(1,&mTriangleVao);
    if (mPointVao      > 0) glDeleteVertexArrays(1,&mPointVao);
    if (mLineVbo       > 0) glDeleteBuffers(1,&mLineVbo);
    if (mTriangleVbo   > 0) glDeleteBuffers(1,&mTriangleVbo);
    if (mPointVbo      > 0) glDeleteBuffers(1,&mPointVbo);
    if (mShaderProgram > 0) glDeleteProgram(mShaderProgram);
    GLCheckError();
  }

  void GLWidget::setTransform(const Transform& tx)
  {
    mTransform = tx;
  }

  Transform GLWidget::getTransform() const
  {
    return mTransform;
  }

  void GLWidget::draw()
  {
    auto& pCtxOpt = getContext().getProjectContext();

    mat4 model(1.f);
    mat4 view(1.f);
    mat4 projection(1.f);

    if (pCtxOpt)
    {
      auto& pCtx = pCtxOpt.value();
      auto& pRuntOpt = pCtx.getProjectRuntime();

      if (pRuntOpt)
      {
        auto&pRunt = pRuntOpt.value();
        auto sRuntOpt = pRunt.getActiveSceneRuntime();

        if (sRuntOpt)
        {
          auto& sRunt = sRuntOpt.value().get();

          auto& cam = sRunt.getCameraRuntime();
          projection = cam.getProjectionMatrix();
          view = cam.getViewMatrix();
        }
        else { return; }
      }
      else { return; }
    }

    model = mTransform.getMatrix();

    glUseProgram(mShaderProgram);
    GLCheckError();

    glUniformMatrix4fv(mModelUniform,      1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(mViewUniform,       1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(mProjectionUniform, 1, GL_FALSE, glm::value_ptr(projection));
    GLCheckError();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    drawTriangleBuffer();
    drawPointBuffer();
    drawLineBuffer();

    glDisable(GL_BLEND);

    GLCheckError();
  }

  void
  GLWidget::setShaderSources
  ()
  {
    mVertexShaderSource =
        "#version 330 core\n"
            "\n"
            "layout (location = 0) in vec3 position;\n"
            "layout (location = 1) in vec4 color;\n"
            "\n"
            "out vec4 Color;\n"
            "\n"
            "uniform mat4 model;\n"
            "uniform mat4 view;\n"
            "uniform mat4 projection;\n"
            "\n"
            "void main () {\n"
            "    gl_Position = projection * view * model * vec4(position,1.0) ;\n"
            "    Color = color;\n"
            "}";

    mFragmentShaderSource =
        "#version 330 core\n"
            "\n"
            "in vec4  Color;\n"
            "\n"
            "out vec4 fragColor;\n"
            "\n"
            "void main()\n"
			"{\n"
            "    fragColor = Color;\n"
            "}";
  }

  void
  GLWidget::initShader()
  {
    LOG_DEBUG("GLWidget: Initialising Shader");

    GLuint vertexShader = 0;
    GLuint fragmentShader = 0;

    // Compile shaders
    GLint success;
    GLchar infoLog[512];

    // Vertex Shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char *vSource = mVertexShaderSource.c_str();
    glShaderSource(vertexShader, 1, &vSource, nullptr);
    glCompileShader(vertexShader);

    // Print compile errors if any
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
      LOG_ERROR("GLWidget: SHADER::VERTEX: COMPILATION_FAILED {}", infoLog);
    }

    // Fragment Shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fSource = mFragmentShaderSource.c_str();
    glShaderSource(fragmentShader, 1, &fSource, nullptr);
    glCompileShader(fragmentShader);

    // Print compile errors if any
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
      LOG_ERROR("GLWidget: SHADER::FRAGMENT: COMPILATION_FAILED {}", infoLog);
    }

    // Shader Program
    mShaderProgram = glCreateProgram();
    glAttachShader(mShaderProgram, vertexShader);
    glAttachShader(mShaderProgram, fragmentShader);
    glLinkProgram(mShaderProgram);

    // Print linking errors if any
    glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(mShaderProgram, 512, nullptr, infoLog);
      LOG_ERROR("GLWidget:  SHADER::PROGRAM: LINKING_FAILED {}", infoLog);
    }

    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Get Uniform Locations
    glUseProgram(mShaderProgram);
    mModelUniform = glGetUniformLocation(mShaderProgram, "model");
    mViewUniform = glGetUniformLocation(mShaderProgram,"view");
    mProjectionUniform = glGetUniformLocation(mShaderProgram, "projection");
    glUseProgram(0);
    GLCheckError();
  }

  bool GLWidget::initTriangleBuffers()
  {
    LOG_TRACE("GLWidget: {}", __FUNCTION__);

    // VAO
    glGenVertexArrays(1,&mTriangleVao);

    if (mTriangleVao < 0)
    {
      LOG_ERROR("GLWidget: Error creating Triangle VAO");
      return false;
    }

    glBindVertexArray(mTriangleVao);

    // VBO
    glGenBuffers(1,&mTriangleVbo);
    if (mTriangleVbo < 0)
    {
      LOG_ERROR("GLWidget: Error creating Triangle VBO");
      return false;
    }
    glBindBuffer(GL_ARRAY_BUFFER,mTriangleVbo);
    // Vertex Position Attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          static_cast<GLint>(sizeof(TranslationColorVertex)),
                          static_cast<GLvoid*>(0));
    glEnableVertexAttribArray(0);
    // Vertex Color Attributes
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
                          static_cast<GLint>(sizeof(TranslationColorVertex)),
                          (GLvoid*)offsetof(TranslationColorVertex,Color));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    GLCheckError();

    //  Final Check
    if (mTriangleVao != 0 && mTriangleVbo != 0)
    {
      LOG_INFO("GLWidget: Triangle VAO/VBO Init Successful");
      return true;
    }
    else
    {
      LOG_ERROR("GLWidget: Triangle VAO/VBO Error VAO:{} VBO:{}",mTriangleVao,mTriangleVbo);
      return false;
    }
  }

  bool GLWidget::initLineBuffers()
  {
    LOG_TRACE("GLWidget: {}", __FUNCTION__);

    // VAO
    glGenVertexArrays(1,&mLineVao);
    if (mLineVao < 0)
    {
      LOG_ERROR("GLWidget: Error creating Line VAO");
      return false;
    }

    glBindVertexArray(mLineVao);

    // VBO
    glGenBuffers(1,&mLineVbo);
    if (mLineVbo < 0)
    {
      LOG_ERROR("GLWidget: Error creating Line VBO");
      return false;
    }
    glBindBuffer(GL_ARRAY_BUFFER,mLineVbo);
    // Vertex Position Attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          static_cast<GLint>(sizeof(TranslationColorVertex)),
                          static_cast<GLvoid*>(0));
    glEnableVertexAttribArray(0);
    // Vertex Color Attributes
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
                          static_cast<GLint>(sizeof(TranslationColorVertex)),
                          (GLvoid*)offsetof(TranslationColorVertex,Color));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    GLCheckError();

    //  Final Check
    if (mLineVao != 0 && mLineVbo != 0)
    {
      LOG_INFO("GLWidget: Line VAO/VBO Init Successful");
      return true;
    }
    else
    {
      LOG_ERROR("GLWidget: Line VAO/VBO Error VAO:{} VBO:{}",mLineVao,mLineVbo);
    }
    return false;
  }

  bool GLWidget::initPointBuffers()
  {
    LOG_TRACE("GLWidget: {}", __FUNCTION__);

    // VAO
    glGenVertexArrays(1,&mPointVao);
    if (mPointVao < 0)
    {
      LOG_ERROR("GLWidget: Error creating Point VAO");
      return false;
    }

    glBindVertexArray(mPointVao);

    // VBO
    glGenBuffers(1,&mPointVbo);
    if (mLineVbo < 0)
    {
      LOG_ERROR("GLWidget: Error creating Point VBO");
      return false;
    }

    glBindBuffer(GL_ARRAY_BUFFER,mPointVbo);
    // Vertex Position Attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          static_cast<GLint>(sizeof(TranslationColorVertex)),
                          static_cast<GLvoid*>(0));
    glEnableVertexAttribArray(0);
    // Vertex Color Attributes
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
                          static_cast<GLint>(sizeof(TranslationColorVertex)),
                          (GLvoid*)offsetof(TranslationColorVertex,Color));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    GLCheckError();

    //  Final Check
    if (mPointVao != 0 && mPointVbo != 0)
    {
      LOG_INFO("GLWidget: Point VAO/VBO Init Successful");
      return true;
    }
    else
    {
      LOG_ERROR("GLWidget: Point VAO/VBO Error VAO:{} VBO:{}",mPointVao,mPointVbo);
      return false;
    }
  }

  void GLWidget::drawTriangleBuffer()
  {
    LOG_TRACE("GLWidget: {}",__FUNCTION__);
    if (!mTriangleVertexBuffer.empty())
    {
      // Vertex Array
      glBindVertexArray(mTriangleVao);
      GLCheckError();
      GLuint sz = mTriangleVertexBuffer.size();
      LOG_TRACE("GLWidget: Drawing {} Triangles", sz/3);
      assert(sz % 3 == 0);
      glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(sz));
      GLCheckError();
      glBindVertexArray(0);
      GLCheckError();
    }
  }

  void GLWidget::drawLineBuffer()
  {
    LOG_TRACE("GLWidget: {}",__FUNCTION__);
    if (!mLineVertexBuffer.empty())
    {
      glEnable(GL_LINE_SMOOTH);
      glBindVertexArray(mLineVao);
      GLuint sz = mLineVertexBuffer.size();
      LOG_TRACE("GLWidget: Drawing {} lines", sz/2);
      assert(sz % 2 == 0);
      glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(sz));
      glBindVertexArray(0);
      glDisable(GL_LINE_SMOOTH);
      GLCheckError();
    }
  }

  void GLWidget::drawPointBuffer()
  {
    LOG_TRACE("GLWidget: {}",__FUNCTION__);
    if (!mPointVertexBuffer.empty())
    {
      glBindVertexArray(mPointVao);
      GLuint sz = mPointVertexBuffer.size();
      LOG_TRACE("GLWidget: Drawing {} Points", sz);
      glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(sz));
      glBindVertexArray(0);
      GLCheckError();
    }
  }

  void GLWidget::addLineVertex(const TranslationColorVertex& lv)
  {
    mLineVertexBuffer.push_back(lv);
  }

  void GLWidget::addLineVertices(const vector<TranslationColorVertex>& lv)
  {
    mLineVertexBuffer.insert(mLineVertexBuffer.end(),lv.begin(),lv.end());
  }

  void GLWidget::addTriangleVertex(const TranslationColorVertex& lv)
  {
    mTriangleVertexBuffer.push_back(lv);
  }

  void GLWidget::addTriangleVertices(const vector<TranslationColorVertex>& lv)
  {
    mTriangleVertexBuffer.insert(mTriangleVertexBuffer.end(), lv.begin(), lv.end());
  }

  void GLWidget::addPointVertex(const TranslationColorVertex& lv)
  {
    mPointVertexBuffer.push_back(lv);
  }

  void GLWidget::addPointVertices(const vector<TranslationColorVertex>& lv)
  {
    mPointVertexBuffer.insert(mPointVertexBuffer.end(), lv.begin(), lv.end());
  }

  void GLWidget::clearLineVertexBuffer()
  {
    mLineVertexBuffer.clear();
  }

  void GLWidget::clearTriangleVertexBuffer()
  {
    mTriangleVertexBuffer.clear();
  }

  void GLWidget::clearPointVertexBuffer()
  {
    mPointVertexBuffer.clear();
  }

  void GLWidget::submitLineVertexBuffer()
  {
    LOG_TRACE("GLWidget: {}",__FUNCTION__);
    if (!mLineVertexBuffer.empty())
    {

      GLCheckError();
      // Vertex Array
      glBindVertexArray(mLineVao);
      GLCheckError();
      glBindBuffer(GL_ARRAY_BUFFER, mLineVbo);
      GLCheckError();
      assert(mLineVertexBuffer.size() % 2 == 0);
      glBufferData(GL_ARRAY_BUFFER,
                   static_cast<GLint>(mLineVertexBuffer.size() * sizeof(TranslationColorVertex)),
                   &mLineVertexBuffer[0], GL_STATIC_DRAW);
      GLCheckError();
      glBindVertexArray(0);
      GLCheckError();
    }
  }

  void GLWidget::submitTriangleVertexBuffer()
  {
    LOG_TRACE("GLWidget: {}",__FUNCTION__);
    if (!mTriangleVertexBuffer.empty())
    {
      // Vertex Array
      glBindVertexArray(mTriangleVao);
      GLCheckError();
      glBindBuffer(GL_ARRAY_BUFFER, mTriangleVbo);
      GLCheckError();
      assert(mTriangleVertexBuffer.size() % 3 == 0);
      glBufferData(GL_ARRAY_BUFFER,
                   static_cast<GLint>(mTriangleVertexBuffer.size() * sizeof(TranslationColorVertex)),
                   &mTriangleVertexBuffer[0], GL_STATIC_DRAW);
      GLCheckError();
      glBindVertexArray(0);
      GLCheckError();
    }
  }

  void GLWidget::submitPointVertexBuffer()
  {
    LOG_TRACE("GLWidget: {}",__FUNCTION__);
    if (!mPointVertexBuffer.empty())
    {
      // Vertex Array
      glBindVertexArray(mPointVao);
      glBindBuffer(GL_ARRAY_BUFFER, mPointVbo);
      glBufferData(GL_ARRAY_BUFFER,
                   static_cast<GLint>(mPointVertexBuffer.size() * sizeof(TranslationColorVertex)),
                   &mPointVertexBuffer[0], GL_STATIC_DRAW);
      glBindVertexArray(0);
      GLCheckError();
    }
  }

  float GLWidget::getLineWidth() const
  {
    return mLineWidth;
  }

  void GLWidget::setLineWidth(float lineWidth)
  {
    mLineWidth = lineWidth;
  }
}
