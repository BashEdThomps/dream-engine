#include "SelectionHighlighter.h"
#include "../../DTState.h"
#include "../../../DreamCore/Common/Constants.h"
#include "../../../DreamCore/Project/Project.h"
#include "../../../DreamCore/Project/ProjectRuntime.h"
#include "../../../DreamCore/Scene/SceneRuntime.h"
#include "../../../DreamCore/Scene/SceneObject/SceneObjectRuntime.h"
#include "../../../DreamCore/Components/Graphics/GraphicsComponent.h"
#include "../../../DreamCore/Components/Graphics/Shader/ShaderRuntime.h"

namespace DreamTool
{

    SelectionHighlighter::SelectionHighlighter
    (DTState* project)
        :GLWidget (project,false),
          mSelectedSceneObjectRuntime(nullptr),
          mSelectionColour(vec3(0.0,1.0f,0.40f)),
          mOffset(0.25f),
          mXColour(vec3(1.0f, 0.0f, 0.0f)),
          mYColour(vec3(0.0f, 1.0f, 0.0f)),
          mZColour(vec3(0.0f, 0.0f, 1.0f)),
          mOutlineOnly(true)
    {
        #ifdef DREAM_LOG
        setLogClassName("SelectionHighlighterWidget");
        getLog()->trace("Constructing");
        #endif
    }

    SelectionHighlighter::~SelectionHighlighter
    ()
    {
        #ifdef DREAM_LOG
        getLog()->trace("Destructing");
        #endif
    }

    void
    SelectionHighlighter::setSelectedSceneObject
    (SceneObjectRuntime* selected)
    {
        mSelectedSceneObjectRuntime = selected;
        #ifdef DREAM_LOG
        getLog()->error("SelectedSceneObject changed to {}",mSelectedSceneObjectRuntime->getNameAndUuidString());
        #endif
        updateGeometry();
    }

    void SelectionHighlighter::init()
    {
       GLWidget::init();
    }

    void
    SelectionHighlighter::updateGeometry
    ()
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        log->error("Updating");
        #endif
        if (mSelectedSceneObjectRuntime == nullptr)
        {
            return;
        }

        BoundingBox bounds = mSelectedSceneObjectRuntime->getBoundingBox();
        #ifdef DREAM_LOG
        log->error("Minimum Bounds {},{},{}",bounds.minimum.x() ,bounds.minimum.y(), bounds.minimum.z());
        log->error("Maximum Bounds {},{},{}",bounds.maximum.x() ,bounds.maximum.y(), bounds.maximum.z());
        #endif

        mVertexBuffer.clear();
        // Top Quad

        GLWidgetVertex
            topFrontL, topFrontR,
            topBackL, topBackR,
            bottomFrontL, bottomFrontR,
            bottomBackL, bottomBackR;

        topFrontL.Color    = mSelectionColour;
        topFrontR.Color    = mSelectionColour;
        topBackL.Color     = mSelectionColour;
        topBackR.Color     = mSelectionColour;
        bottomFrontL.Color = mSelectionColour;
        bottomFrontR.Color = mSelectionColour;
        bottomBackL.Color  = mSelectionColour;
        bottomBackR.Color  = mSelectionColour;

        topFrontL.Position    = vec3(bounds.minimum.x()-mOffset, bounds.maximum.y()+mOffset, bounds.maximum.z()+mOffset);
        topFrontR.Position    = vec3(bounds.maximum.x()+mOffset, bounds.maximum.y()+mOffset, bounds.maximum.z()+mOffset);
        topBackL.Position     = vec3(bounds.minimum.x()-mOffset, bounds.maximum.y()+mOffset, bounds.minimum.z()-mOffset);
        topBackR.Position     = vec3(bounds.maximum.x()+mOffset, bounds.maximum.y()+mOffset, bounds.minimum.z()-mOffset);
        bottomFrontL.Position = vec3(bounds.minimum.x()-mOffset, bounds.minimum.y()-mOffset, bounds.maximum.z()+mOffset);
        bottomFrontR.Position = vec3(bounds.maximum.x()+mOffset, bounds.minimum.y()-mOffset, bounds.maximum.z()+mOffset);
        bottomBackL.Position  = vec3(bounds.minimum.x()-mOffset, bounds.minimum.y()-mOffset, bounds.minimum.z()-mOffset);
        bottomBackR.Position  = vec3(bounds.maximum.x()+mOffset, bounds.minimum.y()-mOffset, bounds.minimum.z()-mOffset);

