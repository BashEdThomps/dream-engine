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

#include "PhysicsComponent.h"
#include "PhysicsObjectRuntime.h"
#include "PhysicsTasks.h"
#include "Common/Logger.h"
#include "Components/Component.h"
#include "Math/Transform.h"
#include "Components/Event.h"
#include "Components/Time.h"
#include "Scene/SceneRuntime.h"
#include "Entity/EntityRuntime.h"
#include "Project/ProjectRuntime.h"

#include <btBulletDynamicsCommon.h>
#include <iostream>

using std::make_shared;
using std::static_pointer_cast;

namespace octronic::dream
{
  PhysicsComponent::PhysicsComponent
  (const weak_ptr<ProjectRuntime>& pr)
    : Component(pr)
  {
  }

  PhysicsComponent::~PhysicsComponent
  ()
  {
    LOG_DEBUG( "PhysicsComponent: Destroying Object" );

    int i = 0;

    //removed/delete constraints
    for (i=mDynamicsWorld->getNumConstraints()-1; i>=0 ;i--)
    {
      btTypedConstraint* constraint = mDynamicsWorld->getConstraint(i);
      mDynamicsWorld->removeConstraint(constraint);
      delete constraint;
    }

    vector<btCollisionShape*> shapes;

    for (i=mDynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--)
    {
      btCollisionObject* obj = mDynamicsWorld->getCollisionObjectArray()[i];
      btRigidBody* body = btRigidBody::upcast(obj);
      btCollisionShape* shape = body->getCollisionShape();
      if (shape)
      {
        shapes.push_back(shape);
      }
      if (body && body->getMotionState())
      {
        LOG_DEBUG( "PhysicsComponent: Deleting a motion state" );
        delete body->getMotionState();
      }

      LOG_DEBUG( "PhysicsComponent: Deleting a collision object" );
      mDynamicsWorld->removeCollisionObject(obj);
      delete obj;
    }

    for (btCollisionShape* shape : shapes)
    {
      LOG_DEBUG( "PhysicsComponent: Deleting Shape" );
      delete shape;
    }
  }

