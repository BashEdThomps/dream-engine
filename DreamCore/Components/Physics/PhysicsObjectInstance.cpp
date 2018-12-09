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


#include "PhysicsObjectInstance.h"

#include "PhysicsMotionState.h"
#include "PhysicsComponent.h"
#include "PhysicsObjectDefinition.h"

#include "../../Scene/SceneRuntime.h"
#include "../../Project/ProjectDefinition.h"
#include "../../Project/ProjectRuntime.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"
#include "../Graphics/Model/ModelCache.h"
#include "../Graphics/Model/ModelInstance.h"
#include "../Graphics/Model/ModelMesh.h"

namespace Dream
{
    PhysicsObjectInstance::PhysicsObjectInstance
    (
        PhysicsObjectDefinition* definition,
        PhysicsComponent* comp,
        ModelCache* modelCache,
        SceneObjectRuntime* transform)
        : DiscreteAssetInstance(definition,transform),
         mCollisionShape(nullptr),
         mMotionState(nullptr),
         mRigidBody(nullptr),
         mRigidBodyConstructionInfo(nullptr),
         mInPhysicsWorld(false),
         mPhysicsComponentHandle(comp),
         mModelCache(modelCache)
    {
        setLogClassName("PhysicsObjectInstance");
        auto log = getLog();
        log->trace( "Constructing" );
        return;
    }

    PhysicsObjectInstance::~PhysicsObjectInstance
    ()
    {
        auto log = getLog();
        log->trace( "Destroying" );

        /***** Deletes are handled by PhysicsComponent! *****/

        if (mRigidBody != nullptr)
        {
            //delete mRigidBody;
            mRigidBody = nullptr;
        }

        if (mMotionState != nullptr)
        {
            //delete mMotionState;
            mMotionState = nullptr;
        }

        if (mCollisionShape != nullptr)
        {
            //delete mCollisionShape;
            mCollisionShape = nullptr;

        }

        if (mRigidBodyConstructionInfo != nullptr)
        {
            delete mRigidBodyConstructionInfo;
            mRigidBodyConstructionInfo = nullptr;
        }
    }

    btCollisionShape*
    PhysicsObjectInstance::getCollisionShape
    ()
    {
        return mCollisionShape;
    }

    bool
    PhysicsObjectInstance::load
    ()
    {
        auto log = getLog();
        auto pod = dynamic_cast<PhysicsObjectDefinition*>(mDefinition);
        mCollisionShape = createCollisionShape(pod);
        if (!mCollisionShape)
        {
            log->error( "Unable to create collision shape" );
            return false;
        }
        float mass = mDefinition->getJson()[Constants::ASSET_ATTR_MASS];
        // Transform and CentreOfMass
        mMotionState = new PhysicsMotionState(mSceneObjectRuntime->getTransform());
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

        if (pod->getControllableCharacter())
        {
            mRigidBody->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
            mRigidBody->setActivationState(DISABLE_DEACTIVATION);
            mRigidBody->setAngularFactor(btVector3(0,1,0));
        }

        if (pod->getKinematic())
        {
            mRigidBody->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
            mRigidBody->setActivationState(DISABLE_DEACTIVATION);
        }

        setRestitution(pod->getRestitution());
        setFriction(pod->getFriction());
        setCcdSweptSphereRadius(pod->getCcdSweptSphereRadius());

        mLoaded = (mRigidBody != nullptr);

        return mLoaded;
    }

