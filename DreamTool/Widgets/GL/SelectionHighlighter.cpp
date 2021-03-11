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
          mOffset(0.1f),
          mSelectionColour(0.0,1.0f,0.40f,0.5f),
          mOutlineColor(0.f,0.f,0.f,1.f),
          mXColour(1.0f, 0.0f, 0.0f,1.f),
          mYColour(0.0f, 1.0f, 0.0f,1.f),
          mZColour(0.0f, 0.0f, 1.0f,1.f)
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
        LOG_INFO("SelectionHighlighter: SelectedEntity changed to {}",mSelectedEntityRuntime->getNameAndUuidString());
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
        LOG_TRACE("SelectionHighlighter: Updating");

        clearLineVertexBuffer();
        clearPointVertexBuffer();
        clearTriangleVertexBuffer();

        if (mSelectedEntityRuntime == nullptr)
        {
            return;
        }

        BoundingBox bounds = mSelectedEntityRuntime->getBoundingBox();
        vec3 min, max;
        min = bounds.getMinimum();
        max = bounds.getMaximum();
        LOG_INFO("SelectionHighlighter: Minimum Bounds {},{},{}",min.x ,min.y, min.z);
        LOG_INFO("SelectionHighlighter: Maximum Bounds {},{},{}",max.x ,max.y, max.z);

        // Top Quad

        TranslationColorVertex
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

        topFrontL.Translation    = vec3(min.x-mOffset, max.y+mOffset, max.z+mOffset);
        topFrontR.Translation    = vec3(max.x+mOffset, max.y+mOffset, max.z+mOffset);
        topBackL.Translation     = vec3(min.x-mOffset, max.y+mOffset, min.z-mOffset);
        topBackR.Translation     = vec3(max.x+mOffset, max.y+mOffset, min.z-mOffset);
        bottomFrontL.Translation = vec3(min.x-mOffset, min.y-mOffset, max.z+mOffset);
        bottomFrontR.Translation = vec3(max.x+mOffset, min.y-mOffset, max.z+mOffset);
        bottomBackL.Translation  = vec3(min.x-mOffset, min.y-mOffset, min.z-mOffset);
        bottomBackR.Translation  = vec3(max.x+mOffset, min.y-mOffset, min.z-mOffset);

        // Cube ================================================================

        // Top
        addTriangleVertex(topBackL);
        addTriangleVertex(topFrontR);
        addTriangleVertex(topFrontL);

        addTriangleVertex(topBackL);
        addTriangleVertex(topBackR);
        addTriangleVertex(topFrontR);

        // Bottom
        addTriangleVertex(bottomBackR);
        addTriangleVertex(bottomFrontL);
        addTriangleVertex(bottomFrontR);

        addTriangleVertex(bottomBackR);
        addTriangleVertex(bottomBackL);
        addTriangleVertex(bottomFrontL);

        // Left
        addTriangleVertex(topBackL);
        addTriangleVertex(topFrontL);
        addTriangleVertex(bottomFrontL);

        addTriangleVertex(bottomFrontL);
        addTriangleVertex(bottomBackL);
        addTriangleVertex(topBackL);

        // Right
        addTriangleVertex(bottomBackR);
        addTriangleVertex(bottomFrontR);
        addTriangleVertex(topFrontR);

        addTriangleVertex(topFrontR);
        addTriangleVertex(topBackR);
        addTriangleVertex(bottomBackR);

        // Front
        addTriangleVertex(bottomFrontR);
        addTriangleVertex(topFrontL);
        addTriangleVertex(topFrontR);

        addTriangleVertex(bottomFrontL);
        addTriangleVertex(topFrontL);
        addTriangleVertex(bottomFrontR);

        // Back
        addTriangleVertex(bottomBackL);
        addTriangleVertex(topBackR);
        addTriangleVertex(topBackL);

        addTriangleVertex(bottomBackL);
        addTriangleVertex(bottomBackR);
        addTriangleVertex(topBackR);

        // XYZ Lines ===========================================================


        // Outlines ============================================================

        // Vertical
        addLineVertex({bottomFrontL.Translation, mOutlineColor});
        addLineVertex({topFrontL.Translation, mOutlineColor});
        addLineVertex({bottomFrontR.Translation, mOutlineColor});
        addLineVertex({topFrontR.Translation, mOutlineColor});
        addLineVertex({bottomBackL.Translation, mOutlineColor});
        addLineVertex({topBackL.Translation, mOutlineColor});
        addLineVertex({bottomBackR.Translation, mOutlineColor});
        addLineVertex({topBackR.Translation, mOutlineColor});

        // Top
        addLineVertex({topBackR.Translation, mOutlineColor});
        addLineVertex({topBackL.Translation, mOutlineColor});
        addLineVertex({topBackL.Translation, mOutlineColor});
        addLineVertex({topFrontL.Translation, mOutlineColor});
        addLineVertex({topFrontL.Translation, mOutlineColor});
        addLineVertex({topFrontR.Translation, mOutlineColor});
        addLineVertex({topFrontR.Translation, mOutlineColor});
        addLineVertex({topBackR.Translation, mOutlineColor});

        // Bottom
        addLineVertex({bottomBackR.Translation, mOutlineColor});
        addLineVertex({bottomBackL.Translation, mOutlineColor});
        addLineVertex({bottomBackL.Translation, mOutlineColor});
        addLineVertex({bottomFrontL.Translation, mOutlineColor});
        addLineVertex({bottomFrontL.Translation, mOutlineColor});
        addLineVertex({bottomFrontR.Translation, mOutlineColor});
        addLineVertex({bottomFrontR.Translation, mOutlineColor});
        addLineVertex({bottomBackR.Translation, mOutlineColor});

        submitLineVertexBuffer();
        submitTriangleVertexBuffer();
    }

    void
    SelectionHighlighter::clearSelection
    ()
    {
        mSelectedEntityRuntime =  nullptr;
    }

    void SelectionHighlighter::draw()
    {
        Project* project = mContext->getProject();

        mat4 model(1.f);
        mat4 view(1.f);
        mat4 projection(1.f);

        if (project)
        {
            ProjectRuntime* pRuntime = project->getRuntime();
            if (pRuntime)
            {
                SceneRuntime* sRunt = pRuntime->getActiveSceneRuntime();
                if (sRunt)
                {
                    CameraRuntime* cam = sRunt->getCamera();
                    if (cam)
                    {
                        projection = cam->getProjectionMatrix();
                        view = cam->getViewMatrix();
                    }
                    else { return; }
                }
                else { return; }
            }
            else { return; }
        }

        if (mSelectedEntityRuntime)
        {
	        model = mSelectedEntityRuntime->getTransform().getMatrix();
        }
        else
        {
            model = mTransform.getMatrix();
        }

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
}
