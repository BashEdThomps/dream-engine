/*
* Dream::Plugins::Animaition::AnimationInterface
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


#ifndef AnimationInterface_hpp
#define AnimationInterface_hpp

#include "../PluginInterface.h"

namespace Dream {
namespace Plugins {
namespace Animation {
	
	class AnimationInterface : public Dream::Plugins::PluginInterface {
	public:
    	AnimationInterface();
    	~AnimationInterface();
	}; // End of AnimationInterface
	
} // End of Animation
} // End of Plugins
} // End of Dream

#endif /* AnimationInterface_hpp */
