/*
 * Component
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

#include "IComponent.h"
#include "Time.h"

namespace Dream
{

    IComponent::IComponent
    () : DreamObject("IComponent")
    {

    }


    IComponent::~IComponent
    ()
    {

    }


    void
    IComponent::setTime
    (Time* time)
    {
        mTime = time;
    }

    void
    IComponent::beginUpdate
    ()
    {
        auto log = getLog();
        log->debug("Updating Component");
        mUpdateBeginTime = mTime->nowLL();
        setBusy(true);
    }

    void
    IComponent::endUpdate
    ()
    {
        auto log = getLog();
        mUpdateEndTime =  mTime->nowLL();
        setBusy(false);
        log->debug("Update Complete in {}",getUpdateTime());
    }

    void
    IComponent::setBusy
    (bool complete)
    {
       mBusy = complete;
    }

    bool
    IComponent::isBusy
    ()
    {
       return mBusy;
    }

    long long
    IComponent::getUpdateBeginTime
    ()
    const
    {
        return mUpdateBeginTime;
    }

    long long
    IComponent::getUpdateTime
    ()
    const
    {
        return mUpdateEndTime-mUpdateBeginTime;
    }

    long long
    IComponent::getYieldedTime
    ()
    const
    {
       return abs(mUpdateBeginTime-mUpdateEndTime);
    }

    long long
    IComponent::getUpdateEndTime
    ()
    const
    {
        return mUpdateEndTime;
    }

    void
    IComponent::setActiveSceneRuntime
    (SceneRuntime* runtime)
    {
       mActiveSceneRuntime = runtime;
    }

} // End of Dream
