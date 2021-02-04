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

#include "AnimationRuntime.h"
#include "AnimationEasing.h"

#include "Common/Constants.h"
#include "Common/Logger.h"
#include "Components/Time.h"
#include "Scene/Entity/EntityRuntime.h"
#include "Scene/SceneRuntime.h"
#include "Project/ProjectRuntime.h"

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace octronic::dream
{
    AnimationRuntime::AnimationRuntime
    (AnimationDefinition* definition,EntityRuntime* runtime)
        : DiscreteAssetRuntime("AnimationDefinition",definition,runtime),
          mRunning(false),
          mCurrentTime(0),
          mDuration(1),
          mRelative(false),
          mOriginalTransform(runtime->getTransform().getMatrix()),
          mUpdateTask(this)
    {
        LOG_TRACE("AnimationRuntime: Constructing Object");
    }

    AnimationRuntime::~AnimationRuntime()
    {
        LOG_TRACE("AnimationRuntime: Destroying Object");
    }

    bool // public
    AnimationRuntime::useDefinition()
    {
        if (dreamTryLock())
        {
            dreamLock();
            auto animDef = static_cast<AnimationDefinition*>(mDefinition);
            mKeyframes = animDef->getKeyframes();
            mRelative = animDef->getRelative();
            createTweens();
            mLoaded = true;
            return mLoaded;
        }
        dreamElseLockFailed
    }

    void // public
    AnimationRuntime::stepAnimation
    (double deltaTime)
    {
        if (dreamTryLock())
        {
            dreamLock();
            if (mRunning)
            {
                LOG_TRACE("AnimationRuntime: Delta Time {} | mCurrentTime {}",deltaTime,mCurrentTime);
                mCurrentTime += deltaTime;
                if (mCurrentTime > mDuration)
                {
                    mCurrentTime = mDuration;
                    mRunning = false;
                }
                seekAll(mCurrentTime);
            }
        }
        dreamElseLockFailed
    }

    long // public
    AnimationRuntime::getCurrentTime
    ()
    {
        if (dreamTryLock())
        {
            dreamLock();
            return mCurrentTime;
        }
        dreamElseLockFailed
    }

    void // public
    AnimationRuntime::setCurrentTime
    (long currentTime)
    {
        if (dreamTryLock())
        {
            dreamLock();
            mCurrentTime = currentTime;
        }
        dreamElseLockFailed
    }

    bool // public
    AnimationRuntime::getRunning
    ()
    {
        if (dreamTryLock())
        {
            dreamLock();
            return mRunning;
        }
        dreamElseLockFailed
    }

    void // public
    AnimationRuntime::setRunning
    (bool running)
    {
        if (dreamTryLock())
        {
            dreamLock();
            mRunning = running;
        }
        dreamElseLockFailed
    }

    AnimationUpdateTask* // public
    AnimationRuntime::getUpdateTask()
    {
        if (dreamTryLock())
        {
            dreamLock();
            return &mUpdateTask;
        }
        dreamElseLockFailed
    }

    void // public
    AnimationRuntime::createTweens
    ()
    {
        if (dreamTryLock())
        {
            dreamLock();
            orderByTime();
            int numKeyframes = mKeyframes.size();
            mDuration = mKeyframes.at(numKeyframes-1).getTime();

            for (int i=0; i < numKeyframes; i++)
            {
                vec3 tx = mKeyframes.at(i).getTranslation().toGLM();
                vec3 rx = mKeyframes.at(i).getRotation().toGLM();
                vec3 sx = mKeyframes.at(i).getScale().toGLM();
                AnimationEasing::EasingType easing = mKeyframes.at(i).getEasingType();

                if (i==0)
                {
                    mTweenTranslationX = tweeny::from(tx.x);
                    mTweenTranslationY = tweeny::from(tx.y);
                    mTweenTranslationZ = tweeny::from(tx.z);

                    mTweenRotationX = tweeny::from(rx.x);
                    mTweenRotationY = tweeny::from(rx.y);
                    mTweenRotationZ = tweeny::from(rx.z);

                    mTweenScaleX = tweeny::from(sx.x);
                    mTweenScaleY = tweeny::from(sx.y);
                    mTweenScaleZ = tweeny::from(sx.z);
                }
                else
                {
                    long lastFrameTime = mKeyframes.at(i-1).getTime();
                    long thisFrameTime = mKeyframes.at(i).getTime();
                    long duringTime = (thisFrameTime-lastFrameTime);

                    mTweenTranslationX.to(tx.x).during(duringTime);
                    applyEasing(mTweenTranslationX,easing);
                    mTweenTranslationY.to(tx.y).during(duringTime);
                    applyEasing(mTweenTranslationY,easing);
                    mTweenTranslationZ.to(tx.z).during(duringTime);
                    applyEasing(mTweenTranslationZ,easing);

                    mTweenRotationX.to(rx.x).during(duringTime);
                    applyEasing(mTweenRotationX,easing);
                    mTweenRotationY.to(rx.y).during(duringTime);
                    applyEasing(mTweenRotationY,easing);
                    mTweenRotationZ.to(rx.z).during(duringTime);
                    applyEasing(mTweenRotationZ,easing);

                    mTweenScaleX.to(sx.x).during(duringTime);
                    applyEasing(mTweenScaleX,easing);
                    mTweenScaleY.to(sx.y).during(duringTime);
                    applyEasing(mTweenScaleY,easing);
                    mTweenScaleZ.to(sx.z).during(duringTime);
                    applyEasing(mTweenScaleZ,easing);
                }
            }
        }
        dreamElseLockFailed
    }

    void // public
    AnimationRuntime::run
    ()
    {
        if (dreamTryLock())
        {
            dreamLock();
            mRunning = true;
        }
        dreamElseLockFailed
    }

    void // public
    AnimationRuntime::pause
    ()
    {
        if (dreamTryLock())
        {
            dreamLock();
            mRunning = !mRunning;
        }
        dreamElseLockFailed
    }

    void // public
    AnimationRuntime::reset
    ()
    {
        if (dreamTryLock())
        {
            dreamLock();
            mCurrentTime = 0;
            seekAll(0);
        }
        dreamElseLockFailed
    }

    void // public
    AnimationRuntime::seekAll
    (unsigned int pos)
    {
        if (dreamTryLock())
        {
            dreamLock();
            LOG_TRACE("AnimationRuntime: Seeing to {}",pos);
            vec3 newTx, newRx, newSx;
            newTx.x = mTweenTranslationX.seek(pos);
            newTx.y = mTweenTranslationY.seek(pos);
            newTx.z = mTweenTranslationZ.seek(pos);

            newRx.x = mTweenRotationX.seek(pos);
            newRx.y = mTweenRotationY.seek(pos);
            newRx.z = mTweenRotationZ.seek(pos);

            newSx.x = mTweenScaleX.seek(pos);
            newSx.y = mTweenScaleY.seek(pos);
            newSx.z = mTweenScaleZ.seek(pos);

            mat4 matrix(1.0f);
            if (mRelative)
            {
                matrix = glm::translate(mOriginalTransform,newTx);
            }
            else
            {
                matrix = glm::translate(matrix,newTx);
            }
            matrix = matrix*mat4_cast(quat(newRx));
            matrix = glm::scale(matrix,newSx);
            mEntityRuntime->getTransform().setMatrix(matrix);
        }
        dreamElseLockFailed

    }

    long // public
    AnimationRuntime::getDuration
    ()
    {
        if (dreamTryLock())
        {
            dreamLock();
            return mDuration;
        }
        dreamElseLockFailed
    }

    void // public
    AnimationRuntime::orderByTime
    ()
    {
        if (dreamTryLock())
        {
            dreamLock();
            std::sort(mKeyframes.begin(),mKeyframes.end());
        }
        dreamElseLockFailed
    }

    void // public
    AnimationRuntime::update()
    {
        if (dreamTryLock())
        {
            dreamLock();
            auto timeDelta =
                    mEntityRuntime
                    ->getSceneRuntime()
                    ->getProjectRuntime()
                    ->getTime()
                    ->getFrameTimeDelta();
            stepAnimation(timeDelta);
        }
        dreamElseLockFailed
    }

    void // private
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
}
