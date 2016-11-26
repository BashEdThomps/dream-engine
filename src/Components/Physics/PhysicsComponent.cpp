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
        cout << "PhysicsComponent: Initialising...";
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
        cout << "done." << endl;
        return true;
      }

      void PhysicsComponent::update(Dream::Scene* scene) {
        btScalar stepValue = mTime->getTimeDelta();
        //cout << "BulletPhysics: Step Simulation by "
        //          << stepValue << endl;
        mDynamicsWorld->stepSimulation(stepValue);
        //cout << "BulletPhysics: Drawing Debug World"
        //          << endl;
        mDynamicsWorld->debugDrawWorld();
      }

      void PhysicsComponent::addRigidBody(btRigidBody *rigidBody) {
        cout << "BulletPhysics: Adding Rigid Body to Dynamics World" << endl;
        mDynamicsWorld->addRigidBody(rigidBody);
        cout << "BulletPhysicsWorld has "
                  << mDynamicsWorld->getNumCollisionObjects()
                  << " rigid bodies."
                  << endl;
      }

      void PhysicsComponent::removeRigidBody(btRigidBody *rigidBody) {
        cout << "BulletPhysics: Removing Rigid Body from Dynamics World" << endl;
        mDynamicsWorld->removeRigidBody(rigidBody);
      }

      void PhysicsComponent::addPhysicsObjectInstance(PhysicsObjectInstance *physicsObjejct) {
        addRigidBody(physicsObjejct->getRigidBody());
      }

      void PhysicsComponent::populatePhysicsWorld(vector<SceneObject*> soWithPhysicsObjects) {
        cout << "PhysicsComponent: Populating Physics World" << endl;
        vector<SceneObject*>::iterator soIter;

        for (soIter = soWithPhysicsObjects.begin(); soIter != soWithPhysicsObjects.end(); soIter++) {
          cout << "PhysicsComponent: Adding SceneObject " << (*soIter)->getUUID()
                    << " to PhysicsComponent World"
                    << endl;

          PhysicsObjectInstance* physicsObject;
          physicsObject = dynamic_cast<PhysicsObjectInstance*>(
            (*soIter)->getPhysicsObjectInstance()
          );
          addPhysicsObjectInstance(physicsObject);
        }
      }

} // End of Dream
