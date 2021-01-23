/*
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */

#include "PathViewer.h"

#include <DreamCore.h>


using octronic::dream::Constants;
using octronic::dream::PathDefinition;
using octronic::dream::PathRuntime;
using octronic::dream::ShaderRuntime;
using glm::quat;
using glm::mat4;
using glm::vec3;

namespace octronic::dream::tool
{
    PathViewer::PathViewer
    (DreamToolContext* state, bool visible)
        : GLWidget(state, visible),
          mSelectedColour(vec3(0.0f, 1.0f, 0.0f)),
          mPathRuntime(nullptr),
          mUnselectedColour(vec3(0.75f, 0.75f, 0.0f)),
          mCurveColour(vec3(1.0f, 0.0f, 1.0f)),
          mTangentColour(vec3(0.25f,1.0f,0.0f)),
          mDrawTangent(false),
          mNodeSize(0.25f),
          mSelectedCp(0),
          mUStep(0.05)
    {
        LOG_TRACE("PathViewer: Constructing Object");
    }

    PathViewer::~PathViewer
    ()
    {
        LOG_DEBUG("PathViewer: Destructing Object");
        clearRuntime();

    }

    void
    PathViewer::init
    ()
    {
        LOG_DEBUG("PathViewer: Initialising");
        GLWidget::init();
    }

    void
    PathViewer::setPathDefinition
    (PathDefinition* selected)
    {
        bool regen = (selected != mPathDefinition);
        mPathDefinition = selected;
        if (regen){ regenerate(); }
    }

    bool
    PathViewer::getDrawTangent
    ()
    const
    {
        return mDrawTangent;
    }

    void
    PathViewer::setDrawTangent
    (bool drawTangent)
    {
        mDrawTangent = drawTangent;
    }

    void
    PathViewer::clearRuntime
    ()
    {
        if (mPathRuntime)
        {
            delete mPathRuntime;
            mPathRuntime = nullptr;
        }
    }

    void
    PathViewer::setSelectedControlPoint
    (size_t cp)
    {
       mSelectedCp = cp;
    }

    size_t
    PathViewer::getTangentIndex
    ()
    const
    {
        return mTangentIndex;
    }

    void
    PathViewer::setSelectedTangentIndex
    (size_t tangentIndex)
    {
        mTangentIndex = tangentIndex;
    }

    void
    PathViewer::setTangentVisibility
    (bool draw)
    {
        mDrawTangent = draw;
    }

    bool
    PathViewer::getTangentVisibility
    ()
    const
    {
        return mDrawTangent;
    }

    void
    PathViewer::setPathVisible
    (bool visible)
    {
        mVisible = visible;
    }

    bool
    PathViewer::getPathVisible
    ()
    const
    {
       return mVisible;
    }

    void
    PathViewer::regenerate
    ()
    {
        if (mPathDefinition == nullptr)
        {
            LOG_DEBUG("PathViewer: No object selected");
            return;
        }
        else
        {
            if (mPathRuntime == nullptr)
            {
               mPathRuntime = new PathRuntime(mPathDefinition,nullptr);
            }
        }

        mPathRuntime->useDefinition();

        auto controls = mPathDefinition->getControlPoints();

        mVertexBuffer.clear();

        for (auto cp : controls)
        {
            generateNode(cp);
        }

        if (controls.size() > 3)
        {
            generateSpline();
        }

        updateVertexBuffer();
    }

