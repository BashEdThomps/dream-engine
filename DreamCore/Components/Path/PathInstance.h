#pragma once

#pragma once

#ifdef __APPLE__
#include <nlohmann/json.hpp>
#else
#include <json.hpp>
#endif


#include <memory>
#include <vector>

#include "PathDefinition.h"
#include "../IAssetInstance.h"

using namespace tinyspline;
using namespace std;
using namespace nlohmann;

namespace Dream
{
    class PathInstance : public IAssetInstance
    {

    public:
        PathInstance(shared_ptr<PathDefinition>,shared_ptr<SceneObjectRuntime>);
        ~PathInstance() override;
        bool load(string) override;

        double getUStep() const;
        void setUStep(double uStep);
        vector<vec3> getSplinePoints() const;
        vector<quat> getSplineTangents() const;
        vec3 getSplinePoint(int) const;
        void recalculate();

        void generate();
        const static int SPLINE_DIMENSIONS;
        const static int SPLINE_DEGREES;

        size_t getCurrentIndex() const;
        void setCurrentIndex(size_t currentIndex);

        Transform3D stepPath();

        bool getWrapPath() const;
        void setWrapPath(bool wrapPath);
        quat getHeading(vec3 point, vec3 v1, vec3 v2);

        vector<pair<vec3, vec3> > getSplineDerivatives() const;

    private:
        void loadExtraAttributes(json) override;

    private:
        bool mWrapPath;
        size_t mCurrentIndex;
        double mUStep;
        vector<vec3> mSplinePoints;
        vector<pair<vec3,vec3>> mSplineDerivatives;
        vector<quat> mSplineTangents;

    }; // End of PathInstance

} // End of Dream
