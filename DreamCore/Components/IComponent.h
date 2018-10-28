/*
* Dream::IComponent
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

#include "../Common/DreamObject.h"

namespace Dream
{
    class Time;
    class SceneRuntime;

    class IComponent : public DreamObject
    {

    public:
        IComponent();
        virtual ~IComponent();
        virtual bool init() = 0;
        virtual void updateComponent(SceneRuntime*) = 0;

        long long getUpdateEndTime() const;
        long long getUpdateBeginTime() const;
        long long getUpdateTime() const;
        long long getYieldedTime() const;
        void setTime(Time*);

    protected:
        void beginUpdate();
        void endUpdate();

        void setBusy(bool complete);
        bool isBusy();

        long long mUpdateBeginTime;
        long long mUpdateEndTime;
        bool mBusy;

        Time* mTime;

    }; // End of IComponent

} // End of Dream
