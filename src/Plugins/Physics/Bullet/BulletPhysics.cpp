/*
* Dream::Physics::BulletPhysics
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

#include "BulletPhysics.h"
#include "../../Video/OpenGL/GLFWTime.h"

namespace Dream {
namespace Plugins {
namespace Physics {
namespace Bullet {
	
	BulletPhysics::BulletPhysics(void) : PhysicsPluginInterface() {}

	BulletPhysics::~BulletPhysics(void) {
		delete mSolver;
		delete mDispatcher;
		delete mCollisionConfiguration;
		delete mBroadphase;
		delete mDynamicsWorld;
	}

	void BulletPhysics::setGravity3f(float x, float y, float z) {
		btVector3 gravity = btVector3(x,y,z);
		setGravityBtVector3(gravity);
	}

	void BulletPhysics::setGravityBtVector3(btVector3 gravity) {
		mDynamicsWorld->setGravity(gravity);
	}

	bool BulletPhysics::init(void) {
		std::cout << "BulletPhysics: Initialising...";
		mBroadphase = new btDbvtBroadphase();
		mCollisionConfiguration = new btDefaultCollisionConfiguration();
		mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
		mSolver = new btSequentialImpulseConstraintSolver();
		mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher,mBroadphase,mSolver,mCollisionConfiguration);
		std::cout << "done." << std::endl;
		return true;
	}

	void BulletPhysics::update(Dream::Scene::Scene* scene) {
		btScalar stepValue = Video::OpenGL::GLFWTime::getInstance()->getTimeDelta();
		#ifdef VERBOSE
			std::cout << "BulletPhysics: Step Simulation by " << stepValue << std::endl;
		#endif
		mDynamicsWorld->stepSimulation(stepValue);
	}
	
	void BulletPhysics::addRigidBody(btRigidBody *rigidBody) {
    #ifdef VERBOSE
			std::cout << "BulletPhysics: Adding Rigid Body to Dynamics World" << std::endl;
    #endif
		mDynamicsWorld->addRigidBody(rigidBody);
	}
	
	void BulletPhysics::removeRigidBody(btRigidBody *rigidBody) {
		#ifdef VERBOSE
			std::cout << "BulletPhysics: Removing Rigid Body from Dynamics World" << std::endl;
    #endif
		mDynamicsWorld->removeRigidBody(rigidBody);
	}
	
} // End of Bullet
} // End of Scene
} // End of Plugins
} // End of Dream
