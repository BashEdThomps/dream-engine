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
#include <iostream>
#include "PhysicsDebugDrawer.h"
#include "PhysicsObjectRuntime.h"
#include <btBulletDynamicsCommon.h>
#include "PhysicsTasks.h"
#include "../Component.h"
#include "../Transform.h"
#include "../Event.h"
#include "../Time.h"
#include "../../Scene/SceneRuntime.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"
#include "../../Project/ProjectRuntime.h"

namespace Dream
{
    PhysicsComponent::PhysicsComponent
    (ProjectRuntime* pr)
        : Component(pr),
          mDebugDrawer(nullptr),
          mDynamicsWorld(nullptr),
          mBroadphase(nullptr),
          mCollisionConfiguration(nullptr),
          mDispatcher(nullptr),
          mSolver(nullptr),
          mProjectionMatrix(mat4(1.0f)),
          mCharacter(nullptr),
          mUpdateWorldTask(this),
          mDrawDebugTask(this),
          mDebug(false)
    {
        #ifdef DREAM_LOG
        setLogClassName("PhysicsComponent");
        #endif
    }

    PhysicsComponent::~PhysicsComponent
    ()
    {
        #ifdef DREAM_LOG
        getLog()->debug( "Destroying Object" );
        #endif

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
                #ifdef DREAM_LOG
                getLog()->debug( "Deleting a motion state" );
                #endif
                delete body->getMotionState();
            }

