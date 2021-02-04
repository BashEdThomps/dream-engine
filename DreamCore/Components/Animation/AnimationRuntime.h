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

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

using tweeny::tween;

namespace octronic::dream
{
    class AnimationRuntime : public DiscreteAssetRuntime
    {
    public:
        AnimationRuntime(AnimationDefinition*, EntityRuntime*);
        ~AnimationRuntime() override;
        bool useDefinition() override;

        void createTweens();
        void run();
        void pause();
        void reset();
        void update();
        void stepAnimation(double time);
        void orderByTime();
        void seekAll(unsigned int pos);

        long getDuration();

        void setCurrentTime(long currentTime);
        long getCurrentTime();

        bool getRunning();
        void setRunning(bool running);

        AnimationUpdateTask* getUpdateTask();

    private:
        void applyEasing(tweeny::tween<float>& twn, AnimationEasing::EasingType easing);

    private:
        bool mRunning;
        vector<AnimationKeyframe> mKeyframes;
        long mCurrentTime;
        long mDuration;

        bool mRelative;
        mat4 mOriginalTransform;

        tween<float> mTweenTranslationX;
        tween<float> mTweenTranslationY;
        tween<float> mTweenTranslationZ;

        tween<float> mTweenRotationX;
        tween<float> mTweenRotationY;
        tween<float> mTweenRotationZ;

        tween<float> mTweenScaleX;
        tween<float> mTweenScaleY;
        tween<float> mTweenScaleZ;
        AnimationUpdateTask mUpdateTask;
    };
}
