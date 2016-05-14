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

#ifndef PHYSICSWORLD_H
#define PHYSICSWORLD_H

#include "../../../Plugins/Interfaces/PhysicsInterface.h"
#include <btBulletDynamicsCommon.h>
#include <LinearMath/btVector3.h>
#include <iostream>

namespace Dream {
	namespace Plugins {
		namespace Physics {
			namespace Bullet {
				class BulletPhysics : public PhysicsInterface {
				private:
					// Bullet
					btDynamicsWorld                     *mDynamicsWorld;
					btBroadphaseInterface               *mBroadphase;
					btDefaultCollisionConfiguration     *mCollisionConfiguration;
					btCollisionDispatcher               *mDispatcher;
					btSequentialImpulseConstraintSolver *mSolver;
				public:
					BulletPhysics(void);
					~BulletPhysics(void);
					void setGravity3f(float,float,float);
					void setGravityBtVector3(btVector3);
					bool init(void);
					void update(void);

				}; // End of BulletPhysics
			} // End of Bullet
		} // End of Scene
	} // End of Plugins
} // End of Dream

#endif // End of PHYSICSWORLD_H