            #ifdef DREAM_LOG
            getLog()->debug( "Deleting a collision object" );
            #endif
            mDynamicsWorld->removeCollisionObject(obj);
            delete obj;
        }

        for (btCollisionShape* shape : shapes)
        {
            #ifdef DREAM_LOG
            getLog()->debug( "Deleting Shape" );
            #endif
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
    PhysicsComponent::stepSimulation
    ()
    {
        mDynamicsWorld->stepSimulation(mTime->getFrameTimeDelta()/1000.0);
        checkContactManifolds();
    }

    void
    PhysicsComponent::setGravity
    (const vec3& gravity)
    {
        #ifdef DREAM_LOG
        getLog()->debug("Setting Gravity {},{},{}" , gravity.x, gravity.y, gravity.z);
        #endif
        if (mDynamicsWorld != nullptr)
        {
            mDynamicsWorld->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
        }
    }

    vec3
    PhysicsComponent::getGravity
    ()
    {
        if (mDynamicsWorld != nullptr)
        {
            auto gv = mDynamicsWorld->getGravity();
            return vec3(gv.x(),gv.y(),gv.z());
        }
        return vec3(0.0f);
    }

    bool
    PhysicsComponent::init
    ()
    {
        #ifdef DREAM_LOG
        getLog()->debug( "Initialising Component");
        #endif
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

        #ifdef DREAM_LOG
        getLog()->debug("Finished initialising PhysicsComponent");
        #endif
        return true;
    }

    void
    PhysicsComponent::addRigidBody
    (btRigidBody *rigidBody)
    {
        #ifdef DREAM_LOG
        getLog()->debug( "Adding Rigid Body to Dynamics World" );
        #endif
        if (rigidBody != nullptr)
        {
            mDynamicsWorld->addRigidBody(rigidBody);
        }
        #ifdef DREAM_LOG
        else
        {
            getLog()->error("Unable to add rigid body to dynamics world, nullptr");
        }
        getLog()->debug( "World has {} rigid bodies" , mDynamicsWorld->getNumCollisionObjects());
        #endif
    }

    void
    PhysicsComponent::removePhysicsObjectRuntime
    (PhysicsObjectRuntime* obj)
    {
        removeRigidBody(obj->getRigidBody());
    }

    void
    PhysicsComponent::removeRigidBody
    (btRigidBody *rigidBody)
    {
        #ifdef DREAM_LOG
        getLog()->debug( "Removing Rigid Body from Dynamics World" );
        #endif
        if (rigidBody != nullptr)
        {
            mDynamicsWorld->removeRigidBody(rigidBody);
        }
        #ifdef DREAM_LOG
        else
        {
            getLog()->error("Unable to remove rigidBody, nullptr");
        }
        #endif
    }

    void
    PhysicsComponent::addPhysicsObjectRuntime
    (PhysicsObjectRuntime* physicsObjejct)
    {
        auto rb = physicsObjejct->getRigidBody();
        addRigidBody(rb);
        if (
            (rb->getCollisionFlags() & btCollisionObject::CF_CHARACTER_OBJECT)
            == btCollisionObject::CF_CHARACTER_OBJECT
        )
        {
            mCharacter = physicsObjejct;
        }
    }

    void
    PhysicsComponent::setCamera
    (Camera* camera)
    {
        if (mDebugDrawer != nullptr)
        {
            mDebugDrawer->setCamera(camera);
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
    ()
    {
        #ifdef DREAM_LOG
        getLog()->trace("Checking contact manifolds");
        #endif
        auto scene = mProjectRuntime->getActiveSceneRuntime();
        int numManifolds = mDynamicsWorld->getDispatcher()->getNumManifolds();
        for (int i=0;i<numManifolds;i++)
        {
            #ifdef DREAM_LOG
            getLog()->trace("Checking manifold {}",i);
            #endif
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
                    #ifdef DREAM_LOG
                    getLog()->debug("Contact Manifold Found. Sending Event");
                    #endif

                    Event aHitsB(sObjA);
                    Event bHitsA(sObjB);

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

                        CollisionData aHitsBData;
                        aHitsBData.present = true;
                        aHitsBData.impulse = impulse;
                        aHitsBData.position.x = ptB.x();
                        aHitsBData.position.y = ptB.y();
                        aHitsBData.position.z = ptB.z();
                        aHitsB.setCollisionData(aHitsBData);

                        CollisionData bHitsAData;
                        bHitsAData.present = true;
                        bHitsAData.impulse = impulse;
                        bHitsAData.position.x = ptA.x();
                        bHitsAData.position.y = ptA.y();
                        bHitsAData.position.z = ptA.z();
                        bHitsA.setCollisionData(bHitsAData);
                     }
                    sObjB->addEvent(aHitsB);
                    sObjA->addEvent(bHitsA);

                    // Recover
                    if (sObjA->getPhysicsObjectRuntime() == mCharacter)
                    {
                       recoverCharacter(contactManifold);
                    }
                    else if (sObjB->getPhysicsObjectRuntime() == mCharacter)
                    {
                       recoverCharacter(contactManifold);
                    }
                }
                else
                {
                    #ifdef DREAM_LOG
                    getLog()->error("Contact Manifold Found but SceneObjects are nullptr");
                    #endif
                }
            }
            else
            {
                #ifdef DREAM_LOG
                getLog()->error("Contact Manifold Found but Collision Objects are nullptr");
                #endif
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
                    if (next->hasPhysicsObjectRuntime())
                    {
                        auto nextPO = next->getPhysicsObjectRuntime();
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
        mDebugDrawer->drawAll();
    }

    bool
    PhysicsComponent::recoverFromPenetration
    (btPersistentManifold* manifold)
    {
        bool penetration = false;
        auto body =  mCharacter->getRigidBody();
        auto currentPosition = body->getWorldTransform().getOrigin();
        float maxPenDepth = 0.01f;
        btScalar directionSign = manifold->getBody0() == body ? btScalar(-1.0) : btScalar(1.0);
        for (int p = 0; p < manifold->getNumContacts(); p++)
        {
            const btManifoldPoint& pt = manifold->getContactPoint(p);
            btScalar dist = pt.getDistance();

            #ifdef DREAM_LOG
            getLog()->trace("dist: {} dirSign: {} mpd: {}",dist,directionSign,maxPenDepth);
            #endif
            if (dist < -maxPenDepth)
            {

                #ifdef DREAM_LOG
                getLog()->trace("recovered");
                #endif
                currentPosition += pt.m_normalWorldOnB * directionSign * dist * btScalar(0.2);
                penetration = true;
            }
        }
        btTransform newTrans = body->getWorldTransform();
        newTrans.setOrigin(currentPosition);
        body->setWorldTransform(newTrans);
        return penetration;
    }

    bool
    PhysicsComponent::needsCollision
    (const btCollisionObject* body0, const btCollisionObject* body1)
    {
        bool collides = (body0->getBroadphaseHandle()->m_collisionFilterGroup & body1->getBroadphaseHandle()->m_collisionFilterMask) != 0;
        collides = collides && (body1->getBroadphaseHandle()->m_collisionFilterGroup & body0->getBroadphaseHandle()->m_collisionFilterMask);
        return collides;
    }

    void PhysicsComponent::recoverCharacter(btPersistentManifold* manifold)
    {
        if (mCharacter)
        {
            int recoveryLoops = 0;
            do
            {
                if (!recoverFromPenetration(manifold))
                {
                    recoveryLoops++;
                }
                else
                {
                    break;
                }
            }
            while(recoveryLoops < 10);
        }
    }

    void
    PhysicsComponent::setCharacter
    (PhysicsObjectRuntime* character)
    {
        mCharacter=character;
    }

    PhysicsDebugDrawer*PhysicsComponent::getDebugDrawer()
    {
        return mDebugDrawer;
    }
}
