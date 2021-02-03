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
#include "PathDefinition.h"

#include "Common/Constants.h"

using std::unique_lock;

namespace octronic::dream
{
    PathDefinition::PathDefinition
    (ProjectDefinition* pd, const json& js)
        : AssetDefinition("PathDefinition",pd,js)
    {
    }

    PathDefinition::~PathDefinition
    ()
    {

    }

    void
    PathDefinition::setWrap
    (bool wrap)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJson[Constants::ASSET_ATTR_WRAP] = wrap;
    }

    bool
    PathDefinition::getWrap
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if (mJson[Constants::ASSET_ATTR_WRAP].is_null())
        {
            mJson[Constants::ASSET_ATTR_WRAP] = false;
        }
        return mJson[Constants::ASSET_ATTR_WRAP];
    }

    vector<PathControlPoint>
    PathDefinition::getControlPoints
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        vector<PathControlPoint> retval;
        ensureControlPointsArray();

        for (auto cpJS : mJson[Constants::ASSET_ATTR_CONTROL_POINTS])
        {
            retval.push_back(unwrapControlPoint(cpJS));
        }

        return retval;
    }

    PathControlPoint
    PathDefinition::addControlPoint
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        PathControlPoint cp;
        cp.id = nextID();
        ensureControlPointsArray();
        mJson[Constants::ASSET_ATTR_CONTROL_POINTS].push_back(wrapControlPoint(cp));
        return cp;
    }

    string
    PathDefinition::getSplineType
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if (mJson[Constants::DREAM_PATH_SPLINE_TYPE].is_null())
        {
            mJson[Constants::DREAM_PATH_SPLINE_TYPE] = Constants::DREAM_PATH_TYPE_CLAMPED;
        }
        return mJson[Constants::DREAM_PATH_SPLINE_TYPE];
    }

    void
    PathDefinition::setSplineType
    (const string& type)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJson[Constants::DREAM_PATH_SPLINE_TYPE] = type;
    }

    tsBSplineType
    PathDefinition::getSplineTypeEnum
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        string type = getSplineType();
        if (type.compare(Constants::DREAM_PATH_TYPE_OPEN) == 0)
        {
            return TS_OPENED;
        }
        else if (type.compare(Constants::DREAM_PATH_TYPE_CLAMPED) == 0)
        {
            return TS_CLAMPED;
        }
        else if (type.compare(Constants::DREAM_PATH_TYPE_BEZIER) == 0)
        {
            return TS_BEZIERS;
        }
        else
        {
            return TS_NONE;
        }
    }

    void
    PathDefinition::setStepScalar
    (double scalar)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJson[Constants::ASSET_ATTR_STEP_SCALAR] = scalar;
    }

    double
    PathDefinition::getStepScalar
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if (mJson[Constants::ASSET_ATTR_STEP_SCALAR].is_null())
        {
            mJson[Constants::ASSET_ATTR_STEP_SCALAR] = 1.0;
        }
        return mJson[Constants::ASSET_ATTR_STEP_SCALAR];
    }

    PathControlPoint
    PathDefinition::unwrapControlPoint
    (const json& js)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        PathControlPoint cp;
        cp.id = js[Constants::ASSET_ATTR_ID];
        cp.index = js[Constants::ASSET_ATTR_INDEX];
        cp.position = unwrapVector3(js[Constants::ASSET_ATTR_POSITION]);
        return cp;
    }

    json
    PathDefinition::wrapControlPoint
    (const PathControlPoint& cp)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        json js;
        js[Constants::ASSET_ATTR_POSITION] = wrapVector3(cp.position);
        js[Constants::ASSET_ATTR_ID] = cp.id;
        js[Constants::ASSET_ATTR_INDEX] = cp.index;
        return js;
    }

    void
    PathDefinition::deleteControlPoint
    (const PathControlPoint& cp)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        ensureControlPointsArray();
        auto itr = mJson[Constants::ASSET_ATTR_CONTROL_POINTS].begin();
        auto end = mJson[Constants::ASSET_ATTR_CONTROL_POINTS].end();
        for (; itr != end; itr++)
        {
            if ((*itr)[Constants::ASSET_ATTR_ID] == cp.id)
            {
                mJson[Constants::ASSET_ATTR_CONTROL_POINTS].erase(itr) ;
                return;
            }
        }
    }

    void
    PathDefinition::updateControlPoint
    (const PathControlPoint& cp)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        ensureControlPointsArray();
        auto itr = mJson[Constants::ASSET_ATTR_CONTROL_POINTS].begin();
        auto end = mJson[Constants::ASSET_ATTR_CONTROL_POINTS].end();
        for (; itr != end; itr++)
        {
            if ((*itr)[Constants::ASSET_ATTR_ID] == cp.id)
            {
                (*itr) = wrapControlPoint(cp);
                return;
            }
        }
    }

    int
    PathDefinition::nextID
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        int maxID = 0;
        ensureControlPointsArray();
        auto itr = mJson[Constants::ASSET_ATTR_CONTROL_POINTS].begin();
        auto end = mJson[Constants::ASSET_ATTR_CONTROL_POINTS].end();
        for (;itr != end; itr++)
        {
            int nextID = (*itr)[Constants::ASSET_ATTR_ID];
            if (nextID > maxID)
            {
                maxID = nextID;
            }
        }
        return maxID+1;
    }

    void
    PathDefinition::ensureControlPointsArray
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if (!mJson[Constants::ASSET_ATTR_CONTROL_POINTS].is_array())
        {
            mJson[Constants::ASSET_ATTR_CONTROL_POINTS] = json::array();
        }
    }

    float
    PathDefinition::getVelocity
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if (!mJson[Constants::ASSET_ATTR_VELOCITY].is_number())
        {
            mJson[Constants::ASSET_ATTR_VELOCITY] = 1.0f;
        }
        return mJson[Constants::ASSET_ATTR_VELOCITY];
    }

    void
    PathDefinition::setVelocity
    (float v)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mJson[Constants::ASSET_ATTR_VELOCITY] = v;
    }
}
