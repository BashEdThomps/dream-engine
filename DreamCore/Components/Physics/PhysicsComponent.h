/*
 * PhysicsComponent
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

#pragma once

#include <vector>

#include <LinearMath/btVector3.h>

#include "../Component.h"
#include <glm/matrix.hpp>

using glm::mat4;
using std::vector;
using std::unique_ptr;

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
        btVector3 mGravity;
        PhysicsObjectRuntime* mCharacter;
        bool mDebug;
        bool needsCollision(const btCollisionObject* body0, const btCollisionObject* body1);
        bool recoverFromPenetration(btPersistentManifold* );
        void recoverCharacter(btPersistentManifold*);
    public:
        PhysicsComponent();
        ~PhysicsComponent() override;
        void populatePhysicsWorld(SceneRuntime* scene);
        void setGravity(vector<float>);
        vector<float> getGravity();
        void setDebug(bool);
        bool getDebug();
        bool init() override;
        void updateComponent(SceneRuntime* sr) override;
        void addPhysicsObjectInstance(PhysicsObjectRuntime*);
        void setCharacter(PhysicsObjectRuntime*);
        void addRigidBody(btRigidBody*);
        void removeRigidBody(btRigidBody*);
        void removePhysicsObjectInstance(PhysicsObjectRuntime*);
        void setCamera(Camera* cam);
        void checkContactManifolds(SceneRuntime* scene);
        SceneObjectRuntime* getSceneObjectRuntime(SceneRuntime* scene, const btCollisionObject*);
        void drawDebug();
    };// End of PhysicsComponent

} // End of Dream
