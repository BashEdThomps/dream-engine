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

namespace Dream
{
    PathDefinition::PathDefinition
    (ProjectDefinition* pd, const json& js)
        : AssetDefinition(pd,js)
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
        mJson[Constants::ASSET_ATTR_WRAP] = wrap;
    }

    bool
    PathDefinition::getWrap
    ()
    {
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
        mJson[Constants::DREAM_PATH_SPLINE_TYPE] = type;
    }

    tsBSplineType
    PathDefinition::getSplineTypeEnum
    ()
    {
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
        mJson[Constants::ASSET_ATTR_STEP_SCALAR] = scalar;
    }

    double
    PathDefinition::getStepScalar
    ()
    {
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
        int maxId = 0;
        ensureControlPointsArray();
        auto itr = mJson[Constants::ASSET_ATTR_CONTROL_POINTS].begin();
        auto end = mJson[Constants::ASSET_ATTR_CONTROL_POINTS].end();
        for (;itr != end; itr++)
        {
            int nextId = (*itr)[Constants::ASSET_ATTR_ID];
            if (nextId > maxId)
            {
               maxId = nextId;
            }
        }
        return maxId+1;
    }

    void
    PathDefinition::ensureControlPointsArray
    ()
    {
       if (!mJson[Constants::ASSET_ATTR_CONTROL_POINTS].is_array())
       {
           mJson[Constants::ASSET_ATTR_CONTROL_POINTS] = json::array();
       }
    }

    float
    PathDefinition::getVelocity
    ()
    {
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
        mJson[Constants::ASSET_ATTR_VELOCITY] = v;
    }
}
