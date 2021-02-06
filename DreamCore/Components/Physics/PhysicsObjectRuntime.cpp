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

#include <iostream>

#include "PhysicsObjectRuntime.h"

#include "PhysicsMotionState.h"
#include "PhysicsComponent.h"
#include "PhysicsObjectDefinition.h"
#include "PhysicsTasks.h"

#include "Common/Constants.h"
#include "Components/Graphics/Model/ModelCache.h"
#include "Components/Graphics/Model/ModelRuntime.h"
#include "Components/Graphics/Model/ModelMesh.h"
#include "Scene/SceneRuntime.h"
#include "Scene/Entity/EntityRuntime.h"
#include "Project/ProjectDefinition.h"
#include "Project/ProjectRuntime.h"

namespace octronic::dream
{
    PhysicsObjectRuntime::PhysicsObjectRuntime
    (
            PhysicsObjectDefinition* definition,
            PhysicsComponent* comp,
            ModelCache* modelCache,
            EntityRuntime* transform)
        : DiscreteAssetRuntime("PhysicsObjectRuntime",definition,transform),
          mCollisionShape(nullptr),
          mMotionState(nullptr),
          mRigidBody(nullptr),
          mRigidBodyConstructionInfo(nullptr),
          mInPhysicsWorld(false),
          mPhysicsComponent(comp),
          mModelCache(modelCache),
          mAddObjectTask(mPhysicsComponent,this)
    {
        LOG_TRACE( "PhysicsObjectRuntime: Constructing" );
    }

    PhysicsObjectRuntime::~PhysicsObjectRuntime
    ()
    {
        LOG_TRACE( "PhysicsObjectRuntime: Destroying" );

        /***** Deletes are handled by PhysicsComponent! *****/

        if (mRigidBody != nullptr)
        {
            delete mRigidBody;
            mRigidBody = nullptr;
        }

        if (mMotionState != nullptr)
        {
            delete mMotionState;
            mMotionState = nullptr;
        }

        if (mCollisionShape != nullptr)
        {
            delete mCollisionShape;
            mCollisionShape = nullptr;
        }

        if (mRigidBodyConstructionInfo != nullptr)
        {
            delete mRigidBodyConstructionInfo;
            mRigidBodyConstructionInfo = nullptr;
        }
    }

