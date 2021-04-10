#pragma once

#include "PhysicsTasks.h"
#include "Components/Component.h"
#include <glm/matrix.hpp>
#include <glm/vec3.hpp>

#include <memory>

using glm::vec3;
using glm::mat4;
using std::shared_ptr;

class btDynamicsWorld;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btVector3;
class btRigidBody;
class btCollisionObject;
class btPersistentManifold;
class btIDebugDraw;

namespace octronic::dream
{
    class PhysicsRuntime;
    class SceneRuntime;
    class EntityRuntime;

    class PhysicsComponent : public Component
    {

    public:
        PhysicsComponent(ProjectRuntime& pr);

        PhysicsComponent(PhysicsComponent&&) = default;
        PhysicsComponent& operator=(PhysicsComponent&&) = default;

        ~PhysicsComponent();

        void setGravity(const vec3&);
        vec3 getGravity() const;
        bool init() override;
        void addPhysicsRuntime(PhysicsRuntime&);
        void removePhysicsRuntime(PhysicsRuntime&);
        void addRigidBody(btRigidBody*);
        void removeRigidBody(btRigidBody*);
        void checkContactManifolds();

        EntityRuntime& getEntityRuntimeForCollisionObject(SceneRuntime& scene, const btCollisionObject*);

        void stepSimulation();
        void pushTasks() override;
        void setDebugDrawer(btIDebugDraw* dd);
        bool hasDebugDrawer() const;
    private:
        bool needsCollision(const btCollisionObject* body0, const btCollisionObject* body1);
    private:
        btDynamicsWorld* mDynamicsWorld;
        btBroadphaseInterface* mBroadphase;
        btDefaultCollisionConfiguration* mCollisionConfiguration;
        btCollisionDispatcher* mDispatcher;
        btSequentialImpulseConstraintSolver* mSolver;
        btIDebugDraw* mDebugDrawer;
        shared_ptr<PhysicsUpdateWorldTask> mUpdateWorldTask;
    };
}
