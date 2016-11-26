/*
* AnimationComponent
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


#ifndef AnimationComponent_h
#define AnimationComponent_h

#include "../ComponentInterface.h"
#include "AnimationInstance.h"

namespace Dream {
  namespace Components {
    namespace Animation {

      class AnimationComponent : public Dream::Components::ComponentInterface {
      private:
        Time* mTime;
      public:
        AnimationComponent();
        ~AnimationComponent();
        bool init();
        void update(Scene*);
        void play (AssetInstance*);
        void pause(AssetInstance*);
        void stop (AssetInstance*);
        bool isLooping(AssetInstance*);
        void setLooping(AssetInstance*,bool);
      }; // End of AnimationComponent
    } // End of Animation
  } // End of Components
} // End of Dream

#endif // AnimationComponent_h