    void
    PathViewer::updateVertexBuffer
    ()
    {
        LOG_DEBUG("PathViewer: Updating Vertex Buffer") ;

        // Vertex Array
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

    void
    PathViewer::generateSpline
    ()
    {
        if (!mPathRuntime)
        {
            return;
        }
       vector<vec3> splines = mPathRuntime->getSplinePoints();

       for (size_t i=1; i < splines.size(); i++)
       {
           vec3 current = splines.at(i-1);
           vec3 next = splines.at(i);

           GLWidgetVertex v1,v2;
           v1.Position  = current;
           v1.Color = mCurveColour;
           v2.Position = next;
           v2.Color = mCurveColour;
           mVertexBuffer.push_back(v1);
           mVertexBuffer.push_back(v2);
       }

       auto tans = mPathRuntime->getSplineDerivatives();

       if (mDrawTangent && mTangentIndex > 0 && mTangentIndex < tans.size())
       {
           pair<vec3,vec3> tangent = tans.at(mTangentIndex);
           GLWidgetVertex v1, v2;
           v1.Position = tangent.first;
           v1.Color = vec3(1,0,0);
           v2.Position = tangent.second;
           v2.Color = vec3(0,1,0);
           mVertexBuffer.push_back(v1);
           mVertexBuffer.push_back(v2);
       }
    }

    void
    PathViewer::generateNode
    (const PathControlPoint& cp)
    {
        vec3 pos = cp.position.toGLM();
        int index = cp.index;

        LOG_TRACE("PathViewer: Generating node cube for {} at ({},{},{})",index,pos.x,pos.y,pos.z);

        vec3 colour = (mSelectedCp ==  index ? mSelectedColour : mUnselectedColour);
        LOG_TRACE("PathViewer: Selected? {}",mSelectedCp == index);

        // Top Quad

        GLWidgetVertex topFront1, topFront2;
        topFront1.Position = vec3(pos.x-mNodeSize, pos.y+mNodeSize, pos.z-mNodeSize);
        topFront1.Color = colour;
        mVertexBuffer.push_back(topFront1);
        topFront2.Position = vec3(pos.x+mNodeSize, pos.y+mNodeSize, pos.z-mNodeSize);
        topFront2.Color = colour;
        mVertexBuffer.push_back(topFront2);

        GLWidgetVertex topBack1, topBack2;
        topBack1.Position = vec3(pos.x-mNodeSize, pos.y+mNodeSize, pos.z+mNodeSize);
        topBack1.Color = colour;
        mVertexBuffer.push_back(topBack1);
        topBack2.Position = vec3(pos.x+mNodeSize, pos.y+mNodeSize, pos.z+mNodeSize);
        topBack2.Color = colour;
        mVertexBuffer.push_back(topBack2);

        GLWidgetVertex topLeft1, topLeft2;
        topLeft1.Position = vec3(pos.x-mNodeSize, pos.y+mNodeSize, pos.z-mNodeSize);
        topLeft1.Color = colour;
        mVertexBuffer.push_back(topLeft1);
        topLeft2.Position = vec3(pos.x-mNodeSize, pos.y+mNodeSize, pos.z+mNodeSize);
        topLeft2.Color = colour;
        mVertexBuffer.push_back(topLeft2);

        GLWidgetVertex topRight1, topRight2;
        topRight1.Position = vec3(pos.x+mNodeSize, pos.y+mNodeSize, pos.z-mNodeSize);
        topRight1.Color = colour;
        mVertexBuffer.push_back(topRight1);
        topRight2.Position = vec3(pos.x+mNodeSize, pos.y+mNodeSize, pos.z+mNodeSize);
        topRight2.Color = colour;
        mVertexBuffer.push_back(topRight2);

        // Bottom Quad

        GLWidgetVertex bottomFront1, bottomFront2;
        bottomFront1.Position = vec3(pos.x-mNodeSize, pos.y-mNodeSize, pos.z-mNodeSize);
        bottomFront1.Color = colour;
        mVertexBuffer.push_back(bottomFront1);
        bottomFront2.Position = vec3(pos.x+mNodeSize, pos.y-mNodeSize, pos.z-mNodeSize);
        bottomFront2.Color = colour;
        mVertexBuffer.push_back(bottomFront2);

        GLWidgetVertex bottomBack1, bottomBack2;
        bottomBack1.Position = vec3(pos.x-mNodeSize, pos.y-mNodeSize, pos.z+mNodeSize);
        bottomBack1.Color = colour;
        mVertexBuffer.push_back(bottomBack1);
        bottomBack2.Position = vec3(pos.x+mNodeSize, pos.y-mNodeSize, pos.z+mNodeSize);
        bottomBack2.Color = colour;
        mVertexBuffer.push_back(bottomBack2);

        GLWidgetVertex bottomLeft1, bottomLeft2;
        bottomLeft1.Position = vec3(pos.x-mNodeSize, pos.y-mNodeSize, pos.z-mNodeSize);
        bottomLeft1.Color = colour;
        mVertexBuffer.push_back(bottomLeft1);
        bottomLeft2.Position = vec3(pos.x-mNodeSize, pos.y-mNodeSize, pos.z+mNodeSize);
        bottomLeft2.Color = colour;
        mVertexBuffer.push_back(bottomLeft2);

        GLWidgetVertex bottomRight1, bottomRight2;
        bottomRight1.Position = vec3(pos.x+mNodeSize, pos.y-mNodeSize, pos.z-mNodeSize);
        bottomRight1.Color = colour;
        mVertexBuffer.push_back(bottomRight1);
        bottomRight2.Position = vec3(pos.x+mNodeSize, pos.y-mNodeSize, pos.z+mNodeSize);
        bottomRight2.Color = colour;
        mVertexBuffer.push_back(bottomRight2);

        // Verticals

        GLWidgetVertex frontLeft1, frontLeft2;
        frontLeft1.Position = vec3(pos.x-mNodeSize, pos.y-mNodeSize, pos.z-mNodeSize);
        frontLeft1.Color = colour;
        mVertexBuffer.push_back(frontLeft1);
        frontLeft2.Position = vec3(pos.x-mNodeSize, pos.y+mNodeSize, pos.z-mNodeSize);
        frontLeft2.Color = colour;
        mVertexBuffer.push_back(frontLeft2);

        GLWidgetVertex frontRight1, frontRight2;
        frontRight1.Position = vec3(pos.x+mNodeSize, pos.y-mNodeSize, pos.z-mNodeSize);
        frontRight1.Color = colour;
        mVertexBuffer.push_back(frontRight1);
        frontRight2.Position = vec3(pos.x+mNodeSize, pos.y+mNodeSize, pos.z-mNodeSize);
        frontRight2.Color = colour;
        mVertexBuffer.push_back(frontRight2);

        GLWidgetVertex backLeft1, backLeft2;
        backLeft1.Position = vec3(pos.x-mNodeSize, pos.y-mNodeSize, pos.z+mNodeSize);
        backLeft1.Color = colour;
        mVertexBuffer.push_back(backLeft1);
        backLeft2.Position = vec3(pos.x-mNodeSize, pos.y+mNodeSize, pos.z+mNodeSize);
        backLeft2.Color = colour;
        mVertexBuffer.push_back(backLeft2);

        GLWidgetVertex backRight1, backRight2;
        backRight1.Position = vec3(pos.x+mNodeSize, pos.y-mNodeSize, pos.z+mNodeSize);
        backRight1.Color = colour;
        mVertexBuffer.push_back(backRight1);
        backRight2.Position = vec3(pos.x+mNodeSize, pos.y+mNodeSize, pos.z+mNodeSize);
        backRight2.Color = colour;
        mVertexBuffer.push_back(backRight2);
    }
}
