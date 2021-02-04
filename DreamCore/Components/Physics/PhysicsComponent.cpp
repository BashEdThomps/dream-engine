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
#include "PhysicsDebugDrawer.h"
#include "PhysicsObjectRuntime.h"
#include "PhysicsTasks.h"
#include "Common/Logger.h"
#include "Components/Component.h"
#include "Components/Transform.h"
#include "Components/Event.h"
#include "Components/Time.h"
#include "Scene/SceneRuntime.h"
#include "Scene/Entity/EntityRuntime.h"
#include "Project/ProjectRuntime.h"

#include <btBulletDynamicsCommon.h>
#include <iostream>

namespace octronic::dream
{
    PhysicsComponent::PhysicsComponent
    (ProjectRuntime* pr)
        : Component("PhysicsComponent",pr),
          mDebugDrawer(nullptr),
          mDynamicsWorld(nullptr),
          mBroadphase(nullptr),
          mCollisionConfiguration(nullptr),
          mDispatcher(nullptr),
          mSolver(nullptr),
          mProjectionMatrix(mat4(1.0f)),
          mUpdateWorldTask(this),
          mDrawDebugTask(this),
          mDebug(false)
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

        // Dynamics World
        if (mDynamicsWorld != nullptr)
        {
            delete mDynamicsWorld;
            mDynamicsWorld = nullptr;
        }
        // Solver
        if (mSolver != nullptr)
        {
            delete mSolver;
            mSolver = nullptr;
        }
        // Broadphase
        if (mBroadphase != nullptr)
        {
            delete mBroadphase;
            mBroadphase = nullptr;
        }
        // Dispatcher
        if (mDispatcher != nullptr)
        {
            delete mDispatcher;
            mDispatcher = nullptr;
        }
        // Collision Config
        if (mCollisionConfiguration != nullptr)
        {
            delete mCollisionConfiguration;
            mCollisionConfiguration = nullptr;
        }