        if (mOutlineOnly)
        {
             // Top
            mVertexBuffer.push_back(topBackL);
            mVertexBuffer.push_back(topFrontR);
            mVertexBuffer.push_back(topFrontL);
            mVertexBuffer.push_back(topBackL);
            mVertexBuffer.push_back(topBackR);
            mVertexBuffer.push_back(topFrontR);

            // Bottom
            mVertexBuffer.push_back(bottomBackR);
            mVertexBuffer.push_back(bottomFrontL);
            mVertexBuffer.push_back(bottomFrontR);
            mVertexBuffer.push_back(bottomBackR);
            mVertexBuffer.push_back(bottomBackL);
            mVertexBuffer.push_back(bottomFrontL);

            // Left
            mVertexBuffer.push_back(topBackL);
            mVertexBuffer.push_back(topFrontL);
            mVertexBuffer.push_back(bottomFrontL);

            mVertexBuffer.push_back(bottomFrontL);
            mVertexBuffer.push_back(bottomBackL);
            mVertexBuffer.push_back(topBackL);

            // Right
            mVertexBuffer.push_back(bottomBackR);
            mVertexBuffer.push_back(bottomFrontR);
            mVertexBuffer.push_back(topFrontR);

            mVertexBuffer.push_back(topFrontR);
            mVertexBuffer.push_back(topBackR);
            mVertexBuffer.push_back(bottomBackR);

            // Front
            mVertexBuffer.push_back(bottomFrontR);
            mVertexBuffer.push_back(topFrontL);
            mVertexBuffer.push_back(topFrontR);

            mVertexBuffer.push_back(bottomFrontL);
            mVertexBuffer.push_back(topFrontL);
            mVertexBuffer.push_back(bottomFrontR);

            // Back
            mVertexBuffer.push_back(bottomBackL);
            mVertexBuffer.push_back(topBackR);
            mVertexBuffer.push_back(topBackL);

            mVertexBuffer.push_back(bottomBackL);
            mVertexBuffer.push_back(bottomBackR);
            mVertexBuffer.push_back(topBackR);
        }
        else
        {
            // Top
            mVertexBuffer.push_back(topFrontL);
            mVertexBuffer.push_back(topFrontR);
            mVertexBuffer.push_back(topBackL);
            mVertexBuffer.push_back(topFrontR);
            mVertexBuffer.push_back(topBackR);
            mVertexBuffer.push_back(topBackL);

            // Bottom
            mVertexBuffer.push_back(bottomFrontR);
            mVertexBuffer.push_back(bottomFrontL);
            mVertexBuffer.push_back(bottomBackR);
            mVertexBuffer.push_back(bottomFrontL);
            mVertexBuffer.push_back(bottomBackL);
            mVertexBuffer.push_back(bottomBackR);

            // Left
            mVertexBuffer.push_back(bottomFrontL);
            mVertexBuffer.push_back(topFrontL);
            mVertexBuffer.push_back(topBackL);
            mVertexBuffer.push_back(topBackL);
            mVertexBuffer.push_back(bottomBackL);
            mVertexBuffer.push_back(bottomFrontL);

            // Right
            mVertexBuffer.push_back(topFrontR);
            mVertexBuffer.push_back(bottomFrontR);
            mVertexBuffer.push_back(bottomBackR);
            mVertexBuffer.push_back(bottomBackR);
            mVertexBuffer.push_back(topBackR);
            mVertexBuffer.push_back(topFrontR);

            // Front
            mVertexBuffer.push_back(topFrontR);
            mVertexBuffer.push_back(topFrontL);
            mVertexBuffer.push_back(bottomFrontR);
            mVertexBuffer.push_back(bottomFrontR);
            mVertexBuffer.push_back(topFrontL);
            mVertexBuffer.push_back(bottomFrontL);

            // Back
            mVertexBuffer.push_back(topBackL);
            mVertexBuffer.push_back(topBackR);
            mVertexBuffer.push_back(bottomBackL);
            mVertexBuffer.push_back(topBackR);
            mVertexBuffer.push_back(bottomBackR);
            mVertexBuffer.push_back(bottomBackL);
        }

         // Buffer Data
        glBindVertexArray(mVao);
        ShaderRuntime::CurrentVAO = mVao;
        #ifdef DREAM_LOG
        checkGLError();
        #endif

