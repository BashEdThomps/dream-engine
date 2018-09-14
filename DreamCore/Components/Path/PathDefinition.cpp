/*
 * PathDefinition.cpp
 *
 * Created: 11 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
 *
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

namespace Dream
{
    PathDefinition::PathDefinition
    (const shared_ptr<ProjectDefinition>& pd, json js)
        : IAssetDefinition(pd,js)
    {

    }

    PathDefinition::~PathDefinition()
    {

    }

    void PathDefinition::setWrap(bool wrap)
    {
        mJson[Constants::ASSET_ATTR_WRAP] = wrap;
    }

    bool PathDefinition::getWrap()
    {
       if (mJson[Constants::ASSET_ATTR_WRAP].is_null())
       {
           mJson[Constants::ASSET_ATTR_WRAP] = false;
       }
       return mJson[Constants::ASSET_ATTR_WRAP];
    }

    json* PathDefinition::getControlPoints()
    {
       if (mJson[Constants::ASSET_ATTR_CONTROL_POINTS].is_null())
       {
           mJson[Constants::ASSET_ATTR_CONTROL_POINTS] = json::array();
       }
       return &mJson[Constants::ASSET_ATTR_CONTROL_POINTS];
    }

    size_t PathDefinition::numberOfControlPoints()
    {
        return getControlPoints()->size();
    }

    json PathDefinition::createControlPoint(int index)
    {
       json controlPoint = json::object();
       controlPoint[Constants::ASSET_ATTR_INDEX] = index;
       controlPoint[Constants::X] = 0.0;
       controlPoint[Constants::Y] = 0.0;
       controlPoint[Constants::Z] = 0.0;
       return controlPoint;
    }

    json* PathDefinition::getControlPoint(int i)
    {
        if (i >= 0 && i < getControlPoints()->size())
        {
            return &(getControlPoints()->at(i));
        }
        return nullptr;
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

    void PathDefinition::setSplineType(string type)
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

    void PathDefinition::setStepScalar(double scalar)
    {
        mJson[Constants::ASSET_ATTR_STEP_SCALAR] = scalar;
    }

    double PathDefinition::getStepScalar()
    {
        if (mJson[Constants::ASSET_ATTR_STEP_SCALAR].is_null())
        {
            mJson[Constants::ASSET_ATTR_STEP_SCALAR] = 1.0;
        }
        return mJson[Constants::ASSET_ATTR_STEP_SCALAR];
    }
}