    btCollisionShape*
    PhysicsObjectInstance::createCollisionShape
    (PhysicsObjectDefinition* pod)
    {
        auto log = getLog();
        string format = pod->getFormat();
        btCollisionShape *collisionShape = nullptr;

        if (format.compare(Constants::COLLISION_SHAPE_SPHERE) == 0)
        {
            btScalar radius = pod->getRadius();
            collisionShape = new btSphereShape(radius);
        }
        else if (format.compare(Constants::COLLISION_SHAPE_BOX) == 0)
        {
            btScalar boxX, boxY, boxZ;
            boxX = pod->getHalfExtentsX();
            boxY = pod->getHalfExtentsY();
            boxZ = pod->getHalfExtentsZ();
            collisionShape = new btBoxShape(btVector3(boxX,boxY,boxZ));
        }
        else if (format.compare(Constants::COLLISION_SHAPE_CYLINDER) == 0)
        {
            btScalar boxX, boxY, boxZ;
            boxX = pod->getHalfExtentsX();
            boxY = pod->getHalfExtentsY();
            boxZ = pod->getHalfExtentsZ();
            collisionShape = new btCylinderShape(btVector3(boxX,boxY,boxZ));
        }
        else if (format.compare(Constants::COLLISION_SHAPE_CAPSULE) == 0)
        {
            float radius = pod->getRadius();
            float height = pod->getHeight();
            collisionShape = new btCapsuleShape(radius,height);
        }
        else if (format.compare(Constants::COLLISION_SHAPE_CONE) == 0)
        {
            //collisionShape = new btConeShape();
        }
        else if (format.compare(Constants::COLLISION_SHAPE_MULTI_SPHERE) == 0)
        {
            //collisionShape = new btMultiSphereShape();
        }
        else if (format.compare(Constants::COLLISION_SHAPE_CONVEX_HULL) == 0)
        {
            //collisionShape = new btConvexHullShape();
        }
        else if (format.compare(Constants::COLLISION_SHAPE_CONVEX_TRIANGLE_MESH) == 0)
        {
            //collisionShape = new btConvexTriangleMeshShape();
        }
        else if (format.compare(Constants::COLLISION_SHAPE_BVH_TRIANGLE_MESH) == 0)
        {
            // Load Collision Data
            auto sceneRt = mSceneObjectRuntime->getSceneRuntime();
            if (sceneRt)
            {
                auto modelUuid = pod->getCollisionModel();
                auto pDef = dynamic_cast<ProjectDefinition*>(sceneRt->getProjectRuntime()->getDefinition());
                if (pDef)
                {
                    auto modelDef = pDef->getAssetDefinitionByUuid(modelUuid);
                    if (modelDef)
                    {
                        auto model = dynamic_cast<ModelInstance*>(mModelCache->getInstance(modelDef));
                        if (model)
                        {
                            collisionShape = createTriangleMeshShape(model);
                        }
                    }
                }
            }
        }
        else if (format.compare(Constants::COLLISION_SHAPE_HEIGHTFIELD_TERRAIN) == 0)
        {
            // ???
        }
        else if (format.compare(Constants::COLLISION_SHAPE_STATIC_PLANE) == 0)
        {
            float x = pod->getNormalX();
            float y = pod->getNormalY();
            float z = pod->getNormalZ();
            float constant = pod->getConstant();

            btVector3 planeNormal(x,y,z);
            btScalar planeConstant = btScalar(constant);

            collisionShape = new btStaticPlaneShape(planeNormal,planeConstant);
        }
        else if (format.compare(Constants::COLLISION_SHAPE_COMPOUND) == 0)
        {
            collisionShape = new btCompoundShape();
            btCompoundShape* compound = static_cast<btCompoundShape*>(collisionShape);

            for (CompoundChildDefinition child : pod->getCompoundChildren())
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
    }

    btCollisionShape*
    PhysicsObjectInstance::createTriangleMeshShape
    (ModelInstance* model)
    {
        btTriangleMesh *triMesh = new btTriangleMesh();
        auto meshes = model->getMeshes();

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
    }

    btRigidBody*
    PhysicsObjectInstance::getRigidBody
    ()
    {
        return mRigidBody;
    }

    void
    PhysicsObjectInstance::getWorldTransform
    (btTransform &transform)
    {
        mMotionState->getWorldTransform(transform);
    }

    btCollisionObject*
    PhysicsObjectInstance::getCollisionObject
    ()
    {
        return mRigidBody;
    }

    vec3
    PhysicsObjectInstance::getCenterOfMassPosition
    ()
    {
       auto tx = mRigidBody->getCenterOfMassPosition();
       return vec3(tx.x(),tx.y(),tx.z());
    }

    void
    PhysicsObjectInstance::applyCentralImpulse(vec3 force)
    {
        mRigidBody->applyCentralImpulse(btVector3(force.x,force.y,force.z));
    }

    void
    PhysicsObjectInstance::applyTorqueImpulse(vec3 torque)
    {
        mRigidBody->applyTorqueImpulse(btVector3(torque.x,torque.y,torque.z));
    }

    void
    PhysicsObjectInstance::applyForce
    (vec3 force)
    {
       mRigidBody->applyForce(btVector3(force.x,force.y,force.z),btVector3(0.0f,0.0f,0.0f));
    }

    void
    PhysicsObjectInstance::applyTorque
    (vec3 torque)
    {
        mRigidBody->applyTorque(btVector3(torque.x,torque.y,torque.z));
    }

    void
    PhysicsObjectInstance::clearForces
    ()
    {
       mRigidBody->clearForces();
    }

    void
    PhysicsObjectInstance::setCenterOfMassTransform
    (const Transform& tx)
    {
       mRigidBody->setCenterOfMassTransform(tx.getBtTransform());
    }

    void
    PhysicsObjectInstance::setCenterOfMassTransform
    (const mat4& tx)
    {
       mRigidBody->setCenterOfMassTransform(Transform(tx).getBtTransform());
    }

    void
    PhysicsObjectInstance::setWorldTransform
    (const Transform& tx)
    {
       mRigidBody->setWorldTransform(tx.getBtTransform());
    }

    void
    PhysicsObjectInstance::setLinearVelocity
    (float x, float y, float z)
    {
        mRigidBody->setLinearVelocity(btVector3(x,y,z));
    }

    vec3
    PhysicsObjectInstance::getLinearVelocity
    ()
    {
        auto lv = mRigidBody->getLinearVelocity();
        return vec3(lv.x(), lv.y(), lv.z());
    }

    PhysicsObjectDefinition*
    PhysicsObjectInstance::getAssetDefinitionByUuid
    (string uuid)
    {
        auto proj = mDefinition->getProject();
        return dynamic_cast<PhysicsObjectDefinition*>(proj->getAssetDefinitionByUuid(uuid));
    }

    void
    PhysicsObjectInstance::setLinearFactor
    (float x, float y, float z)
    {
        mRigidBody->setLinearFactor(btVector3(x,y,z));
    }

    void
    PhysicsObjectInstance::setAngularFactor
    (float x, float y, float z)
    {
        mRigidBody->setAngularFactor(btVector3(x,y,z));
    }

    void
    PhysicsObjectInstance::setAngularVelocity
    (float x, float y, float z)
    {
        mRigidBody->setAngularVelocity(btVector3(x,y,z));
    }

    void
    PhysicsObjectInstance::setRestitution
    (float r)
    {
       mRigidBody->setRestitution(r);
    }

    float
    PhysicsObjectInstance::getFriction
    () const
    {
        return mRigidBody->getFriction();
    }

    void
    PhysicsObjectInstance::setFriction
    (float friction)
    {
        mRigidBody->setFriction(friction);
    }

    float
    PhysicsObjectInstance::getMass
    () const
    {
       return mRigidBody->getInvMass();
    }

    void
    PhysicsObjectInstance::setMass
    (float mass)
    {
        mPhysicsComponentHandle->removeRigidBody(mRigidBody);
        btVector3 inertia(0.0f,0.0f,0.0f);
        mRigidBody->getCollisionShape()->calculateLocalInertia(mass,inertia);
        mRigidBody->setMassProps(mass,inertia);
        mPhysicsComponentHandle->addRigidBody(mRigidBody);
    }

    void
    PhysicsObjectInstance::setCcdSweptSphereRadius
    (float ccd)
    {
        if (ccd != 0.0f)
        {
            mRigidBody->setCcdMotionThreshold(1e-7f);
            mRigidBody->setCcdSweptSphereRadius(ccd);
        }
    }

    float
    PhysicsObjectInstance::getCcdSweptSphereRadius
    ()
    {
        return mRigidBody->getCcdSweptSphereRadius();
    }

    float
    PhysicsObjectInstance::getRestitution
    ()
    const
    {
        return mRigidBody->getRestitution();
    }

    bool PhysicsObjectInstance::isInPhysicsWorld()
    {
        return mInPhysicsWorld;
    }

    void PhysicsObjectInstance::setInPhysicsWorld(bool inPhysicsWorld)
    {
        mInPhysicsWorld = inPhysicsWorld;
    }

} // End of Dream
