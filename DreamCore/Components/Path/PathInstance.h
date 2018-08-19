#pragma once

#pragma once

#ifdef __APPLE__
    #include <nlohmann/json.hpp>
#else
    #include <json.hpp>
#endif


#include <memory>
#include <vector>

#include <tinysplinecpp.h>

#include "PathDefinition.h"
#include "../IAssetInstance.h"
#include "../../Common/ILoggable.h"


using namespace tinyspline;
using namespace std;
using namespace nlohmann;

namespace Dream
{
    class PathInstance : public IAssetInstance, ILoggable
    {

    public:
        PathInstance(PathDefinition*,SceneObjectRuntime*);
        ~PathInstance() override;
        bool load(string) override;

        double getUStep() const;
        void setUStep(double uStep);
        vector<vec3> getSplinePoints() const;
        void recalculate();

    private:
        void loadExtraAttributes(json) override;
        unique_ptr<BSpline> mSpline;
        double mUStep;
        vector<vec3> mSplinePoints;

    }; // End of PathInstance

} // End of Dream
