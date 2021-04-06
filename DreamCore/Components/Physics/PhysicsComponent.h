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
    class PhysicsObjectRuntime;
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
        void addPhysicsObjectRuntime(PhysicsObjectRuntime&);
        void removePhysicsObjectRuntime(PhysicsObjectRuntime&);
        void addRigidBody(btRigidBody*);
        void removeRigidBody(btRigidBody*);
        void checkContactManifolds();

        EntityRuntime& getEntityRuntimeForCollisionObject(SceneRuntime& scene, const btCollisionObject*);

        void stepSimulation();
        void pushTasks() override;
        void setDebugDrawer(btIDebugDraw* dd);
    private:
        bool needsCollision(const btCollisionObject* body0, const btCollisionObject* body1);
    private:
        btDynamicsWorld* mDynamicsWorld;
        btBroadphaseInterface* mBroadphase;
        btDefaultCollisionConfiguration* mCollisionConfiguration;
        btCollisionDispatcher* mDispatcher;
        btSequentialImpulseConstraintSolver* mSolver;
        shared_ptr<PhysicsUpdateWorldTask> mUpdateWorldTask;
    };
}
