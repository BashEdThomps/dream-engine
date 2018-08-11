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

#include "../Common/ILoggable.h"

namespace Dream
{
    class Time;
    class SceneRuntime;

    class IComponent : public ILoggable
    {

    public:
        IComponent();
        virtual ~IComponent();
        virtual bool init() = 0;
        virtual void updateComponent() = 0;
        void setTime(Time*);

        bool getRunning() const;
        void setRunning(volatile bool running);

        bool getShouldUpdate() const;
        void setShouldUpdate(volatile bool shouldUpdate);

        long long getUpdateEndTime() const;
        long long getUpdateBeginTime() const;
        long long getUpdateTime() const;
        long long getYieldedTime() const;
        void setActiveSceneRuntime(SceneRuntime* runtime);
        bool getUpdateComplete() const;

    protected:
        void beginUpdate();
        void endUpdate();

        long long mUpdateBeginTime;
        long long mUpdateEndTime;

        Time* mTime;
        volatile bool mRunning;
        volatile bool mShouldUpdate;
        SceneRuntime* mActiveSceneRuntimeHandle;
        volatile bool mUpdateComplete;

    }; // End of IComponent

} // End of Dream
