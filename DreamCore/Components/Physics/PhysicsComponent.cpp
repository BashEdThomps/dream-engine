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
        log->debug( "Destroying Object" );

        int i;

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
                log->debug( "Deleting a motion state" );
                delete body->getMotionState();
            }
            log->debug( "Deleting a collision object" );
            mDynamicsWorld->removeCollisionObject(obj);
            delete obj;
        }

        for (btCollisionShape* shape : shapes)
        {
            log->debug( "Deleting Shape" );
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

        //PhysicsObjectInstance::clearAssimpModelCache();
    }

    void
    PhysicsComponent::setGravity
    (vector<float> gravity)
    {
        auto log = getLog();
        log->debug("Setting Gravity" , String::floatVectorToString(gravity) );
        mGravity = btVector3(gravity[0],gravity[1],gravity[2]);
        if (mDynamicsWorld != nullptr)
        {
            mDynamicsWorld->setGravity(mGravity);
        }
    }

    vector<float> PhysicsComponent::getGravity()
    {
        if (mDynamicsWorld != nullptr)
        {
            auto gv = mDynamicsWorld->getGravity();
            return vector<float>{gv.x(),gv.y(),gv.z()};
        }
        return vector<float>{0.0f,0.0f,0.0f};
    }

    bool PhysicsComponent::init
    ()
    {
        auto log = getLog();
        log->debug( "Initialising Component");
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

        mDebugDrawer = new PhysicsDebugDrawer();
        mDebugDrawer->init();
        mDebugDrawer->setDebugMode(btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE);
        mDynamicsWorld->setDebugDrawer(mDebugDrawer);

        log->debug("Finished initialising PhysicsComponent");
        return true;
    }

    void
    PhysicsComponent::updateComponent
    (SceneRuntime* sr)
    {
            beginUpdate();
            auto log = getLog();
            log->debug( "Update Called" );

            // Setup Physics
            setGravity(sr->getGravity());
            setDebug(sr->getPhysicsDebug());

            populatePhysicsWorld(sr);
            btScalar stepValue = 0.0;
            if (mTime == nullptr )
            {
                log->error("I don't haveTime for this");
                return;
            }
            stepValue = static_cast<btScalar>(mTime->getFrameTimeDelta());
            mDynamicsWorld->stepSimulation(stepValue);
            checkContactManifolds(sr);
            // Put debug info to queue, no draw calls made here sonny
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
        log->debug( "Adding Rigid Body to Dynamics World" );
        if (rigidBody != nullptr)
        {
            mDynamicsWorld->addRigidBody(rigidBody);
        }
        else
        {
            log->error("Unable to add rigid body to dynamics world, nullptr");
        }
        log->debug( "World has {} rigid bodies" , mDynamicsWorld->getNumCollisionObjects());
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
        auto log = getLog();
        log->debug( "Removing Rigid Body from Dynamics World" );
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
    (PhysicsObjectInstance* physicsObjejct)
    {
        addRigidBody(physicsObjejct->getRigidBody());
    }

    void
    PhysicsComponent::populatePhysicsWorld
    (SceneRuntime* scene)
    {
        auto log = getLog();
        scene->getRootSceneObjectRuntime()->applyToAll
        (
            function<SceneObjectRuntime*(SceneObjectRuntime*)>
            (
                [&](SceneObjectRuntime* so)
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

    bool PhysicsComponent::getDebug()
    {
       return mDebug;
    }

    void
    PhysicsComponent::checkContactManifolds
    (SceneRuntime* scene)
    {
        auto log = getLog();
        log->trace("Checking contact manifolds");
        int numManifolds = mDynamicsWorld->getDispatcher()->getNumManifolds();
        for (int i=0;i<numManifolds;i++)
        {
            log->trace("Checking manifold {}",i);
            btPersistentManifold* contactManifold;
            contactManifold = mDynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);

            auto* objA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
            auto* objB = static_cast<const btCollisionObject*>(contactManifold->getBody1());

            if (objA != nullptr && objB != nullptr)
            {
                auto* sObjA = getSceneObjectRuntime(scene, objA);
                auto* sObjB = getSceneObjectRuntime(scene, objB);

                if (sObjA != nullptr && sObjB != nullptr)
                {
                    log->debug("Contact Manifold Found. Sending Event");

                    Event aHitsB(sObjA,Constants::EVENT_TYPE_COLLISION);
                    Event bHitsA(sObjB,Constants::EVENT_TYPE_COLLISION);

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

                        json aHitsBData =
                        {
                            { "impulse", impulse },

                            { "pos",
                              {
                                {"x", ptB.x()},
                                {"y", ptB.y()},
                                {"z", ptB.z()},
                              }
                            }
                        };
                        aHitsB.setJson("physics",aHitsBData);

                        json bHitsAData =
                        {
                            { "impulse", impulse },

                            { "pos",
                              {
                                {"x", ptA.x()},
                                {"y", ptA.y()},
                                {"z", ptA.z()},
                              }
                            }
                        };
                        bHitsA.setJson("physics",bHitsAData);

                     }

                    sObjB->addEvent(aHitsB);
                    sObjA->addEvent(bHitsA);
                }
                else
                {
                    log->error("Contact Manifold Found but SceneObjects are nullptr");
                }
            }
            else
            {
                log->error("Contact Manifold Found but Collision Objects are nullptr");
            }



        }
    }

    SceneObjectRuntime*
    PhysicsComponent::getSceneObjectRuntime
    (SceneRuntime* scene, const btCollisionObject* collObj)
    {
        return scene->getRootSceneObjectRuntime()->applyToAll
        (
            function<SceneObjectRuntime*(SceneObjectRuntime*)>
            (
                [&](SceneObjectRuntime* next)
                {
                    if (next->hasPhysicsObjectInstance())
                    {
                        auto nextPO = next->getPhysicsObjectInstance();
                        if (nextPO->getCollisionObject() == collObj)
                        {
                            return next;
                        }
                    }
                    return static_cast<SceneObjectRuntime*>(nullptr);
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
