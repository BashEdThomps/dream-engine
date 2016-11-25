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

namespace Dream {
  namespace Components {
    namespace Physics {

      PhysicsComponent::PhysicsComponent(void) : ComponentInterface() {
        mDebugDrawer = new GLDebugDrawer();
      }

      PhysicsComponent::~PhysicsComponent(void) {
        if (mSolver) delete mSolver;
        if (mDispatcher) delete mDispatcher;
        if (mCollisionConfiguration) delete mCollisionConfiguration;
        if (mBroadphase) delete mBroadphase;
        if (mDynamicsWorld) delete mDynamicsWorld;
        if (mDebugDrawer) delete mDebugDrawer;
      }

      void PhysicsComponent::setGravity3f(float x, float y, float z) {
        btVector3 gravity = btVector3(x,y,z);
        setGravityBtVector3(gravity);
      }

      void PhysicsComponent::setGravityBtVector3(btVector3 gravity) {
        mDynamicsWorld->setGravity(gravity);
      }

      bool PhysicsComponent::init(void) {
        std::cout << "PhysicsComponent: Initialising...";
        mBroadphase = new btDbvtBroadphase();
        mCollisionConfiguration = new btDefaultCollisionConfiguration();
        mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
        mSolver = new btSequentialImpulseConstraintSolver();
        mDynamicsWorld = new btDiscreteDynamicsWorld(
          mDispatcher,mBroadphase,mSolver,mCollisionConfiguration
        );
        mDynamicsWorld->setGravity(btVector3(0.0f,-1.0f,0.0f));
        mDynamicsWorld->setDebugDrawer(mDebugDrawer);
        mDebugDrawer->setDebugMode(btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE);
        std::cout << "done." << std::endl;
        return true;
      }

      void PhysicsComponent::update(Dream::Scene* scene) {
        btScalar stepValue = mTime->getTimeDelta();
        //std::cout << "BulletPhysics: Step Simulation by "
        //          << stepValue << std::endl;
        mDynamicsWorld->stepSimulation(stepValue);
        //std::cout << "BulletPhysics: Drawing Debug World"
        //          << std::endl;
        mDynamicsWorld->debugDrawWorld();
      }

      void PhysicsComponent::addRigidBody(btRigidBody *rigidBody) {
        std::cout << "BulletPhysics: Adding Rigid Body to Dynamics World" << std::endl;
        mDynamicsWorld->addRigidBody(rigidBody);
        std::cout << "BulletPhysicsWorld has "
                  << mDynamicsWorld->getNumCollisionObjects()
                  << " rigid bodies."
                  << std::endl;
      }

      void PhysicsComponent::removeRigidBody(btRigidBody *rigidBody) {
        std::cout << "BulletPhysics: Removing Rigid Body from Dynamics World" << std::endl;
        mDynamicsWorld->removeRigidBody(rigidBody);
      }

      void PhysicsComponent::addPhysicsObjectInstance(PhysicsObjectInstance *physicsObjejct) {
        addRigidBody(physicsObjejct->getRigidBody());
      }

      void PhysicsComponent::populatePhysicsWorld(std::vector<SceneObject*> soWithPhysicsObjects) {
        std::cout << "PhysicsComponent: Populating Physics World" << std::endl;
        std::vector<SceneObject*>::iterator soIter;

        for (soIter = soWithPhysicsObjects.begin(); soIter != soWithPhysicsObjects.end(); soIter++) {
          std::cout << "PhysicsComponent: Adding SceneObject " << (*soIter)->getUUID()
                    << " to PhysicsComponent World"
                    << std::endl;

          Physics::PhysicsObjectInstance* physicsObject;
          physicsObject = dynamic_cast<Physics::PhysicsObjectInstance*>(
            (*soIter)->getPhysicsObjectAssetInstance()
          );
          addPhysicsObjectInstance(physicsObject);
        }
      }

    } // End of Physics
  } // End of Components
} // End of Dream
