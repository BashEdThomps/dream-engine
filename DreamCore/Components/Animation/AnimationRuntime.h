#pragma once

#include "AnimationDefinition.h"
#include "AnimationKeyframe.h"
#include "AnimationTasks.h"
#include "Components/DiscreteAssetRuntime.h"

#include <tweeny.h>
#include <memory>

using std::unique_ptr;
using tweeny::tween;

namespace octronic::dream
{
  class AnimationRuntime : public DiscreteAssetRuntime
  {
  public:
    AnimationRuntime(ProjectRuntime& pr, AnimationDefinition&, EntityRuntime&);

    bool loadFromDefinition() override;

    void createTweens();
    void run();
    void pause();
    void reset();
    void update();
    void stepAnimation(double time);
    void orderByTime();
    void seekAll(unsigned int pos);

    long getDuration() const;

    void setAnimationSeekTime(long currentTime);
    long getAnimationSeekTime() const;

    bool getRunning() const;
    void setRunning(bool running);

    void pushTasks() override;

  private:
    void applyEasing(tweeny::tween<float>& twn, AnimationEasing::EasingType easing);

  private:
    shared_ptr<AnimationUpdateTask> mUpdateTask;
    vector<AnimationKeyframe> mKeyframes;
    bool mRunning;
    long mAnimationSeekTime;
    long mDuration;

    bool mRelative;
    float mOriginalValue;
    float mCurrentValue;
    tween<float> mTweenValue;
  };
}
