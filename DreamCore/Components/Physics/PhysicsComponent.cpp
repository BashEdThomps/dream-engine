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

#include "../../Scene/Scene.h"
#include "../../Scene/SceneDefinition.h"

#include "../../Scene/SceneObject/SceneObject.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"
#include "../../Scene/SceneObject/SceneObjectDefinition.h"

#include "../Transform3D.h"

#include "../../Utilities/String.h"

namespace Dream
{
    PhysicsComponent::PhysicsComponent
    ()
        : IComponent()
    {
        mDebugDrawer = nullptr;
        mDebug = false;
    }

    PhysicsComponent::~PhysicsComponent
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "PhysicsComponent: Destroying Object" << endl;
        }
        // Dynamics World
        if (mDynamicsWorld != nullptr)
        {
            delete mDynamicsWorld;
            mDynamicsWorld = nullptr;
        }
        // DebugDrawer
        if (mDebugDrawer != nullptr)
        {
            delete mDebugDrawer;
            mDebugDrawer = nullptr;
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
        if (Constants::DEBUG)
        {
            cout <<"PhysicsComponent: Setting Gravity" << String::floatVectorToString(gravity) << endl;
        }
        mGravity = btVector3(gravity[0],gravity[1],gravity[2]);
        if (mDynamicsWorld != nullptr)
        {
            mDynamicsWorld->setGravity(mGravity);
        }
    }

    bool PhysicsComponent::init
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "PhysicsComponent: Initialising...";
        }
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

        if (Constants::DEBUG)
        {
            cout << "done." << endl;
        }
        return true;
    }

    void
    PhysicsComponent::updateComponent
    (Scene* scene)
    {
        if (Constants::VERBOSE)
        {
            cout << "PhysicsComponent: Update Called" << endl;
        }
        // Setup Debug
        if (mDebug && mDebugDrawer == nullptr)
        {
            mDebugDrawer = new PhysicsDebugDrawer();
            mDebugDrawer->init();
            mDebugDrawer->setDebugMode(btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE);
            mDynamicsWorld->setDebugDrawer(mDebugDrawer);
        }
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
        if (Constants::DEBUG)
        {
            cout << "PhysicsComponent: Adding Rigid Body to Dynamics World" << endl;
        }
        mDynamicsWorld->addRigidBody(rigidBody);
        if (Constants::DEBUG)
        {
            cout << "PhysicsComponent: World has " << mDynamicsWorld->getNumCollisionObjects()
                 << " rigid bodies." << endl;
        }
    }

    void
    PhysicsComponent::removePhysicsObjectInstance
    (PhysicsObjectInstance* obj)
    {
        if (obj->getInPhysicsWorld())
        {
            removeRigidBody(obj->getRigidBody());
            obj->setInPhysicsWorld(false);
        }
    }

    void
    PhysicsComponent::removeRigidBody
    (btRigidBody *rigidBody)
    {
        if (Constants::DEBUG)
        {
            cout << "PhysicsComponent: Removing Rigid Body from Dynamics World" << endl;
        }
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
    (Scene* scene)
    {
        scene->getRootSceneObjectHandle()->applyToAll
                (
                    function<void*(SceneObject*)>
                    (
                        [&](SceneObject* so)
                        {
                        // Has physics
                        if (so->getRuntimeHandle()->hasPhysicsObjectInstance())
                        {
                            PhysicsObjectInstance* physicsObject = so->getRuntimeHandle()->getPhysicsObjectInstance();
                            // Marked for deletion and in physics world, remove
                            if (so->getRuntimeHandle()->getDeleteFlag() && physicsObject->getInPhysicsWorld())
                            {
                                if (Constants::DEBUG)
                                {
                                    cout << "PhysicsComponent: Removing SceneObject " << so->getDefinitionHandle()->getUuid()
                                    << " from Physics World" << endl;
                                }
                                removePhysicsObjectInstance(physicsObject);
                            }
                            // Not marked for deletion and not in world, add
                            if (!so->getRuntimeHandle()->getDeleteFlag() && !physicsObject->getInPhysicsWorld())
                            {
                                if (Constants::DEBUG)
                                {
                                    cout << "PhysicsComponent: Adding SceneObject " << so->getDefinitionHandle()->getUuid()
                                    << " to Physics World" << endl;
                                }
                                addPhysicsObjectInstance(physicsObject);
                                physicsObject->setInPhysicsWorld(true);
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
    (Scene* scene)
    {
        int numManifolds = mDynamicsWorld->getDispatcher()->getNumManifolds();
        for (int i=0;i<numManifolds;i++)
        {
            btPersistentManifold* contactManifold;
            contactManifold =  mDynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
            const btCollisionObject* objA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
            const btCollisionObject* objB = static_cast<const btCollisionObject*>(contactManifold->getBody1());
            SceneObject* sObjA = getSceneObject(scene, objA);
            SceneObject* sObjB = getSceneObject(scene, objB);

            Event e(sObjB->getDefinitionHandle()->getUuid(),Constants::EVENT_TYPE_COLLISION);
            sObjA->getRuntimeHandle()->sendEvent(e);

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

    SceneObject*
    PhysicsComponent::getSceneObject
    (Scene* scene, const btCollisionObject* collObj)
    {
        return static_cast<SceneObject*>
                (
                    scene->getRootSceneObjectHandle()->applyToAll
                    (
                        function<void*(SceneObject*)>
                        (
                            [&](SceneObject* next)
        {
                            if (next->getRuntimeHandle()->hasPhysicsObjectInstance())
                            {
                                PhysicsObjectInstance* nextPO = next->getRuntimeHandle()->getPhysicsObjectInstance();
                                if (nextPO->getCollisionObject() == collObj)
                                {
                                    return next;
                                }
                            }
                            return static_cast<SceneObject*>(nullptr);
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

    void
    PhysicsComponent::cleanUp
    (Scene* scene)
    {
        if (Constants::DEBUG)
        {
            cout << "PhysicsComponent: Cleaning up physics for "
                 << scene->getDefinitionHandle()->getNameAndUuidString()
                 << endl;
        }

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
                if (Constants::DEBUG)
                {
                    cout << "PhysicsComponent: Deleting a motion state" << endl;
                }
                delete body->getMotionState();
            }
            if (Constants::DEBUG)
            {
                cout << "PhysicsComponent: Deleting a collision object" << endl;
            }
            mDynamicsWorld->removeCollisionObject(obj);
            delete obj;
        }

        for (btCollisionShape* shape : shapes)
        {
            if (Constants::DEBUG)
            {
                cout << "PhysicsComponent: Deleting Shape" << endl;
            }
            delete shape;
        }
    }

} // End of Dream
