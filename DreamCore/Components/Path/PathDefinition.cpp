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
#include "Math/Vector.h"

namespace octronic::dream
{
    PathDefinition::PathDefinition
    (const shared_ptr<ProjectDefinition>& pd,
     const json& js)
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
        mJson[Constants::ASSET_ATTR_WRAP] = wrap;
    }

    bool
    PathDefinition::getWrap
    ()
    const
    {
        if (mJson.find(Constants::ASSET_ATTR_WRAP) == mJson.end())
        {
            return false;
        }
        return mJson[Constants::ASSET_ATTR_WRAP];
    }

    vector<PathControlPoint>
    PathDefinition::getControlPoints
    ()
    const
    {
        vector<PathControlPoint> retval;

        if (mJson.find(Constants::ASSET_ATTR_CONTROL_POINTS) == mJson.end())
        {
            return retval;
        }

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
        if (mJson.find(Constants::ASSET_ATTR_CONTROL_POINTS) == mJson.end())
        {
            mJson[Constants::ASSET_ATTR_CONTROL_POINTS] = json::array();
        }
        mJson[Constants::ASSET_ATTR_CONTROL_POINTS].push_back(wrapControlPoint(cp));
        return cp;
    }

    string
    PathDefinition::getSplineType
    ()
    const
    {
        if (mJson.find(Constants::DREAM_PATH_SPLINE_TYPE) == mJson.end())
        {
            return Constants::DREAM_PATH_TYPE_CLAMPED;
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
    const
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
    const
    {
        if (mJson.find(Constants::ASSET_ATTR_STEP_SCALAR) == mJson.end())
        {
            return 1.f;
        }
        return mJson[Constants::ASSET_ATTR_STEP_SCALAR];
    }

    PathControlPoint
    PathDefinition::unwrapControlPoint
    (const json& js)
    const
    {
        PathControlPoint cp;
        cp.id = js[Constants::ASSET_ATTR_ID];
        cp.index = js[Constants::ASSET_ATTR_INDEX];
        cp.position = vec3(js[Constants::ASSET_ATTR_POSITION]);
        return cp;
    }

    json
    PathDefinition::wrapControlPoint
    (const PathControlPoint& cp)
    const
    {
        json js;
        js[Constants::ASSET_ATTR_POSITION] = Vector3::toJson(cp.position);
        js[Constants::ASSET_ATTR_ID] = cp.id;
        js[Constants::ASSET_ATTR_INDEX] = cp.index;
        return js;
    }

    void
    PathDefinition::deleteControlPoint
    (const PathControlPoint& cp)
    {
        for (auto itr = mJson[Constants::ASSET_ATTR_CONTROL_POINTS].begin();
             itr != mJson[Constants::ASSET_ATTR_CONTROL_POINTS].end(); itr++)
        {
            auto nextCp = (*itr);
            if (nextCp[Constants::ASSET_ATTR_ID] == cp.id)
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
        int maxID = 0;
        for (auto itr = mJson[Constants::ASSET_ATTR_CONTROL_POINTS].begin();
             itr != mJson[Constants::ASSET_ATTR_CONTROL_POINTS].end(); itr++)
        {
            auto nextCp = (*itr);
            int nextID = nextCp[Constants::ASSET_ATTR_ID];
            if (nextID > maxID)
            {
                maxID = nextID;
            }
        }
        return maxID+1;
    }

    float
    PathDefinition::getVelocity
    ()
    const
    {
        if (!mJson[Constants::ASSET_ATTR_VELOCITY].is_number())
        {
            return 1.0f;
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
