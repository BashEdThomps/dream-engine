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
#include "../../DiscreteAssetRuntime.h"
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif
#include "../../../deps/tweeny/tweeny.h"

using namespace tweeny;

namespace Dream
{
    class AnimationRuntime : public DiscreteAssetRuntime
    {
    public:
        AnimationRuntime(AnimationDefinition*, SceneObjectRuntime*);
        ~AnimationRuntime() override;
        bool useDefinition() override;

        void orderByTime();
        void update();
        void stepAnimation(double time);

        long getCurrentTime() const;
        void setCurrentTime(long currentTime);

        bool mRunning;
        void createTweens();
        void run();
        void pause();
        void reset();
        void seekAll(unsigned int pos);
        long getDuration() const;

        bool getRunning() const;
        void setRunning(bool running);

    private:
        void applyEasing(tweeny::tween<float>& twn, AnimationEasing::Type easing);
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
    };

}
