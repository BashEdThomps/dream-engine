/*
 * PathDefinition.h
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

#pragma once

#include "../../AssetDefinition.h"
#include "../../../deps/tinyspline/tinysplinecpp.h"

namespace Dream
{

    struct PathControlPoint
    {
       int id = 0;
       int index = 0;
       vec3 position = vec3(0.0f);
    };

    class PathDefinition : public AssetDefinition
    {
    public:
        PathDefinition(ProjectDefinition*, json);
        ~PathDefinition() override;

        void setWrap(bool wrap);
        bool getWrap();

        float getVelocity();
        void setVelocity(float v);

        PathControlPoint addControlPoint();
        void deleteControlPoint(const PathControlPoint& cp);
        void updateControlPoint(const PathControlPoint& cp);
        vector<PathControlPoint> getControlPoints();

        string getSplineType();
        void setSplineType(const string& type);
        tsBSplineType getSplineTypeEnum();

        void setStepScalar(double scaler);
        double getStepScalar();

        PathControlPoint unwrapControlPoint(const json&);
        json wrapControlPoint(const PathControlPoint& cp);
    protected:
        int nextID();
        void ensureControlPointsArray();
    };
}
