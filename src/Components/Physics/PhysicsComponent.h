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

#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include <iostream>

#include "PhysicsDebugDrawer.h"
#include "PhysicsObjectInstance.h"
#include <btBulletDynamicsCommon.h>
#include <LinearMath/btVector3.h>

#include "../ComponentInterface.h"
#include "../../String.h"
#include "../../Constants.h"

namespace Dream {

      class PhysicsComponent : public ComponentInterface {
      protected:
        PhysicsDebugDrawer *mDebugDrawer;
        btDynamicsWorld *mDynamicsWorld;
        btBroadphaseInterface *mBroadphase;
        btDefaultCollisionConfiguration *mCollisionConfiguration;
        btCollisionDispatcher *mDispatcher;
        btSequentialImpulseConstraintSolver *mSolver;
        glm::mat4 mProjectionMatrix;
        btVector3 mGravity;
        bool mDebug;
      public:
        PhysicsComponent();
        ~PhysicsComponent();
        void populatePhysicsWorld(vector<SceneObject*>);
        void setGravity(vector<float>);
        void setDebug(bool);
        bool init();
        void update(Scene*);
        void addPhysicsObjectInstance(PhysicsObjectInstance*);
        void addRigidBody(btRigidBody*);
        void removeRigidBody(btRigidBody*);
        void removePhysicsObjectInstance(PhysicsObjectInstance*);
        void setViewProjectionMatrix(glm::mat4, glm::mat4);
      };// End of PhysicsComponent

} // End of Dream

#endif // End of PHYSICSCOMPONENT_H
