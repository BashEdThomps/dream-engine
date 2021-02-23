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

#pragma once

#include "AnimationDefinition.h"
#include "AnimationKeyframe.h"
#include "AnimationTasks.h"
#include "Components/DiscreteAssetRuntime.h"

#include <tweeny.h>

using tweeny::tween;

namespace octronic::dream
{
    class AnimationRuntime : public DiscreteAssetRuntime
    {
    public:
        AnimationRuntime(ProjectRuntime* pr, AnimationDefinition*, EntityRuntime*);
        ~AnimationRuntime() override;
        bool loadFromDefinition() override;

        void createTweens();
        void run();
        void pause();
        void reset();
        void update();
        void stepAnimation(double time);
        void orderByTime();
        void seekAll(unsigned int pos);

        long getDuration();

        void setAnimationSeekTime(long currentTime);
        long getAnimationSeekTime();

        bool getRunning();
        void setRunning(bool running);

        shared_ptr<AnimationUpdateTask> getUpdateTask();

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
        Transform mOriginalTransform;

        tween<float> mTweenTranslationX;
        tween<float> mTweenTranslationY;
        tween<float> mTweenTranslationZ;

        tween<float> mTweenOrientation;
        /*
        tween<float> mTweenOrientationX;
        tween<float> mTweenOrientationY;
        tween<float> mTweenOrientationZ;
        tween<float> mTweenOrientationW;
        */

        tween<float> mTweenScaleX;
        tween<float> mTweenScaleY;
        tween<float> mTweenScaleZ;
    };
}
