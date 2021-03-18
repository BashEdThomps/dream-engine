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

using std::make_shared;
using std::dynamic_pointer_cast;
using std::static_pointer_cast;

namespace octronic::dream
{
    AnimationRuntime::AnimationRuntime
    (const weak_ptr<ProjectRuntime>& pr,
     const weak_ptr<AnimationDefinition>& definition,
     const weak_ptr<EntityRuntime>& runtime)
        : DiscreteAssetRuntime(pr, definition,runtime),
          mRunning(false),
          mAnimationSeekTime(0),
          mDuration(1),
          mRelative(false)
    {
        LOG_TRACE("AnimationRuntime: Constructing Object");

        if (auto entityLock = mEntityRuntime.lock())
        {
            mOriginalTransform = entityLock->getTransform();
        }
    }

    AnimationRuntime::~AnimationRuntime()
    {
        LOG_TRACE("AnimationRuntime: Destroying Object");
    }

    bool AnimationRuntime::init()
    {
        if (!DiscreteAssetRuntime::init()) return false;
        mUpdateTask = make_shared<AnimationUpdateTask>(mProjectRuntime,static_pointer_cast<AnimationRuntime>(shared_from_this()));
        return true;
    }

    bool
    AnimationRuntime::loadFromDefinition
    ()
    {
        if (auto defLock = mDefinition.lock())
        {
            auto animDef = static_pointer_cast<AnimationDefinition>(defLock);
            mKeyframes = animDef->getKeyframes();
            mRelative = animDef->getRelative();
            createTweens();
            mLoaded = true;
            return mLoaded;
        }
        return false;
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

    weak_ptr<AnimationUpdateTask>
    AnimationRuntime::getUpdateTask
    ()
    const
    {
        return mUpdateTask;
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
            Transform transf = mKeyframes.at(i).getTransform();
            vec3 tx = transf.getTranslation();
            mat4 rx = transf.getOrientation();
            vec3 sx = transf.getScale();

            AnimationEasing::EasingType easing = mKeyframes.at(i).getEasingType();

            if (i==0)
            {
                mTweenTranslationX = tweeny::from(tx.x);
                mTweenTranslationY = tweeny::from(tx.y);
                mTweenTranslationZ = tweeny::from(tx.z);

                mTweenOrientation = tweeny::from(0.f);
                /*
                mTweenOrientationX = tweeny::from(rx.x());
                mTweenOrientationY = tweeny::from(rx.y());
                mTweenOrientationZ = tweeny::from(rx.z());
                mTweenOrientationW = tweeny::from(rx.w());
                */

                mTweenScaleX = tweeny::from(sx.x);
                mTweenScaleY = tweeny::from(sx.y);
                mTweenScaleZ = tweeny::from(sx.z);
            }
            else
            {
                long lastFrameTime = mKeyframes.at(i-1).getTime();
                long thisFrameTime = mKeyframes.at(i).getTime();
                long duringTime = (thisFrameTime-lastFrameTime);

                // Translation
                mTweenTranslationX.to(tx.x).during(duringTime);
                applyEasing(mTweenTranslationX,easing);

                mTweenTranslationY.to(tx.y).during(duringTime);
                applyEasing(mTweenTranslationY,easing);

                mTweenTranslationZ.to(tx.z).during(duringTime);
                applyEasing(mTweenTranslationZ,easing);

                // Orientation

                mTweenOrientation.to(1.f).during(duringTime);
                applyEasing(mTweenOrientation,easing);


                /*
                mTweenOrientationX.to(rx.x()).during(duringTime);
                applyEasing(mTweenOrientationX,easing);

                mTweenOrientationY.to(rx.y()).during(duringTime);
                applyEasing(mTweenOrientationY,easing);

                mTweenOrientationZ.to(rx.z()).during(duringTime);
                applyEasing(mTweenOrientationZ,easing);

                mTweenOrientationW.to(rx.w()).during(duringTime);
                applyEasing(mTweenOrientationW,easing);
                */

                // Scale

                mTweenScaleX.to(sx.x).during(duringTime);
                applyEasing(mTweenScaleX,easing);

                mTweenScaleY.to(sx.y).during(duringTime);
                applyEasing(mTweenScaleY,easing);

                mTweenScaleZ.to(sx.z).during(duringTime);
                applyEasing(mTweenScaleZ,easing);
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
        vec3 newTx, newSx;
        newTx.x = mTweenTranslationX.seek(pos);
        newTx.y = mTweenTranslationY.seek(pos);
        newTx.z = mTweenTranslationZ.seek(pos);

        mTweenOrientation.seek(pos);
        //mat4 orient=glm::interpolate(mOriginalTransform.getMatrix(), mTarge);
        /*
        newOrient.x = mTweenOrientationX.seek(pos);
        newOrient.y = mTweenOrientationY.seek(pos);
        newOrient.z = mTweenOrientationZ.seek(pos);
        */

        newSx.x = mTweenScaleX.seek(pos);
        newSx.y = mTweenScaleY.seek(pos);
        newSx.z = mTweenScaleZ.seek(pos);

        Transform tx;

        if (mRelative)
        {
            tx.setTranslation(mOriginalTransform.getTranslation() + newTx);
            //tx.setOrientation(mOriginalTransform.getOrientation() + newOrient);
            tx.setScale(mOriginalTransform.getScale() + newSx);
        }
        else
        {
            tx.setTranslation(newTx);
            //tx.setOrientation(newOrient);
            tx.setScale(newSx);
        }
        if (auto entityLock = mEntityRuntime.lock())
        {
            entityLock->setTransform(tx);
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
        if (auto prLock = mProjectRuntime.lock())
        {
            if (auto timeLock = prLock->getTime().lock())
            {
                stepAnimation(timeLock->getFrameTimeDelta());
            }
        }
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
		if (mLoadFromDefinitionTask->hasState(TASK_STATE_COMPLETED) && mLoaded)
        {
           if (auto prLock = mProjectRuntime.lock())
           {
               if (auto taskQueue = prLock->getTaskQueue().lock())
               {
                   taskQueue->pushTask(mUpdateTask);
               }
           }
        }
    }
}
