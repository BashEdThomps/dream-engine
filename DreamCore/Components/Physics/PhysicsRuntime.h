#pragma once

#include <string>
#include <map>
#include <vector>

#include <glm/glm.hpp>
#include <btBulletDynamicsCommon.h>

#include "Components/DiscreteAssetRuntime.h"
#include "Math/Transform.h"
#include "PhysicsTasks.h"


namespace octronic::dream
{
    class PhysicsMotionState;
    class PhysicsDefinition;
    class PhysicsComponent;
    class ModelRuntime;
    class EntityRuntime;

    class PhysicsRuntime : public DiscreteAssetRuntime
    {
    public:
        PhysicsRuntime(ProjectRuntime&, PhysicsDefinition&, EntityRuntime&);

        bool loadFromDefinition() override;

        btCollisionShape* createCollisionShape(PhysicsDefinition&);
        btCollisionShape* getCollisionShape() const;
        btRigidBody* getRigidBody() const;
        void getWorldTransform(btTransform&);
        btCollisionObject* getCollisionObject() const;

        vec3 getCenterOfMassPosition() const;
        void applyCentralImpulse(const vec3&);
        void applyTorqueImpulse(const vec3&);
        void applyForce(const vec3&);
        void applyTorque(const vec3&);
        void clearForces();

        void setCenterOfMassTransformTx(const Transform& tx);
        void setCenterOfMassTransform3fv(const vec3& tx);
        void setCenterOfMassTransformMat4(const mat4& tx);
        void setCenterOfMassTransform3f(float x, float y, float z);

        void setWorldTransform(const Transform& tx);

        vec3 getLinearVelocity() const;
        void setLinearVelocity(const vec3&);

        bool isInPhysicsWorld() const;
        void setInPhysicsWorld(bool inPhysicsWorld);

        void setLinearFactor(const vec3&);

        void setAngularFactor(const vec3&);
        void setAngularVelocity(const vec3&);

        float getRestitution() const;
        void setRestitution(float r);

        float getFriction() const;
        void setFriction(float friction);

        float getMass() const;
        void setMass(float mass);

        void  setCcdSweptSphereRadius(float);
        float getCcdSweptSphereRadius() const;
        void setCameraControllableCharacter();
        void setKinematic(bool setKenematic);

        void pushTasks() override;

    private:
        btCollisionShape* createTriangleMeshShape(ModelRuntime&);
        btCollisionShape* mCollisionShape;
        btMotionState* mMotionState;
        btRigidBody* mRigidBody;
        btRigidBody::btRigidBodyConstructionInfo* mRigidBodyConstructionInfo;
        shared_ptr<PhysicsAddObjectTask> mAddObjectTask;
        bool mInPhysicsWorld;
    };
}
