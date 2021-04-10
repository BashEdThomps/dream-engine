#pragma once

#include <memory>
#include <vector>

#include "PathDefinition.h"
#include "PathTasks.h"
#include "Components/DiscreteAssetRuntime.h"

using std::vector;
using std::pair;

namespace octronic::dream
{
    class PathRuntime : public DiscreteAssetRuntime
    {
    public:// static
        const static int SPLINE_DIMENSIONS;
        const static int SPLINE_DEGREES;
    public:
        PathRuntime(ProjectRuntime&,PathDefinition&, EntityRuntime&);
        PathRuntime(PathRuntime&&) = default;
        PathRuntime& operator=(PathRuntime&&) = default;

        bool loadFromDefinition() override;

        double getUStep() const;
        void setUStep(double uStep);
        vector<vec3> getSplinePoints() const;
        vector<mat4> getSplineTangents() const;
        vec3 getSplinePoint(unsigned int) const;
        void update();

        void generate();


        size_t getCurrentIndex() const;
        void setCurrentIndex(size_t currentIndex);

        Transform stepPath();

        bool getWrapPath() const;
        void setWrapPath(bool wrapPath);
        mat4 getHeading(const vec3& point, const vec3& v1, const vec3& v2) const;

        vector<pair<vec3,vec3>> getSplineDerivatives() const;
        void setToCurrentPoint();
        void nextPoint();

        shared_ptr<PathUpdateTask> getUpdateTask() const;

        void pushTasks() override;

    private:
        bool mWrapPath;
        size_t mCurrentIndex;
        double mUStep;
        vector<vec3> mSplinePoints;
        vector<pair<vec3,vec3> > mSplineDerivatives;
        vector<mat4> mSplineTangents;
        Transform mCurrentTransform;
        float mVelocity;
        float mDistanceToTravel;
        shared_ptr<PathUpdateTask> mUpdateTask;
    };
}
