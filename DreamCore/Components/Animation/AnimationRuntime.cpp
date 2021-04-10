#include "AnimationRuntime.h"
#include "AnimationEasing.h"

#include "Components/Cache.h"
#include "Common/Constants.h"
#include "Common/Logger.h"
#include "Components/Time.h"
#include "Entity/EntityRuntime.h"
#include "Scene/SceneRuntime.h"
#include "Project/ProjectRuntime.h"

#include <algorithm>
#include <glm/gtx/matrix_interpolation.hpp>

using std::make_unique;
using std::make_shared;

namespace octronic::dream
{
  AnimationRuntime::AnimationRuntime
  (ProjectRuntime& pr,
   AnimationDefinition& definition,
   EntityRuntime& runtime)
    : DiscreteAssetRuntime(pr, definition,runtime),
      mUpdateTask(nullptr),
      mRunning(false),
      mAnimationSeekTime(0),
      mDuration(1),
      mRelative(false),
      mOriginalValue(0.f),
      mCurrentValue(0.f)
  {
    LOG_TRACE("AnimationRuntime: Constructing Object");
    mUpdateTask = make_shared<AnimationUpdateTask>(getProjectRuntime(),*this);
  }

  bool
  AnimationRuntime::loadFromDefinition
  ()
  {
    auto& animDef = static_cast<AnimationDefinition&>(getDefinition());
    mKeyframes = animDef.getKeyframes();
    mRelative = animDef.getRelative();
    createTweens();
    mLoaded = true;
    return mLoaded;
  }

  void
  AnimationRuntime::stepAnimation
  (double deltaTime)
  {
    if (mRunning)
    {
      LOG_TRACE("AnimationRuntime: Delta Time {} | mAnimationSeekTime {}",deltaTime,mAnimationSeekTime);
      mAnimationSeekTime += deltaTime;
      if (mAnimationSeekTime > mDuration)
      {
        mAnimationSeekTime = mDuration;
        mRunning = false;
      }
      seekAll(mAnimationSeekTime);
    }
  }

  long
  AnimationRuntime::getAnimationSeekTime
  ()
  const
  {
    return mAnimationSeekTime;
  }

  void
  AnimationRuntime::setAnimationSeekTime
  (long currentTime)
  {
    mAnimationSeekTime = currentTime;
  }

  bool
  AnimationRuntime::getRunning
  ()
  const
  {
    return mRunning;
  }

  void
  AnimationRuntime::setRunning
  (bool running)
  {
    mRunning = running;
  }

  void
  AnimationRuntime::createTweens
  ()
  {
    orderByTime();
    int numKeyframes = mKeyframes.size();
    mDuration = mKeyframes.at(numKeyframes-1).getTime();

    for (int i=0; i < numKeyframes; i++)
    {
      float val = mKeyframes.at(i).getValue();

      AnimationEasing::EasingType easing = mKeyframes.at(i).getEasingType();

      if (i==0)
      {
        mTweenValue = tweeny::from(mOriginalValue);
      }
      else
      {
        long lastFrameTime = mKeyframes.at(i-1).getTime();
        long thisFrameTime = mKeyframes.at(i).getTime();
        long duringTime = (thisFrameTime-lastFrameTime);

        // Translation
        mTweenValue.to(val).during(duringTime);
        applyEasing(mTweenValue,easing);
      }
    }
  }

  void
  AnimationRuntime::run
  ()
  {
    mRunning = true;
  }

  void
  AnimationRuntime::pause
  ()
  {
    mRunning = !mRunning;
  }

  void
  AnimationRuntime::reset
  ()
  {
    mAnimationSeekTime = 0;
    seekAll(0);
  }

  void
  AnimationRuntime::seekAll
  (unsigned int pos)
  {
    LOG_TRACE("AnimationRuntime: Seeing to {}",pos);
    float newValue = mTweenValue.seek(pos);
    if (mRelative)
    {
      mCurrentValue = mOriginalValue + newValue;
    }
    else
    {
      mCurrentValue = newValue;
    }
  }

  long
  AnimationRuntime::getDuration
  ()
  const
  {
    return mDuration;
  }

  void
  AnimationRuntime::orderByTime
  ()
  {
    std::sort(mKeyframes.begin(),mKeyframes.end());
  }

  void
  AnimationRuntime::update()
  {
    auto& time = getProjectRuntime().getTime();
    stepAnimation(time.getFrameTimeDelta());
  }

