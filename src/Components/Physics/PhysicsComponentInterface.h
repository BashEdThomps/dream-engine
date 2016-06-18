/*
* Dream::Components::Physics::PhysicsInterface
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

#ifndef PHYSICSINTERFACE_H
#define PHYSICSINTERFACE_H

#include "../ComponentInterface.h"

namespace Dream {
namespace Components {
namespace Physics {
	
	class PhysicsComponentInterface : public Dream::Components::ComponentInterface {
	public:
		PhysicsComponentInterface(void);
		virtual ~PhysicsComponentInterface(void) {};
		
	};// End of PhysicsInterface
	
} // End of Physics
} // End of Components
} // End of Dream

#endif // End of PHYSICSINTERFACE_H
