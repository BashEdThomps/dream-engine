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
#pragma once

#include "GLWidget.h"
#include <glm/vec3.hpp>

namespace octronic::dream
{
	class PathDefinition;
	class PathRuntime;
}

using glm::vec3;

namespace octronic::dream::tool
{
    class DreamToolContext;
    class PathViewer : public GLWidget
    {
    public:
        PathViewer(DreamToolContext& state, bool visible = false);
        ~PathViewer() override;

        void setPathDefinition(PathDefinition& object);
        void init() override;

        vector<vec3> generateSplines();
        void setPathVisible(bool visible);
        bool getPathVisible() const;

        size_t getTangentIndex() const;
        void setTangentIndex( size_t& tangentIndex);

        bool getDrawTangent() const;
        void setDrawTangent(bool drawTangent);
        void clearRuntime();

        void setSelectedControlPoint(size_t);
        void setSelectedTangentIndex(size_t);

        void setTangentVisibility(bool);
        bool getTangentVisibility() const;

        void regenerate();

    private:
        optional<reference_wrapper<PathDefinition>> mPathDefinition;
        optional<reference_wrapper<PathRuntime>> mPathRuntime;
        vec4 mSelectedColour;
        vec4 mUnselectedColour;
        vec4 mCurveColour;
        vec4 mTangentColour;
        size_t mTangentIndex;
        bool mDrawTangent;
        float mNodeSize;
        size_t mSelectedCp;
        double mUStep;

        void generateSpline();
        void generateNode(const PathControlPoint& cp);
    };
}
