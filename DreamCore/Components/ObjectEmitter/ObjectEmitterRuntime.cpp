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


#include "ObjectEmitterRuntime.h"

#include "ObjectEmitterDefinition.h"
#include "Common/Logger.h"
#include "Components/Physics/PhysicsObjectRuntime.h"
#include "Components/Time.h"
#include "Project/ProjectRuntime.h"
#include "Scene/Entity/EntityRuntime.h"
#include "Scene/SceneRuntime.h"

namespace Dream
{
    ObjectEmitterRuntime::ObjectEmitterRuntime
    (
        ObjectEmitterDefinition* definition,
        EntityRuntime* transform
    ) : DiscreteAssetRuntime(definition,transform),
        mEntityUuid(0),
        mEmitInterval(0),
        mObjectVelocity(0.0f),
        mStartRadius(0.0f),
        mStartTheta(0.0f),
        mEndTheta(0.0f),
        mLoops(0),
        mObjectCount(0),
        mEmitIntervalTime(0),
        mLoopCount(0),
        mState(Emit),
        mUpdateTask(this)
    {
        LOG_TRACE("ObjectEmitterRuntime: Constructing");
    }

    ObjectEmitterRuntime::~ObjectEmitterRuntime
    ()
    {
        LOG_TRACE("ObjectEmitterRuntime: Destroying");
    }

    bool
    ObjectEmitterRuntime::useDefinition
    ()
    {
        auto def = static_cast<ObjectEmitterDefinition*>(mDefinition);
        mEntityUuid = def->getEntityUuid();
        mLoops = def->getLoops();
        mEmitInterval = def->getEmitInterval();
        mObjectCount = def->getObjectCount();
        mObjectVelocity = def->getObjectVelocity();
        mStartRadius = def->getStartRadius();
        mStartTheta = def->getStartTheta();
        mEndTheta = def->getEndTheta();
        mLoopCount = mLoops;
        mLoaded = true;
        return mLoaded;
    }

    ObjectEmitterUpdateTask*
    ObjectEmitterRuntime::getUpdateTask
    ()
    {
        return &mUpdateTask;
    }

    bool
    ObjectEmitterRuntime::update()
    {
       if (mEntityRuntime->tryLock())
       {
           auto time = mEntityRuntime->getSceneRuntime()->getProjectRuntime()->getTime();
           long frameDelta = time->getFrameTimeDelta();

           switch (mState)
           {
               case Emit:
                   LOG_DEBUG("ObjectEmitterRuntime: Emit");
                   emitObjects();
                   mEmitIntervalTime = mEmitInterval;
                   LOG_DEBUG("ObjectEmitterRuntime: EmitInterval");
                   mState = EmitInterval;
                   break;
               case EmitInterval:
                   mEmitIntervalTime -= frameDelta;
                   if (mEmitIntervalTime < 0)
                   {
                       mLoopCount -= 1;
                       if (mLoopCount > 0)
                       {
                           mState = Emit;
                       }
                       else
                       {
                          LOG_DEBUG("ObjectEmitterRuntime: Done");
                          mState = Done;
                       }
                   }
                   break;
               case Done:
                   break;
           }
           mEntityRuntime->unlock();
           return true;
       }
       return false;
    }

    void
    ObjectEmitterRuntime::emitObjects
    ()
    {
        if (mEntityUuid != 0)
        {
            LOG_DEBUG("ObjectEmitterRuntime: Emitting Objects");
            double deltaTheta = (mEndTheta-mStartTheta)/mObjectCount;
            float theta = mStartTheta;

            for (int i = 0; i < mObjectCount; i++)
            {
                float x = cos(theta);
                float y = sin(theta);

                EntityRuntime* newObj = mEntityRuntime->addChildFromTemplateUuid(mEntityUuid);
                Vector3 startTx = mEntityRuntime->getTransform().getTranslation();
                if (newObj->hasPhysicsObjectRuntime())
                {
                    PhysicsObjectRuntime* po = newObj->getPhysicsObjectRuntime();
                    float newX = startTx.x() + (mStartRadius*x);
                    float newZ = startTx.z() + (mStartRadius*y);
                    po->setCenterOfMassTransform(newX, startTx.y(), newZ);
                    po->setLinearFactor(1,0,1);
                    po->setLinearVelocity(mObjectVelocity * x, 0, mObjectVelocity * y);
                }
                theta += deltaTheta;
            }
        }
    }
}
