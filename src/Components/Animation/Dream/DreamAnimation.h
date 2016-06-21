/*
* Dream::Components::Animation::DreamAnimation
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


#ifndef DreamAnimation_hpp
#define DreamAnimation_hpp

#include "../AnimationComponentInterface.h"
#include "../../../Asset/Instances/Animation/Dream/AnimationInstance.h"

namespace Dream      {
namespace Components {
namespace Animation  {
namespace Dream      {
	
	class DreamAnimation : public AnimationComponentInterface {
	public :
		DreamAnimation();
		~DreamAnimation();
		bool init();
		void update(::Dream::Scene::Scene*);
		void play (Asset::AssetInstance*);
		void pause(Asset::AssetInstance*);
		void stop (Asset::AssetInstance*);
		bool isLooping(Asset::AssetInstance*);
		void setLooping(Asset::AssetInstance*,bool);
	};
	
} // End of Dream
} // End of Animation
} // End of Components
} // End of Dream

#endif /* DreamAnimation_hpp */
