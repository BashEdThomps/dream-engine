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
      mDebugDrawer = nullptr;
      mDebug = false;
  }

  PhysicsComponent::~PhysicsComponent() {
      if (DEBUG) {
            cout << "PhysicsComponent: Destroying Object" << endl;
      }
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

  void PhysicsComponent::setGravity(vector<float> gravity) {
      if (DEBUG) {
          cout <<"PhysicsComponent: Setting Gravity" << String::floatVectorToString(gravity) << endl;
      }
      mGravity = btVector3(gravity[0],gravity[1],gravity[2]);
      if (mDynamicsWorld != nullptr) {
          mDynamicsWorld->setGravity(mGravity);
      }
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
      mDynamicsWorld->setGravity(mGravity);

      if (DEBUG) {
          cout << "done." << endl;
      }
      return true;
  }

  void PhysicsComponent::update(Dream::Scene* scene) {
      if (VERBOSE) {
          cout << "PhysicsComponent: Update Called" << endl;
      }
      // Setup Debug
      if (mDebug && mDebugDrawer == nullptr) {
          mDebugDrawer = new PhysicsDebugDrawer();
          mDebugDrawer->init();
          mDebugDrawer->setDebugMode(btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE);
          mDynamicsWorld->setDebugDrawer(mDebugDrawer);
      }
      vector<SceneObject*> tmpSG = scene->getScenegraphVector();
      populatePhysicsWorld(&tmpSG);
      btScalar stepValue = static_cast<btScalar>(mTime->getTimeDelta());
      mDynamicsWorld->stepSimulation(stepValue);
      checkContactManifolds();
      if (mDebug) {
          mDynamicsWorld->debugDrawWorld();
      }
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

  void PhysicsComponent::populatePhysicsWorld(vector<SceneObject*>* soVec) {
      mScenegraph = soVec;
      vector<SceneObject*>::iterator soIter;
      for (soIter = soVec->begin(); soIter != soVec->end(); soIter++) {
          // Has physics
          if ((*soIter)->hasPhysicsObjectInstance()) {
              PhysicsObjectInstance* physicsObject = (*soIter)->getPhysicsObjectInstance();
              // Marked for deletion and in physics world, remove
              if ((*soIter)->getDeleteFlag() && physicsObject->getInPhysicsWorld()) {
                  if (DEBUG) {
                      cout << "PhysicsComponent: Removing SceneObject " << (*soIter)->getUuid()
                           << " from Physics World" << endl;
                  }
                  removePhysicsObjectInstance(physicsObject);
              }
              // Not marked for deletion and not in world, add
              if (!(*soIter)->getDeleteFlag() && !physicsObject->getInPhysicsWorld()) {
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

  void PhysicsComponent::setViewProjectionMatrix(glm::mat4 view, glm::mat4 proj) {
      if (mDebugDrawer != nullptr) {
          mDebugDrawer->setViewMatrix(view);
          mDebugDrawer->setProjectionMatrix(proj);
      }
  }

  void PhysicsComponent::setDebug(bool debug) {
      mDebug = debug;
  }

  void PhysicsComponent::checkContactManifolds() {
      int numManifolds = mDynamicsWorld->getDispatcher()->getNumManifolds();
      for (int i=0;i<numManifolds;i++) {
          btPersistentManifold* contactManifold;
          contactManifold =  mDynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
          const btCollisionObject* objA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
          const btCollisionObject* objB = static_cast<const btCollisionObject*>(contactManifold->getBody1());
          SceneObject* sObjA = getSceneObject(objA);
          SceneObject* sObjB = getSceneObject(objB);

          Event* e = new Event(sObjB->getUuid(),EVENT_TYPE_COLLISION);
          sObjA->sendEvent(e);

          /*
           * More detail about contact.
           * Not needed for simple collison detection.
          int numContacts = contactManifold->getNumContacts();
          for (int j=0;j<numContacts;j++) {
              btManifoldPoint& pt = contactManifold->getContactPoint(j);
              if (pt.getDistance()<0.f) {
                  const btVector3& ptA = pt.getPositionWorldOnA();
                  const btVector3& ptB = pt.getPositionWorldOnB();
                  const btVector3& normalOnB = pt.m_normalWorldOnB;
              }
          }
          */
      }
  }

  SceneObject* PhysicsComponent::getSceneObject(const btCollisionObject* collObj) {
      vector<SceneObject*>::iterator it;
      for (it=mScenegraph->begin(); it!=mScenegraph->end(); it++) {
          SceneObject* next = (*it);
          if (next->hasPhysicsObjectInstance()) {
              PhysicsObjectInstance* nextPO = next->getPhysicsObjectInstance();
              if (nextPO->getCollisionObject() == collObj) {
                  return (*it);
              }
          }
      }
      return nullptr;
  }

  void PhysicsComponent::drawDebug() {
      if (mDebug) {
        mDebugDrawer->drawAll();
      }
  }

} // End of Dream
