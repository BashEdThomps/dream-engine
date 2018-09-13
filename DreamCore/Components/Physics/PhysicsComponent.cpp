/*
* PhysicsInterface
*
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

#include <iostream>

#include "PhysicsDebugDrawer.h"
#include "PhysicsObjectInstance.h"
#include <btBulletDynamicsCommon.h>

#include "../IComponent.h"

#include "../Event.h"
#include "../../Common/Constants.h"

#include "../Time.h"

#include "../../Scene/SceneRuntime.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"

#include "../Transform3D.h"

#include "../../Utilities/String.h"

namespace Dream
{
    PhysicsComponent::PhysicsComponent
    ()
        : IComponent()
    {
        setLogClassName("PhysicsComponent");
        mDebug = false;
    }

    PhysicsComponent::~PhysicsComponent
    ()
    {
        auto log = getLog();
        log->info( "Destroying Object" );

        vector<btCollisionShape*> shapes;

        int i;
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
                log->info( "Deleting a motion state" );
                delete body->getMotionState();
            }
            log->info( "Deleting a collision object" );
            mDynamicsWorld->removeCollisionObject(obj);
            delete obj;
        }

        for (btCollisionShape* shape : shapes)
        {
            log->info( "Deleting Shape" );
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

        PhysicsObjectInstance::clearAssimpModelCache();
    }

    void
    PhysicsComponent::setGravity
    (vector<float> gravity)
    {
        auto log = getLog();
        log->info("Setting Gravity" , String::floatVectorToString(gravity) );
        mGravity = btVector3(gravity[0],gravity[1],gravity[2]);
        if (mDynamicsWorld != nullptr)
        {
            mDynamicsWorld->setGravity(mGravity);
        }
    }

    bool PhysicsComponent::init
    ()
    {
        auto log = getLog();
        log->info( "Initialising Component");
        mBroadphase = new btDbvtBroadphase();
        mCollisionConfiguration = new btDefaultCollisionConfiguration();
        mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
        mSolver = new btSequentialImpulseConstraintSolver();
        mDynamicsWorld = new btDiscreteDynamicsWorld(
                    mDispatcher,
                    mBroadphase,
                    mSolver,
                    mCollisionConfiguration
                    );
        mDynamicsWorld->setGravity(mGravity);

        mDebugDrawer.reset(new PhysicsDebugDrawer());
        mDebugDrawer->init();
        mDebugDrawer->setDebugMode(btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE);
        mDynamicsWorld->setDebugDrawer(mDebugDrawer.get());

        log->info("Finished initialising PhysicsComponent");
        return true;
    }

    void
    PhysicsComponent::updateComponent
    ()
    {
            beginUpdate();
            auto log = getLog();
            log->info( "Update Called" );

            populatePhysicsWorld(mActiveSceneRuntime);
            btScalar stepValue = 0.0;
            if (mTime != nullptr )
            {
                log->error("I don't haveTime for this");
            }
            stepValue = static_cast<btScalar>(mTime->getFrameTimeDelta());
            mDynamicsWorld->stepSimulation(stepValue);
            checkContactManifolds(mActiveSceneRuntime);
            // Put debug info to queue, no GL calls made
            if (mDebug)
            {
                mDynamicsWorld->debugDrawWorld();
            }
            endUpdate();
    }

    void
    PhysicsComponent::addRigidBody
    (btRigidBody *rigidBody)
    {
        auto log = getLog();
        log->info( "Adding Rigid Body to Dynamics World" );
        if (rigidBody != nullptr)
        {
            mDynamicsWorld->addRigidBody(rigidBody);
        }
        else
        {
            log->error("Unable to add rigid body to dynamics world, nullptr");
        }
        log->info( "World has {} rigid bodies" , mDynamicsWorld->getNumCollisionObjects());
    }

    void
    PhysicsComponent::removePhysicsObjectInstance
    (shared_ptr<PhysicsObjectInstance> obj)
    {
        removeRigidBody(obj->getRigidBody());
    }

    void
    PhysicsComponent::removeRigidBody
    (btRigidBody *rigidBody)
    {
        auto log = getLog();
        log->info( "Removing Rigid Body from Dynamics World" );
        if (rigidBody != nullptr)
        {
            mDynamicsWorld->removeRigidBody(rigidBody);
        }
        else
        {
            log->error("Unable to remove rigidBody, nullptr");
        }
    }

    void
    PhysicsComponent::addPhysicsObjectInstance
    (shared_ptr<PhysicsObjectInstance> physicsObjejct)
    {
        addRigidBody(physicsObjejct->getRigidBody());
    }

    void
    PhysicsComponent::populatePhysicsWorld
    (shared_ptr<SceneRuntime> scene)
    {
        auto log = getLog();
        scene->getRootSceneObjectRuntime()->applyToAll
                (
                    function<shared_ptr<SceneObjectRuntime> (shared_ptr<SceneObjectRuntime>)>
                    (
                        [&](shared_ptr<SceneObjectRuntime> so)
                        {
                        // Has physics
                        if (so->hasPhysicsObjectInstance())
                        {
                            auto physicsObject = so->getPhysicsObjectInstance();
                            if (!physicsObject->isInPhysicsWorld())
                            {
                                log->trace( "Adding SceneObject {} to physics world", so->getNameAndUuidString());
                                addPhysicsObjectInstance(physicsObject);
                                physicsObject->setInPhysicsWorld(true);
                            }
                            else
                            {
                                log->trace( "SceneObject {} is in the physics world",so->getNameAndUuidString());
                            }
                        }
                        return nullptr;
                    }
                    )
                );
    }

    void
    PhysicsComponent::setViewProjectionMatrix
    (glm::mat4 view, glm::mat4 proj)
    {
        if (mDebugDrawer != nullptr)
        {
            mDebugDrawer->setViewMatrix(view);
            mDebugDrawer->setProjectionMatrix(proj);
        }
    }

    void
    PhysicsComponent::setDebug
    (bool debug)
    {
        mDebug = debug;
    }

    void
    PhysicsComponent::checkContactManifolds
    (shared_ptr<SceneRuntime> scene)
    {
        int numManifolds = mDynamicsWorld->getDispatcher()->getNumManifolds();
        for (int i=0;i<numManifolds;i++)
        {
            btPersistentManifold* contactManifold;
            contactManifold =  mDynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
            const btCollisionObject* objA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
            const btCollisionObject* objB = static_cast<const btCollisionObject*>(contactManifold->getBody1());
            if (objA != nullptr && objB != nullptr)
            {
                auto sObjA = getSceneObjectRuntime(scene, objA);
                auto sObjB = getSceneObjectRuntime(scene, objB);
                if (sObjA != nullptr && sObjB != nullptr)
                {
                    Event e(sObjB->getUuid(),Constants::EVENT_TYPE_COLLISION);
                    sObjA->sendEvent(e);
                }
            }

            /*
           * More detail about contact.
           * Not needed for simple collison detection.
          int numContacts = contactManifold->getNumContacts();
          for (int j=0;j<numContacts;j++) {
              btManifoldPoint& pt = contactManifold->getContactPoint(j);
              if (pt.getDistance()<0.f) {
                  const btVector3& ptA = pt.getPositionWorldOnA();
                  const btVector3& ptB = pt.getPositionWorldOnB();
                  const btVector3& normalOnB = pt.m_normalWorldOnB;
              }
          }
          */
        }
    }

    shared_ptr<SceneObjectRuntime>
    PhysicsComponent::getSceneObjectRuntime
    (shared_ptr<SceneRuntime> scene, const btCollisionObject* collObj)
    {
        return scene->getRootSceneObjectRuntime()->applyToAll
        (
            function<shared_ptr<SceneObjectRuntime>(shared_ptr<SceneObjectRuntime>)>
            (
                [&](shared_ptr<SceneObjectRuntime> next)
                {
                    if (next->hasPhysicsObjectInstance())
                    {
                        auto nextPO = next->getPhysicsObjectInstance();
                        if (nextPO->getCollisionObject() == collObj)
                        {
                            return next;
                        }
                    }
                    return shared_ptr<SceneObjectRuntime>();
                }
            )
        );
    }

    void
    PhysicsComponent::drawDebug
    ()
    {
        if (mDebug)
        {
            mDebugDrawer->drawAll();
        }
    }

} // End of Dream
