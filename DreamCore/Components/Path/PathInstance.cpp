
#include <algorithm>

#include "PathInstance.h"

#include "../../Common/Constants.h"
#include "../../Utilities/String.h"


namespace Dream
{

    PathInstance::PathInstance
    (PathDefinition* definition, SceneObjectRuntime* runtime)
        : IAssetInstance(definition,runtime), ILoggable ("PathInstance"),
          mUStep(0.05)
    {
        auto log = getLog();
        log->trace("Constructing Object");

    }


    PathInstance::~PathInstance
    ()
    {
        auto log = getLog();
        log->trace("Destroying Object");
    }

    void PathInstance::recalculate
    ()
    {
        load("");
    }

    bool
    PathInstance::load
    (string projectPath)
    {
        auto animDef = dynamic_cast<PathDefinition*>(mDefinitionHandle);
        auto log = getLog();
        log->info(
            "Loading spline for {} with {} control points",
            getNameAndUuidString(),
            animDef->numberOfControlPoints()
        );

        if (animDef->numberOfControlPoints() < 2)
        {
            log->warn("Skipping curve, not enough control points");
            mLoaded = true;
           return true;
        }

        loadExtraAttributes(mDefinitionHandle->getJson());
        mSpline.reset(
            new BSpline(
                animDef->numberOfControlPoints(),
                3, 3, TS_CLAMPED
            )
        );

        vector<tinyspline::real> ctrlp = mSpline->controlPoints();

        size_t i=0;
        for (json cp : *animDef->getControlPoints())
        {
            double x = cp[Constants::X];
            double y = cp[Constants::Y];
            double z = cp[Constants::Z];
            // Setup control points.
            ctrlp[i++] = x;
            ctrlp[i++] = y;
            ctrlp[i++] = z;
        }
        mSpline->setControlPoints(ctrlp);

        // Stores our evaluation results.
        vector<real> result;
        for (double u = 0.0; u <= 1.0; u+= mUStep)
        {
            // Evaluate `spline` at u = 0.4 using 'evaluate'.
            result = mSpline->eval(u).result();

            log->info("Got spline point ({},{},{})",
                result[0], result[1], result[2]
            );
            mSplinePoints.push_back(vec3(result[0], result[1], result[2]));
        }


        log->info("Finished Loading spline for {}",getNameAndUuidString());
        mLoaded = true;
        return mLoaded;
    }

    void PathInstance::loadExtraAttributes(nlohmann::json)
    {

    }

    vector<vec3> PathInstance::getSplinePoints() const
    {
        return mSplinePoints;
    }

    double PathInstance::getUStep() const
    {
        return mUStep;
    }

    void PathInstance::setUStep(double uStep)
    {
        mUStep = uStep;
    }
} // End of Dream