  void
  AnimationRuntime::applyEasing
  (tweeny::tween<float>& twn, AnimationEasing::EasingType easing)
  {
    switch(easing)
    {
      case AnimationEasing::EasingType::EasingLinear:
        twn.via(tweeny::easing::linear);
        break;

      case AnimationEasing::EasingType::EasingQuadraticIn:
        twn.via(tweeny::easing::quadraticIn);
        break;
      case AnimationEasing::EasingType::EasingQuadraticOut:
        twn.via(tweeny::easing::quadraticOut);
        break;
      case AnimationEasing::EasingType::EasingQuadraticInOut:
        twn.via(tweeny::easing::quadraticInOut);
        break;

      case AnimationEasing::EasingType::EasingCubicIn:
        twn.via(tweeny::easing::cubicIn);
        break;
      case AnimationEasing::EasingType::EasingCubicOut:
        twn.via(tweeny::easing::cubicOut);
        break;
      case AnimationEasing::EasingType::EasingCubicInOut:
        twn.via(tweeny::easing::cubicInOut);
        break;

      case AnimationEasing::EasingType::EasingQuarticIn:
        twn.via(tweeny::easing::quarticIn);
        break;
      case AnimationEasing::EasingType::EasingQuarticOut:
        twn.via(tweeny::easing::quarticOut);
        break;
      case AnimationEasing::EasingType::EasingQuarticInOut:
        twn.via(tweeny::easing::quarticInOut);
        break;

      case AnimationEasing::EasingType::EasingQuinticIn:
        twn.via(tweeny::easing::quinticIn);
        break;
      case AnimationEasing::EasingType::EasingQuinticOut:
        twn.via(tweeny::easing::quinticOut);
        break;
      case AnimationEasing::EasingType::EasingQuinticInOut:
        twn.via(tweeny::easing::quinticInOut);
        break;

      case AnimationEasing::EasingType::EasingSinusoidalIn:
        twn.via(tweeny::easing::sinusoidalIn);
        break;
      case AnimationEasing::EasingType::EasingSinusoidalOut:
        twn.via(tweeny::easing::sinusoidalOut);
        break;
      case AnimationEasing::EasingType::EasingSinusoidalInOut:
        twn.via(tweeny::easing::sinusoidalInOut);
        break;

      case AnimationEasing::EasingType::EasingExponentialIn:
        twn.via(tweeny::easing::exponentialIn);
        break;
      case AnimationEasing::EasingType::EasingExponentialOut:
        twn.via(tweeny::easing::exponentialOut);
        break;
      case AnimationEasing::EasingType::EasingExponentialInOut:
        twn.via(tweeny::easing::exponentialInOut);
        break;

      case AnimationEasing::EasingType::EasingCircularIn:
        twn.via(tweeny::easing::circularIn);
        break;
      case AnimationEasing::EasingType::EasingCircularOut:
        twn.via(tweeny::easing::circularOut);
        break;
      case AnimationEasing::EasingType::EasingCircularInOut:
        twn.via(tweeny::easing::circularInOut);
        break;

      case AnimationEasing::EasingType::EasingBounceIn:
        twn.via(tweeny::easing::bounceIn);
        break;
      case AnimationEasing::EasingType::EasingBounceOut:
        twn.via(tweeny::easing::bounceOut);
        break;
      case AnimationEasing::EasingType::EasingBounceInOut:
        twn.via(tweeny::easing::bounceInOut);
        break;

      case AnimationEasing::EasingType::EasingElasticIn:
        twn.via(tweeny::easing::elasticIn);
        break;
      case AnimationEasing::EasingType::EasingElasticOut:
        twn.via(tweeny::easing::elasticOut);
        break;
      case AnimationEasing::EasingType::EasingElasticInOut:
        twn.via(tweeny::easing::elasticInOut);
        break;

      case AnimationEasing::EasingType::EasingBackIn:
        twn.via(tweeny::easing::backIn);
        break;
      case AnimationEasing::EasingType::EasingBackOut:
        twn.via(tweeny::easing::backOut);
        break;
      case AnimationEasing::EasingType::EasingBackInOut:
        twn.via(tweeny::easing::backInOut);
        break;
    }
  }

  void AnimationRuntime::pushTasks()
  {
    auto& taskQueue = getProjectRuntime().getTaskQueue();
    if (mLoadFromDefinitionTask->hasState(TASK_STATE_COMPLETED) && mLoaded)
    {
      taskQueue.pushTask(mUpdateTask);
    }
  }
}
