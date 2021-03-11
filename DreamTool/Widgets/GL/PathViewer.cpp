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
          mSelectedColour(0.0f, 1.0f, 0.0f,1.f),
          mPathRuntime(nullptr),
          mUnselectedColour(0.75f, 0.75f, 0.0f,1.f),
          mCurveColour(1.0f, 0.0f, 1.0f,1.f),
          mTangentColour(0.25f,1.0f,0.0f,1.f),
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
        if (regen)
        {
            regenerate();
        }
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

        auto controls = mPathDefinition->getControlPoints();

        clearLineVertexBuffer();
        clearPointVertexBuffer();
        clearTriangleVertexBuffer();

        for (auto cp : controls)
        {
            generateNode(cp);
        }

        if (controls.size() > 3)
        {
            generateSpline();
        }

        submitLineVertexBuffer();
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

           TranslationColorVertex v1,v2;
           v1.Translation  = current;
           v1.Color = mCurveColour;
           v2.Translation = next;
           v2.Color = mCurveColour;
           addLineVertex(v1);
           addLineVertex(v2);
       }

       auto tans = mPathRuntime->getSplineDerivatives();

       if (mDrawTangent && mTangentIndex > 0 && mTangentIndex < tans.size())
       {
           pair<vec3,vec3> tangent = tans.at(mTangentIndex);
           TranslationColorVertex v1, v2;
           v1.Translation = tangent.first;
           v1.Color = vec4(1,0,0,1);
           v2.Translation = tangent.second;
           v2.Color = vec4(0,1,0,1);
           addLineVertex(v1);
           addLineVertex(v2);
       }
    }

    void
    PathViewer::generateNode
    (const PathControlPoint& cp)
    {
        vec3 pos = cp.position;
        int index = cp.index;

        LOG_TRACE("PathViewer: Generating node cube for {} at ({},{},{})",index,pos.x,pos.y,pos.z);

        vec4 colour = (mSelectedCp ==  index ? mSelectedColour : mUnselectedColour);
        LOG_TRACE("PathViewer: Selected? {}",mSelectedCp == index);

        // Top Quad

        TranslationColorVertex topFront1, topFront2;
        topFront1.Translation = vec3(pos.x-mNodeSize, pos.y+mNodeSize, pos.z-mNodeSize);
        topFront1.Color = colour;
        addLineVertex(topFront1);
        topFront2.Translation = vec3(pos.x+mNodeSize, pos.y+mNodeSize, pos.z-mNodeSize);
        topFront2.Color = colour;
        addLineVertex(topFront2);

        TranslationColorVertex topBack1, topBack2;
        topBack1.Translation = vec3(pos.x-mNodeSize, pos.y+mNodeSize, pos.z+mNodeSize);
        topBack1.Color = colour;
        addLineVertex(topBack1);
        topBack2.Translation = vec3(pos.x+mNodeSize, pos.y+mNodeSize, pos.z+mNodeSize);
        topBack2.Color = colour;
        addLineVertex(topBack2);

        TranslationColorVertex topLeft1, topLeft2;
        topLeft1.Translation = vec3(pos.x-mNodeSize, pos.y+mNodeSize, pos.z-mNodeSize);
        topLeft1.Color = colour;
        addLineVertex(topLeft1);
        topLeft2.Translation = vec3(pos.x-mNodeSize, pos.y+mNodeSize, pos.z+mNodeSize);
        topLeft2.Color = colour;
        addLineVertex(topLeft2);

        TranslationColorVertex topRight1, topRight2;
        topRight1.Translation = vec3(pos.x+mNodeSize, pos.y+mNodeSize, pos.z-mNodeSize);
        topRight1.Color = colour;
        addLineVertex(topRight1);
        topRight2.Translation = vec3(pos.x+mNodeSize, pos.y+mNodeSize, pos.z+mNodeSize);
        topRight2.Color = colour;
        addLineVertex(topRight2);

        // Bottom Quad

        TranslationColorVertex bottomFront1, bottomFront2;
        bottomFront1.Translation = vec3(pos.x-mNodeSize, pos.y-mNodeSize, pos.z-mNodeSize);
        bottomFront1.Color = colour;
        addLineVertex(bottomFront1);
        bottomFront2.Translation = vec3(pos.x+mNodeSize, pos.y-mNodeSize, pos.z-mNodeSize);
        bottomFront2.Color = colour;
        addLineVertex(bottomFront2);

        TranslationColorVertex bottomBack1, bottomBack2;
        bottomBack1.Translation = vec3(pos.x-mNodeSize, pos.y-mNodeSize, pos.z+mNodeSize);
        bottomBack1.Color = colour;
        addLineVertex(bottomBack1);
        bottomBack2.Translation = vec3(pos.x+mNodeSize, pos.y-mNodeSize, pos.z+mNodeSize);
        bottomBack2.Color = colour;
        addLineVertex(bottomBack2);

        TranslationColorVertex bottomLeft1, bottomLeft2;
        bottomLeft1.Translation = vec3(pos.x-mNodeSize, pos.y-mNodeSize, pos.z-mNodeSize);
        bottomLeft1.Color = colour;
        addLineVertex(bottomLeft1);
        bottomLeft2.Translation = vec3(pos.x-mNodeSize, pos.y-mNodeSize, pos.z+mNodeSize);
        bottomLeft2.Color = colour;
        addLineVertex(bottomLeft2);

        TranslationColorVertex bottomRight1, bottomRight2;
        bottomRight1.Translation = vec3(pos.x+mNodeSize, pos.y-mNodeSize, pos.z-mNodeSize);
        bottomRight1.Color = colour;
        addLineVertex(bottomRight1);
        bottomRight2.Translation = vec3(pos.x+mNodeSize, pos.y-mNodeSize, pos.z+mNodeSize);
        bottomRight2.Color = colour;
        addLineVertex(bottomRight2);

        // Verticals

        TranslationColorVertex frontLeft1, frontLeft2;
        frontLeft1.Translation = vec3(pos.x-mNodeSize, pos.y-mNodeSize, pos.z-mNodeSize);
        frontLeft1.Color = colour;
        addLineVertex(frontLeft1);
        frontLeft2.Translation = vec3(pos.x-mNodeSize, pos.y+mNodeSize, pos.z-mNodeSize);
        frontLeft2.Color = colour;
        addLineVertex(frontLeft2);

        TranslationColorVertex frontRight1, frontRight2;
        frontRight1.Translation = vec3(pos.x+mNodeSize, pos.y-mNodeSize, pos.z-mNodeSize);
        frontRight1.Color = colour;
        addLineVertex(frontRight1);
        frontRight2.Translation = vec3(pos.x+mNodeSize, pos.y+mNodeSize, pos.z-mNodeSize);
        frontRight2.Color = colour;
        addLineVertex(frontRight2);

        TranslationColorVertex backLeft1, backLeft2;
        backLeft1.Translation = vec3(pos.x-mNodeSize, pos.y-mNodeSize, pos.z+mNodeSize);
        backLeft1.Color = colour;
        addLineVertex(backLeft1);
        backLeft2.Translation = vec3(pos.x-mNodeSize, pos.y+mNodeSize, pos.z+mNodeSize);
        backLeft2.Color = colour;
        addLineVertex(backLeft2);

        TranslationColorVertex backRight1, backRight2;
        backRight1.Translation = vec3(pos.x+mNodeSize, pos.y-mNodeSize, pos.z+mNodeSize);
        backRight1.Color = colour;
        addLineVertex(backRight1);
        backRight2.Translation = vec3(pos.x+mNodeSize, pos.y+mNodeSize, pos.z+mNodeSize);
        backRight2.Color = colour;
        addLineVertex(backRight2);
    }
}
