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
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ASSET_ATTR_WRAP] = wrap;

        } dreamElseLockFailed
    }

    bool
    PathDefinition::getWrap
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if (mJson.find(Constants::ASSET_ATTR_WRAP) == mJson.end())
            {
                mJson[Constants::ASSET_ATTR_WRAP] = false;
            }
            return mJson[Constants::ASSET_ATTR_WRAP];
        } dreamElseLockFailed
    }

    vector<PathControlPoint>
    PathDefinition::getControlPoints
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            vector<PathControlPoint> retval;
            ensureControlPointsArray();

            for (auto cpJS : mJson[Constants::ASSET_ATTR_CONTROL_POINTS])
            {
                retval.push_back(unwrapControlPoint(cpJS));
            }

            return retval;
        } dreamElseLockFailed
    }

    PathControlPoint
    PathDefinition::addControlPoint
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            PathControlPoint cp;
            cp.id = nextID();
            ensureControlPointsArray();
            mJson[Constants::ASSET_ATTR_CONTROL_POINTS].push_back(wrapControlPoint(cp));
            return cp;
        } dreamElseLockFailed
    }

    string
    PathDefinition::getSplineType
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if (mJson.find(Constants::DREAM_PATH_SPLINE_TYPE) == mJson.end())
            {
                mJson[Constants::DREAM_PATH_SPLINE_TYPE] = Constants::DREAM_PATH_TYPE_CLAMPED;
            }
            return mJson[Constants::DREAM_PATH_SPLINE_TYPE];
        } dreamElseLockFailed
    }

    void
    PathDefinition::setSplineType
    (const string& type)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::DREAM_PATH_SPLINE_TYPE] = type;
        } dreamElseLockFailed
    }

    tsBSplineType
    PathDefinition::getSplineTypeEnum
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
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
        } dreamElseLockFailed
    }

    void
    PathDefinition::setStepScalar
    (double scalar)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ASSET_ATTR_STEP_SCALAR] = scalar;
        } dreamElseLockFailed
    }

    double
    PathDefinition::getStepScalar
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if (mJson.find(Constants::ASSET_ATTR_STEP_SCALAR) == mJson.end())
            {
                mJson[Constants::ASSET_ATTR_STEP_SCALAR] = 1.0;
            }
            return mJson[Constants::ASSET_ATTR_STEP_SCALAR];
        } dreamElseLockFailed
    }

    PathControlPoint
    PathDefinition::unwrapControlPoint
    (const json& js)
    {
        if(dreamTryLock()) {
            dreamLock();
            PathControlPoint cp;
            cp.id = js[Constants::ASSET_ATTR_ID];
            cp.index = js[Constants::ASSET_ATTR_INDEX];
            cp.position = Vector3(js[Constants::ASSET_ATTR_POSITION]);
            return cp;
        } dreamElseLockFailed
    }

    json
    PathDefinition::wrapControlPoint
    (const PathControlPoint& cp)
    {
        if(dreamTryLock()) {
            dreamLock();
            json js;
            js[Constants::ASSET_ATTR_POSITION] = cp.position.toJson();
            js[Constants::ASSET_ATTR_ID] = cp.id;
            js[Constants::ASSET_ATTR_INDEX] = cp.index;
            return js;
        } dreamElseLockFailed
    }

    void
    PathDefinition::deleteControlPoint
    (const PathControlPoint& cp)
    {
        if(dreamTryLock()) {
            dreamLock();
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
        } dreamElseLockFailed
    }

    void
    PathDefinition::updateControlPoint
    (const PathControlPoint& cp)
    {
        if(dreamTryLock()) {
            dreamLock();
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
        } dreamElseLockFailed
    }

    int
    PathDefinition::nextID
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
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
        } dreamElseLockFailed
    }

    void
    PathDefinition::ensureControlPointsArray
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if (!mJson[Constants::ASSET_ATTR_CONTROL_POINTS].is_array())
            {
                mJson[Constants::ASSET_ATTR_CONTROL_POINTS] = json::array();
            }
        } dreamElseLockFailed
    }

    float
    PathDefinition::getVelocity
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            if (!mJson[Constants::ASSET_ATTR_VELOCITY].is_number())
            {
                mJson[Constants::ASSET_ATTR_VELOCITY] = 1.0f;
            }
            return mJson[Constants::ASSET_ATTR_VELOCITY];
        } dreamElseLockFailed
    }

    void
    PathDefinition::setVelocity
    (float v)
    {
        if(dreamTryLock()) {
            dreamLock();
            mJson[Constants::ASSET_ATTR_VELOCITY] = v;
        } dreamElseLockFailed
    }
}
