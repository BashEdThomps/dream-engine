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

#include <glm/vec3.hpp>
#include <LinearMath/btVector3.h>
#include <glm/matrix.hpp>
#include "../Component.h"
#include "PhysicsTasks.h"

using glm::vec3;
using glm::mat4;

class btDynamicsWorld;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btVector3;
class btRigidBody;
class btCollisionObject;
class btPersistentManifold;

namespace Dream
{
    class PhysicsDebugDrawer;
    class PhysicsObjectRuntime;
    class SceneRuntime;
    class SceneObjectRuntime;
    class Camera;


    class PhysicsComponent : public Component
    {
    protected:
        PhysicsDebugDrawer* mDebugDrawer;
        btDynamicsWorld *mDynamicsWorld;
        btBroadphaseInterface *mBroadphase;
        btDefaultCollisionConfiguration *mCollisionConfiguration;
        btCollisionDispatcher *mDispatcher;
        btSequentialImpulseConstraintSolver *mSolver;
        mat4 mProjectionMatrix;
        PhysicsObjectRuntime* mCharacter;
        PhysicsUpdateWorldTask mUpdateWorldTask;
        PhysicsDrawDebugTask mDrawDebugTask;

        bool mDebug;
        bool needsCollision(const btCollisionObject* body0, const btCollisionObject* body1);
        bool recoverFromPenetration(btPersistentManifold* );
        void recoverCharacter(btPersistentManifold*);

    public:
        PhysicsComponent(ProjectRuntime* pr);
        ~PhysicsComponent() override;
        void setGravity(const vec3&);
        vec3 getGravity();
        void setDebug(bool);
        bool getDebug();
        bool init() override;
        void addPhysicsObjectRuntime(PhysicsObjectRuntime*);
        void setCharacter(PhysicsObjectRuntime*);
        void addRigidBody(btRigidBody*);
        void removeRigidBody(btRigidBody*);
        void removePhysicsObjectRuntime(PhysicsObjectRuntime*);
        void setCamera(Camera* cam);
        void checkContactManifolds();
        SceneObjectRuntime* getSceneObjectRuntime(SceneRuntime* scene, const btCollisionObject*);
        void drawDebug();
        void stepSimulation();
        PhysicsDebugDrawer* getDebugDrawer();

        PhysicsUpdateWorldTask* getUpdateWorldTask();
        bool updateWorldTaskActive();
    };
}
