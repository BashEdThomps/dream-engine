#pragma once

#include "Components/AssetDefinition.h"
#include "PathControlPoint.h"
#include <tinysplinecpp.h>

namespace octronic::dream
{
  class PathDefinition : public AssetDefinition
  {
  public:
    PathDefinition(ProjectDefinition&, const json&);

    PathDefinition(PathDefinition&&) = default;
    PathDefinition& operator=(PathDefinition&&) = default;


    void setWrap(bool wrap);
    bool getWrap() const;

    float getVelocity() const;
    void setVelocity(float v);

    PathControlPoint addControlPoint();
    void deleteControlPoint(const PathControlPoint& cp);
    void updateControlPoint(const PathControlPoint& cp);
    vector<PathControlPoint> getControlPoints() const;

    string getSplineType() const;
    void setSplineType(const string& type);
    tsBSplineType getSplineTypeEnum() const;

    void setStepScalar(double scaler);
    double getStepScalar() const;

    PathControlPoint unwrapControlPoint(const json&) const;
    json wrapControlPoint(const PathControlPoint& cp) const;
  protected:
    int nextID();
  };
}
