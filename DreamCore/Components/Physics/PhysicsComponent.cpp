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
        mDebug = false;
    }

    PhysicsComponent::~PhysicsComponent
    ()
    {
            cout << "PhysicsComponent: Destroying Object" << endl;

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
                    cout << "PhysicsComponent: Deleting a motion state" << endl;
                delete body->getMotionState();
            }
                cout << "PhysicsComponent: Deleting a collision object" << endl;
            mDynamicsWorld->removeCollisionObject(obj);
            delete obj;
        }

        for (btCollisionShape* shape : shapes)
        {
                cout << "PhysicsComponent: Deleting Shape" << endl;
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
    }

    void
    PhysicsComponent::setGravity
    (vector<float> gravity)
    {
            cout <<"PhysicsComponent: Setting Gravity" << String::floatVectorToString(gravity) << endl;
        mGravity = btVector3(gravity[0],gravity[1],gravity[2]);
        if (mDynamicsWorld != nullptr)
        {
            mDynamicsWorld->setGravity(mGravity);
        }
    }

    bool PhysicsComponent::init
    ()
    {
            cout << "PhysicsComponent: Initialising...";
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

            cout << "done." << endl;
        return true;
    }

    void
    PhysicsComponent::updateComponent
    (SceneRuntime* scene)
    {
            cout << "PhysicsComponent: Update Called" << endl;

        populatePhysicsWorld(scene);

        btScalar stepValue = static_cast<btScalar>(mTime->getTimeDelta());
        mDynamicsWorld->stepSimulation(stepValue);
        checkContactManifolds(scene);

        if (mDebug)
        {
            mDynamicsWorld->debugDrawWorld();
        }
    }

    void
    PhysicsComponent::addRigidBody
    (btRigidBody *rigidBody)
    {
            cout << "PhysicsComponent: Adding Rigid Body to Dynamics World" << endl;
        mDynamicsWorld->addRigidBody(rigidBody);
            cout << "PhysicsComponent: World has " << mDynamicsWorld->getNumCollisionObjects()
                 << " rigid bodies." << endl;
    }

    void
    PhysicsComponent::removePhysicsObjectInstance
    (PhysicsObjectInstance* obj)
    {
        removeRigidBody(obj->getRigidBody());
    }

    void
    PhysicsComponent::removeRigidBody
    (btRigidBody *rigidBody)
    {
            cout << "PhysicsComponent: Removing Rigid Body from Dynamics World" << endl;
        mDynamicsWorld->removeRigidBody(rigidBody);
    }

    void
    PhysicsComponent::addPhysicsObjectInstance
    (PhysicsObjectInstance *physicsObjejct)
    {
        addRigidBody(physicsObjejct->getRigidBody());
    }

    void
    PhysicsComponent::populatePhysicsWorld
    (SceneRuntime* scene)
    {
        scene->getRootSceneObjectRuntimeHandle()->applyToAll
        (
            function<void*(SceneObjectRuntime*)>
            (
                [&](SceneObjectRuntime* so)
                {
                    // Has physics
                    if (so->hasPhysicsObjectInstance())
                    {
                        PhysicsObjectInstance* physicsObject = so->getPhysicsObjectInstance();
                        if (!physicsObject->isInPhysicsWorld())
                        {
                                cout << "PhysicsComponent: Adding SceneObject "
                                     << so->getNameAndUuidString()
                                     << " to Physics World" << endl;
                            addPhysicsObjectInstance(physicsObject);
                            physicsObject->setInPhysicsWorld(true);
                        }
                        else
                        {
                                cout << "PhysicsComponent: SceneObject "
                                     << so->getNameAndUuidString()
                                     << " is in the Physics World" << endl;
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
    (SceneRuntime* scene)
    {
        int numManifolds = mDynamicsWorld->getDispatcher()->getNumManifolds();
        for (int i=0;i<numManifolds;i++)
        {
            btPersistentManifold* contactManifold;
            contactManifold =  mDynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
            const btCollisionObject* objA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
            const btCollisionObject* objB = static_cast<const btCollisionObject*>(contactManifold->getBody1());
            SceneObjectRuntime* sObjA = getSceneObjectRuntime(scene, objA);
            SceneObjectRuntime* sObjB = getSceneObjectRuntime(scene, objB);

            Event e(sObjB->getUuid(),Constants::EVENT_TYPE_COLLISION);
            sObjA->sendEvent(e);

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

    SceneObjectRuntime*
    PhysicsComponent::getSceneObjectRuntime
    (SceneRuntime* scene, const btCollisionObject* collObj)
    {
        return static_cast<SceneObjectRuntime*>
                (
                    scene->getRootSceneObjectRuntimeHandle()->applyToAll
                    (
                        function<void*(SceneObjectRuntime*)>
                        (
                            [&](SceneObjectRuntime* next)
        {
                            if (next->hasPhysicsObjectInstance())
                            {
                                PhysicsObjectInstance* nextPO = next->getPhysicsObjectInstance();
                                if (nextPO->getCollisionObject() == collObj)
                                {
                                    return next;
                                }
                            }
                            return static_cast<SceneObjectRuntime*>(nullptr);
                        }
                        )
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
