
#include <algorithm>

#include "PathInstance.h"

#include "../../Common/Constants.h"
#include "../../Utilities/String.h"


namespace Dream
{

    PathInstance::PathInstance
    (PathDefinition* definition, SceneObjectRuntime* runtime)
        : IAssetInstance(definition,runtime), ILoggable ("PathInstance"),
          mWrapPath(true),
          mCurrentIndex(0),
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
    (string)
    {
        auto animDef = dynamic_cast<PathDefinition*>(mDefinitionHandle);
        auto log = getLog();
        log->info(
            "Loading {} spline with {} control points for {} ",
            animDef->getSplineType(),
            animDef->numberOfControlPoints(),
            getNameAndUuidString()
        );

        if (animDef->numberOfControlPoints() < 2)
        {
            log->warn("Skipping curve, not enough control points");
            mLoaded = true;
            return true;
        }

        loadExtraAttributes(mDefinitionHandle->getJson());

        mUStep = 1.0/(animDef->numberOfControlPoints()*10);

        if (animDef->numberOfControlPoints() >= SPLINE_DIMENSIONS)
        {
            generate();
        }
        else
        {
            log->error("Not enough control points to generate spline");
        }

        mLoaded = true;
        return mLoaded;

    }

    void
    PathInstance::generate
    ()
    {
        auto animDef = dynamic_cast<PathDefinition*>(mDefinitionHandle);
        auto log = getLog();
        mSpline.reset(new BSpline(animDef->numberOfControlPoints(),3, 3, animDef->getSplineTypeEnum()));
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
        for (double u = 0.0; u <= 1.0; u += mUStep)
        {
            // Evaluate `spline` at u = 0.4 using 'evaluate'.
            result = mSpline->eval(u).result();

            log->info("Got spline point ({},{},{})",
                      result[0], result[1], result[2]
                    );
            mSplinePoints.push_back(vec3(result[0], result[1], result[2]));
        }

        log->info("Finished Loading spline for {}",getNameAndUuidString());
    }

    void PathInstance::loadExtraAttributes(nlohmann::json)
    {

    }

    bool PathInstance::getWrapPath() const
    {
        return mWrapPath;
    }

    void PathInstance::setWrapPath(bool wrapPath)
    {
        mWrapPath = wrapPath;
    }

    int PathInstance::getCurrentIndex() const
    {
        return mCurrentIndex;
    }

    void PathInstance::setCurrentIndex(int currentIndex)
    {
        mCurrentIndex = currentIndex;
    }

    glm::vec3 PathInstance::stepPath()
    {
        auto log = getLog();

        if (mSplinePoints.empty())
        {
            return vec3(0);
        }

        mCurrentIndex++;

        if (mCurrentIndex < 0)
        {
            mCurrentIndex = 0;
        }

        if (mCurrentIndex >= mSplinePoints.size())
        {
            if (mWrapPath)
            {
                mCurrentIndex = 0;
            }
            else
            {
                mCurrentIndex = mSplinePoints.size()-1;
            }
        }

        log->critical("Getting spline point {}/{}",mCurrentIndex,mSplinePoints.size());

        return mSplinePoints.at(mCurrentIndex);
    }

    vector<vec3> PathInstance::getSplinePoints() const
    {
        return mSplinePoints;
    }

    glm::vec3 PathInstance::getSplinePoint(int index) const
    {
        if (index >=0 && index < mSplinePoints.size())
        {
            return mSplinePoints.at(index);
        }
        return vec3(0);
    }

    double PathInstance::getUStep() const
    {
        return mUStep;
    }

    void PathInstance::setUStep(double uStep)
    {
        mUStep = uStep;
    }

    const int PathInstance::SPLINE_DIMENSIONS = 3;
    const int PathInstance::SPLINE_DEGREES = 3;


} // End of Dream