        if (mDebugDrawer)
        {
            delete mDebugDrawer;
            mDebugDrawer = nullptr;
        }
    }

    void
    PhysicsComponent::stepSimulation
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            double time_delta = mTime->getFrameTimeDelta()/1000.0;

            LOG_TRACE("PhysicsComponent: {}", __FUNCTION__, time_delta);

            if (time_delta > 0.0)
            {
                mDynamicsWorld->stepSimulation(time_delta);
                checkContactManifolds();
            }
        } dreamElseLockFailed
    }

    void
    PhysicsComponent::setGravity
    (const Vector3& gravity)
    {
        if(dreamTryLock())
        {
            dreamLock();
            LOG_DEBUG("PhysicsComponent: Setting Gravity {},{},{}" , gravity.x(), gravity.y(), gravity.z());
            if (mDynamicsWorld != nullptr)
            {
                mDynamicsWorld->setGravity(btVector3(gravity.x(), gravity.y(), gravity.z()));
            }
        } dreamElseLockFailed
    }

    Vector3
    PhysicsComponent::getGravity
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            if (mDynamicsWorld != nullptr)
            {
                auto gv = mDynamicsWorld->getGravity();
                return Vector3(gv.x(),gv.y(),gv.z());
            }
            return Vector3(0.0f);
        } dreamElseLockFailed
    }

    bool
    PhysicsComponent::init
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            LOG_DEBUG( "PhysicsComponent: Initialising Component");
            mBroadphase = new btDbvtBroadphase();
            mCollisionConfiguration = new btDefaultCollisionConfiguration();
            mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
            mSolver = new btSequentialImpulseConstraintSolver();
            mDynamicsWorld = new btDiscreteDynamicsWorld(
                        mDispatcher, mBroadphase, mSolver, mCollisionConfiguration
                        );
            //mDynamicsWorld->setGravity(mGravity);
            mDebugDrawer = new PhysicsDebugDrawer();
            mDebugDrawer->init();
            mDebugDrawer->setDebugMode(btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE);
            mDynamicsWorld->setDebugDrawer(mDebugDrawer);

            LOG_DEBUG("PhysicsComponent: Finished initialising PhysicsComponent");
            return true;
        } dreamElseLockFailed
    }

    void
    PhysicsComponent::addRigidBody
    (btRigidBody *rigidBody)
    {
        if(dreamTryLock())
        {
            dreamLock();
            LOG_DEBUG( "PhysicsComponent: Adding Rigid Body to Dynamics World" );
            if (rigidBody != nullptr)
            {
                mDynamicsWorld->addRigidBody(rigidBody);
            }
            else
            {
                LOG_ERROR("PhysicsComponent: Unable to add rigid body to dynamics world, nullptr");
            }
            LOG_DEBUG( "PhysicsComponent: World has {} rigid bodies" , mDynamicsWorld->getNumCollisionObjects());
        } dreamElseLockFailed
    }

    void
    PhysicsComponent::removePhysicsObjectRuntime
    (PhysicsObjectRuntime* obj)
    {
        if(dreamTryLock())
        {
            dreamLock();
            removeRigidBody(obj->getRigidBody());
        } dreamElseLockFailed
    }

    void
    PhysicsComponent::removeRigidBody
    (btRigidBody *rigidBody)
    {
        if(dreamTryLock())
        {
            dreamLock();
            LOG_DEBUG( "PhysicsComponent: Removing Rigid Body from Dynamics World" );
            if (rigidBody != nullptr)
            {
                mDynamicsWorld->removeRigidBody(rigidBody);
            }
            else
            {
                LOG_ERROR("PhysicsComponent: Unable to remove rigidBody, nullptr");
            }
        } dreamElseLockFailed
    }

    void
    PhysicsComponent::addPhysicsObjectRuntime
    (PhysicsObjectRuntime* physicsObjejct)
    {
        if(dreamTryLock())
        {
            dreamLock();
            auto rb = physicsObjejct->getRigidBody();
            addRigidBody(rb);
        } dreamElseLockFailed
    }

    void
    PhysicsComponent::setCamera
    (Camera* camera)
    {
        if(dreamTryLock())
        {
            dreamLock();
            if (mDebugDrawer != nullptr)
            {
                mDebugDrawer->setCamera(camera);
            }
        } dreamElseLockFailed
    }

    void
    PhysicsComponent::setDebug
    (bool debug)
    {
        if(dreamTryLock())
        {
            dreamLock();
            mDebug = debug;
        } dreamElseLockFailed
    }

    bool PhysicsComponent::getDebug()
    {
        if(dreamTryLock())
        {
            dreamLock();
            return mDebug;
        } dreamElseLockFailed
    }

    void
    PhysicsComponent::checkContactManifolds
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            LOG_TRACE("PhysicsComponent: Checking contact manifolds");
            auto scene = mProjectRuntime->getActiveSceneRuntime();
            int numManifolds = mDynamicsWorld->getDispatcher()->getNumManifolds();
            for (int i=0;i<numManifolds;i++)
            {
                LOG_TRACE("PhysicsComponent: Checking manifold {}",i);
                btPersistentManifold* contactManifold;
                contactManifold = mDynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);

                auto* objA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
                auto* objB = static_cast<const btCollisionObject*>(contactManifold->getBody1());

                if (objA != nullptr && objB != nullptr)
                {
                    auto* sObjA = getEntityRuntime(scene, objA);
                    auto* sObjB = getEntityRuntime(scene, objB);

                    if (sObjA != nullptr && sObjB != nullptr)
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

                            if (sObjA->isPlayerObject())
                            {
                                aHitsB.setAttribute("character","true");
                            }
                            else if (sObjB->isPlayerObject())
                            {
                                bHitsA.setAttribute("character","true");
                            }

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
                    else
                    {
                        LOG_ERROR("PhysicsComponent: Contact Manifold Found but Entitys are nullptr");
                    }
                }
                else
                {
                    LOG_ERROR("PhysicsComponent: Contact Manifold Found but Collision Objects are nullptr");
                }
            }
        } dreamElseLockFailed
    }

    EntityRuntime*
    PhysicsComponent::getEntityRuntime
    (SceneRuntime* scene, const btCollisionObject* collObj)
    {
        if(dreamTryLock()) {
            dreamLock();
            return scene->getRootEntityRuntime()->applyToAll
                    (
                        function<EntityRuntime*(EntityRuntime*)>
                        (
                            [&](EntityRuntime* next)
                        {
                            if (next->hasPhysicsObjectRuntime())
                            {
                                auto nextPO = next->getPhysicsObjectRuntime();
                                if (nextPO->getCollisionObject() == collObj)
                                {
                                    return next;
                                }
                            }
                            return static_cast<EntityRuntime*>(nullptr);
                        }
                        )
                    );
        } dreamElseLockFailed
    }

    void
    PhysicsComponent::drawDebug
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            mDebugDrawer->drawAll();
        } dreamElseLockFailed
    }

    PhysicsDebugDrawer* PhysicsComponent::getDebugDrawer()
    {
        if(dreamTryLock()) {
            dreamLock();
            return mDebugDrawer;
        } dreamElseLockFailed
    }

    PhysicsUpdateWorldTask *PhysicsComponent::getUpdateWorldTask()
    {
        if(dreamTryLock()) {
            dreamLock();
            return &mUpdateWorldTask;
        } dreamElseLockFailed
    }

    PhysicsDrawDebugTask* PhysicsComponent::getDrawDebugTask()
    {
        if(dreamTryLock())
        {
            dreamLock();
            return &mDrawDebugTask;
        } dreamElseLockFailed
    }
}
