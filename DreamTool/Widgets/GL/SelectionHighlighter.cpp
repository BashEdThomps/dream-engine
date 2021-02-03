#include "SelectionHighlighter.h"
#include "DreamToolContext.h"
#include <DreamCore.h>

using octronic::dream::BoundingBox;
using octronic::dream::Project;
using octronic::dream::ProjectRuntime;

namespace octronic::dream::tool
{

    SelectionHighlighter::SelectionHighlighter
    (DreamToolContext* project)
        :GLWidget (project,false),
          mSelectedEntityRuntime(nullptr),
          mSelectionColour(vec3(0.0,1.0f,0.40f)),
          mOffset(0.25f),
          mXColour(vec3(1.0f, 0.0f, 0.0f)),
          mYColour(vec3(0.0f, 1.0f, 0.0f)),
          mZColour(vec3(0.0f, 0.0f, 1.0f)),
          mOutlineOnly(true)
    {
    }

    SelectionHighlighter::~SelectionHighlighter
    ()
    {
        LOG_TRACE("SelectionHighlighter: Destructing");
    }

    void
    SelectionHighlighter::setSelectedEntity
    (EntityRuntime* selected)
    {
        mSelectedEntityRuntime = selected;
        LOG_ERROR("SelectionHighlighter: SelectedEntity changed to {}",mSelectedEntityRuntime->getNameAndUuidString());
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
        LOG_ERROR("SelectionHighlighter: Updating");
        if (mSelectedEntityRuntime == nullptr)
        {
            return;
        }

        BoundingBox bounds = mSelectedEntityRuntime->getBoundingBox();
        LOG_ERROR("SelectionHighlighter: Minimum Bounds {},{},{}",bounds.getMinimum().x() ,bounds.getMinimum().y(), bounds.getMinimum().z());
        LOG_ERROR("SelectionHighlighter: Maximum Bounds {},{},{}",bounds.getMaximum().x() ,bounds.getMaximum().y(), bounds.getMaximum().z());

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

        topFrontL.Position    = vec3(bounds.getMinimum().x()-mOffset, bounds.getMaximum().y()+mOffset, bounds.getMaximum().z()+mOffset);
        topFrontR.Position    = vec3(bounds.getMaximum().x()+mOffset, bounds.getMaximum().y()+mOffset, bounds.getMaximum().z()+mOffset);
        topBackL.Position     = vec3(bounds.getMinimum().x()-mOffset, bounds.getMaximum().y()+mOffset, bounds.getMinimum().z()-mOffset);
        topBackR.Position     = vec3(bounds.getMaximum().x()+mOffset, bounds.getMaximum().y()+mOffset, bounds.getMinimum().z()-mOffset);
        bottomFrontL.Position = vec3(bounds.getMinimum().x()-mOffset, bounds.getMinimum().y()-mOffset, bounds.getMaximum().z()+mOffset);
        bottomFrontR.Position = vec3(bounds.getMaximum().x()+mOffset, bounds.getMinimum().y()-mOffset, bounds.getMaximum().z()+mOffset);
        bottomBackL.Position  = vec3(bounds.getMinimum().x()-mOffset, bounds.getMinimum().y()-mOffset, bounds.getMinimum().z()-mOffset);
        bottomBackR.Position  = vec3(bounds.getMaximum().x()+mOffset, bounds.getMinimum().y()-mOffset, bounds.getMinimum().z()-mOffset);

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
        GLCheckError();

        glBindBuffer(GL_ARRAY_BUFFER, mVbo);
        ShaderRuntime::CurrentVBO = mVbo;
        GLCheckError();
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLint>(mVertexBuffer.size() * sizeof(GLWidgetVertex)), &mVertexBuffer[0], GL_STATIC_DRAW);
        GLCheckError();
        glBindVertexArray(0);
    }

    void SelectionHighlighter::draw()
    {
        GLCheckError();

        if (!mSelectedEntityRuntime)
        {
            return;
        }

        Project* proj = mContext->getProject();
        if (proj)
        {
            ProjectRuntime* pRuntime = proj->getRuntime();
            if (pRuntime)
            {
                SceneRuntime* sRunt = pRuntime->getActiveSceneRuntime();
                if (sRunt)
                {
                    Camera* cam = sRunt->getCamera();
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
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

            // Enable shader program
            glUseProgram(mShaderProgram);
            ShaderRuntime::CurrentShaderProgram = mShaderProgram;
            GLCheckError();

            // Vertex Array
            glBindVertexArray(mVao);
            ShaderRuntime::CurrentVAO = mVao;
            GLCheckError();

            glBindBuffer(GL_ARRAY_BUFFER, mVbo);
            ShaderRuntime::CurrentVBO = mVbo;
            GLCheckError();
            if (mProjectionUniform == -1)
            {
                LOG_ERROR("SelectionHighlighter: Unable to find Uniform Location for projection");
                return;
            }
            else
            {
                glUniformMatrix4fv(mProjectionUniform, 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));
                GLCheckError();
            }

            // Set the view matrix
            GLCheckError();
            if (mViewUniform == -1)
            {
                LOG_ERROR("SelectionHighlighter: Unable to find Uniform Location for view");
                return;
            }
            else
            {
                glUniformMatrix4fv(mViewUniform, 1, GL_FALSE, glm::value_ptr(mViewMatrix));
                GLCheckError();
            }

            mModelMatrix = mSelectedEntityRuntime->getTransform().getMatrix();
            // Set the projection matrix
            if (mModelUniform == -1)
            {
                LOG_ERROR("SelectionHighlighter: Unable to find Uniform Location for model");
                return;
            }
            else
            {
                glUniformMatrix4fv(mModelUniform, 1, GL_FALSE, glm::value_ptr(mModelMatrix));
                GLCheckError();
            }

            // Draw
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(mVertexBuffer.size()));
            GLCheckError();

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
       mSelectedEntityRuntime =  nullptr;
    }
}