        glBindBuffer(GL_ARRAY_BUFFER, mVbo);
        ShaderRuntime::CurrentVBO = mVbo;
        #ifdef DREAM_LOG
        checkGLError();
        #endif
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLint>(mVertexBuffer.size() * sizeof(GLWidgetVertex)), &mVertexBuffer[0], GL_STATIC_DRAW);
        #ifdef DREAM_LOG
        checkGLError();
        #endif
        glBindVertexArray(0);
    }

    void SelectionHighlighter::draw()
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        checkGLError();
        #endif

        if (!mSelectedSceneObjectRuntime)
        {
            return;
        }

        if (mState->project)
        {
            auto pRuntime = mState->project->getRuntime();
            if (pRuntime)
            {
                auto sRunt = pRuntime->getActiveSceneRuntime();
                if (sRunt)
                {
                    auto cam = sRunt->getCamera();
                    if (cam)
                    {
                        mProjectionMatrix = cam->getProjectionMatrix();
                        mViewMatrix = cam->getViewMatrix();
                    }
                }
            }
        }

        if (!mVertexBuffer.empty())
        {
            #ifndef __APPLE__
            glEnable(GL_LINE_SMOOTH);
            #ifdef DREAM_LOG
            checkGLError();
            #endif
            glLineWidth(3.0f);

            #ifdef DREAM_LOG
            checkGLError();
            #endif
            #endif
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

            // Enable shader program
            glUseProgram(mShaderProgram);
            ShaderRuntime::CurrentShaderProgram = mShaderProgram;
            #ifdef DREAM_LOG
            checkGLError();
            #endif

            // Vertex Array
            glBindVertexArray(mVao);
            ShaderRuntime::CurrentVAO = mVao;
            #ifdef DREAM_LOG
            checkGLError();
            #endif

            glBindBuffer(GL_ARRAY_BUFFER, mVbo);
            ShaderRuntime::CurrentVBO = mVbo;
            #ifdef DREAM_LOG
            checkGLError();
            #endif
            if (mProjectionUniform == -1)
            {
                #ifdef DREAM_LOG
                log->error("Unable to find Uniform Location for projection");
                #endif
                return;
            }
            else
            {
                glUniformMatrix4fv(mProjectionUniform, 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));
                #ifdef DREAM_LOG
                checkGLError();
                #endif
            }

            // Set the view matrix
            #ifdef DREAM_LOG
            checkGLError();
            #endif
            if (mViewUniform == -1)
            {
                #ifdef DREAM_LOG
                log->error("Unable to find Uniform Location for view");
                #endif
                return;
            }
            else
            {
                glUniformMatrix4fv(mViewUniform, 1, GL_FALSE, glm::value_ptr(mViewMatrix));
#ifdef DREAM_LOG
                checkGLError();
#endif
            }

            mModelMatrix = mSelectedSceneObjectRuntime->getTransform().getMatrix();
            // Set the projection matrix
            if (mModelUniform == -1)
            {
#ifdef DREAM_LOG
                log->error("Unable to find Uniform Location for model");
#endif
                return;
            }
            else
            {
                glUniformMatrix4fv(mModelUniform, 1, GL_FALSE, glm::value_ptr(mModelMatrix));
#ifdef DREAM_LOG
                checkGLError();
#endif
            }

            // Draw
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(mVertexBuffer.size()));
#ifdef DREAM_LOG
            checkGLError();
#endif

            // Revert State
#ifndef __APPLE__
            glDisable(GL_LINE_SMOOTH);
            glLineWidth(1.0f);

            #ifdef DREAM_LOG
            checkGLError();
            #endif
#endif
            glDisable(GL_BLEND);
        }
    }

    void
    SelectionHighlighter::setShader
    ()
    {
        mVertexShaderSource =
            "#version 330 core\n"
            "\n"
            "layout (location = 0) in vec3 position;\n"
            "layout (location = 1) in vec3 color;\n"
            "\n"
            "out vec3 Color;\n"
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
            "in vec3  Color;\n"
            "\n"
            "out vec4 fragColor;\n"
            "\n"
            "void main() { \n"
            "    fragColor = vec4(Color,0.5);\n"
            "}";
    }

    bool
    SelectionHighlighter::getOutlineOnly
    ()
    const
    {
        return mOutlineOnly;
    }

    void
    SelectionHighlighter::setOutlineOnly
    (bool outlineOnly)
    {
        mOutlineOnly = outlineOnly;
    }

    void
    SelectionHighlighter::clearSelection
    ()
    {
       mSelectedSceneObjectRuntime =  nullptr;
    }
}