  void
  PhysicsComponent::stepSimulation
  ()
  {
    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto time = prLock->getTime().lock())
      {
        double time_delta = time->getFrameTimeDelta()/1000.0;

        LOG_TRACE("PhysicsComponent: {}", __FUNCTION__, time_delta);

        if (time_delta > 0.0)
        {
          mDynamicsWorld->stepSimulation(time_delta);
          checkContactManifolds();
        }
      }
    }
  }

  void
  PhysicsComponent::setGravity
  (const vec3& gravity)
  {
    LOG_DEBUG("PhysicsComponent: Setting Gravity {},{},{}" , gravity.x, gravity.y, gravity.z);
    if (mDynamicsWorld != nullptr)
    {
      mDynamicsWorld->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
    }
  }

  vec3
  PhysicsComponent::getGravity
  ()
  const
  {
    if (mDynamicsWorld != nullptr)
    {
      auto gv = mDynamicsWorld->getGravity();
      return Vector3::fromBullet(gv);
    }
    return vec3(0.0f);
  }

  bool
  PhysicsComponent::init
  ()
  {
    LOG_DEBUG( "PhysicsComponent: Initialising Component");

    mUpdateWorldTask = make_shared<PhysicsUpdateWorldTask>(mProjectRuntime,static_pointer_cast<PhysicsComponent>(shared_from_this()));
    mBroadphase = make_shared<btDbvtBroadphase>();
    mCollisionConfiguration = make_shared<btDefaultCollisionConfiguration>();
    mDispatcher = make_shared<btCollisionDispatcher>(mCollisionConfiguration.get());
    mSolver = make_shared<btSequentialImpulseConstraintSolver>();
    mDynamicsWorld = make_shared<btDiscreteDynamicsWorld>(mDispatcher.get(), mBroadphase.get(), mSolver.get(), mCollisionConfiguration.get());

    LOG_DEBUG("PhysicsComponent: Finished initialising PhysicsComponent");
    return true;
  }

  void
  PhysicsComponent::addRigidBody
  (const weak_ptr<btRigidBody>& rigidBody)
  {
    LOG_DEBUG( "PhysicsComponent: Adding Rigid Body to Dynamics World" );
    if (auto rbLock = rigidBody.lock())
    {
      mDynamicsWorld->addRigidBody(rbLock.get());
    }
    else
    {
      LOG_ERROR("PhysicsComponent: Unable to add rigid body to dynamics world, nullptr");
    }
    LOG_DEBUG( "PhysicsComponent: World has {} rigid bodies" , mDynamicsWorld->getNumCollisionObjects());
  }

  void
  PhysicsComponent::removePhysicsObjectRuntime
  (const weak_ptr<PhysicsObjectRuntime>& obj)
  {
    if (auto objLock = obj.lock())
    {
      removeRigidBody(objLock->getRigidBody());
    }
  }

  void
  PhysicsComponent::removeRigidBody
  (const weak_ptr<btRigidBody>& rigidBody)
  {
    LOG_DEBUG( "PhysicsComponent: Removing Rigid Body from Dynamics World" );
    if (auto rbLock = rigidBody.lock())
    {
      mDynamicsWorld->removeRigidBody(rbLock.get());
    }
    else
    {
      LOG_ERROR("PhysicsComponent: Unable to remove rigidBody, nullptr");
    }
  }

  void
  PhysicsComponent::addPhysicsObjectRuntime
  (const weak_ptr<PhysicsObjectRuntime>& physicsObjejct)
  {
    if (auto poLock = physicsObjejct.lock())
    {
      addRigidBody(poLock->getRigidBody());
    }
  }

  void
  PhysicsComponent::checkContactManifolds
  ()
  {
    LOG_TRACE("PhysicsComponent: Checking contact manifolds");
    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto scene = prLock->getActiveSceneRuntime().lock())
      {
        int numManifolds = mDynamicsWorld->getDispatcher()->getNumManifolds();
        for (int i=0;i<numManifolds;i++)
        {
          LOG_TRACE("PhysicsComponent: Checking manifold {}",i);
          btPersistentManifold* contactManifold = mDynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);

          auto objA = contactManifold->getBody0();
          auto objB = contactManifold->getBody1();

          if (objA != nullptr && objB != nullptr)
          {
            auto entityWeakA = getEntityRuntimeForCollisionObject(scene, objA);
            auto entityWeakB = getEntityRuntimeForCollisionObject(scene, objB);

            if (auto sObjA = entityWeakA.lock())
            {
              if (auto sObjB = entityWeakB.lock())
              {
                LOG_DEBUG("PhysicsComponent: Contact Manifold Found. Sending Event");

                Event aHitsB(sObjA->getAttributesMap());
                Event bHitsA(sObjB->getAttributesMap());

                int numContacts = contactManifold->getNumContacts();
                for (int j=0;j<numContacts;j++)
                {
                  btManifoldPoint& pt = contactManifold->getContactPoint(j);
                  if (pt.getDistance()>0.f)
                  {
                    continue;
                  }

                  auto ptA = pt.getPositionWorldOnA();
                  auto ptB = pt.getPositionWorldOnB();
                  auto impulse = pt.getAppliedImpulse();

                  aHitsB.setAttribute("collision","true");
                  aHitsB.setAttribute("collision.impulse",std::to_string(impulse));
                  aHitsB.setAttribute("collision.pos.x",std::to_string(ptB.x()));
                  aHitsB.setAttribute("collision.pos.y",std::to_string(ptB.y()));
                  aHitsB.setAttribute("collision.pos.z",std::to_string(ptB.z()));

                  bHitsA.setAttribute("collision","true");
                  bHitsA.setAttribute("collision.impulse",std::to_string(impulse));
                  bHitsA.setAttribute("collision.pos.x",std::to_string(ptA.x()));
                  bHitsA.setAttribute("collision.pos.y",std::to_string(ptA.y()));
                  bHitsA.setAttribute("collision.pos.z",std::to_string(ptA.z()));

                }
                sObjB->addEvent(aHitsB);
                sObjA->addEvent(bHitsA);
              }
            }
          }
        }
      }
    }
  }

  weak_ptr<EntityRuntime>
  PhysicsComponent::getEntityRuntimeForCollisionObject
  (const weak_ptr<SceneRuntime>& scene,
   const btCollisionObject* collObj)
  {
    if (auto sceneLock = scene.lock())
    {
      auto flatVector = sceneLock->getFlatVector();

      for (auto next : flatVector)
      {
        if (auto erLock = next.lock())
        {
          if (erLock->hasPhysicsObjectRuntime())
          {
            if (auto nextPO = erLock->getPhysicsObjectRuntime().lock())
            {
              if (nextPO->getCollisionObject().lock().get() == collObj)
              {
                return next;
              }
            }
          }
        }
      }
    }
    return weak_ptr<EntityRuntime>();
  }

  weak_ptr<PhysicsUpdateWorldTask>
  PhysicsComponent::getUpdateWorldTask
  ()
  {
    return mUpdateWorldTask;
  }

  void
  PhysicsComponent::setDebugDrawer
  (const weak_ptr<btIDebugDraw>& dd)
  {
    if (auto drawer = dd.lock())
    {
      mDynamicsWorld->setDebugDrawer(drawer.get());
    }
  }

  void PhysicsComponent::pushTasks()
  {
    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto taskQueue = prLock->getTaskQueue().lock())
      {
        taskQueue->pushTask(mUpdateWorldTask);
      }
    }
  }
}
