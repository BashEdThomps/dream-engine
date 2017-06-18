/*
* Dream::AnimationComponent
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


#pragma once

#include "AnimationInstance.h"
#include "../IComponent.h"

namespace Dream
{
    class AnimationComponent : public IComponent
    {
    private:
        Time* mTime;
    public:
        AnimationComponent();
        ~AnimationComponent();

        bool init() override;
        void updateComponent(SceneRuntime*) override;
        void cleanUp(SceneRuntime*) override;

        void play (AnimationInstance*);
        void pause(AnimationInstance*);
        void stop (AnimationInstance*);
        bool isLooping(AnimationInstance*);
        void setLooping(AnimationInstance*,bool);
    }; // End of AnimationComponent

} // End of Dream
