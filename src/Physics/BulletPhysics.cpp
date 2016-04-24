/*
* BulletPhysics
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

namespace Dream {
	namespace Physics {
		BulletPhysics::BulletPhysics(void) : PhysicsInterface() {
			mBroadphase = new btDbvtBroadphase();
			mCollisionConfiguration = new btDefaultCollisionConfiguration();
			mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
			mSolver = new btSequentialImpulseConstraintSolver();
			mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher,mBroadphase,mSolver,mCollisionConfiguration);
		}

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
	} // End of Scene
} // End of Dream
