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
        PathViewer(DreamToolContext& state);

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
