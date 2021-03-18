/*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <algorithm>

#include "PathRuntime.h"
#include "Common/Logger.h"

#include "PathTasks.h"
#include "Math/Transform.h"
#include "Components/Time.h"
#include "Scene/SceneRuntime.h"
#include "Entity/EntityRuntime.h"
#include "Project/ProjectRuntime.h"

#include <tinyspline.h>

using std::static_pointer_cast;

namespace octronic::dream
{

  PathRuntime::PathRuntime
  (const weak_ptr<ProjectRuntime>& pr,
   const weak_ptr<PathDefinition>& definition,
   const weak_ptr<EntityRuntime>& runtime)
    : DiscreteAssetRuntime(pr, definition,runtime),
      mWrapPath(false),
      mCurrentIndex(0),
      mUStep(0.05),
      mVelocity(1.0),
      mDistanceToTravel(0.0f),
      mUpdateTask(nullptr)
  {
    LOG_TRACE("PathRuntime: Constructing Object");
  }

  PathRuntime::~PathRuntime
  ()
  {
    LOG_TRACE("PathRuntime: Destroying Object");
  }

  void
  PathRuntime::update
  ()
  {
    if (auto erLock = mEntityRuntime.lock())
    {
      auto transform = stepPath();
      erLock->setTransform(transform);
    }
  }

  bool
  PathRuntime::loadFromDefinition
  ()
  {
    if (auto defLock = mDefinition.lock())
    {
      auto animDef = static_pointer_cast<PathDefinition>(defLock);

      LOG_DEBUG("PathRuntime: Loading {} spline with {} control points for {} ",
                animDef->getSplineType(),
                animDef->getControlPoints().size(),
                getNameAndUuidString());

      mWrapPath = animDef->getWrap();
      mVelocity = animDef->getVelocity();

      int nControlPoints = animDef->getControlPoints().size();

      if (nControlPoints < 2)
      {
        LOG_WARN("PathRuntime: Skipping curve, not enough control points");
        mLoaded = true;
        return true;
      }

      mUStep = 1.0/(nControlPoints*animDef->getStepScalar());

      if (nControlPoints >= SPLINE_DIMENSIONS)
      {
        generate();
      }
      else
      {
        LOG_ERROR("PathRuntime: Not enough control points to generate spline");
      }
      mLoaded = true;
      return mLoaded;
    }
    return false;
  }

  void
  PathRuntime::generate
  ()
  {
    if (auto defLock = mDefinition.lock())
    {
      auto animDef = static_pointer_cast<PathDefinition>(defLock);

      auto splineType = animDef->getSplineTypeEnum();

      tsBSpline spline, derivative;
      tsReal *ctrlp;
      size_t i=0;

      mSplinePoints.clear();
      mSplineTangents.clear();
      mSplineDerivatives.clear();

      int nControlPoints =  animDef->getControlPoints().size();

      if (nControlPoints <= SPLINE_DIMENSIONS) return;

      ts_bspline_new(nControlPoints, SPLINE_DIMENSIONS, SPLINE_DEGREES, splineType, &spline);

      /* Setup control points. */
      ts_bspline_control_points(&spline, &ctrlp);

      for (auto cp : animDef->getControlPoints())
      {
        auto x = cp.position.x;
        auto y = cp.position.y;
        auto z = cp.position.z;
        // Setup control points.
        ctrlp[i++] = x;
        ctrlp[i++] = y;
        ctrlp[i++] = z;
      }

      ts_bspline_set_control_points(&spline, ctrlp);
      free(ctrlp);

      ts_bspline_derive(&spline, 1, &derivative);

      // Stores our evaluation results.
      if (splineType == TS_CLAMPED)
      {
        tsDeBoorNet net1, net2, net3;
        tsReal *result1 = nullptr, *result2 = nullptr, *result3 = nullptr;
        tsReal *knots;

        /* draw spline */
        ts_bspline_control_points(&spline, &ctrlp);
        ts_bspline_knots(&spline, &knots);

        for (tsReal u = 0.0; u <= 1.0; u += mUStep)
        {
          ts_bspline_eval(&spline, u, &net1);
          ts_deboornet_result(&net1, &result1);
          ts_bspline_eval(&derivative, u, &net2);
          ts_deboornet_result(&net2, &result2);
          ts_bspline_eval(&derivative, u, &net3);
          ts_deboornet_result(&net3, &result3);

          LOG_TRACE("PathRuntime: Generating with u={}",u);
          LOG_TRACE("PathRuntime: Got spline point ({},{},{})",result1[0], result1[1], result1[2]);

          for (i = 0; i < ts_deboornet_dimension(&net2); i++)
          {
            result2[i] = result1[i] + result2[i] / 6.0;
            result3[i] = result1[i] - result3[i] / 6.0;
          }

          // tangent line
          vec3 point(result1[0],result1[1],result1[2]);
          vec3 tan1(result2[0],result2[1],result2[2]);
          vec3 tan2(result3[0],result3[1],result3[2]);

          mSplinePoints.push_back(point);
          mSplineTangents.push_back(getHeading(point, tan1,tan2));
          mSplineDerivatives.push_back(pair<vec3,vec3>(tan1,tan2));

          ts_deboornet_free(&net1);
          ts_deboornet_free(&net2);
          ts_deboornet_free(&net3);

          free(result1);
          free(result2);
          free(result3);
        }

        free(ctrlp);
        free(knots);
      }
      else if (splineType == TS_OPENED)
      {

      }
      else if (splineType == TS_BEZIERS)
      {

      }

      ts_bspline_free(&spline);
      ts_bspline_free(&derivative);

      LOG_DEBUG("PathRuntime: Finished Loading spline for {}",getNameAndUuidString());
    }
  }

  vector<pair<vec3,vec3>>
  PathRuntime::getSplineDerivatives
  () const
  {
    return mSplineDerivatives;
  }

  bool
  PathRuntime::getWrapPath
  () const
  {
    return mWrapPath;
  }

  void
  PathRuntime::setWrapPath
  (bool wrapPath)
  {
    mWrapPath = wrapPath;
  }

  size_t
  PathRuntime::getCurrentIndex
  () const
  {
    return mCurrentIndex;
  }

  void
  PathRuntime::setCurrentIndex
  (size_t currentIndex)
  {
    mCurrentIndex = currentIndex;
  }

  Transform
  PathRuntime::stepPath
  ()
  {

    Transform tx;

    if (mSplinePoints.empty())
    {
      return tx;
    }

    if (mCurrentIndex == 0)
    {
      setToCurrentPoint();
      nextPoint();
    }
    else
    {
      float distanceToNext = 0.0f;
      if (mCurrentIndex != mSplinePoints.size()-1)
      {
        if (auto prLock = mProjectRuntime.lock())
        {
          if (auto time = prLock->getTime().lock())
          {
            mDistanceToTravel += time->perSecond(mVelocity);
            vec3 next = mSplinePoints.at((mCurrentIndex+1) % mSplinePoints.size());
            distanceToNext = glm::distance(vec3(mCurrentTransform.getTranslation()),next);
            LOG_TRACE("PathRuntime: To Travel: {} Distance to next: {}",mDistanceToTravel, distanceToNext);
          }
        }
      }
      else
      {
        mDistanceToTravel = 0.0f;
      }

      if (mDistanceToTravel >= distanceToNext)
      {
        mDistanceToTravel = 0.0f;
        nextPoint();
        setToCurrentPoint();
      }
    }
    return mCurrentTransform;

  }

  void PathRuntime::nextPoint()
  {
    mCurrentIndex++;
    if (mCurrentIndex >= mSplinePoints.size())
    {
      if (mWrapPath)
        mCurrentIndex = 0;
      else
        mCurrentIndex = mSplinePoints.size()-1;
    }
  }

  void
  PathRuntime::setToCurrentPoint
  ()
  {
    vec3 thisPoint = mSplinePoints.at(mCurrentIndex);
    mat4 thisOrient = mSplineTangents.at(mCurrentIndex);

    mCurrentTransform.setTranslation(mCurrentTransform.getTranslation()+thisPoint);
    //mCurrentTransform.setOrientation(mCurrentTransform.getOrientation()+thisOrient);

    LOG_TRACE("PathRuntime: Got spline point {}/{} T({},{},{})",
              mCurrentIndex,mSplinePoints.size(),
              thisPoint.x, thisPoint.y, thisPoint.z);

  }

  vector<vec3>
  PathRuntime::getSplinePoints
  () const
  {
    return mSplinePoints;
  }

  vector<mat4>
  PathRuntime::getSplineTangents
  () const
  {
    return mSplineTangents;
  }

  vec3
  PathRuntime::getSplinePoint
  (int index) const
  {
    if (index >=0 && index < mSplinePoints.size())
    {
      return mSplinePoints.at(index);
    }
    return vec3(0.f);
  }

  double
  PathRuntime::getUStep
  () const
  {
    return mUStep;
  }

  void
  PathRuntime::setUStep
  (double uStep)
  {
    mUStep = uStep;
  }

  mat4
  PathRuntime::getHeading
  (const vec3& point, const vec3& t1, const vec3& t2)
  const
  {
    mat4 mtx = lookAt(t1,t2,vec3(0,1,0));
    mtx = -mtx;
    return mtx;
  }

  shared_ptr<PathUpdateTask>
  PathRuntime::getUpdateTask
  ()
  const
  {
    return mUpdateTask;
  }


  void PathRuntime::pushTasks()
  {

  }

  const int PathRuntime::SPLINE_DIMENSIONS = 3;
  const int PathRuntime::SPLINE_DEGREES = 3;
}