    btCollisionShape*
    PhysicsObjectRuntime::getCollisionShape
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return mCollisionShape;
        } dreamElseLockFailed
    }

    // This has a leak when reusing Physics Object Shapes
    bool
    PhysicsObjectRuntime::useDefinition
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            auto pod = static_cast<PhysicsObjectDefinition*>(mDefinition);
            mCollisionShape = createCollisionShape(pod);
            if (!mCollisionShape)
            {
                LOG_ERROR( "PhysicsObjectRuntime: Unable to create collision shape" );
                return false;
            }
            float mass = mDefinition->getJson()[Constants::ASSET_ATTR_MASS];
            // Transform and CentreOfMass
            mMotionState = new PhysicsMotionState(mEntityRuntime);
            // Mass, MotionState, Shape and LocalInertia
            btVector3 inertia(0, 0, 0);
            mCollisionShape->calculateLocalInertia(mass, inertia);
            mRigidBodyConstructionInfo = new btRigidBody::btRigidBodyConstructionInfo
                    (
                        btScalar(mass),
                        mMotionState,
                        mCollisionShape,
                        inertia
                        );

            mRigidBody = new btRigidBody(*mRigidBodyConstructionInfo);

            Vector3 lf, lv, af, av;
            lf = pod->getLinearFactor();
            lv = pod->getLinearVelocity();
            af = pod->getAngularFactor();
            av = pod->getAngularVelocity();

            setLinearFactor(lf.x(),lf.y(),lf.z());
            setLinearVelocity(lv.x(),lv.y(),lv.z());
            setAngularFactor(af.x(),af.y(),af.z());
            setAngularVelocity(av.x(),av.y(),av.z());

            if (pod->getControllableCharacter())
            {
                setCameraControllableCharacter();
            }

            if (pod->getKinematic())
            {
                setKinematic(true);
            }

            setRestitution(pod->getRestitution());
            setFriction(pod->getFriction());
            setCcdSweptSphereRadius(pod->getCcdSweptSphereRadius());

            mLoaded = (mRigidBody != nullptr);
            return mLoaded;
        } dreamElseLockFailed
    }

    void
    PhysicsObjectRuntime::setCameraControllableCharacter
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            mRigidBody->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
            mRigidBody->setActivationState(DISABLE_DEACTIVATION);
            mRigidBody->setAngularFactor(btVector3(0,1,0));
        } dreamElseLockFailed
    }


    void
    PhysicsObjectRuntime::setKinematic
    (bool setKenematic)
    {
        if(dreamTryLock()) {
            dreamLock();
            if (setKenematic)
            {
                mRigidBody->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
                mRigidBody->setActivationState(DISABLE_DEACTIVATION);
            }
        } dreamElseLockFailed
    }

    PhysicsAddObjectTask*
    PhysicsObjectRuntime::getAddObjectTask()
    {
        if(dreamTryLock()) {
            dreamLock();
            return &mAddObjectTask;
        } dreamElseLockFailed
    }

    btCollisionShape*
    PhysicsObjectRuntime::createCollisionShape
    (PhysicsObjectDefinition* pod)
    {
        if(dreamTryLock()) {
            dreamLock();
            string format = pod->getFormat();
            btCollisionShape *collisionShape = nullptr;

            if (format == Constants::COLLISION_SHAPE_SPHERE)
            {
                btScalar radius = pod->getRadius();
                collisionShape = new btSphereShape(radius);
            }
            else if (format == Constants::COLLISION_SHAPE_BOX)
            {
                btScalar boxX, boxY, boxZ;
                boxX = pod->getHalfExtentsX();
                boxY = pod->getHalfExtentsY();
                boxZ = pod->getHalfExtentsZ();
                collisionShape = new btBoxShape(btVector3(boxX,boxY,boxZ));
            }
            else if (format == Constants::COLLISION_SHAPE_CYLINDER)
            {
                btScalar boxX, boxY, boxZ;
                boxX = pod->getHalfExtentsX();
                boxY = pod->getHalfExtentsY();
                boxZ = pod->getHalfExtentsZ();
                collisionShape = new btCylinderShape(btVector3(boxX,boxY,boxZ));
            }
            else if (format == Constants::COLLISION_SHAPE_CAPSULE)
            {
                float radius = pod->getRadius();
                float height = pod->getHeight();
                collisionShape = new btCapsuleShape(radius,height);
            }
            else if (format == Constants::COLLISION_SHAPE_CONE)
            {
                //collisionShape = new btConeShape();
            }
            else if (format == Constants::COLLISION_SHAPE_MULTI_SPHERE)
            {
                //collisionShape = new btMultiSphereShape();
            }
            else if (format == Constants::COLLISION_SHAPE_CONVEX_HULL)
            {
                //collisionShape = new btConvexHullShape();
            }
            else if (format == Constants::COLLISION_SHAPE_CONVEX_TRIANGLE_MESH)
            {
                //collisionShape = new btConvexTriangleMeshShape();
            }
            else if (format == Constants::COLLISION_SHAPE_BVH_TRIANGLE_MESH)
            {
                // Load Collision Data
                auto sceneRt = mEntityRuntime->getSceneRuntime();
                if (sceneRt)
                {
                    auto modelUuid = pod->getCollisionModel();
                    auto pDef = static_cast<ProjectDefinition*>(sceneRt->getProjectRuntime()->getDefinition());
                    if (pDef)
                    {
                        auto modelDef = pDef->getAssetDefinitionByUuid(modelUuid);
                        if (modelDef)
                        {
                            auto model = static_cast<ModelRuntime*>(mModelCache->getRuntime(modelDef));
                            if (model)
                            {
                                collisionShape = createTriangleMeshShape(model);
                            }
                        }
                    }
                }
            }
            else if (format == Constants::COLLISION_SHAPE_HEIGHTFIELD_TERRAIN)
            {
                // ???
            }
            else if (format == Constants::COLLISION_SHAPE_STATIC_PLANE)
            {
                float x = pod->getNormalX();
                float y = pod->getNormalY();
                float z = pod->getNormalZ();
                float constant = pod->getConstant();

                btVector3 planeNormal(x,y,z);
                auto planeConstant = btScalar(constant);

                collisionShape = new btStaticPlaneShape(planeNormal,planeConstant);
            }
            else if (format == Constants::COLLISION_SHAPE_COMPOUND)
            {
                collisionShape = new btCompoundShape();
                btCompoundShape* compound = static_cast<btCompoundShape*>(collisionShape);

                for (CompoundChildDefinition& child : pod->getCompoundChildren())
                {
                    auto def = getAssetDefinitionByUuid(child.uuid);
                    btCollisionShape *shape = createCollisionShape(def);
                    if (shape)
                    {
                        compound->addChildShape(child.transform.getBtTransform(),shape);
                    }
                }
            }

            if (collisionShape)
            {
                btScalar margin = pod->getMargin();
                collisionShape->setMargin(margin);
            }

            return collisionShape;
        } dreamElseLockFailed
    }

    btCollisionShape*
    PhysicsObjectRuntime::createTriangleMeshShape
    (ModelRuntime* model)
    {
        if(dreamTryLock()) {
            dreamLock();
            auto* triMesh = new btTriangleMesh();
            auto  meshes = model->getMeshes();

            if (meshes.empty())
            {
                return nullptr;
            }

            for (auto mesh : meshes)
            {
                auto idx = mesh->getIndices();
                auto verts = mesh->getVertices();
                for (size_t i=0; i<idx.size()-3;)
                {
                    btVector3 v1,v2,v3;

                    auto i1 = verts.at(idx.at(i)).Position;
                    i++;
                    auto i2 = verts.at(idx.at(i)).Position;
                    i++;
                    auto i3 = verts.at(idx.at(i)).Position;
                    i++;

                    v1.setX(i1.x);
                    v1.setY(i1.y);
                    v1.setZ(i1.z);

                    v2.setX(i2.x);
                    v2.setY(i2.y);
                    v2.setZ(i2.z);

                    v3.setX(i3.x);
                    v3.setY(i3.y);
                    v3.setZ(i3.z);

                    triMesh->addTriangle(v1,v2,v3);
                }
            }
            return new btBvhTriangleMeshShape(triMesh,true,true);
        } dreamElseLockFailed
    }

    btRigidBody*
    PhysicsObjectRuntime::getRigidBody
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return mRigidBody;
        } dreamElseLockFailed
    }

    void
    PhysicsObjectRuntime::getWorldTransform
    (btTransform &transform)
    {
        if(dreamTryLock()) {
            dreamLock();
            mMotionState->getWorldTransform(transform);
        } dreamElseLockFailed
    }

    btCollisionObject*
    PhysicsObjectRuntime::getCollisionObject
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return mRigidBody;
        } dreamElseLockFailed
    }

    Vector3
    PhysicsObjectRuntime::getCenterOfMassPosition
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return Vector3(mRigidBody->getCenterOfMassPosition());
        } dreamElseLockFailed
    }

    void
    PhysicsObjectRuntime::applyCentralImpulse(const Vector3& force)
    {
        if(dreamTryLock()) {
            dreamLock();
            mRigidBody->applyCentralImpulse(force.toBullet());
        } dreamElseLockFailed
    }

    void
    PhysicsObjectRuntime::applyTorqueImpulse(const Vector3& torque)
    {
        if(dreamTryLock()) {
            dreamLock();
            mRigidBody->applyTorqueImpulse(torque.toBullet());
        } dreamElseLockFailed
    }

    void
    PhysicsObjectRuntime::applyForce
    (const Vector3& force)
    {
        if(dreamTryLock()) {
            dreamLock();
            mRigidBody->applyForce(force.toBullet(),btVector3(0.0f,0.0f,0.0f));
        } dreamElseLockFailed
    }

    void
    PhysicsObjectRuntime::applyTorque
    (const Vector3& torque)
    {
        if(dreamTryLock()) {
            dreamLock();
            mRigidBody->applyTorque(torque.toBullet());
        } dreamElseLockFailed
    }

    void
    PhysicsObjectRuntime::clearForces
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            mRigidBody->clearForces();
        } dreamElseLockFailed
    }

    void
    PhysicsObjectRuntime::setCenterOfMassTransformTx
    (Transform& tx)
    {
        if(dreamTryLock()) {
            dreamLock();
            mRigidBody->setCenterOfMassTransform(tx.getBtTransform());
        } dreamElseLockFailed
    }

    void
    PhysicsObjectRuntime::setCenterOfMassTransformMat4
    (mat4 tx)
    {
        if(dreamTryLock()) {
            dreamLock();
            btTransform transform;
            transform.setFromOpenGLMatrix(value_ptr(tx));
            mRigidBody->setCenterOfMassTransform(transform);
        } dreamElseLockFailed
    }

    void
    PhysicsObjectRuntime::setCenterOfMassTransform3f
    (float x, float y, float z)
    {
        if(dreamTryLock()) {
            dreamLock();
            btTransform transform;
            auto current = mRigidBody->getCenterOfMassTransform();
            current.setOrigin(btVector3(x,y,z));
            mRigidBody->setCenterOfMassTransform(current);
        } dreamElseLockFailed
    }


    void
    PhysicsObjectRuntime::setCenterOfMassTransform3fv
    (const Vector3& tx)
    {
        if(dreamTryLock()) {
            dreamLock();
            LOG_DEBUG("PhysicsObjectRuntime: Setting Center of mass {},{},{}", tx.x(), tx.y(), tx.z());
            auto mtx = mRigidBody->getCenterOfMassTransform();
            mtx.setOrigin(tx.toBullet());
            mRigidBody->setCenterOfMassTransform(mtx);
        } dreamElseLockFailed
    }

    void
    PhysicsObjectRuntime::setWorldTransform
    (Transform& tx)
    {
        if(dreamTryLock()) {
            dreamLock();
            mRigidBody->setWorldTransform(tx.getBtTransform());
        } dreamElseLockFailed
    }

    void
    PhysicsObjectRuntime::setLinearVelocity
    (float x, float y, float z)
    {
        if(dreamTryLock()) {
            dreamLock();
            mRigidBody->setLinearVelocity(btVector3(x,y,z));
        } dreamElseLockFailed
    }

    Vector3
    PhysicsObjectRuntime::getLinearVelocity
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return Vector3(mRigidBody->getLinearVelocity());
        } dreamElseLockFailed
    }

    PhysicsObjectDefinition*
    PhysicsObjectRuntime::getAssetDefinitionByUuid
    (UuidType uuid)
    {
        if(dreamTryLock()) {
            dreamLock();
            // TODO why from def?
            auto proj = static_cast<AssetDefinition*>(mDefinition)->getProject();
            return static_cast<PhysicsObjectDefinition*>(proj->getAssetDefinitionByUuid(uuid));
        } dreamElseLockFailed
    }

    void
    PhysicsObjectRuntime::setLinearFactor
    (float x, float y, float z)
    {
        if(dreamTryLock()) {
            dreamLock();
            mRigidBody->setLinearFactor(btVector3(x,y,z));
        } dreamElseLockFailed
    }

    void
    PhysicsObjectRuntime::setAngularFactor
    (float x, float y, float z)
    {
        if(dreamTryLock()) {
            dreamLock();
            mRigidBody->setAngularFactor(btVector3(x,y,z));
        } dreamElseLockFailed
    }

    void
    PhysicsObjectRuntime::setAngularVelocity
    (float x, float y, float z)
    {
        if(dreamTryLock()) {
            dreamLock();
            mRigidBody->setAngularVelocity(btVector3(x,y,z));
        } dreamElseLockFailed
    }

    void
    PhysicsObjectRuntime::setRestitution
    (float r)
    {
        if(dreamTryLock()) {
            dreamLock();
            mRigidBody->setRestitution(r);
        } dreamElseLockFailed
    }

    float
    PhysicsObjectRuntime::getFriction
    () const
    {
        return mRigidBody->getFriction();
    }

    void
    PhysicsObjectRuntime::setFriction
    (float friction)
    {
        if(dreamTryLock()) {
            dreamLock();
            mRigidBody->setFriction(friction);
        } dreamElseLockFailed
    }

    float
    PhysicsObjectRuntime::getMass
    () const
    {
        return mRigidBody->getInvMass();
    }

    void
    PhysicsObjectRuntime::setMass
    (float mass)
    {
        if(dreamTryLock()) {
            dreamLock();
            mPhysicsComponent->removeRigidBody(mRigidBody);
            btVector3 inertia(0.0f,0.0f,0.0f);
            mRigidBody->getCollisionShape()->calculateLocalInertia(mass,inertia);
            mRigidBody->setMassProps(mass,inertia);
            mPhysicsComponent->addRigidBody(mRigidBody);
        } dreamElseLockFailed
    }

    void
    PhysicsObjectRuntime::setCcdSweptSphereRadius
    (float ccd)
    {
        if(dreamTryLock()) {
            dreamLock();
            if (ccd != 0.0f)
            {
                mRigidBody->setCcdMotionThreshold(1e-7f);
                mRigidBody->setCcdSweptSphereRadius(ccd);
            }
        } dreamElseLockFailed
    }

    float
    PhysicsObjectRuntime::getCcdSweptSphereRadius
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return mRigidBody->getCcdSweptSphereRadius();
        } dreamElseLockFailed
    }

    float
    PhysicsObjectRuntime::getRestitution
    ()
    const
    {
        return mRigidBody->getRestitution();
    }

    bool PhysicsObjectRuntime::isInPhysicsWorld()
    {
        if(dreamTryLock()) {
            dreamLock();
            return mInPhysicsWorld;
        } dreamElseLockFailed
    }

    void PhysicsObjectRuntime::setInPhysicsWorld(bool inPhysicsWorld)
    {
        if(dreamTryLock()) {
            dreamLock();
            mInPhysicsWorld = inPhysicsWorld;
        } dreamElseLockFailed
    }
}
