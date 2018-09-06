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

    IComponent::IComponent(bool parallel)
        :DreamObject("IComponent"),
          mParallel(parallel),
          mRunning(false),
          mShouldUpdate(false)
    {

    }


    IComponent::~IComponent()
    {

    }


    void IComponent::setTime(shared_ptr<Time> time)
    {
        mTime = time;
    }

    bool IComponent::getRunning() const
    {
        return mRunning;
    }

    void IComponent::setRunning(bool running)
    {
        mRunning = running;
    }

    bool IComponent::getShouldUpdate()
    {
        return mShouldUpdate;
    }

    void IComponent::setShouldUpdate(bool shouldUpdate)
    {
        mShouldUpdate = shouldUpdate;
    }

    void IComponent::beginUpdate()
    {
        auto log = getLog();
        log->info("Updating Component");
        mUpdateBeginTime = mTime->nowLL();
        setUpdateComplete(false);
        setShouldUpdate(false);
    }

    void IComponent::endUpdate()
    {
        auto log = getLog();
        mUpdateEndTime =  mTime->nowLL();
        setUpdateComplete(true);
        log->info("Update Complete in {}",getUpdateTime());

    }

    void IComponent::setUpdateComplete(bool complete)
    {
        mUpdateComplete = complete;
    }

    bool IComponent::getParallel() const
    {
        return mParallel;
    }

    void IComponent::setParallel(bool parallel)
    {
        mParallel = parallel;
    }

    bool IComponent::getUpdateComplete()
    {
        return mUpdateComplete;
    }

    long long IComponent::getUpdateBeginTime() const
    {
        return mUpdateBeginTime;
    }

    long long IComponent::getUpdateTime() const
    {
        return mUpdateEndTime-mUpdateBeginTime;
    }

    long long IComponent::getYieldedTime() const
    {
       return abs(mUpdateBeginTime-mUpdateEndTime);
    }

    long long IComponent::getUpdateEndTime() const
    {
        return mUpdateEndTime;
    }

    void IComponent::setActiveSceneRuntime(shared_ptr<SceneRuntime> runtime)
    {
       mActiveSceneRuntime = runtime;
    }

} // End of Dream
