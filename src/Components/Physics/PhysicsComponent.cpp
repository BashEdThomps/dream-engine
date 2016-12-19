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

  PhysicsComponent::PhysicsComponent() : ComponentInterface() {
      mDebugDrawer = new GLDebugDrawer();
  }

  PhysicsComponent::~PhysicsComponent() {
      if (mSolver != nullptr) {
          delete mSolver;
          mSolver = nullptr;
      }
      if (mDispatcher != nullptr) {
          delete mDispatcher;
          mDispatcher = nullptr;
      }
      if (mCollisionConfiguration != nullptr) {
          delete mCollisionConfiguration;
          mCollisionConfiguration = nullptr;
      }
      if (mBroadphase != nullptr) {
          delete mBroadphase;
          mBroadphase = nullptr;
      }
      if (mDynamicsWorld != nullptr) {
          delete mDynamicsWorld;
          mDynamicsWorld = nullptr;
      }
      if (mDebugDrawer != nullptr) {
          delete mDebugDrawer;
          mDebugDrawer = nullptr;
      }
  }

  void PhysicsComponent::setGravity3f(float x, float y, float z) {
      btVector3 gravity = btVector3(x,y,z);
      setGravityBtVector3(gravity);
  }

  void PhysicsComponent::setGravityBtVector3(btVector3 gravity) {
      mDynamicsWorld->setGravity(gravity);
  }

  bool PhysicsComponent::init() {
      if (DEBUG) {
          cout << "PhysicsComponent: Initialising...";
      }
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
      if (DEBUG) {
          cout << "done." << endl;
      }
      return true;
  }

  void PhysicsComponent::update(Dream::Scene* scene) {
    populatePhysicsWorld(scene->getScenegraphVector());
    btScalar stepValue = mTime->getTimeDelta();
    mDynamicsWorld->stepSimulation(stepValue);
    mDynamicsWorld->debugDrawWorld();
  }

  void PhysicsComponent::addRigidBody(btRigidBody *rigidBody) {
      if (DEBUG) {
          cout << "PhysicsComponent: Adding Rigid Body to Dynamics World" << endl;
      }
      mDynamicsWorld->addRigidBody(rigidBody);
      if (DEBUG) {
        cout << "PhysicsComponent: World has " << mDynamicsWorld->getNumCollisionObjects()
             << " rigid bodies." << endl;
      }
  }

  void PhysicsComponent::removePhysicsObjectInstance(PhysicsObjectInstance* obj) {
    if (obj->getInPhysicsWorld()) {
      removeRigidBody(obj->getRigidBody());
      obj->setInPhysicsWorld(false);
    }
  }

  void PhysicsComponent::removeRigidBody(btRigidBody *rigidBody) {
      if (DEBUG) {
          cout << "PhysicsComponent: Removing Rigid Body from Dynamics World" << endl;
      }
      mDynamicsWorld->removeRigidBody(rigidBody);
  }

  void PhysicsComponent::addPhysicsObjectInstance(PhysicsObjectInstance *physicsObjejct) {
      addRigidBody(physicsObjejct->getRigidBody());
  }

  void PhysicsComponent::populatePhysicsWorld(vector<SceneObject*> soVec) {
      vector<SceneObject*>::iterator soIter;
      for (soIter = soVec.begin(); soIter != soVec.end(); soIter++) {
          if ((*soIter)->hasPhysicsObjectInstance()) {
              PhysicsObjectInstance* physicsObject = (*soIter)->getPhysicsObjectInstance();
              if (!physicsObject->getInPhysicsWorld()) {
                if (DEBUG) {
                  cout << "PhysicsComponent: Adding SceneObject " << (*soIter)->getUuid()
                       << " to Physics World" << endl;
                }
                addPhysicsObjectInstance(physicsObject);
                physicsObject->setInPhysicsWorld(true);
              }
          }
      }
  }

} // End of Dream
