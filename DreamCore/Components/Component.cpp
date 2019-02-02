/*
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

#include "Component.h"
#include "Time.h"

namespace Dream
{

    Component::Component
    (ProjectRuntime* pr)
        : LockableObject("Component"),
          mProjectRuntime(pr),
          mEnabled(true)
    {

    }

    Component::~Component
    ()
    {

    }


    void
    Component::setTime
    (Time* time)
    {
        mTime = time;
    }

    void
    Component::beginUpdate
    ()
    {
        #ifdef DREAM_LOG
        getLog()->debug("\n"
        "=======================================================================");
        #endif
        mUpdateBeginTime = mTime->getAbsoluteTime();
    }

    void
    Component::endUpdate
    ()
    {
        mUpdateEndTime =  mTime->getAbsoluteTime();
        #ifdef DREAM_LOG
        getLog()->debug("Update Complete in {}ms\n"
        "=======================================================================\n\n",
        getUpdateDuration());
        #endif
    }

    bool Component::getEnabled() const
    {
        return mEnabled;
    }

    void Component::setEnabled(bool enabled)
    {
        mEnabled = enabled;
    }

    long long
    Component::getUpdateBeginTime
    ()
    const
    {
        return mUpdateBeginTime;
    }

    long long
    Component::getUpdateDuration
    ()
    const
    {
        return mUpdateEndTime-mUpdateBeginTime;
    }

    long long
    Component::getUpdateEndTime
    ()
    const
    {
        return mUpdateEndTime;
    }
}
